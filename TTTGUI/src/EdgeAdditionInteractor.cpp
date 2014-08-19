/*
 * EdgeAdditionInteractor.cpp
 *
 *  Created on: 04/02/2014
 *      Author: rod
 */

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPropPicker.h>
#include <vtkPointPicker.h>
#include "EdgeAdditionInteractor.h"
EdgeAdditionInteractor::EdgeAdditionInteractor() {
	m_SourceSelected=false;
	m_Source=0;
	m_Target=0;
	m_Renderer=0;
	m_SourceProperty=vtkSmartPointer<vtkProperty>::New();
}

void EdgeAdditionInteractor::OnLeftButtonDown(){
	int* clickPos = this->GetInteractor()->GetEventPosition();

	// Pick from this location.
	vtkSmartPointer<vtkPropPicker>  picker =  	vtkSmartPointer<vtkPropPicker>::New();
	picker->Pick(clickPos[0], clickPos[1], 0, m_Renderer);

	vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();


	if(pickedActor && pickedActor!=m_Source){
		if(m_SourceSelected){
			m_Target=pickedActor;

			emit edgeAdded(m_Source,m_Target);

			this->m_Source->GetProperty()->DeepCopy(m_SourceProperty);
			m_SourceSelected=false;
			m_Source=0;
			m_Target=0;
		}else{
			m_SourceSelected=true;

			m_Source=pickedActor;

			m_SourceProperty->DeepCopy(this->m_Source->GetProperty());

			this->m_Source->GetProperty()->SetColor(1.0, 0.0, 0.0);
			this->m_Source->GetProperty()->SetDiffuse(1.0);
			this->m_Source->GetProperty()->SetSpecular(0.0);
			this->m_Renderer->GetRenderWindow()->Render();

		}
	}
}
void EdgeAdditionInteractor::OnRightButtonUp(){
	//m_Renderer->RemoveActor(m_LineActor);

	if(m_SourceSelected){

		m_SourceSelected=false;
	}
	emit edgeAdditionCancelled();
}

void EdgeAdditionInteractor::OnMouseMove(){
	if(m_SourceSelected){
		int* clickPos = this->GetInteractor()->GetEventPosition();
		double picked[3];

		vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
		picker->Pick(clickPos[0], clickPos[1],  0, m_Renderer);
		picker->GetPickPosition(picked);



//		m_Line->SetPoint2(picked[0],picked[1],picked[2]);
//		m_Line->Update();
	}
}
