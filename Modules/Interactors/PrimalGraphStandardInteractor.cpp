/*
 * PrimalGraphStandardInteractor.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: morgan
 */
#include <vtkProperty.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include "PrimalGraphStandardInteractor.h"

PrimalGraphStandardInteractor::PrimalGraphStandardInteractor() {
	m_SelectionCleared=0;
	m_SomethingSelected=0;
	m_SelectedEdge=0;
	m_SelectedVertex=0;
}

PrimalGraphStandardInteractor::~PrimalGraphStandardInteractor() {
	assert(m_SelectionCleared);
	delete m_SelectionCleared;
	assert(m_SomethingSelected);
	delete m_SomethingSelected;
}

void PrimalGraphStandardInteractor::OnRightButtonUp(){
    vtkInteractorStyle::OnRightButtonUp();

    if(m_SelectedVertex){
    	UnhighlightActor(m_SelectedVertex);
    	m_SelectedVertex=0;
    }
    if(m_SelectedEdge){
    	UnhighlightActor(m_SelectedEdge);
    	m_SelectedEdge=0;
    }
    assert(m_SelectedVertex == 0 && m_SelectedEdge==0);

    assert(m_SelectionCleared);
    m_SelectionCleared->Notify();
}


void PrimalGraphStandardInteractor::UnhighlightActor(const vtkSmartPointer<vtkActor> & actor){
	actor->GetProperty()->SetColor(1,1,1);
}
void PrimalGraphStandardInteractor::HighlightActor(const vtkSmartPointer<vtkActor> & actor){
	actor->GetProperty()->SetColor(1,0,0);
}

void PrimalGraphStandardInteractor::OnLeftButtonDown(){

    vtkInteractorStyle::OnLeftButtonDown();

    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();

    picker->SetTolerance(0.01);
    bool somethingPicked = picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_Renderer);

    if(!somethingPicked) return;


    vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();
    if(!pickedActor) return;
    assert(pickedActor);

    HighlightActor(pickedActor);

    if(m_Drawer->IsVertex(pickedActor)){
    	//pickedActor is a Vertex
    	if(m_SelectedVertex){
    		UnhighlightActor(m_SelectedVertex);
    	}
    	m_SelectedVertex=pickedActor;

    	if(m_SelectedEdge){
    		assert(m_SelectedEdge);
    		UnhighlightActor(m_SelectedEdge);
    		m_SelectedEdge=0;
    	}

    }else if(m_Drawer->IsEdge(pickedActor)){

    	//pickedActor is an Edge

    	if(m_SelectedEdge){
    		UnhighlightActor(m_SelectedEdge);
    	}
    	m_SelectedEdge=pickedActor;

    	if(m_SelectedVertex){
    		UnhighlightActor(m_SelectedVertex);
    		m_SelectedVertex=0;
    	}
    }else{
    	return;
    }
    m_Renderer->GetRenderWindow()->Render();
    assert (m_SelectedVertex!=0 || m_SelectedEdge!=0);

    assert(m_SomethingSelected);
    m_SomethingSelected->SetCaller(this);
    m_SomethingSelected->Notify();
    //TODO NOTIFY SELECTION FINISHED WITH NOTHING SELECTED -> Update controls

}
