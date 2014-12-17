/*
 * MinCostMaxFlowTrackAssociationCommand.cpp
 *
 *  Created on: May 20, 2014
 *      Author: morgan
 */

#include <MinCostMaxFlowTrackAssociationCommand.h>
#include "CellMomentCalculator.h"
#include "AssociationGraph.h"
#include "CoinSolver.h"
#include <limits>
namespace ttt {

template<class TrackedCellVertexType> struct CompareSecond {
	bool operator()(const std::pair<TrackedCellVertexType, double>& left,
			const std::pair<TrackedCellVertexType, double>& right) const {
		return left.second < right.second;
	}
};
template<class TissueDescriptorType> static double PointToSegmentDistance(const itk::Point<double, TissueDescriptorType::NumDimensions > & x,
		const itk::Point<double,TissueDescriptorType::NumDimensions> & a, const itk::Point<double, TissueDescriptorType::NumDimensions> & b) {

	itk::Vector<double, TissueDescriptorType::NumDimensions> ax = x - a;
	double normAX = ax.GetNorm();
	if (normAX == 0)
		return 0;
	itk::Vector<double, TissueDescriptorType::NumDimensions> ab = b - a;
	double r = ab * ax;

	r /= ax.GetNorm();

	double dist;
	if (r < 0) {
		dist = ax.GetNorm();
	} else if (r > 1) {
		itk::Vector<double, TissueDescriptorType::NumDimensions> xb = b - x;
		dist = xb.GetNorm();
	} else {
		dist = sqrt(fabs(pow(ax.GetNorm(), 2) - r * pow(ab.GetNorm(), 2)));
	}
	assert(dist == dist);
	return dist;
}

template<class TissueDescriptorType> double DistToPerimeter(
		const itk::Point<double, TissueDescriptorType::NumDimensions> & x,
		const typename TissueDescriptorType::Pointer & descriptor) {
	std::vector<double> dists;

	typedef typename ttt::TissueDescriptorTraits<TissueDescriptorType>::SkeletonVertexType SkeletonVertexType;
	typename std::vector<SkeletonVertexType>::iterator it =
			descriptor->BeginPerimeter();
	typename std::vector<SkeletonVertexType>::iterator it2 = it + 1;
	while (it2 != descriptor->EndPerimeter()) {
		itk::Point<double, TissueDescriptorType::NumDimensions> a = boost::get(ttt::SkeletonPointPropertyTag<TissueDescriptorType::NumDimensions>(),
				descriptor->GetAJGraph(), *it).position;
		itk::Point<double, TissueDescriptorType::NumDimensions> b = boost::get(ttt::SkeletonPointPropertyTag<TissueDescriptorType::NumDimensions>(),
				descriptor->GetAJGraph(), *it2).position;

		dists.push_back(PointToSegmentDistance<TissueDescriptorType>(x, a, b));
		++it;
		++it2;
	}
	it2 = descriptor->BeginPerimeter();

	itk::Point<double, TissueDescriptorType::NumDimensions> a = boost::get(ttt::SkeletonPointPropertyTag<TissueDescriptorType::NumDimensions>(),
			descriptor->GetAJGraph(), *it2).position;
	itk::Point<double, TissueDescriptorType::NumDimensions> b = boost::get(ttt::SkeletonPointPropertyTag<TissueDescriptorType::NumDimensions>(),
			descriptor->GetAJGraph(), *it).position;

	dists.push_back(PointToSegmentDistance<TissueDescriptorType>(x, a, b));
	double dist = *(std::min_element(dists.begin(), dists.end()));
	assert(dist == dist);
	return dist;

}

template<int dim> MinCostMaxFlowTrackAssociationCommand<dim>::MinCostMaxFlowTrackAssociationCommand() {
	// TODO Auto-generated constructor stub

}

template<int dim> MinCostMaxFlowTrackAssociationCommand<dim>::~MinCostMaxFlowTrackAssociationCommand() {
	// TODO Auto-generated destructor stub
}

template<int dim> void MinCostMaxFlowTrackAssociationCommand<dim>::Do() {

	typedef unsigned long int CellVertexType;
	typedef unsigned long int SkeletonVertexType;
	typedef unsigned long int TrackedCellVertexType;
	std::map<CellVertexType, TrackedCellVertexType> obsToTrack;

	typedef CellMomentCalculator<ttt::TrackedTissueDescriptor<dim>> TrackMomentCalculator;
	typedef CellMomentCalculator<ttt::TissueDescriptor<dim>> ObsMomentCalculator;

	TrackMomentCalculator m_TrackMoments;
	ObsMomentCalculator m_ObservationMoments;

	typedef ttt::AssociationGraph<CellVertexType, TrackedCellVertexType> AssociationGraphType;
	AssociationGraphType associationGraph;

	std::map<TrackedCellVertexType, double> distances;

	m_TrackMoments.SetTissueDescriptor(this->m_PreviousTrackedTissueDescriptor);
	m_ObservationMoments.SetTissueDescriptor(this->m_ObservedTissueDescriptor);

	m_TrackMoments.Compute();
	m_ObservationMoments.Compute();

	typename TrackMomentCalculator::CentroidFeatureMapType trackCentroids = m_TrackMoments.GetCentroid();
	typename ObsMomentCalculator::CentroidFeatureMapType obsCentroids = m_ObservationMoments.GetCentroid();

	typename TrackMomentCalculator::AreaFeatureMapType trackAreas = m_TrackMoments.GetAreas();
	typename ObsMomentCalculator::AreaFeatureMapType obsAreas = m_ObservationMoments.GetAreas();

	typename TrackMomentCalculator::PerimeterFeatureMapType trackPerimeter = m_TrackMoments.GetPerimeter();
	typename ObsMomentCalculator::PerimeterFeatureMapType obsPerimeter = m_ObservationMoments.GetPerimeter();

	typename TrackMomentCalculator::XXFeatureMapType trackXX = m_TrackMoments.GetXX();
	typename ObsMomentCalculator::XXFeatureMapType obsXX = m_ObservationMoments.GetXX();

	typename TrackMomentCalculator::YYFeatureMapType trackYY = m_TrackMoments.GetYY();
	typename ObsMomentCalculator::YYFeatureMapType obsYY = m_ObservationMoments.GetYY();

	typename TrackMomentCalculator::XYFeatureMapType trackXY = m_TrackMoments.GetXY();
	typename ObsMomentCalculator::XYFeatureMapType obsXY = m_ObservationMoments.GetXY();

	obsToTrack.clear();

	int total = 0;
	BGL_FORALL_VERTICES_T(v, m_PreviousTrackedTissueDescriptor->GetCellGraph(), TrackedCellGraph<dim>){
		associationGraph.AddTrack(v);
		/*
		 * Out of scene HYPOTHESIS
		 */

		itk::Point<double, dim> trackCentroid = trackCentroids[v].GetValue();
		double costDistance = DistToPerimeter<ttt::TrackedTissueDescriptor<dim> >(trackCentroid, m_PreviousTrackedTissueDescriptor);

		double terminationCost = this->m_TerminationWeight * this->m_DistanceWeight * costDistance;

		/*
		 * Apoptosis HYPOTHESIS
		 */
		double costArea= trackAreas[v].GetValue();

		double apoptosisCost = this->m_ApoptosisWeight * this->m_AreaWeight*costArea;

		associationGraph.AddTerminationHypothesis(v, std::min(terminationCost,apoptosisCost));



	}

	BGL_FORALL_VERTICES_T(v, m_ObservedTissueDescriptor->GetCellGraph(), CellGraph<dim>){

		associationGraph.AddObservation(v);

		itk::Point<double, dim> observationCentroid = obsCentroids[v].GetValue();

		double costDistance = DistToPerimeter<ttt::TissueDescriptor<dim> >(observationCentroid, m_ObservedTissueDescriptor);
		//double costArea = fabs((m_ObservationAreas[v]-meanObsArea)/stdObsArea);
		//std::cout << "Creation " <<  v << " CostDistance: " << costDistance << std::endl;

		double creationCost = this->m_CreationWeight * this->m_DistanceWeight * costDistance;
		associationGraph.AddCreationHypothesis(v, (creationCost));
	}

	BGL_FORALL_VERTICES_T(obs, m_ObservedTissueDescriptor->GetCellGraph(), CellGraph<dim>){
		//std::cout << "ASSOCIATION " << obs << std::endl;

		ttt::Cell<dim> currentCell = boost::get(ttt::CellPropertyTag<dim>(),m_ObservedTissueDescriptor->GetCellGraph(), obs);

		itk::Point<double, dim> observationCentroid =	obsCentroids[obs].GetValue();


		/*
		 * ASSOCIATION HYPOTHESIS
		 */
		BGL_FORALL_VERTICES_T(track, m_PreviousTrackedTissueDescriptor->GetCellGraph(), TrackedCellGraph<dim> ){
			//itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,track).GetCentroid();
			itk::Point<double, dim> trackCentroid = trackCentroids[track].GetValue();

			itk::Vector<double, dim> diff = trackCentroid - observationCentroid;

			double distance = diff.GetNorm();
			//std::cout << observationCentroid << "\t-\t" << trackCentroid << "\t=\t" << diff << "\t" << distance << std::endl;
			distances[track] = distance;
		}


		for (int k = 0; k < m_K; k++) {

			TrackedCellVertexType min;
			typename std::map<TrackedCellVertexType, double>::iterator minimum = std::min_element(distances.begin(), distances.end(), CompareSecond<TrackedCellVertexType>());
			min = minimum->first;
			double costDistance = minimum->second;
			double costArea = fabs(obsAreas[obs].GetValue() - trackAreas[min].GetValue());
			double costPerimeter = fabs(obsPerimeter[obs].GetValue() - trackPerimeter[min].GetValue());
			double costXX = fabs(obsXX[obs].GetValue() - trackXX[min].GetValue());
			double costXY = fabs(obsXY[obs].GetValue() - trackXY[min].GetValue());
			double costYY = fabs(obsYY[obs].GetValue() - trackYY[min].GetValue());


			double associationCost = this->m_AssociationWeight * (this->m_DistanceWeight * costDistance + this->m_AreaWeight * costArea + this->m_PerimeterWeight * costPerimeter + this->m_XXWeight * costXX + this->m_XYWeight*costXY + this->m_YYWeight*costYY);

			//std::cout << obs << " to " << min <<  " CostDistance: " << costDistance <<  " CostArea: " << " " << costArea<<" CostPerimeter: " << " " << costPerimeter<< " CostXX: " << costXX << " CostXY: " << costXY << " CostYY: " << costYY <<  " --> " << associationCost << std::endl ;

			associationGraph.AddAssociationHypothesis(min, obs, associationCost);

			minimum->second = std::numeric_limits<double>::max();
		}
		distances.clear();

	/*
	 * MYTHOSIS HYPOTHESIS
	 */
	//std::cout << "MITOSIS" << std::endl;
	typedef typename boost::graph_traits<ttt::CellGraph<dim> >::adjacency_iterator adjacency_iterator;

	std::pair<adjacency_iterator, adjacency_iterator> neighbors =
	boost::adjacent_vertices(obs, m_ObservedTissueDescriptor->GetCellGraph());

	for (; neighbors.first != neighbors.second; ++neighbors.first) {

		ttt::Cell<dim> neighborCell = boost::get(ttt::CellPropertyTag<dim>(),
				m_ObservedTissueDescriptor->GetCellGraph(), *neighbors.first);

		itk::Point<double, dim> neighborCentroid = neighborCell.GetCentroid();

		ttt::Cell<dim> mitosisSuperCell;

		ttt::CellUnion(currentCell, neighborCell, mitosisSuperCell);

		Feature<double> mitosisArea, mitosisPerimeter, mitosisXX, mitosisXY,
		mitosisYY;
		Feature<itk::Vector<double, 3> > mitosisNormal;
		Feature<itk::Point<double, 3> > mitosisCentroid;

		m_ObservationMoments.GetCellMoments(mitosisSuperCell,
				mitosisPerimeter, mitosisArea, mitosisCentroid,
				mitosisNormal, mitosisXX, mitosisYY, mitosisXY);

		BGL_FORALL_VERTICES_T(track, m_PreviousTrackedTissueDescriptor->GetCellGraph(),
				TrackedCellGraph<dim>)
		{
			itk::Point<double, dim> trackCentroid = boost::get(
					ttt::TrackedCellPropertyTag<dim>(),
					m_PreviousTrackedTissueDescriptor->GetCellGraph(), track).GetCentroid();

			itk::Vector<double, dim> diff = trackCentroid
			- mitosisCentroid.GetValue();
			double distance = diff.GetNorm();
			distances[track] = distance;
		}
		for (int k = 0; k < m_KMitosis; k++) {

			TrackedCellVertexType min;
			std::map<TrackedCellVertexType, double>::iterator minimum =
			std::min_element(distances.begin(), distances.end(),
					CompareSecond<TrackedCellVertexType>());

			min = minimum->first;


			double costDistance = minimum->second;
			double costArea = fabs(mitosisArea.GetValue()- trackAreas[min].GetValue());
			double costPerimeter = fabs(mitosisPerimeter.GetValue() - trackPerimeter[min].GetValue());
			double costXX = fabs(mitosisXX.GetValue() - trackXX[min].GetValue());
			double costXY = fabs(mitosisXY.GetValue() - trackXY[min].GetValue());
			double costYY = fabs(mitosisYY.GetValue() - trackYY[min].GetValue());


			double mitosisCost = this->m_MitosisWeight * (this->m_DistanceWeight * costDistance + this->m_AreaWeight * costArea + this->m_PerimeterWeight * costPerimeter + this->m_XXWeight * costXX + this->m_XYWeight*costXY + this->m_YYWeight*costYY);

			associationGraph.AddMitosisHypothesis(min, obs,
					*neighbors.first, mitosisCost);
			//associationGraph.AddMitosisHypothesis(min,obs,*neighbors.first,0);
			minimum->second = std::numeric_limits<double>::max();
		}
		distances.clear();
	}

}

	//associationGraph.Print();
	vnl_sparse_matrix<double> flowMatrix;
	vnl_vector<double> flowVector, costVector, capacityVector;
	associationGraph.GetProblem(costVector, flowVector, capacityVector,
			flowMatrix);

	//std::cout << costVector<< std::endl;

	//std::cout << flowMatrix<< std::endl;
	for (unsigned int r = 0; r < flowMatrix.rows(); r++) {
		for (unsigned int c = 0; c < flowMatrix.cols(); c++) {
			//	std::cout << flowMatrix.get(r,c) << "\t";
		}
		//std::cout << "=" << flowVector.get(r) <<std::endl;
	}
	CoinSolver solver(costVector, capacityVector, flowMatrix, flowVector);
	solver.Solve();

	vnl_vector<double> solution = solver.GetSolution();

	for (int i = 0; i < flowMatrix.rows(); i++) {
		double flow = 0;
		for (int c = 0; c < flowMatrix.cols(); c++) {
			flow += flowMatrix.get(i, c) * solution(c);
		}
		assert(flow == flowVector.get(i));
	}

	std::vector<std::pair<CellVertexType, TrackedCellVertexType> > associations;
	std::vector<CellVertexType> toCreate;
	std::vector<TrackedCellVertexType> toDelete;
	std::vector<AssociationGraphType::MitosisType> mitosis;

	associationGraph.DecodeSolution(solution, associations, toCreate, toDelete,
			mitosis);

	//std::cout << "Associations: " << associations.size() << std::endl;
	//std::cout << "Mitosis: " << mitosis.size() << std::endl;
	//std::cout << "ToCreate: " << toCreate.size() << std::endl;
	//std::cout << "ToDelete: " << toDelete.size() << std::endl;
	//std::cout << "Observations: "
//			<< boost::num_vertices(*m_ObservedTissueDescriptor->m_CellGraph)
	//			<< std::endl;
	assert(
			associations.size() + 2 * mitosis.size() + toCreate.size()
					== boost::num_vertices(m_ObservedTissueDescriptor->GetCellGraph()));

	m_CurrentTrackedTissueDescriptor = ttt::TrackedTissueDescriptor<dim>::New();

	m_CurrentTrackedTissueDescriptor->SetAJGraph(m_ObservedTissueDescriptor->GetAJGraph());

	for (typename std::vector<SkeletonVertexType>::iterator it =
			m_ObservedTissueDescriptor->BeginPerimeter();
			it != m_ObservedTissueDescriptor->EndPerimeter(); ++it) {
		m_CurrentTrackedTissueDescriptor->AddVertexToPerimeter(*it);
	}

	for (std::vector<std::pair<CellVertexType, TrackedCellVertexType> >::iterator it =
			associations.begin(); it != associations.end(); it++) {

		int trackID = boost::get(ttt::TrackedCellPropertyTag<dim>(), m_PreviousTrackedTissueDescriptor->GetCellGraph(),
						it->second).GetID();

		TrackedCellVertexType n = boost::add_vertex(m_CurrentTrackedTissueDescriptor->GetCellGraph());

		boost::get(ttt::TrackedCellPropertyTag<dim>(),
				m_CurrentTrackedTissueDescriptor->GetCellGraph(), n) = boost::get(
				ttt::CellPropertyTag<dim>(),
				m_ObservedTissueDescriptor->GetCellGraph(), it->first);

		boost::get(TrackedCellPropertyTag<dim>(),
				m_CurrentTrackedTissueDescriptor->GetCellGraph(), n).SetID(
				trackID);

		boost::get(ttt::TrackedCellPropertyTag<dim>(),
				m_CurrentTrackedTissueDescriptor->GetCellGraph(), n).SetObservedCell(
				it->first);

		//std::cout << "Associate observation " << boost::get(ttt::TrackedCellPropertyTag(),*m_CurrentTrackedTissueDescriptor->m_CellGraph,n).GetObservedCell() << " to " << trackID << " track ID " << trackID << " newTrack " << n << std::endl;

		obsToTrack[it->first] = n;
	}

	for (std::vector<CellVertexType>::iterator it = toCreate.begin();
			it != toCreate.end(); ++it) {
		//std::cout << "Create " << *it << std::endl;

		TrackedCellVertexType n = boost::add_vertex(
				m_CurrentTrackedTissueDescriptor->GetCellGraph());

		boost::get(ttt::TrackedCellPropertyTag<dim>(),
				m_CurrentTrackedTissueDescriptor->GetCellGraph(), n) = boost::get(
				ttt::CellPropertyTag<dim>(),
				m_ObservedTissueDescriptor->GetCellGraph(), *it);

		boost::get(TrackedCellPropertyTag<dim>(),
				m_CurrentTrackedTissueDescriptor->GetCellGraph(), n).SetID(
				m_NextID++);

		boost::get(ttt::TrackedCellPropertyTag<dim>(),
				m_CurrentTrackedTissueDescriptor->GetCellGraph(), n).SetObservedCell(
				*it);

		obsToTrack[*it] = n;

	}
	for (std::vector<AssociationGraphType::MitosisType>::iterator it =
			mitosis.begin(); it != mitosis.end(); ++it) {
		//Create two new tracks
		{
			TrackedCellVertexType a = boost::add_vertex(
					m_CurrentTrackedTissueDescriptor->GetCellGraph());

			boost::get(ttt::TrackedCellPropertyTag<dim>(),
					m_CurrentTrackedTissueDescriptor->GetCellGraph(), a) =
					boost::get(ttt::CellPropertyTag<dim>(),
							m_ObservedTissueDescriptor->GetCellGraph(), it->A);

			boost::get(TrackedCellPropertyTag<dim>(),
					m_CurrentTrackedTissueDescriptor->GetCellGraph(), a).SetID(
					m_NextID++);
			boost::get(TrackedCellPropertyTag<dim>(),
					m_CurrentTrackedTissueDescriptor->GetCellGraph(), a).SetParentID(
					boost::get(ttt::TrackedCellPropertyTag<dim>(),
							m_PreviousTrackedTissueDescriptor->GetCellGraph(),
							it->track).GetID());

			boost::get(ttt::TrackedCellPropertyTag<dim>(),
					m_CurrentTrackedTissueDescriptor->GetCellGraph(), a).SetObservedCell(
					it->A);
			obsToTrack[it->A] = a;
		}
		{
			TrackedCellVertexType b = boost::add_vertex(
					m_CurrentTrackedTissueDescriptor->GetCellGraph());

			boost::get(ttt::TrackedCellPropertyTag<dim>(),
					m_CurrentTrackedTissueDescriptor->GetCellGraph(), b) =
					boost::get(ttt::CellPropertyTag<dim>(),
							m_ObservedTissueDescriptor->GetCellGraph(), it->B);

			boost::get(TrackedCellPropertyTag<dim>(),
					m_CurrentTrackedTissueDescriptor->GetCellGraph(), b).SetID(
					m_NextID++);

			boost::get(TrackedCellPropertyTag<dim>(),
					m_CurrentTrackedTissueDescriptor->GetCellGraph(), b).SetParentID(
					boost::get(ttt::TrackedCellPropertyTag<dim>(),
							m_PreviousTrackedTissueDescriptor->GetCellGraph(),
							it->track).GetID());

			boost::get(ttt::TrackedCellPropertyTag<dim>(),
					m_CurrentTrackedTissueDescriptor->GetCellGraph(), b).SetObservedCell(
					it->B);
			obsToTrack[it->B] = b;
		}
	}

	BGL_FORALL_EDGES_T(e, m_ObservedTissueDescriptor->GetCellGraph(), CellGraph<dim>){
	CellVertexType source = boost::source(e,m_ObservedTissueDescriptor->GetCellGraph());
	CellVertexType target = boost::target(e,m_ObservedTissueDescriptor->GetCellGraph());
	boost::add_edge(obsToTrack[source], obsToTrack[target],
			m_CurrentTrackedTissueDescriptor->GetCellGraph());
	}
}

}
/* namespace ttt */
