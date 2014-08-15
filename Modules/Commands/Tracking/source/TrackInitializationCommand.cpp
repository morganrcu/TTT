/*
 * TrackInitializationCommand.cpp
 *
 *  Created on: May 20, 2014
 *      Author: morgan
 */

#include "TrackInitializationCommand.h"
#include <boost/graph/iteration_macros.hpp>

void ttt::TrackInitializationCommand::Do(){
	std::map<CellVertexType, TrackedCellVertexType> obsToTrack;

	m_TrackedTissueDescriptor=ttt::TrackedTissueDescriptor::New();

	m_TrackedTissueDescriptor->m_SkeletonGraph = m_TissueDescriptor->m_SkeletonGraph;

	m_TrackedTissueDescriptor->m_CellGraph = boost::shared_ptr<ttt::TrackedCellGraph>(
				new ttt::TrackedCellGraph);

	BGL_FORALL_VERTICES(v,*m_TissueDescriptor->m_CellGraph,CellGraph){

		TrackedCellVertexType n= boost::add_vertex(*m_TrackedTissueDescriptor->m_CellGraph);

		boost::get(TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,n)=
				boost::get(CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v);

		boost::get(TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,n).SetID(m_NextID++);

		boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,n).SetObservedCell(v);

		obsToTrack[v]=n;
	}

	BGL_FORALL_EDGES(e,*m_TissueDescriptor->m_CellGraph,CellGraph){
		CellVertexType source = boost::source(e,*m_TissueDescriptor->m_CellGraph);
		CellVertexType target = boost::target(e,*m_TissueDescriptor->m_CellGraph);
		boost::add_edge(obsToTrack[source],obsToTrack[target],*m_TrackedTissueDescriptor->m_CellGraph);
	}

	for (std::vector<ttt::SkeletonVertexType>::iterator it = m_TissueDescriptor->BeginPerimeter(); it != m_TissueDescriptor->EndPerimeter(); ++it) {
		m_TrackedTissueDescriptor->AddVertexToPerimeter(obsToTrack[*it]);
	}
};
