/*
 * EdgeAdditionInteractor.h
 *
 *  Created on: 04/02/2014
 *      Author: rod
 */

#ifndef EDGEADDITIONINTERACTOR_H_
#define EDGEADDITIONINTERACTOR_H_

#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
//#include <vtkLineSource.h>
//#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include "PrimalGraphDrawer.h"
#include <QObject>

class EdgeAdditionInteractor : public QObject, public vtkInteractorStyleTrackballCamera{
	Q_OBJECT
private:

signals:
	void edgeAdded(vtkSmartPointer<vtkActor> & a, vtkSmartPointer<vtkActor> & b);
	void edgeAdditionCancelled();
protected:

	ttt::PrimalGraphDrawer<ttt::TissueDescriptor<3> > * m_Drawer;
	vtkSmartPointer<vtkRenderer> m_Renderer;
	vtkSmartPointer<vtkActor> m_Source;
	vtkSmartPointer<vtkActor> m_Target;
	vtkSmartPointer<vtkProperty> m_SourceProperty;


#if 0
	vtkSmartPointer<vtkLineSource> m_Line;
	vtkSmartPointer<vtkPolyDataMapper> m_LineMapper;
	vtkSmartPointer<vtkActor> m_LineActor;
#endif
	bool m_SourceSelected;
public:
	static EdgeAdditionInteractor* New(){
		return new EdgeAdditionInteractor;
	}

public:

	void SetDrawer(ttt::PrimalGraphDrawer<ttt::TissueDescriptor<3> > * drawer){
		m_Drawer=drawer;
	}
	EdgeAdditionInteractor();

	virtual ~EdgeAdditionInteractor(){

	}

	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}

	virtual void OnMouseMove();

	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();
};

#endif /* EDGEADDITIONINTERACTOR_H_ */
