#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProbeFilter.h>
#include <vtkPointWidget.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include "VertexLocationsDrawer.h"
void ttt::VertexLocationsDrawer::Draw() {
	this->Reset();
	for (ttt::AdherensJunctionVertices::iterator it = m_VertexLocations->begin(); it != m_VertexLocations->end(); it++) {
		this->DrawAdherensJunctionVertex(*it);
	}
}

ttt::VertexLocationsDrawer::VertexSphereMapperAndActor ttt::VertexLocationsDrawer::DrawAdherensJunctionVertex(const ttt::AdherensJunctionVertex::Pointer & vertex){

	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(vertex->GetPosition()[0] * m_Spacing[0],vertex->GetPosition()[1] * m_Spacing[1],vertex->GetPosition()[2] * m_Spacing[2]);

	//sphereSource->SetRadius(2 * m_Spacing[0]);
	sphereSource->SetRadius(0.1);

	sphereSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();

	sphereActor->SetMapper(sphereMapper);
	VertexSphereMapperAndActor resultTuple=boost::make_tuple(vertex,sphereSource,sphereMapper,sphereActor);
	this->m_VertexSphereMapperAndActorList.push_back(resultTuple);
	m_Renderer->AddActor(sphereActor);
	sphereActor->VisibilityOff();
	sphereActor->PickableOn();
	sphereActor->GetProperty()->SetColor(0.0,1.0,0.0);
	return resultTuple;

}
