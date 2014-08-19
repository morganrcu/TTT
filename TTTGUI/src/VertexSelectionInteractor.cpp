#include <vtkRenderWindowInteractor.h>
#include <vtkPropPicker.h>

#include "VertexSelectionInteractor.h"

VertexSelectionInteractor::VertexSelectionInteractor(){

		m_PickedVertexActor=0;
		m_PickedProperty=vtkSmartPointer<vtkProperty>::New();

		m_PointWidget= vtkSmartPointer<vtkPointWidget>::New();
	}

void VertexSelectionInteractor::OnLeftButtonDown(){
		int* clickPos = this->GetInteractor()->GetEventPosition();

			// Pick from this location.
			vtkSmartPointer<vtkPropPicker>  picker =  	vtkSmartPointer<vtkPropPicker>::New();
			picker->Pick(clickPos[0], clickPos[1], 0, m_Renderer);

			vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();

			vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
			if(pickedActor){
				this->SetSelection(pickedActor);
				emit vertexSelected(this->m_PickedVertexActor);;
			}


	}
void VertexSelectionInteractor::OnRightButtonUp(){

	vtkInteractorStyleTrackballCamera::OnRightButtonUp();
	this->UnsetSelection();
	emit vertexUnselected(this->m_PickedVertexActor);
	}
