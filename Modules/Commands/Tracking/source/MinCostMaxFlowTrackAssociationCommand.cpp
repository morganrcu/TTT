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

struct CompareSecond {
	bool operator()(const std::pair<TrackedCellVertexType, double>& left,
			const std::pair<TrackedCellVertexType, double>& right) const {
		return left.second < right.second;
	}
};
static double PointToSegmentDistance(const itk::Point<double, 3> & x,
		const itk::Point<double, 3> & a, const itk::Point<double, 3> & b) {

	itk::Vector<double, 3> ax = x - a;
	double normAX = ax.GetNorm();
	if (normAX == 0)
		return 0;
	itk::Vector<double, 3> ab = b - a;
	double r = ab * ax;

	r /= ax.GetNorm();

	double dist;
	if (r < 0) {
		dist = ax.GetNorm();
	} else if (r > 1) {
		itk::Vector<double, 3> xb = b - x;
		dist = xb.GetNorm();
	} else {
		dist = sqrt(fabs(pow(ax.GetNorm(), 2) - r * pow(ab.GetNorm(), 2)));
	}
	assert(dist == dist);
	return dist;
}

template<class TissueDescriptorType> double DistToPerimeter(
		const itk::Point<double, 3> & x,
		const typename TissueDescriptorType::Pointer & descriptor) {
	std::vector<double> dists;
	std::vector<ttt::SkeletonVertexType>::iterator it =
			descriptor->BeginPerimeter();
	std::vector<ttt::SkeletonVertexType>::iterator it2 = it + 1;
	while (it2 != descriptor->EndPerimeter()) {
		itk::Point<double, 3> a = boost::get(ttt::SkeletonPointPropertyTag(),
				*descriptor->m_SkeletonGraph, *it).position;
		itk::Point<double, 3> b = boost::get(ttt::SkeletonPointPropertyTag(),
				*descriptor->m_SkeletonGraph, *it2).position;

		dists.push_back(PointToSegmentDistance(x, a, b));
		++it;
		++it2;
	}
	it2 = descriptor->BeginPerimeter();

	itk::Point<double, 3> a = boost::get(ttt::SkeletonPointPropertyTag(),
			*descriptor->m_SkeletonGraph, *it2).position;
	itk::Point<double, 3> b = boost::get(ttt::SkeletonPointPropertyTag(),
			*descriptor->m_SkeletonGraph, *it).position;

	dists.push_back(PointToSegmentDistance(x, a, b));
	double dist = *(std::min_element(dists.begin(), dists.end()));
	assert(dist == dist);
	return dist;

}

MinCostMaxFlowTrackAssociationCommand::MinCostMaxFlowTrackAssociationCommand() {
	// TODO Auto-generated constructor stub

}

MinCostMaxFlowTrackAssociationCommand::~MinCostMaxFlowTrackAssociationCommand() {
	// TODO Auto-generated destructor stub
}

void MinCostMaxFlowTrackAssociationCommand::Do(){

	std::map<CellVertexType, TrackedCellVertexType> obsToTrack;

	typedef CellMomentCalculator<ttt::TrackedTissueDescriptor> TrackMomentCalculator;
	typedef CellMomentCalculator<ttt::TissueDescriptor> ObsMomentCalculator;

	TrackMomentCalculator m_TrackMoments;
	ObsMomentCalculator m_ObservationMoments;

	typedef ttt::AssociationGraph<CellVertexType, TrackedCellVertexType> AssociationGraphType;
	AssociationGraphType associationGraph;

	std::map<ttt::TrackedCellVertexType, double> distances;

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

	typename TrackMomentCalculator::XYFeatureMapType trackXY =m_TrackMoments.GetXY();

	typename ObsMomentCalculator::XYFeatureMapType obsXY = m_ObservationMoments.GetXY();

	obsToTrack.clear();


	//double meanObsArea=m_ObservationAreas.GetMean();
	//double stdObsArea=m_ObservationAreas.GetStd();
	//if(stdObsArea==0) stdObsArea=1;

	//double meanTrackArea=m_TrackAreas.GetMean();
	//	double stdTrackArea=m_TrackAreas.GetStd();
	//if(stdTrackArea==0) stdTrackArea=1;

	//double sx=0,sy=0,sz=0,sx2=0,sy2=0,sz2=0,mux,muy,muz,stdx,stdy,stdz;

	int total = 0;
	BGL_FORALL_VERTICES(v, *m_PreviousTrackedTissueDescriptor->m_CellGraph, TrackedCellGraph)
	{
		associationGraph.AddTrack(v);
		/*
		 * Destruction HYPOTHESIS
		 */

		//itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,v).GetCentroid();
		itk::Point<double, 3> trackCentroid = trackCentroids[v].GetValue();
		double costDistance = DistToPerimeter<ttt::TrackedTissueDescriptor>(
				trackCentroid, m_PreviousTrackedTissueDescriptor);
//			double costArea = fabs((m_TrackAreas[v]-meanTrackArea)/stdTrackArea);
		//double costArea = fabs((m_TrackAreas[v]-meanTrackArea)/stdTrackArea);

		//std::cout << "Termination " << v << " CostArea: " << " " << costArea<< " CostDistance: " << costDistance << std::endl;

		double terminationCost = this->m_TerminationWeight * this->m_DistanceWeight * costDistance;

		associationGraph.AddTerminationHypothesis(v, terminationCost);

	}

	BGL_FORALL_VERTICES(v, *m_ObservedTissueDescriptor->m_CellGraph, CellGraph)
	{

		associationGraph.AddObservation(v);

		itk::Point<double, 3> observationCentroid = obsCentroids[v].GetValue();

		double costDistance = DistToPerimeter<ttt::TissueDescriptor>(
				observationCentroid, m_ObservedTissueDescriptor);
		//double costArea = fabs((m_ObservationAreas[v]-meanObsArea)/stdObsArea);
		//std::cout << "Creation " <<  v << " CostArea: " << " " << costArea<< " CostDistance: " << costDistance << std::endl;

		double creationCost = this->m_CreationWeight * this->m_DistanceWeight
				* costDistance;
		associationGraph.AddCreationHypothesis(v, (creationCost));
	}

	BGL_FORALL_VERTICES(obs, *m_ObservedTissueDescriptor->m_CellGraph, CellGraph)
	{
		//std::cout << "ASSOCIATION " << obs << std::endl;

		ttt::Cell currentCell = boost::get(ttt::CellPropertyTag(),
				*m_ObservedTissueDescriptor->m_CellGraph, obs);

		itk::Point<double, 3> observationCentroid =
				obsCentroids[obs].GetValue();
		//std::cout << "Association" << std::endl;

		/*
		 * ASSOCIATION HYPOTHESIS
		 */
		BGL_FORALL_VERTICES(track, *m_PreviousTrackedTissueDescriptor->m_CellGraph,
				TrackedCellGraph)
		{
			//itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,track).GetCentroid();
			itk::Point<double, 3> trackCentroid =
					trackCentroids[track].GetValue();

			itk::Vector<double, 3> diff = trackCentroid - observationCentroid;

			double distance = diff.GetNorm();
			//std::cout << observationCentroid << "\t-\t" << trackCentroid << "\t=\t" << diff << "\t" << distance << std::endl;
			distances[track] = distance;
		}

		for (int k = 0; k < m_K; k++) {

			TrackedCellVertexType min;
			std::map<ttt::TrackedCellVertexType, double>::iterator minimum =
					std::min_element(distances.begin(), distances.end(),
							CompareSecond());
			min = minimum->first;
			double costDistance = minimum->second;

			double costArea = fabs(obsAreas[min].GetValue() - trackAreas[min].GetValue());

			double costEllipse = 0;

			double costPerimeter = fabs(trackPerimeter[min].GetValue()- obsPerimeter[obs].GetValue());
			//std::cout << "CostArea: " << " " << costArea<< " CostDistance: " << costDistance <<  " CostEllipse: " << costEllipse <<   std::endl;

			double associationCost = this->m_AssociationWeight
					* (this->m_DistanceWeight * costDistance
							+ this->m_AreaWeight * costArea
							+ this->m_EllipseWeight * costEllipse
							+ this->m_PerimeterWeight * costPerimeter);

			associationGraph.AddAssociationHypothesis(min, obs,	associationCost);

			minimum->second = std::numeric_limits<double>::max();
		}
		distances.clear();

		/*
		 * MYTHOSIS HYPOTHESIS
		 */
		//std::cout << "MITOSIS" << std::endl;
		typedef boost::graph_traits<ttt::CellGraph>::adjacency_iterator adjacency_iterator;

		std::pair<adjacency_iterator, adjacency_iterator> neighbors =
				boost::adjacent_vertices(obs, *m_ObservedTissueDescriptor->m_CellGraph);

		for (; neighbors.first != neighbors.second; ++neighbors.first) {

			ttt::Cell neighborCell = boost::get(ttt::CellPropertyTag(),
					*m_ObservedTissueDescriptor->m_CellGraph, *neighbors.first);

			itk::Point<double, 3> neighborCentroid = neighborCell.GetCentroid();

			ttt::Cell mitosisSuperCell;

			ttt::CellUnion(currentCell, neighborCell, mitosisSuperCell);

			Feature<double> mitosisArea, mitosisPerimeter, mitosisXX, mitosisXY,
					mitosisYY;
			Feature<itk::Vector<double, 3> > mitosisNormal;
			Feature<itk::Point<double, 3> > mitosisCentroid;

			m_ObservationMoments.GetCellMoments(mitosisSuperCell,
					mitosisPerimeter, mitosisArea, mitosisCentroid,
					mitosisNormal, mitosisXX, mitosisYY, mitosisXY);

			BGL_FORALL_VERTICES(track, *m_PreviousTrackedTissueDescriptor->m_CellGraph,
					TrackedCellGraph)
			{
				itk::Point<double, 3> trackCentroid = boost::get(
						ttt::TrackedCellPropertyTag(),
						*m_PreviousTrackedTissueDescriptor->m_CellGraph, track).GetCentroid();

				itk::Vector<double, 3> diff = trackCentroid
						- mitosisCentroid.GetValue();
				double distance = diff.GetNorm();
				distances[track] = distance;
			}
			for (int k = 0; k < m_K; k++) {

				TrackedCellVertexType min;
				std::map<ttt::TrackedCellVertexType, double>::iterator minimum =
						std::min_element(distances.begin(), distances.end(),
								CompareSecond());

				min = minimum->first;

//					double costArea= fabs((m_TrackAreas[min] -meanTrackArea)/stdTrackArea - (m_ObservationAreas[obs]+m_ObservationAreas[*neighbors.first]-2*meanObsArea)/stdObsArea);
				double costArea = fabs(
						trackAreas[min].GetValue() - mitosisArea.GetValue());

				double costDistance = distances[min];

				//vnl_matrix_fixed<double,2,2> trackEllipseMatrix;
				//parametricEllipseToMatrixEllipse(m_TrackEllipses[min].m_Xrad,m_TrackEllipses[min].m_Yrad,m_TrackEllipses[min].m_Theta,trackEllipseMatrix);

				//double costEllipse = (trackEllipseMatrix - mitosisEllipseMatrix).frobenius_norm();

				double costPerimeter =
						(trackPerimeter[min] - mitosisPerimeter).GetValue();

				//double costAspectRatio = fabs(m_TrackEllipses[min].GetAspectRatio()-mitosisEllipse.GetAspectRatio());
				//double costOrientation = fabs(m_TrackEllipses[min].m_Theta - mitosisEllipse.m_Theta);

				//std::cout << "CostArea: " << " " << costArea<< " CostDistance: " << costDistance <<  " CostEllipse: " << costEllipse <<  std::endl;
				double costEllipse = 0;
				double mitosisCost = this->m_MitosisWeight
						* (this->m_DistanceWeight * costDistance
								+ this->m_AreaWeight * costArea
								+ this->m_EllipseWeight * costEllipse
								+ this->m_PerimeterWeight * costPerimeter);
				associationGraph.AddMitosisHypothesis(min, obs,
						*neighbors.first, mitosisCost);
				//associationGraph.AddMitosisHypothesis(min,obs,*neighbors.first,0);
				minimum->second = std::numeric_limits<double>::max();
			}
			distances.clear();
		}

	}
	associationGraph.Print();
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
	std::cout << "Mitosis: " << mitosis.size() << std::endl;
	//std::cout << "ToCreate: " << toCreate.size() << std::endl;
	//std::cout << "ToDelete: " << toDelete.size() << std::endl;
	//std::cout << "Observations: " << boost::num_vertices(*m_Observations[t]->m_CellGraph) << std::endl;
	assert(
			associations.size() + 2 * mitosis.size() + toCreate.size()
					== boost::num_vertices(*m_ObservedTissueDescriptor->m_CellGraph));

	m_CurrentTrackedTissueDescriptor = ttt::TrackedTissueDescriptor::New();

	m_CurrentTrackedTissueDescriptor->m_SkeletonGraph = m_ObservedTissueDescriptor->m_SkeletonGraph;

	for (std::vector<ttt::SkeletonVertexType>::iterator it =
			m_ObservedTissueDescriptor->BeginPerimeter();
			it !=m_ObservedTissueDescriptor->EndPerimeter(); ++it) {
		m_CurrentTrackedTissueDescriptor->AddVertexToPerimeter(*it);
	}

	m_CurrentTrackedTissueDescriptor->m_CellGraph = boost::shared_ptr<ttt::TrackedCellGraph>(
			new ttt::TrackedCellGraph);

	for (std::vector<std::pair<CellVertexType, TrackedCellVertexType> >::iterator it =
			associations.begin(); it != associations.end(); it++) {

		int trackID = boost::get(ttt::TrackedCellPropertyTag(),
				*m_PreviousTrackedTissueDescriptor->m_CellGraph, it->second).GetID();

		TrackedCellVertexType n = boost::add_vertex(*m_CurrentTrackedTissueDescriptor->m_CellGraph);

		boost::get(ttt::TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, n) =
				boost::get(ttt::CellPropertyTag(),
						*m_ObservedTissueDescriptor->m_CellGraph, it->first);

		boost::get(TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, n).SetID(
				trackID);

		boost::get(ttt::TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, n).SetObservedCell(
				it->first);



		//std::cout << "Associate observation " << boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ObservedCell << " to " << trackID << " track ID " << trackID << " newTrack " << n << std::endl;

		obsToTrack[it->first] = n;
	}

	for (std::vector<CellVertexType>::iterator it = toCreate.begin();
			it != toCreate.end(); ++it) {
		//std::cout << "Create " << *it << std::endl;

		TrackedCellVertexType n = boost::add_vertex(*m_CurrentTrackedTissueDescriptor->m_CellGraph);

		boost::get(ttt::TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, n) =
				boost::get(ttt::CellPropertyTag(),
						*m_ObservedTissueDescriptor->m_CellGraph, *it);

		boost::get(TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, n).SetID(
				m_NextID++);

		boost::get(ttt::TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, n).SetObservedCell(
				*it);

		obsToTrack[*it] = n;

	}
	for (std::vector<AssociationGraphType::MitosisType>::iterator it =
			mitosis.begin(); it != mitosis.end(); ++it) {
		//Create two new tracks
		{
			TrackedCellVertexType a = boost::add_vertex(
					*m_CurrentTrackedTissueDescriptor->m_CellGraph);

			boost::get(ttt::TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph,
					a) = boost::get(ttt::CellPropertyTag(),
					*m_ObservedTissueDescriptor->m_CellGraph, it->A);

			boost::get(TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, a).SetID(
					m_NextID++);
			boost::get(TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, a).SetParentID(
					boost::get(ttt::TrackedCellPropertyTag(),
							*m_PreviousTrackedTissueDescriptor->m_CellGraph, it->track).GetID());

			boost::get(ttt::TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph,
					a).SetObservedCell(it->A);
			obsToTrack[it->A] = a;
		}
		{
			TrackedCellVertexType b = boost::add_vertex(
					*m_CurrentTrackedTissueDescriptor->m_CellGraph);

			boost::get(ttt::TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph,
					b) = boost::get(ttt::CellPropertyTag(),
					*m_ObservedTissueDescriptor->m_CellGraph, it->B);

			boost::get(TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, b).SetID(
					m_NextID++);

			boost::get(TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph, b).SetParentID(
					boost::get(ttt::TrackedCellPropertyTag(),
							*m_PreviousTrackedTissueDescriptor->m_CellGraph, it->track).GetID());

			boost::get(ttt::TrackedCellPropertyTag(), *m_CurrentTrackedTissueDescriptor->m_CellGraph,
					b).SetObservedCell(it->B);
			obsToTrack[it->B] = b;
		}
	}

	BGL_FORALL_EDGES(e, *m_ObservedTissueDescriptor->m_CellGraph, CellGraph){
		CellVertexType source = boost::source(e,
				*m_ObservedTissueDescriptor->m_CellGraph);
		CellVertexType target = boost::target(e,
				*m_ObservedTissueDescriptor->m_CellGraph);
		boost::add_edge(obsToTrack[source], obsToTrack[target],
				*m_CurrentTrackedTissueDescriptor->m_CellGraph);
	}
}

} /* namespace ttt */
