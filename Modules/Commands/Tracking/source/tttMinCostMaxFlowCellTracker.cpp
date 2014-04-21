#include <coin/CbcModel.hpp>
#include <coin/OsiClpSolverInterface.hpp>
#include <coin/CoinPackedVector.hpp>
#include <coin/CoinPackedVector.hpp>
#include "tttMinCostMaxFlowCellTracker.h"
#include "CellArea.h"

class CoinSolver{

private:
	vnl_sparse_matrix<double> m_A;
	vnl_vector<double> m_b;
	vnl_vector<double> m_cost;
	vnl_vector<double> m_capacity;

	vnl_vector<double> m_x;
public:
	CoinSolver(const vnl_vector<double> & cost, const vnl_vector<double> & capacity, const vnl_sparse_matrix<double> & A,const vnl_vector<double> & b){
		m_A=A;
		m_b=b;
		m_cost=cost;
		m_capacity=capacity;
	}
	vnl_vector<double> GetSolution(){
		return m_x;
	}
	void Solve() {
		OsiClpSolverInterface solver;

		int n_cols=m_cost.size();

		double * objective=m_cost.data_block();

		double * col_lb = new double[n_cols];//the column lower bounds
//		double * col_ub = new double[n_cols];//the column upper bounds
		double * col_ub = m_capacity.data_block();//the column upper bounds


		for(int i=0;i<n_cols;i++) {
			col_lb[i] = 0.0;
			//col_ub[i] = 1;
		}

		int n_rows=m_b.size();
		CoinPackedMatrix * matrix = new CoinPackedMatrix(false,0,0);
		matrix->setDimensions(0, n_cols);
		//double * row_lb = new double[n_rows];//the row lower bounds
		//double * row_ub = new double[n_rows];//the row upper bounds
		double * row_lb= m_b.data_block();
		double * row_ub= m_b.data_block();

		for(int c=0;c<n_rows;c++){
			//row_lb[c]=-COIN_DBL_MAX;
			//row_lb[c]=
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
	m_AreaWeight=0.1;
	m_DistanceWeight=0.9;
	m_MitosisWeight=5;
	m_CreationWeight=1;
	m_TerminationWeight=1;
	m_AssociationWeight=1;
}


static double PointToSegmentDistance(const itk::Point<double,3> & x,const itk::Point<double,3> & a,const itk::Point<double,3> & b){

				itk::Vector<double,3> ax = x -a;
				double normAX=ax.GetNorm();
				if(normAX==0) return 0;
				itk::Vector<double,3> ab = b-a;
				double r= ab* ax;


				r/= ax.GetNorm();

				double dist;
				if(r<0){
					dist = ax.GetNorm();
				}else if(r>1){
					itk::Vector<double,3> xb = b-x;
					dist = xb.GetNorm();
				}else{
					dist = sqrt(fabs( pow(ax.GetNorm(),2)-r*pow(ab.GetNorm(),2)));
				}
				assert(dist==dist);
				return dist;
}

template<class TissueDescriptorType> double DistToPerimeter(const itk::Point<double,3> & x,const typename TissueDescriptorType::Pointer & descriptor ){
	std::vector<double> dists;
	std::vector<ttt::SkeletonVertexType>::iterator it=descriptor->BeginPerimeter();
	std::vector<ttt::SkeletonVertexType>::iterator it2=it+1;
	while(it2!=descriptor->EndPerimeter()){
		itk::Point<double,3> a = boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,*it).position;
		itk::Point<double,3> b = boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,*it2).position;

		dists.push_back(PointToSegmentDistance(x,a,b));
		++it;
		++it2;
	}
	it2=descriptor->BeginPerimeter();

	itk::Point<double,3> a = boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,*it2).position;
	itk::Point<double,3> b = boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,*it).position;

	dists.push_back(PointToSegmentDistance(x,a,b));
	double dist =*(std::min_element(dists.begin(),dists.end()));
	assert(dist==dist);
	return dist;

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

		//std::cout << "Init track: " << boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ID << " "
		//	<< boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_Centroid << std::endl;

		idsToSkeletonVertex[0][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ID]=n;

		obsToTrack[v]=n;
	}




	BGL_FORALL_EDGES(e,*m_Observations[0]->m_CellGraph,CellGraph){
		CellVertexType source = boost::source(e,*m_Observations[0]->m_CellGraph);
		CellVertexType target = boost::target(e,*m_Observations[0]->m_CellGraph);
		boost::add_edge(obsToTrack[source],obsToTrack[target],*m_Tracks[0]->m_CellGraph);
	}

	for(std::vector<ttt::SkeletonVertexType>::iterator it=m_Observations[0]->BeginPerimeter();it!=m_Observations[0]->EndPerimeter();++it){
		m_Tracks[0]->AddVertexToPerimeter(*it);
	}

	int m_K = 10;

	for (unsigned int t = 1; t < m_Observations.size(); t++) {


		CellArea<ttt::TrackedTissueDescriptor> m_TrackAreas;
		CellArea<ttt::TissueDescriptor> m_ObservationAreas;

		typedef ttt::AssociationGraph<CellVertexType, TrackedCellVertexType> AssociationGraphType;
		AssociationGraphType associationGraph;

		std::map<ttt::TrackedCellVertexType, double> distances;

		m_TrackAreas.SetTissueDescriptor(m_Tracks[t-1]);
		m_ObservationAreas.SetTissueDescriptor(m_Observations[t]);

		m_TrackAreas.Compute();
		m_ObservationAreas.Compute();

		obsToTrack.clear();


		double meanObsArea=m_ObservationAreas.GetMean();
		double stdObsArea=m_ObservationAreas.GetStd();


		double meanTrackArea=m_TrackAreas.GetMean();
		double stdTrackArea=m_TrackAreas.GetStd();

		double sx=0,sy=0,sz=0,sx2=0,sy2=0,sz2=0,mux,muy,muz,stdx,stdy,stdz;
		int total=0;
		BGL_FORALL_VERTICES(v,*m_Observations[t]->m_CellGraph,CellGraph){
			itk::Point<double,3> centroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,v).m_Centroid;
			sx+=centroid[0];
			sy+=centroid[1];
			sz+=centroid[2];

			sx2+=centroid[0]* centroid[0];
			sy2+=centroid[1]* centroid[1];
			sz2+=centroid[2]* centroid[2];

			total++;
		}
		BGL_FORALL_VERTICES(v,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph){
			itk::Point<double,3> centroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,v).m_Centroid;
			sx += centroid[0];
			sy += centroid[1];
			sz += centroid[2];
			sx2 += centroid[0]* centroid[0];
			sy2 += centroid[1]* centroid[1];
			sz2 += centroid[2]* centroid[2];
			total++;
		}
		double ss = sx+sy+sz;
		double mu=ss/(3*total);
		//mux=sx/total;
		//muy=sy/total;
		//muz=sz/total;

		//mux=0;
		//muy=0;
		//muz=0;


		//mux=mu;
		//muy=mu;
		//muz=mu;

		double ss2= sx2 + sy2 + sz2;

		//stdx= sqrt(sx2/total - mux*mux);

		//stdy= sqrt(sy2/total - muy*muy);

		//stdz= sqrt(sz2/total - muz*muz);

		stdx= sqrt(ss2/(3*total) - mu*mu);

		stdy= sqrt(ss2/(3*total) - mu*mu);

		stdz= sqrt(ss2/(3*total) - mu*mu);

		//stdx=1;
		//stdy=1;
		//stdz=1;

		BGL_FORALL_VERTICES(v,*m_Observations[t]->m_CellGraph,CellGraph){

			itk::Point<double,3> centroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,v).m_Centroid;
			centroid[0] -= mux;
			centroid[0] /= stdx;

			centroid[1] -= muy;
			centroid[1] /= stdy;

			centroid[2] -= muz;
			centroid[2] /= stdz;

			boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,v).m_Centroid=centroid;
		}

		for(std::vector<ttt::SkeletonVertexType>::iterator it=m_Observations[t]->BeginPerimeter();it!=m_Observations[t]->EndPerimeter();++it){
			itk::Point<double,3> position = boost::get(ttt::SkeletonPointPropertyTag(),*m_Observations[t]->m_SkeletonGraph,*it).position;
			position[0] -= mux;
			position[0] /= stdx;

			position[1] -= muy;
			position[1] /= stdy;

			position[2] -= muz;
			position[2] /= stdz;

			boost::get(ttt::SkeletonPointPropertyTag(),*m_Observations[t]->m_SkeletonGraph,*it).position=position;
		}

		BGL_FORALL_VERTICES(v,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph){
			itk::Point<double,3> centroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,v).m_Centroid;
			centroid[0] -= mux;
			centroid[0] /= stdx;

			centroid[1] -= muy;
			centroid[1] /= stdy;

			centroid[2] -= muz;
			centroid[2] /= stdz;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,v).m_Centroid=centroid;

		}

		for(std::vector<ttt::SkeletonVertexType>::iterator it=m_Tracks[t-1]->BeginPerimeter();it!=m_Tracks[t-1]->EndPerimeter();++it){
			itk::Point<double,3> position = boost::get(ttt::SkeletonPointPropertyTag(),*m_Tracks[t-1]->m_SkeletonGraph,*it).position;
			position[0] -= mux;
			position[0] /= stdx;

			position[1] -= muy;
			position[1] /= stdy;

			position[2] -= muz;
			position[2] /= stdz;

			boost::get(ttt::SkeletonPointPropertyTag(),*m_Tracks[t-1]->m_SkeletonGraph,*it).position=position;
		}


		BGL_FORALL_VERTICES(v,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph){
			associationGraph.AddTrack(v);
			/*
			 * Destruction HYPOTHESIS
			 */

			itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,v).m_Centroid;

			double costDistance = DistToPerimeter<ttt::TrackedTissueDescriptor>(trackCentroid,m_Tracks[t-1]);
			double costArea = fabs((m_TrackAreas[v]-meanTrackArea)/stdTrackArea);



			//std::cout << "Termination " << v << " CostArea: " << " " << costArea<< " CostDistance: " << costDistance << std::endl;

			double terminationCost= this->m_TerminationWeight*costDistance;

			associationGraph.AddTerminationHypothesis(v,terminationCost);

		}




		BGL_FORALL_VERTICES(v,*m_Observations[t]->m_CellGraph,CellGraph){
			associationGraph.AddObservation(v);

			itk::Point<double,3> observationCentroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,v).m_Centroid;

			double costDistance = DistToPerimeter<ttt::TissueDescriptor>(observationCentroid,m_Observations[t]);
			double costArea = fabs((m_ObservationAreas[v]-meanObsArea)/stdObsArea);
			//std::cout << "Creation " <<  v << " CostArea: " << " " << costArea<< " CostDistance: " << costDistance << std::endl;


			double creationCost= this->m_CreationWeight*costDistance;
			associationGraph.AddCreationHypothesis(v,(creationCost));
		}


		BGL_FORALL_VERTICES(obs,*m_Observations[t]->m_CellGraph,CellGraph){
			//std::cout << "ASSOCIATION " << obs << std::endl;
			itk::Point<double,3> observationCentroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,obs).m_Centroid;


			/*
			 * ASSOCIATION HYPOTHESIS
			 */
			BGL_FORALL_VERTICES(track,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph) {
				itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,track).m_Centroid;
				itk::Vector<double,3> diff = trackCentroid - observationCentroid;

				double distance = sqrt(pow(diff[0],2)+pow(diff[1],2) + pow(diff[2],2));
				//std::cout << observationCentroid << "\t-\t" << trackCentroid << "\t=\t" << diff << "\t" << distance << std::endl;
				distances[track] = distance;
			}


			for(int k=0;k<m_K;k++) {

				TrackedCellVertexType min;
				std::map<ttt::TrackedCellVertexType, double>::iterator minimum=std::min_element(distances.begin(),distances.end(),CompareSecond());
				min=minimum->first;
				double costDistance=minimum->second;

				double costArea=  fabs((m_TrackAreas[min] -meanTrackArea)/stdTrackArea - (m_ObservationAreas[obs]-meanObsArea)/stdObsArea);

				//std::cout << "CostArea: " << " " << costArea<< " CostDistance: " << costDistance << std::endl;

				double associationCost= this->m_AssociationWeight*(costDistance + this->m_AreaWeight*costArea);

				associationGraph.AddAssociationHypothesis(min,obs,associationCost);

				minimum->second=DBL_MAX;
			}
			distances.clear();

			/*
			 * MYTHOSIS HYPOTHESIS
			 */
			//std::cout << "MITOSIS" << std::endl;
			typedef boost::graph_traits <ttt::CellGraph>::adjacency_iterator adjacency_iterator;

			std::pair<adjacency_iterator, adjacency_iterator> neighbors = boost::adjacent_vertices(obs, *m_Observations[t]->m_CellGraph);

			for(; neighbors.first != neighbors.second; ++neighbors.first){
				itk::Point<double,3> neighborCentroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,*neighbors.first).m_Centroid;

				itk::Point<double,3> mythosisCentroid;
				for(int i=0;i<3;i++){
					mythosisCentroid[i]=observationCentroid[i]*m_ObservationAreas[obs]+neighborCentroid[i]*m_ObservationAreas[*neighbors.first];
					mythosisCentroid[i]=mythosisCentroid[i]/(m_ObservationAreas[obs]+m_ObservationAreas[*neighbors.first]);
				}
				BGL_FORALL_VERTICES(track,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph) {
					itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,track).m_Centroid;

					itk::Vector<double,3> diff = trackCentroid - mythosisCentroid;
					double distance = sqrt(pow(diff[0],2)+pow(diff[1],2) + pow(diff[2],2));
					distances[track] = distance;
				}
				for(int k=0;k<m_K;k++) {

					TrackedCellVertexType min;
					std::map<ttt::TrackedCellVertexType, double>::iterator minimum=std::min_element(distances.begin(),distances.end(),CompareSecond());

					min=minimum->first;

					double costArea= fabs((m_TrackAreas[min] -meanTrackArea)/stdTrackArea - (m_ObservationAreas[obs]+m_ObservationAreas[*neighbors.first]-2*meanObsArea)/stdObsArea);

					double costDistance = distances[min];
					//std::cout << "CostArea: " << " " << costArea<< " CostDistance: " << costDistance << std::endl;
					double mitosisCost= this->m_MitosisWeight*(this->m_DistanceWeight*costDistance + this->m_AreaWeight*costArea);
					associationGraph.AddMitosisHypothesis(min,obs,*neighbors.first,mitosisCost);
					minimum->second=DBL_MAX;
				}
				distances.clear();
			}

		}

		BGL_FORALL_VERTICES(v,*m_Observations[t]->m_CellGraph,CellGraph){
			itk::Point<double,3> centroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,v).m_Centroid;
			centroid[0] *= stdx;
			centroid[0] += mux;

			centroid[1] *= stdy;
			centroid[1] += muy;

			centroid[2] *= stdz;
			centroid[2] += muz;

			boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,v).m_Centroid=centroid;
		}
		BGL_FORALL_VERTICES(v,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph){
			itk::Point<double,3> centroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,v).m_Centroid;
			centroid[0] *= stdx;
			centroid[0] += mux;

			centroid[1] *= stdy;
			centroid[1] += muy;

			centroid[2] *= stdz;
			centroid[2] += muz;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,v).m_Centroid=centroid;

		}

		for(std::vector<ttt::SkeletonVertexType>::iterator it=m_Observations[t]->BeginPerimeter();it!=m_Observations[t]->EndPerimeter();++it){
			itk::Point<double,3> position = boost::get(ttt::SkeletonPointPropertyTag(),*m_Observations[t]->m_SkeletonGraph,*it).position;

			position[0] *= stdx;
			position[0] += mux;

			position[1] *= stdy;
			position[1] += muy;

			position[2] *= stdz;
			position[2] += muz;

			boost::get(ttt::SkeletonPointPropertyTag(),*m_Observations[t]->m_SkeletonGraph,*it).position=position;
		}


		for(std::vector<ttt::SkeletonVertexType>::iterator it=m_Tracks[t-1]->BeginPerimeter();it!=m_Tracks[t-1]->EndPerimeter();++it){
			itk::Point<double,3> position = boost::get(ttt::SkeletonPointPropertyTag(),*m_Tracks[t-1]->m_SkeletonGraph,*it).position;

			position[0] *= stdx;
			position[0] += mux;

			position[1] *= stdy;
			position[1] += muy;

			position[2] *= stdz;
			position[2] += muz;

			boost::get(ttt::SkeletonPointPropertyTag(),*m_Tracks[t-1]->m_SkeletonGraph,*it).position=position;
		}
		associationGraph.Print();
		vnl_sparse_matrix<double> flowMatrix;
		vnl_vector<double> flowVector,costVector,capacityVector;
		associationGraph.GetProblem(costVector,flowVector,capacityVector,flowMatrix);

		//std::cout << costVector<< std::endl;


		//std::cout << flowMatrix<< std::endl;
		for(unsigned int r=0;r<flowMatrix.rows();r++){
			for(unsigned int c=0;c<flowMatrix.cols();c++){
			//	std::cout << flowMatrix.get(r,c) << "\t";
			}
			//std::cout << "=" << flowVector.get(r) <<std::endl;
		}
		CoinSolver solver(costVector,capacityVector,flowMatrix,flowVector);
		solver.Solve();


		vnl_vector<double> solution = solver.GetSolution();

		for(int i=0;i<flowMatrix.rows();i++){
			double flow=0;
			for(int c=0;c< flowMatrix.cols();c++){
				flow+=flowMatrix.get(i,c)*solution(c);
			}
			assert(flow==flowVector.get(i));
		}

		std::vector<std::pair<CellVertexType,TrackedCellVertexType> > associations;
		std::vector<CellVertexType>  toCreate;
		std::vector<TrackedCellVertexType>   toDelete;
		std::vector<AssociationGraphType::MitosisType>   mitosis;

		associationGraph.DecodeSolution(solution,associations,toCreate,toDelete,mitosis);

		//std::cout << "Associations: " << associations.size() << std::endl;
		//std::cout << "Mitosis: " << mitosis.size() << std::endl;
		//std::cout << "ToCreate: " << toCreate.size() << std::endl;
		//std::cout << "ToDelete: " << toDelete.size() << std::endl;
		//std::cout << "Observations: " << boost::num_vertices(*m_Observations[t]->m_CellGraph) << std::endl;
		assert(associations.size()+2*mitosis.size()+toCreate.size()==boost::num_vertices(*m_Observations[t]->m_CellGraph));

		m_Tracks[t]=ttt::TrackedTissueDescriptor::New();

		m_Tracks[t]->m_SkeletonGraph=m_Observations[t]->m_SkeletonGraph;

		for(std::vector<ttt::SkeletonVertexType>::iterator it=m_Observations[t]->BeginPerimeter();it!=m_Observations[t]->EndPerimeter();++it){
				m_Tracks[t]->AddVertexToPerimeter(*it);
		}

		m_Tracks[t]->m_CellGraph = boost::shared_ptr<ttt::TrackedCellGraph>(new ttt::TrackedCellGraph);

		for(std::vector<std::pair<CellVertexType,TrackedCellVertexType> >::iterator it= associations.begin();it!=associations.end();it++){



			int trackID= boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,it->second).m_ID;

			TrackedCellVertexType n= boost::add_vertex(*m_Tracks[t]->m_CellGraph);

			boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID=trackID;

           	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_Centroid=
           			boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->first).m_Centroid;

           	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_SkeletonNodes=
           			boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->first).m_SkeletonNodes;

           	assert(boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_SkeletonNodes.size()==boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->first).m_SkeletonNodes.size());

           	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ObservedCell=it->first;

           	idsToSkeletonVertex[t][ trackID]=n;


           	//std::cout << "Associate observation " << boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ObservedCell << " to " << trackID << " track ID " << trackID << " newTrack " << n << std::endl;

           	obsToTrack[it->first]=n;
		}

		for(std::vector<CellVertexType>::iterator it= toCreate.begin();it!=toCreate.end();++it){
			//std::cout << "Create " << *it << std::endl;

			TrackedCellVertexType n = boost::add_vertex(*m_Tracks[t]->m_CellGraph);

			boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID=m_NextID++;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_Centroid=
					boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,*it).m_Centroid;

			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_SkeletonNodes=
					boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,*it).m_SkeletonNodes;

			assert(boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_SkeletonNodes.size()==boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,*it).m_SkeletonNodes.size());

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
			assert(boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,a).m_SkeletonNodes.size()==boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->A).m_SkeletonNodes.size());

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

			assert(boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,b).m_SkeletonNodes.size()==boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->B).m_SkeletonNodes.size());

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
#if 0
		int sumInTracks=0;
		BGL_FORALL_VERTICES(track,*m_Tracks[t]->m_CellGraph,TrackedCellGraph) {
			sumInTracks+=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_SkeletonNodes.size();
		}
		int sumInObs=0;
		BGL_FORALL_VERTICES(obs,*m_Observations[t]->m_CellGraph,CellGraph) {
			sumInObs+=boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,obs).m_SkeletonNodes.size();
		}
		assert(sumInTracks==sumInObs);
#endif
	}


    for(unsigned int t=0;t<m_Tracks.size();t++){

        BGL_FORALL_VERTICES(track,*m_Tracks[t]->m_CellGraph,TrackedCellGraph) {
        	itk::Point<double,3> t_1,t0,t1;
        	int id=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_ID;

        	t0=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_Centroid;
        	bool previous=false;
        	bool next=false;

        	if(t>0 && idsToSkeletonVertex[t-1].count(id)>0 ){
        		previous=true;
        		t_1=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,idsToSkeletonVertex[t-1][id]).m_Centroid;
        	}

        	if(t<m_Tracks.size()-1 && idsToSkeletonVertex[t+1].count(id)>0 ){
        		next=true;
        	    t1=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t+1]->m_CellGraph,idsToSkeletonVertex[t+1][id]).m_Centroid;
        	}

        	itk::Vector<double,3> vel;
        	if(previous && next){
        		itk::Vector<double,3> vl=t0 - t_1;
        		itk::Vector<double,3> vr=t1 - t0;
        		//vel=(vl +vr)/2;
        		vel=(t1-t_1)/2;

        	}else if(previous && !next){

        		itk::Vector<double,3> vl=t0 - t_1;
        		vel=vl;


        	}else if(!previous && next){
        		itk::Vector<double,3> vr=t1 - t0;
        		vel=vr;

        	}else{
        		vel.Fill(0);
        	}

        	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_Velocity=vel;
        }
    }

}

}
/* namespace ttt */

