/*
 * VertexAdditionInteractor.h
 *
 *  Created on: Dec 9, 2013
 *      Author: morgan
 */

#ifndef VERTEXADDITIONINTERACTOR_H_
#define VERTEXADDITIONINTERACTOR_H_

#include <QObject>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkPointWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "VertexLocationsDrawer.h"


class VertexAdditionInteractor :  public QObject, public vtkInteractorStyleTrackballCamera {

	Q_OBJECT
signals:
		void vertexAdded(vtkSmartPointer<vtkActor> & actor);
		void additionCancelled();
private:

    itk::FixedArray<double,3> m_Spacing;
    ttt::AdherensJunctionVertices::Pointer m_AdherensJunctionsVertices;
    vtkSmartPointer<ttt::VertexLocationsDrawer> m_VertexLocationsDrawer;

    vtkSmartPointer<vtkActor> m_AddedActor;
    vtkSmartPointer<vtkSphereSource> m_AddedSphere;
    vtkSmartPointer<vtkPolyDataMapper> m_AddedMapper;
    ttt::AdherensJunctionVertex::Pointer m_AddedVertex;

protected:
	vtkSmartPointer<vtkRenderer> m_Renderer;

public:

	vtkTypeMacro(VertexAdditionInteractor, vtkInteractorStyleTrackballCamera);

	inline vtkSmartPointer<vtkActor> GetAddedActor(){
		return m_AddedActor;
	}
	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}

	inline void SetAdherensJunctionVertices(const ttt::AdherensJunctionVertices::Pointer & vertices){
		m_AdherensJunctionsVertices=vertices;
	}

	inline void SetVertexLocationsDrawer(const vtkSmartPointer<ttt::VertexLocationsDrawer> & vertexLocationsDrawer){
		m_VertexLocationsDrawer=vertexLocationsDrawer;
	}

	inline void SetSpacing(const itk::FixedArray<double,3> spacing){
		m_Spacing=spacing;
	}

	static VertexAdditionInteractor* New(){
		return new VertexAdditionInteractor;
	}

public:
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnMouseMove();
};



#endif /* VERTEXADDITIONINTERACTOR_H_ */
