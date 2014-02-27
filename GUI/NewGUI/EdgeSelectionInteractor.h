/*
 * EdgeSelectionInteractor.h
 *
 *  Created on: 04/02/2014
 *      Author: rod
 */

#ifndef EDGESELECTIONINTERACTOR_H_
#define EDGESELECTIONINTERACTOR_H_
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <QObject>

#include "PrimalGraphDrawer.h"

class EdgeSelectionInteractor : public QObject, public vtkInteractorStyleTrackballCamera   {

	Q_OBJECT
private:

protected:

	vtkSmartPointer<ttt::PrimalGraphDrawer<ttt::TissueDescriptor> > m_Drawer;
	vtkSmartPointer<vtkRenderer> m_Renderer;
	vtkSmartPointer<vtkProperty> m_PickedProperty;
	vtkSmartPointer<vtkActor> m_PickedEdgeActor;


public:

	vtkTypeMacro(EdgeSelectionInteractor, vtkInteractorStyleTrackballCamera);

	inline vtkSmartPointer<vtkActor> GetSelectedEdgeActor(){
		return m_PickedEdgeActor;
	}
signals:
	void edgeSelected(vtkSmartPointer<vtkActor> & actor);
	void edgeUnselected(vtkSmartPointer<vtkActor> & actor);
public:
	inline void SetPrimalGraphDrawer(const vtkSmartPointer<ttt::PrimalGraphDrawer<ttt::TissueDescriptor> > & drawer){
		m_Drawer=drawer;
	}

	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}

	void SetSelection(const vtkSmartPointer<vtkActor> & actor){
		this->UnsetSelection();

		// Save the property of the picked actor so that we can
		// restore it next time
		this->m_PickedEdgeActor = actor;

		this->m_PickedProperty->DeepCopy(this->m_PickedEdgeActor->GetProperty());
		// Highlight the picked actor by changing its properties
		this->m_PickedEdgeActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		this->m_PickedEdgeActor->GetProperty()->SetDiffuse(1.0);
		this->m_PickedEdgeActor->GetProperty()->SetSpecular(0.0);
		emit edgeSelected(this->m_PickedEdgeActor);;
#if 0
		if (this->m_pVertexSelected) {
			this->m_pVertexSelected->SetCaller(this);
			this->m_pVertexSelected->Notify();
		}
#endif
	}

	void UnsetSelection(){
		if (this->m_PickedEdgeActor){
			emit edgeUnselected(this->m_PickedEdgeActor);
			this->m_PickedEdgeActor->GetProperty()->DeepCopy(this->m_PickedProperty);
		}
		this->m_PickedEdgeActor=0;
	}

	static EdgeSelectionInteractor* New(){
		return new EdgeSelectionInteractor;
	}
public:
	EdgeSelectionInteractor();
	virtual ~EdgeSelectionInteractor(){

	}
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();
};

#endif /* EDGESELECTIONINTERACTOR_H_ */

