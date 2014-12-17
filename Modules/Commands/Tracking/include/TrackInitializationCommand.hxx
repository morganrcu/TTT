/*
 * TrackInitializationCommand.cpp
 *
 *  Created on: May 20, 2014
 *      Author: morgan
 */

#include "TrackInitializationCommand.h"
#include <boost/graph/iteration_macros.hpp>

template<int dim> void ttt::TrackInitializationCommand<dim>::Do(){
	typedef long unsigned int CellVertexType;
	typedef long unsigned int SkeletonVertexType;
	typedef long unsigned int TrackedCellVertexType;
	std::map<CellVertexType, TrackedCellVertexType> obsToTrack;

	m_TrackedTissueDescriptor=ttt::TrackedTissueDescriptor<dim>::New();

	m_TrackedTissueDescriptor->SetAJGraph(m_TissueDescriptor->GetAJGraph());

	BGL_FORALL_VERTICES_T(v,m_TissueDescriptor->GetCellGraph(),CellGraph<dim>){

		TrackedCellVertexType n= boost::add_vertex(m_TrackedTissueDescriptor->GetCellGraph());

		boost::get(TrackedCellPropertyTag<dim>(),m_TrackedTissueDescriptor->GetCellGraph(),n)=
				boost::get(CellPropertyTag<dim>(),m_TissueDescriptor->GetCellGraph(),v);

		boost::get(TrackedCellPropertyTag<dim>(),m_TrackedTissueDescriptor->GetCellGraph(),n).SetID(m_NextID++);

		boost::get(ttt::TrackedCellPropertyTag<dim>(),m_TrackedTissueDescriptor->GetCellGraph(),n).SetObservedCell(v);

		obsToTrack[v]=n;
	}

	BGL_FORALL_EDGES_T(e,m_TissueDescriptor->GetCellGraph(),CellGraph<dim>){
		CellVertexType source = boost::source(e,m_TissueDescriptor->GetCellGraph());
		CellVertexType target = boost::target(e,m_TissueDescriptor->GetCellGraph());
		boost::add_edge(obsToTrack[source],obsToTrack[target],m_TrackedTissueDescriptor->GetCellGraph());
	}

	for (std::vector<SkeletonVertexType>::iterator it = m_TissueDescriptor->BeginPerimeter(); it != m_TissueDescriptor->EndPerimeter(); ++it) {
		m_TrackedTissueDescriptor->AddVertexToPerimeter(obsToTrack[*it]);
	}
};
