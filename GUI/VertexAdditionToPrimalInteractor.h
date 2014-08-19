/*
 * VertexAdditionInteractor.h
 *
 *  Created on: Dec 9, 2013
 *      Author: morgan
 */

#ifndef VERTEXADDITIONTOPRIMALINTERACTOR_H_
#define VERTEXADDITIONTOPRIMALINTERACTOR_H_

#include <QObject>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkPointWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "PrimalGraphDrawer.h"


class VertexAdditionToPrimalInteractor :  public QObject, public vtkInteractorStyleTrackballCamera {

	Q_OBJECT
signals:
		void vertexAdded(vtkSmartPointer<vtkActor> & actor);
		void additionCancelled();
private:


    ttt::TissueDescriptor::Pointer m_TissueDescriptor;
    vtkSmartPointer<ttt::PrimalGraphDrawer<ttt::TissueDescriptor> > m_PrimalGraphDrawer;

    vtkSmartPointer<vtkActor> m_AddedActor;
    vtkSmartPointer<vtkSphereSource> m_AddedSphere;
    vtkSmartPointer<vtkPolyDataMapper> m_AddedMapper;

    ttt::SkeletonVertexType m_AddedVertex;

protected:
	vtkSmartPointer<vtkRenderer> m_Renderer;

public:
	vtkTypeMacro(VertexAdditionToPrimalInteractor, vtkInteractorStyleTrackballCamera);

	inline vtkSmartPointer<vtkActor> GetAddedActor(){
		return m_AddedActor;
	}
	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}

	inline void SetTissueDescriptor(const ttt::TissueDescriptor::Pointer & descriptor){
		m_TissueDescriptor=descriptor;
	}

	inline void SetPrimalGraphDrawer(const vtkSmartPointer<ttt::PrimalGraphDrawer<ttt::TissueDescriptor> > & primalGraphDrawer){
		m_PrimalGraphDrawer=primalGraphDrawer;
	}

	static VertexAdditionToPrimalInteractor* New(){
		return new VertexAdditionToPrimalInteractor;
	}

public:
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnMouseMove();
};



#endif /* VERTEXADDITIONINTERACTOR_H_ */
