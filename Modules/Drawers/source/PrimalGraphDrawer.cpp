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
#include "FeatureColorer.h"
#include "AdherensJunctionEdgeOrientation.h"
void ttt::PrimalGraphDrawer::Reset(){

	for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
		m_Renderer->RemoveActor(it->first);
	}
	for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
		m_Renderer->RemoveActor(it->first);
	}

	m_Actor2SkeletonVertex.clear();
	m_Actor2SkeletonEdge.clear();
}
void ttt::PrimalGraphDrawer::Draw() {

	this->Reset();

	BGL_FORALL_VERTICES(v,*m_Descriptor->m_SkeletonGraph,SkeletonGraph){
		this->DrawVertex(v);
	}

	BGL_FORALL_EDGES(e,*m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
		this->DrawEdge(e);
	}
}
#if 0
void ttt::PrimalGraphDrawer::DrawEdge(const ttt::SkeletonEdgeType & edge){

	typedef itk::Point<double,3> itkpt;
	ttt::SkeletonVertexType src = boost::source(edge,*m_Descriptor->m_SkeletonGraph);
	ttt::SkeletonVertexType tgt = boost::target(edge,*m_Descriptor->m_SkeletonGraph);

	vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
	itkpt a= boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,boost::source(edge,*m_Descriptor->m_SkeletonGraph)).position;

	newLine->SetPoint1(a[0],a[1],a[2]);
	itkpt b= boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,boost::target(edge,*m_Descriptor->m_SkeletonGraph)).position;

	newLine->SetPoint2(b[0],b[1],b[2]);
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->ImmediateModeRenderingOn();
	mapper->SetInputConnection(newLine->GetOutputPort());
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	double  color[3];
	m_pEdgeColorer->GetObjectColor(edge,color);
	actor->GetProperty()->SetColor(color[0],color[1],color[2]);


	actor->GetProperty()->SetLineWidth(5);
	actor->VisibilityOff();
	boost::tuple<ttt::SkeletonVertexType,ttt::SkeletonVertexType> pts (src,tgt);

	m_Actor2SkeletonEdge[actor]=edge;

	boost::tuple<ttt::SkeletonVertexType,ttt::SkeletonVertexType> ptt (tgt,src);

	m_Renderer->AddActor(actor);
}
ttt::PrimalGraphDrawer::VertexSphereMapperAndActor ttt::PrimalGraphDrawer::DrawVertex(const ttt::SkeletonVertexType & vertex){

	itk::Point<double, 3> a =boost::get(SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,vertex).position;


	vtkSmartPointer<vtkSphereSource> newSphere = vtkSmartPointer<vtkSphereSource>::New();

	newSphere->SetCenter(a[0],a[1],a[2]);

	newSphere->SetRadius(0.15);

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//sphereMapper->ImmediateModeRenderingOn();
	sphereMapper->SetInputConnection(newSphere->GetOutputPort());
	//sphereMapper->ImmediateModeRenderingOn();
	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	double  color[3];
	m_pVertexColorer->GetObjectColor(vertex,color);
	sphereActor->GetProperty()->SetColor(color[0],color[1],color[2]);

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
	sphereActor->VisibilityOff();
	m_Actor2SkeletonVertex[sphereActor] = vertex;
	m_Renderer->AddActor(sphereActor);

	return VertexSphereMapperAndActor(vertex,newSphere,sphereMapper,sphereActor);
}
#endif
void ttt::PrimalGraphDrawer::Show(){
	for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
		it->first->VisibilityOn();
	}
	for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
		it->first->VisibilityOn();
	}
}

void ttt::PrimalGraphDrawer::Hide(){
	for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
		it->first->VisibilityOff();
	}
	for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
		it->first->VisibilityOff();
	}
}
