
#ifndef VERTEXSELECTIONINTERACTOR_H_
#define VERTEXSELECTIONINTERACTOR_H_

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkPointWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <QObject>
#include "VertexLocationsDrawer.h"

class VertexSelectionInteractor : public QObject, public vtkInteractorStyleTrackballCamera   {
	Q_OBJECT
private:

    vtkSmartPointer<vtkPointWidget> m_PointWidget;

protected:
	vtkSmartPointer<ttt::VertexLocationsDrawer> m_Drawer;
	vtkSmartPointer<vtkRenderer> m_Renderer;
	vtkSmartPointer<vtkProperty> m_PickedProperty;
	vtkSmartPointer<vtkActor> m_PickedVertexActor;


public:

	vtkTypeMacro(VertexSelectionInteractor, vtkInteractorStyleTrackballCamera);

	inline vtkSmartPointer<vtkActor> GetPickedVertexActor(){
		return m_PickedVertexActor;
	}

signals:
	void vertexSelected(vtkSmartPointer<vtkActor> & actor);
	void vertexUnselected(vtkSmartPointer<vtkActor> & actor);
public:
	inline void SetVertexLocationsDrawer(const vtkSmartPointer<ttt::VertexLocationsDrawer> & drawer){
		m_Drawer=drawer;
	}

	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}

	void SetSelection(const vtkSmartPointer<vtkActor> & actor){
		this->UnsetSelection();

		// Save the property of the picked actor so that we can
		// restore it next time
		this->m_PickedVertexActor = actor;

		this->m_PickedProperty->DeepCopy(this->m_PickedVertexActor->GetProperty());
		// Highlight the picked actor by changing its properties
		this->m_PickedVertexActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		this->m_PickedVertexActor->GetProperty()->SetDiffuse(1.0);
		this->m_PickedVertexActor->GetProperty()->SetSpecular(0.0);

#if 0
		if (this->m_pVertexSelected) {
			this->m_pVertexSelected->SetCaller(this);
			this->m_pVertexSelected->Notify();
		}
#endif
	}

	void UnsetSelection(){
		if (this->m_PickedVertexActor){

			this->m_PickedVertexActor->GetProperty()->DeepCopy(this->m_PickedProperty);
			//m_PointWidget->Off();

#if 0
			if(this->m_pVertexUnselected){
				this->m_pVertexUnselected->SetCaller(this);
				this->m_pVertexUnselected->Notify();
			}
#endif
		}
		this->m_PickedVertexActor=0;
	}

	static VertexSelectionInteractor* New(){
		return new VertexSelectionInteractor;
	}
public:
	VertexSelectionInteractor();
	virtual ~VertexSelectionInteractor(){

	}
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();
};
#endif /* VERTEXSELECTIONINTERACTOR_H_ */
