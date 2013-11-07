/*
 * PrimalGraphMoveInteractor.cpp
 *
 *  Created on: Oct 1, 2013
 *      Author: morgan
 */

#include "PrimalGraphMoveInteractor.h"
#include <vtkRenderWindow.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindowInteractor.h>
PrimalGraphMoveInteractor::PrimalGraphMoveInteractor() {

}

PrimalGraphMoveInteractor::~PrimalGraphMoveInteractor() {

}


void PrimalGraphMoveInteractor::OnMouseMove(){
	double picked[3];

    vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
    picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_Renderer);
    picker->GetPickPosition(picked);

    itk::Point<double,3> center=boost::get(giaa::SkeletonPointPropertyTag(),*m_TissueDescriptor->m_SkeletonGraph,m_Target).position;

    m_TargetActor->SetPosition(picked);

    center[0]=picked[0];
    center[1]=picked[1];

    boost::get(giaa::SkeletonPointPropertyTag(),*m_TissueDescriptor->m_SkeletonGraph,m_Target).position=center;

	m_Renderer->Render();
}

void PrimalGraphMoveInteractor::OnLeftButtonDown(){
	m_MoveFinished->SetCaller(this);
	m_Moved=true;
	m_MoveFinished->Notify();

}

void PrimalGraphMoveInteractor::OnRightButtonUp(){
	boost::get(giaa::SkeletonPointPropertyTag(),*m_TissueDescriptor->m_SkeletonGraph,m_Target).position=m_OriginalPoint;
	m_MoveFinished->SetCaller(this);
	m_Moved=false;
	m_MoveFinished->Notify();
}
