#include <vtkRenderWindowInteractor.h>
#include <vtkPropPicker.h>

#include "VertexAdditionInteractor.h"

void VertexAdditionInteractor::OnLeftButtonDown(){

		int* clickPos = this->GetInteractor()->GetEventPosition();

		// Pick from this location.
		vtkSmartPointer<vtkPropPicker>  picker =  	vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], 0, m_Renderer);

		double * clickPosition=picker->GetPickPosition();

		m_AddedVertex = ttt::AdherensJunctionVertex::New() ;

		itk::Index<3> index;

		index[0]=round(clickPosition[0]/m_Spacing[0]);
		index[1]=round(clickPosition[1]/m_Spacing[1]);
		index[2]=round(clickPosition[2]/m_Spacing[2]);

		m_AddedVertex->SetPosition(index);

		m_AdherensJunctionsVertices->push_back(m_AddedVertex);

		ttt::VertexLocationsDrawer::VertexSphereMapperAndActor drawnStuff= m_VertexLocationsDrawer->DrawAdherensJunctionVertex(m_AddedVertex);

		m_Renderer->Render();
		m_AddedSphere=drawnStuff.get<1>();
		m_AddedMapper=drawnStuff.get<2>();
		m_AddedActor=drawnStuff.get<3>();

		emit vertexAdded(m_AddedActor);

	    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

	}
void VertexAdditionInteractor::OnRightButtonUp(){
		emit additionCancelled();
		vtkInteractorStyleTrackballCamera::OnRightButtonUp();
	}
