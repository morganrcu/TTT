#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>

#include "VertexAdditionInteractor.h"

void VertexAdditionInteractor::OnLeftButtonDown(){

		int* clickPos = this->GetInteractor()->GetEventPosition();

		// Pick from this location.
		vtkSmartPointer<vtkPointPicker>  picker =  	vtkSmartPointer<vtkPointPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], clickPos[2], m_Renderer);

		double * clickPosition=picker->GetPickPosition();

		std::cout << "ClickPosition " << clickPosition[0] << " "<< clickPosition[1] << " "<< clickPosition[2]<< std::endl;

		m_AddedVertex = ttt::AdherensJunctionVertex::New() ;

		itk::Index<3> index;

		index[0]=round(clickPosition[0]/m_Spacing[0]);
		index[1]=round(clickPosition[1]/m_Spacing[1]);
		index[2]=round(clickPosition[2]/m_Spacing[2]);


		std::cout << "Add to " << index << std::endl;
		m_AddedVertex->SetPosition(index);

		m_AdherensJunctionsVertices->push_back(m_AddedVertex);

		ttt::VertexLocationsDrawer::VertexSphereMapperAndActor drawnStuff= m_VertexLocationsDrawer->DrawAdherensJunctionVertex(m_AddedVertex);



		m_AddedSphere=drawnStuff.get<1>();
		m_AddedMapper=drawnStuff.get<2>();
		m_AddedActor=drawnStuff.get<3>();

		m_AddedActor->VisibilityOn();

		m_Renderer->GetRenderWindow()->Render();

	    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	    emit vertexAdded(m_AddedActor);

	}
void VertexAdditionInteractor::OnRightButtonUp(){

		vtkInteractorStyleTrackballCamera::OnRightButtonUp();
		emit additionCancelled();
}

void VertexAdditionInteractor::OnMouseMove(){
	//vtkInteractorStyleTrackballCamera::OnMouseMove();

	//FIXME this is a temporal patch to avoid the rotation of the scene during the addition of the second
}

