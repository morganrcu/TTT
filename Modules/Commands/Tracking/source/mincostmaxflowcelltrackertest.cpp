#include <boost/graph/adjacency_list.hpp>
#include "tttDescriptionDataTypes.h"
#include "tttMinCostMaxFlowCellTracker.h"

int main(){
#if 0
	ttt::TissueDescriptor::Pointer obs0 = ttt::TissueDescriptor::New();

	ttt::Cell cellA0;
	cellA0.m_Centroid[0]=0.5;
	cellA0.m_Centroid[1]=0.5;
	cellA0.m_Centroid[2]=0;


	ttt::Cell cellB0;
	cellB0.m_Centroid[0]=-0.5;
	cellB0.m_Centroid[1]=0.5;
	cellB0.m_Centroid[2]=0;

	ttt::Cell cellC0;
	cellC0.m_Centroid[0]=-0.5;
	cellC0.m_Centroid[1]=-0.5;
	cellC0.m_Centroid[2]=0;

	ttt::Cell cellD0;
	cellD0.m_Centroid[0]=0.5;
	cellD0.m_Centroid[1]=-0.5;
	cellD0.m_Centroid[2]=0;

	ttt::CellVertexType va0 = boost::add_vertex(ttt::CellProperty(),*obs0->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,va0).m_Centroid=cellA0.m_Centroid;
	std::cout << boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,va0).m_Centroid << std::endl;

	ttt::CellVertexType vb0 = boost::add_vertex(ttt::CellProperty(),*obs0->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,vb0).m_Centroid=cellB0.m_Centroid;
	std::cout << boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,vb0).m_Centroid << std::endl;

	ttt::CellVertexType vc0 = boost::add_vertex(ttt::CellProperty(),*obs0->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,vc0).m_Centroid=cellC0.m_Centroid;
	std::cout << boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,vc0).m_Centroid << std::endl;

	ttt::CellVertexType vd0 = boost::add_vertex(ttt::CellProperty(),*obs0->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,vd0).m_Centroid=cellD0.m_Centroid;
	std::cout << boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,vd0).m_Centroid << std::endl;


	ttt::TissueDescriptor::Pointer obs1 = ttt::TissueDescriptor::New();

	ttt::Cell cellA1;
	cellA1.m_Centroid[0]=0.6;
	cellA1.m_Centroid[1]=0.6;
	cellA1.m_Centroid[2]=0;

	ttt::Cell cellB1;
	cellB1.m_Centroid[0]=-0.6;
	cellB1.m_Centroid[1]=0.6;
	cellB1.m_Centroid[2]=0;

	ttt::Cell cellC1;
	cellC1.m_Centroid[0]=-0.6;
	cellC1.m_Centroid[1]=-0.6;
	cellC1.m_Centroid[2]=0;

	ttt::Cell cellD1;
	cellD1.m_Centroid[0]=0.6;
	cellD1.m_Centroid[1]=-0.6;
	cellD1.m_Centroid[2]=0;

	ttt::CellVertexType va1 = boost::add_vertex(ttt::CellProperty(),*obs1->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,va1).m_Centroid= cellA1.m_Centroid;
	std::cout << boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,va1).m_Centroid << std::endl;

	ttt::CellVertexType vb1 = boost::add_vertex(ttt::CellProperty(),*obs1->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vb1).m_Centroid=cellB1.m_Centroid;
	std::cout << boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vb1).m_Centroid << std::endl;
#if 0
	ttt::CellVertexType vc1 = boost::add_vertex(ttt::CellProperty(),*obs1->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vc1).m_Centroid=cellC1.m_Centroid;
	std::cout << boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vc1).m_Centroid << std::endl;

	ttt::CellVertexType vd1 = boost::add_vertex(ttt::CellProperty(),*obs1->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vd1).m_Centroid=cellD1.m_Centroid;
	std::cout << boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vd1).m_Centroid << std::endl;
#endif
#endif
	ttt::MinCostMaxFlowCellTracker::Pointer tracker = ttt::MinCostMaxFlowCellTracker::New();

	ttt::TissueDescriptor::Pointer obs0 = ttt::TissueDescriptor::New();

	ttt::Cell cellA0;
	itk::Point<double,3> cellA0Centroid;
	cellA0Centroid[0]=0;
	cellA0Centroid[1]=-0.5;
	cellA0Centroid[2]=0;
	cellA0.SetCentroid(cellA0Centroid);

	ttt::Cell cellB0;
	itk::Point<double,3> cellB0Centroid;
	cellB0Centroid[0]=0;
	cellB0Centroid[1]=0.5;
	cellB0Centroid[2]=0;

	cellB0.SetCentroid(cellB0Centroid);

	ttt::CellVertexType va0 = boost::add_vertex(ttt::CellProperty(),*obs0->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,va0).SetCentroid(cellA0.GetCentroid());
	std::cout << boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,va0).GetCentroid() << std::endl;

	ttt::CellVertexType vb0 = boost::add_vertex(ttt::CellProperty(),*obs0->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,vb0).SetCentroid(cellB0.GetCentroid());
	std::cout << boost::get(ttt::CellPropertyTag(),*obs0->m_CellGraph,vb0).GetCentroid() << std::endl;

	boost::add_edge(va0,vb0,*obs0->m_CellGraph);

	ttt::TissueDescriptor::Pointer obs1 = ttt::TissueDescriptor::New();

	ttt::Cell cellA1;
	itk::Point<double,3> cellA1Centroid;

	cellA1Centroid[0]=0;
	cellA1Centroid[1]=-0.6;
	cellA1Centroid[2]=0;
	cellA1.SetCentroid(cellA1Centroid);

	ttt::Cell cellB1;
	itk::Point<double,3> cellB1Centroid;
	cellB1Centroid[0]=0.5;
	cellB1Centroid[1]=0.5;
	cellB1Centroid[2]=0;
	cellA1.SetCentroid(cellB1Centroid);

	ttt::Cell cellC1;
	itk::Point<double,3> cellC1Centroid;
	cellC1Centroid[0]=-0.5;
	cellC1Centroid[1]=0.5;
	cellC1Centroid[2]=0;

	cellC1.SetCentroid(cellC1Centroid);



	ttt::CellVertexType va1 = boost::add_vertex(ttt::CellProperty(),*obs1->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,va1).SetCentroid(cellA1.GetCentroid());
	std::cout << boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,va1).GetCentroid() << std::endl;

	ttt::CellVertexType vb1 = boost::add_vertex(ttt::CellProperty(),*obs1->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vb1).SetCentroid(cellB1.GetCentroid());
	std::cout << boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vb1).GetCentroid() << std::endl;

	ttt::CellVertexType vc1 = boost::add_vertex(ttt::CellProperty(),*obs1->m_CellGraph);
	boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vc1).SetCentroid(cellC1.GetCentroid());
	std::cout << boost::get(ttt::CellPropertyTag(),*obs1->m_CellGraph,vc1).GetCentroid() << std::endl;

	boost::add_edge(va1,vb1,*obs1->m_CellGraph);
	boost::add_edge(va1,vc1,*obs1->m_CellGraph);
	boost::add_edge(vb1,vc1,*obs1->m_CellGraph);



	std::vector<ttt::TissueDescriptor::Pointer > observations;
	observations.push_back(obs0);
	observations.push_back(obs1);
	tracker->SetObservations(observations);
	tracker->Track();

	std::vector<ttt::TrackedTissueDescriptor::Pointer > tracks = tracker->GetTracks();

	for(int t=0;t<tracks.size();t++){
		std::cout << "T: "<< t << std::endl;
		BGL_FORALL_VERTICES_T(v,*tracks[t]->m_CellGraph,ttt::TrackedCellGraph){
			std::cout <<boost::get(ttt::TrackedCellPropertyTag(),*tracks[t]->m_CellGraph,v).GetID() << " "  <<boost::get(ttt::TrackedCellPropertyTag(),*tracks[t]->m_CellGraph,v).GetCentroid() << " " << boost::get(ttt::TrackedCellPropertyTag(),*tracks[t]->m_CellGraph,v).GetParentID() <<  std::endl;
		}
	}
}
