/*
 * PrimalGraphDrawer.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: morgan
 */
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>
#include "PrimalGraphDrawer.h"

void ttt::PrimalGraphDrawer::Draw() {

	typedef itk::Point<double, 3> itkpt;

	std::cout << "size of SG " 	<< boost::num_vertices(*m_Descriptor->m_SkeletonGraph) << std::endl;
	std::cout << "size of SG edges" << boost::num_edges(*m_Descriptor->m_SkeletonGraph) << std::endl;

	for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
		m_Renderer->RemoveActor(it->first);
	}
	for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
		m_Renderer->RemoveActor(it->first);
	}

	m_Actor2SkeletonVertex.clear();
	m_Actor2SkeletonEdge.clear();

//	m_Actor2SkeletonVertex.clear();
//	m_SkeletonEdge2Actor.clear();

	BGL_FORALL_VERTICES(v,*m_Descriptor->m_SkeletonGraph,SkeletonGraph){
		itkpt a= boost::get(SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,v).position;
//		itkpt color = boost::get(ttt::SkeletonPointPropertyTag(),m_Descriptor->m_SkeletonGraph,v).color;
		vtkSmartPointer<vtkSphereSource> newSphere = vtkSmartPointer<vtkSphereSource>::New();
		//newSphere->SetCenter(m_Spacing[0]*a[0],m_Spacing[1]*a[1],m_Spacing[2]*a[2]);
		newSphere->SetCenter(a[0],a[1],a[2]);
		//std::cout << v << " " << s[0]*a[0] << " " << s[1]*a[1] << " " << s[2]*a[2] << std::endl;
		newSphere->SetRadius(0.15);

		vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		sphereMapper->ImmediateModeRenderingOn();
		sphereMapper->SetInputConnection(newSphere->GetOutputPort());
		sphereMapper->ImmediateModeRenderingOn();
		vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
		//std::cout << "primal " << v << " " << color[0] << " " << color[1] << " " << color[2] << std::endl;
		sphereActor->GetProperty()->SetColor(1,1,1);
		/*if(boost::degree(v,descriptor->m_SkeletonGraph) == 1){
	 	 	 sphereActor->GetProperty()->SetColor(1,0,0);
	 	 }
	 	if(boost::degree(v,descriptor->m_SkeletonGraph) == 2){
	 		sphereActor->GetProperty()->SetColor(0,1,0);
	 	 }
	 	 if(boost::degree(v,descriptor->m_SkeletonGraph) == 3){
	 	 	 sphereActor->GetProperty()->SetColor(0,0,1);
	 	 }
	 	 if(boost::degree(v,descriptor->m_SkeletonGraph) > 3){
	 	 	 sphereActor->GetProperty()->SetColor(0,0,0);
	 	 }*/

		sphereActor->SetMapper(sphereMapper);

		m_Actor2SkeletonVertex[sphereActor] = v;
//		m_SkeletonVertex2Actor[v] = sphereActor;


		// giaaTovtkVertexToPrimal_ls[v] = sphereActor;
		m_Renderer->AddActor(sphereActor);
	}

	//this->CellBoundaryReduction();

	BGL_FORALL_EDGES(e,*m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){

		typedef itk::Point<double,3> itkpt;
		ttt::SkeletonVertexType src = boost::source(e,*m_Descriptor->m_SkeletonGraph);
		ttt::SkeletonVertexType tgt = boost::target(e,*m_Descriptor->m_SkeletonGraph);
		//std::cout<< "In edges " << src << " " << tgt << std::endl;
		vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
		itkpt a= boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,boost::source(e,*m_Descriptor->m_SkeletonGraph)).position;
		//std::cout<< a[0] << " " << a[1] << " " << a[2] << std::endl;
		//IndexType a=m_SkeletonGraph[].position;
		//newLine->SetPoint1(a[0]*m_Spacing[0],a[1]*m_Spacing[1],a[2]*m_Spacing[2]);
		newLine->SetPoint1(a[0],a[1],a[2]);
		itkpt b= boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,boost::target(e,*m_Descriptor->m_SkeletonGraph)).position;
		//IndexType b=m_SkeletonGraph[boost::target(e,)].position;
		//newLine->SetPoint2(b[0]*m_Spacing[0],b[1]*m_Spacing[1],b[2]*m_Spacing[2]);
		newLine->SetPoint2(b[0],b[1],b[2]);
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->ImmediateModeRenderingOn();
		mapper->SetInputConnection(newLine->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->GetProperty()->SetLineWidth(5);
		boost::tuple<ttt::SkeletonVertexType,ttt::SkeletonVertexType> pts (src,tgt);

		m_Actor2SkeletonEdge[actor]=e;
//		m_SkeletonEdge2Actor[pts] = actor;

		//std::cout<< src << " " << tgt << " "<< actor <<std::endl;
		boost::tuple<ttt::SkeletonVertexType,ttt::SkeletonVertexType> ptt (tgt,src);

//		m_SkeletonEdge2Actor[ptt] = actor;

		m_Renderer->AddActor(actor);
	}
}

