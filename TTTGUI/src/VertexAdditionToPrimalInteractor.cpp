#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>

#include "VertexAdditionToPrimalInteractor.h"

void VertexAdditionToPrimalInteractor::OnLeftButtonDown(){

		int* clickPos = this->GetInteractor()->GetEventPosition();

		// Pick from this location.
		vtkSmartPointer<vtkPointPicker>  picker =  	vtkSmartPointer<vtkPointPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], clickPos[2], m_Renderer);

		double * clickPosition=picker->GetPickPosition();

		std::cout << "ClickPosition " << clickPosition[0] << " "<< clickPosition[1] << " "<< clickPosition[2]<< std::endl;

		m_AddedVertex = boost::add_vertex(m_TissueDescriptor->GetAJGraph());

		boost::get(ttt::SkeletonPointPropertyTag<3>(),m_TissueDescriptor->GetAJGraph(),m_AddedVertex).position[0]=clickPosition[0];
		boost::get(ttt::SkeletonPointPropertyTag<3>(),m_TissueDescriptor->GetAJGraph(),m_AddedVertex).position[1]=clickPosition[1];
		boost::get(ttt::SkeletonPointPropertyTag<3>(),m_TissueDescriptor->GetAJGraph(),m_AddedVertex).position[2]=clickPosition[2];

		ttt::PrimalGraphDrawer<ttt::TissueDescriptor<3> >::VertexSphereMapperAndActor drawnStuff = m_PrimalGraphDrawer->DrawVertex(m_AddedVertex);


		m_AddedSphere=drawnStuff.get<1>();
		m_AddedMapper=drawnStuff.get<2>();
		m_AddedActor=drawnStuff.get<3>();

		m_AddedActor->VisibilityOn();

		m_Renderer->GetRenderWindow()->Render();

	    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	    emit vertexAdded(m_AddedActor);

	}
void VertexAdditionToPrimalInteractor::OnRightButtonUp(){

		vtkInteractorStyleTrackballCamera::OnRightButtonUp();
		emit additionCancelled();
}

void VertexAdditionToPrimalInteractor::OnMouseMove(){
	//vtkInteractorStyleTrackballCamera::OnMouseMove();

	//FIXME this is a temporal patch to avoid the rotation of the scene during the addition of the second
}

