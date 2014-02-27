#include <coin/CbcModel.hpp>
#include <coin/OsiClpSolverInterface.hpp>
#include <coin/CoinPackedVector.hpp>
#include <coin/CoinPackedVector.hpp>
#include "tttMinCostMaxFlowCellTracker.h"


class CoinSolver{

private:
	vnl_sparse_matrix<double> m_A;
	vnl_vector<double> m_b;
	vnl_vector<double> m_c;

	vnl_vector<double> m_x;
public:
	CoinSolver(const vnl_vector<double> & c,const vnl_sparse_matrix<double> & A,const vnl_vector<double> & b){
		m_A=A;
		m_b=b;
		m_c=c;

	}
	vnl_vector<double> GetSolution(){
		return m_x;
	}
	void Solve() {
		OsiClpSolverInterface solver;

		int n_cols=m_c.size();

		double * objective=m_c.data_block();

		double * col_lb = new double[n_cols];//the column lower bounds
		double * col_ub = new double[n_cols];//the column upper bounds


		for(int i=0;i<n_cols;i++) {
			col_lb[i] = 0.0;
			col_ub[i] = 1;
		}

		int n_rows=m_b.size();
		CoinPackedMatrix * matrix = new CoinPackedMatrix(false,0,0);
		matrix->setDimensions(0, n_cols);
		double * row_lb = new double[n_rows];//the row lower bounds
//		double * row_ub = new double[n_rows];//the row upper bounds
//		double * row_lb= m_b.data_block();
		double * row_ub= m_b.data_block();

		for(int c=0;c<n_rows;c++){
			row_lb[c]=-COIN_DBL_MAX;
		}
		for(int r=0;r<n_rows;r++){
			CoinPackedVector row;

			 vnl_sparse_matrix< double >::row aRow=m_A.get_row(r);

			 for(unsigned int i=0;i<aRow.size();i++){
				 //std::cout << "I:" << i << " " << aRow[i].first << " " << aRow[i].second << std::endl;
				 row.insert(aRow[i].first,aRow[i].second);
			 }

			matrix->appendRow(row);
		}


		solver.loadProblem(*matrix, col_lb, col_ub, objective, row_lb, row_ub);
		solver.writeLp("problem","mps");

		for(int i=0;i<n_cols;i++){
			solver.setInteger(i);
		}

		// Pass the solver with the problem to be solved to CbcModel
		CbcModel model(solver);

		  // Do complete search
		model.branchAndBound();


		m_x.clear();
		m_x.set_size(n_cols);
		/* Print the solution.  CbcModel clones the solver so we
		 need to get current copy from the CbcModel */
		int numberColumns = model.solver()->getNumCols();


		 const double * solution = model.bestSolution();
		 memcpy(m_x.data_block(),solution,sizeof(double)*n_cols);
		for (int iColumn=0;iColumn<numberColumns;iColumn++) {

		    double value=solution[iColumn];
		    //if (fabs(value)>1.0e-7&&model.solver()->isInteger(iColumn))
		      //printf("%d has value %g\n",iColumn,value);

		 }
	}

};
namespace ttt {

struct CompareSecond {
	bool operator()(const std::pair<TrackedCellVertexType, double>& left,
			const std::pair<TrackedCellVertexType, double>& right) const {
		return left.second < right.second;
	}
};
MinCostMaxFlowCellTracker::MinCostMaxFlowCellTracker(){
	m_NextID=0;
}
void MinCostMaxFlowCellTracker::Track() {

	std::vector<std::map<int, TrackedCellVertexType> > idsToSkeletonVertex;
	idsToSkeletonVertex.resize(m_Observations.size());

	//First frame: Init a track per observed cell
	m_Tracks.resize(m_Observations.size());
	m_Tracks[0] = TrackedTissueDescriptor::New();

	m_Tracks[0]->m_SkeletonGraph = m_Observations[0]->m_SkeletonGraph;
	m_Tracks[0]->m_CellGraph = boost::shared_ptr<ttt::TrackedCellGraph>(
			new ttt::TrackedCellGraph);

	std::map<CellVertexType, TrackedCellVertexType> obsToTrack;

	BGL_FORALL_VERTICES(v,*m_Observations[0]->m_CellGraph,CellGraph){

		TrackedCellVertexType n= boost::add_vertex(*m_Tracks[0]->m_CellGraph);
		boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ID=m_NextID++;
		boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_SkeletonNodes=
		boost::get(ttt::CellPropertyTag(),*m_Observations[0]->m_CellGraph,v).m_SkeletonNodes;
		boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_Centroid=
		boost::get(ttt::CellPropertyTag(),*m_Observations[0]->m_CellGraph,v).m_Centroid;
		boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ObservedCell=v;
		std::cout << "Init track: " << boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ID << " "
				<< boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_Centroid << std::endl;

		idsToSkeletonVertex[0][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ID]=n;

		obsToTrack[v]=n;

	}

	BGL_FORALL_EDGES(e,*m_Observations[0]->m_CellGraph,CellGraph){
		CellVertexType source = boost::source(e,*m_Observations[0]->m_CellGraph);
		CellVertexType target = boost::target(e,*m_Observations[0]->m_CellGraph);
		boost::add_edge(obsToTrack[source],obsToTrack[target],*m_Tracks[0]->m_CellGraph);
	}

	int m_K = 2;

	for (unsigned int t = 1; t < m_Observations.size(); t++) {
		typedef ttt::AssociationGraph<CellVertexType, TrackedCellVertexType> AssociationGraphType;
		AssociationGraphType associationGraph;

		std::map<ttt::TrackedCellVertexType, double> distances;


		obsToTrack.clear();

		BGL_FORALL_VERTICES(v,*m_Tracks[t-1]->m_CellGraph,CellGraph){
			associationGraph.AddTrack(v);
			/*
			 * Destruction HYPOTHESIS
			 */
			//TODO Compute cost
			associationGraph.AddTerminationHypothesis(v,1000);

		}

		BGL_FORALL_VERTICES(v,*m_Observations[t]->m_CellGraph,CellGraph){
			associationGraph.AddObservation(v);
			/*
			 * Appearance HYPOTHESIS
			 */
			//TODO Compute cost

			associationGraph.AddCreationHypothesis(v,1000);
		}


		BGL_FORALL_VERTICES(obs,*m_Observations[t]->m_CellGraph,CellGraph){
			itk::Point<double,3> observationCentroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,obs).m_Centroid;


			//TODO ADD AREAS TO COST
			/*
			 * ASSOCIATION HYPOTHESIS
			 */
			BGL_FORALL_VERTICES(track,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph) {
				itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,track).m_Centroid;
				itk::Vector<double,3> diff = trackCentroid - observationCentroid;
				double distance = sqrt(pow(diff[0],2)+pow(diff[1],2) + pow(diff[2],2));
				distances[track] = distance;
			}


			for(int k=0;k<m_K;k++) {
				double minDistance;
				TrackedCellVertexType min;
				std::map<ttt::TrackedCellVertexType, double>::iterator minimum=std::min_element(distances.begin(),distances.end(),CompareSecond());
				min=minimum->first;
				minDistance=minimum->second;
				associationGraph.AddAssociationHypothesis(min,obs,minDistance);
				minimum->second=1000000000;
			}
			distances.clear();

			/*
			 * MYTHOSIS HYPOTHESIS
			 */
			typedef boost::graph_traits <ttt::CellGraph>::adjacency_iterator adjacency_iterator;

			std::pair<adjacency_iterator, adjacency_iterator> neighbors = boost::adjacent_vertices(obs, *m_Observations[t]->m_CellGraph);

			for(; neighbors.first != neighbors.second; ++neighbors.first){
				itk::Point<double,3> neighborCentroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,*neighbors.first).m_Centroid;
				itk::Point<double,3> mythosisCentroid;
				for(int i=0;i<3;i++){
					mythosisCentroid[i]=observationCentroid[i]+neighborCentroid[i];
					mythosisCentroid[i]=mythosisCentroid[i]/2;
				}

				BGL_FORALL_VERTICES(track,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph) {
					itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,track).m_Centroid;
					itk::Vector<double,3> diff = trackCentroid - mythosisCentroid;
					double distance = sqrt(pow(diff[0],2)+pow(diff[1],2) + pow(diff[2],2));
					distances[track] = distance;
				}
				for(int k=0;k<m_K;k++) {
					double minDistance;
					TrackedCellVertexType min;
					std::map<ttt::TrackedCellVertexType, double>::iterator minimum=std::min_element(distances.begin(),distances.end(),CompareSecond());
					min=minimum->first;
					minDistance=minimum->second;
					associationGraph.AddMitosisHypothesis(min,obs,*neighbors.first,distances[min]);
					minimum->second=1000000000000;
				}
				distances.clear();
			}
		}

		vnl_sparse_matrix<double> flowMatrix;
		vnl_vector<double> flowVector,costVector;
		associationGraph.GetProblem(costVector,flowVector,flowMatrix);

		//std::cout << costVector<< std::endl;


		//std::cout << flowMatrix<< std::endl;
		for(unsigned int r=0;r<flowMatrix.rows();r++){
			for(unsigned int c=0;c<flowMatrix.cols();c++){
			//	std::cout << flowMatrix.get(r,c) << "\t";
			}
			//std::cout << "=" << flowVector.get(r) <<std::endl;
		}
		CoinSolver solver(costVector,flowMatrix,flowVector);
		solver.Solve();


		vnl_vector<double> solution = solver.GetSolution();
		std::vector<std::pair<CellVertexType,TrackedCellVertexType> > associations;
		std::vector<CellVertexType>  toCreate;
		std::vector<TrackedCellVertexType>   toDelete;
		std::vector<AssociationGraphType::MitosisType>   mitosis;
		associationGraph.DecodeSolution(solution,associations,toCreate,toDelete,mitosis);

		m_Tracks[t]=ttt::TrackedTissueDescriptor::New();
		for(std::vector<std::pair<CellVertexType,TrackedCellVertexType> >::iterator it= associations.begin();it!=associations.end();it++){

			std::cout << "Associate " << it->first << " to " << it->second << std::endl;
			TrackedCellVertexType n= boost::add_vertex(*m_Tracks[t]->m_CellGraph);

			boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID=
					boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,it->second).m_ID;
           	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_Centroid=
           			boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->first).m_Centroid;

           	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_SkeletonNodes=
           			boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->first).m_SkeletonNodes;

           	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ObservedCell=it->first;

           	idsToSkeletonVertex[t][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID]=it->first;

           	obsToTrack[it->first]=n;
		}

		for(std::vector<CellVertexType>::iterator it= toCreate.begin();it!=toCreate.end();++it){
			std::cout << "Create " << *it << std::endl;

			TrackedCellVertexType n = boost::add_vertex(*m_Tracks[t]->m_CellGraph);

			boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID=m_NextID++;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_Centroid=
					boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,*it).m_Centroid;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_SkeletonNodes=
					boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,*it).m_SkeletonNodes;
			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ObservedCell=*it;

			idsToSkeletonVertex[t][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID]=n;

			obsToTrack[*it]=n;

		}
		for(std::vector<AssociationGraphType::MitosisType>::iterator it=mitosis.begin();it!=mitosis.end();++it){
			//Create two new tracks
			{
			TrackedCellVertexType a = boost::add_vertex(*m_Tracks[t]->m_CellGraph);

			boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,a).m_ID=m_NextID++;
			boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,a).m_ParentID=
					boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,it->track).m_ID;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,a).m_Centroid=
					boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->A).m_Centroid;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,a).m_SkeletonNodes=
					boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->A).m_SkeletonNodes;
			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,a).m_ObservedCell=it->A;
				idsToSkeletonVertex[t][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,a).m_ID]=a;

				obsToTrack[it->A]=a;
			}
			{
			TrackedCellVertexType b = boost::add_vertex(*m_Tracks[t]->m_CellGraph);

			boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,b).m_ID=m_NextID++;

			boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,b).m_ParentID=
								boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,it->track).m_ID;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,b).m_Centroid=
					boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->B).m_Centroid;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,b).m_SkeletonNodes=
					boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->B).m_SkeletonNodes;
			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,b).m_ObservedCell=it->B;
			idsToSkeletonVertex[t][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,b).m_ID]=b;
			obsToTrack[it->B]=b;
			}

		}

		BGL_FORALL_EDGES(e,*m_Observations[t]->m_CellGraph,CellGraph){
			CellVertexType source = boost::source(e,*m_Observations[t]->m_CellGraph);
			CellVertexType target = boost::target(e,*m_Observations[t]->m_CellGraph);
			boost::add_edge(obsToTrack[source],obsToTrack[target],*m_Tracks[t]->m_CellGraph);
		}
	}

#if 0
//Compute velocity vectors

	for (unsigned int t = 0; t < m_Tracks.size(); t++) {

		BGL_FORALL_VERTICES(track,*m_Tracks[t]->m_CellGraph,TrackedCellGraph){
		itk::Point<double,3> t_1,t0,t1;
		int id=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_ID;
		t0=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_Centroid;
		bool previous=false;
		bool next=false;

		if(t>0 && idsToSkeletonVertex[t-1].count(id)>0 ) {
			previous=true;
			t_1=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,idsToSkeletonVertex[t-1][id]).m_Centroid;
		}

		if(t<m_Tracks.size()-1 && idsToSkeletonVertex[t+1].count(id)>0 ) {
			next=true;
			t1=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t+1]->m_CellGraph,idsToSkeletonVertex[t+1][id]).m_Centroid;
		}

		itk::Vector<double,3> vel;
		if(previous && next) {
			itk::Vector<double,3> vl=t0 - t_1;
			itk::Vector<double,3> vr=t1 - t0;
			vel=(vl +vr)/2;
		} else if(previous && !next) {
			itk::Vector<double,3> vl=t0 - t_1;
			vel=vl;
		} else if(!previous && next) {
			itk::Vector<double,3> vr=t1 - t0;
			vel=vr;
		} else {
			vel.Fill(0);
		}
		boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_Velocity=vel;
	}
}
#endif
}

}
/* namespace ttt */

