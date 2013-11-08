/*
 * PrimalGraphInsertionInteractor.cpp
 *
 *  Created on: Oct 1, 2013
 *      Author: morgan
 */
#include <vtkRenderWindow.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindowInteractor.h>
#include "PrimalGraphInsertionInteractor.h"

ttt::PrimalGraphInsertionInteractor::PrimalGraphInsertionInteractor() {

	m_NewVertexSphere=vtkSmartPointer<vtkSphereSource>::New();
	m_NewVertexMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
	m_NewVertexMapper->SetInputConnection(m_NewVertexSphere->GetOutputPort());

	m_NewVertexActor= vtkSmartPointer<vtkActor>::New();
	m_NewVertexActor->SetMapper(m_NewVertexMapper);

	m_NewEdgeLine=vtkSmartPointer<vtkLineSource>::New();

	m_NewEdgeMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
	m_NewEdgeMapper->SetInputConnection(m_NewEdgeLine->GetOutputPort());

	m_NewEdgeActor= vtkSmartPointer<vtkActor>::New();
	m_NewEdgeActor->SetMapper(m_NewEdgeMapper);

	m_Origin=-1;
}

void ttt::PrimalGraphInsertionInteractor::Init(){
	m_Renderer->AddActor(m_NewEdgeActor);
	m_Renderer->AddActor(m_NewVertexActor);
}
ttt::PrimalGraphInsertionInteractor::~PrimalGraphInsertionInteractor() {
	delete m_InsertionFinished;
}

void ttt::PrimalGraphInsertionInteractor::OnMouseMove(){

	double picked[3];

    vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
    picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_Renderer);
    picker->GetPickPosition(picked);

	m_NewVertexSphere->SetCenter(picked[0],picked[1],picked[2]); //TODO recover positions

	itk::Point<float,3> center=boost::get(SkeletonPointPropertyTag(),*m_TissueDescriptor->m_SkeletonGraph,m_Origin).position;
	m_NewEdgeLine->SetPoint1(center[0],center[1],center[2]);
	m_NewEdgeLine->SetPoint2(picked[0],picked[1],picked[2]);

	std::cout << center  << " " << picked[0]<< picked[1] << picked[2]<< std::endl;


	m_Renderer->GetRenderWindow()->Render();

}

void ttt::PrimalGraphInsertionInteractor::OnLeftButtonDown(){

	m_Renderer->RemoveActor(m_NewVertexActor);
	m_Renderer->RemoveActor(m_NewEdgeActor);
    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    cellPicker->SetTolerance(0.01);

    cellPicker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_Renderer);

    m_Renderer->AddActor(m_NewVertexActor);
    m_Renderer->AddActor(m_NewEdgeActor);

    vtkSmartPointer<vtkActor> pickedActor;
    pickedActor=cellPicker->GetActor();
    SkeletonVertexType newVertexDescriptor;

    std::cout << "Actor " << m_Drawer->IsVertex(pickedActor) << " " << m_Drawer->IsEdge(pickedActor) << std::endl;



    if(!pickedActor || !m_Drawer->IsVertex(pickedActor)){
    	double pickPosition[3];
        vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();
        pointPicker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_Renderer);
        pointPicker->GetPickPosition(pickPosition);

        SkeletonPoint newVertex;

        newVertex.position[0]=pickPosition[0];
        newVertex.position[1]=pickPosition[1];
        newVertex.position[2]=pickPosition[2];
        newVertexDescriptor=boost::add_vertex(newVertex,*m_TissueDescriptor->m_SkeletonGraph);

    	boost::add_edge(m_Origin,newVertexDescriptor,*m_TissueDescriptor->m_SkeletonGraph);

        m_Origin=newVertexDescriptor;
        m_OriginActor= m_NewVertexActor;

    	m_NewVertexSphere=vtkSmartPointer<vtkSphereSource>::New();
    	m_NewVertexMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    	m_NewVertexMapper->SetInputConnection(m_NewVertexSphere->GetOutputPort());

    	m_NewVertexActor= vtkSmartPointer<vtkActor>::New();
    	m_NewVertexActor->SetMapper(m_NewVertexMapper);

    	m_NewEdgeLine=vtkSmartPointer<vtkLineSource>::New();

    	m_NewEdgeMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    	m_NewEdgeMapper->SetInputConnection(m_NewEdgeLine->GetOutputPort());

    	m_NewEdgeActor= vtkSmartPointer<vtkActor>::New();
    	m_NewEdgeActor->SetMapper(m_NewEdgeMapper);

    	m_Renderer->AddActor(m_NewEdgeActor);
    	m_Renderer->AddActor(m_NewVertexActor);

    }else{
    	newVertexDescriptor=m_Drawer->GetActorSkeletonVertex(pickedActor);
        if(pickedActor!=m_OriginActor){
        	boost::add_edge(m_Origin,newVertexDescriptor,*m_TissueDescriptor->m_SkeletonGraph);
        }
        assert(m_InsertionFinished);
        m_InsertionFinished->SetCaller(this);
        m_InsertionFinished->Notify();
    }


}

void ttt::PrimalGraphInsertionInteractor::OnRightButtonUp(){

	m_Renderer->RemoveActor(m_NewEdgeActor);
	m_Renderer->RemoveActor(m_NewVertexActor);
	assert(m_InsertionFinished);
	m_InsertionFinished->SetCaller(this);
	m_InsertionFinished->Notify();
}
