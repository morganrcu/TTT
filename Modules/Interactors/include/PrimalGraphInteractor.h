/*
 * PrimalGraphInteractor.h
 *
 *  Created on: Sep 30, 2013
 *      Author: morgan
 */

#ifndef PRIMAL_GRAPH_INTERACTOR_H_
#define PRIMAL_GRAPH_INTERACTOR_H_
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include "PrimalGraphDrawer.h"
namespace ttt{
class PrimalGraphInteractor : public vtkInteractorStyleTrackballCamera {
protected:
	vtkSmartPointer<PrimalGraphDrawer> m_Drawer;
	vtkSmartPointer<vtkRenderer> m_Renderer;
	vtkSmartPointer<vtkActor> m_PickedVertex;
	vtkSmartPointer<vtkProperty> m_PickedVertexProperty;

public:
	PrimalGraphInteractor();
	virtual ~PrimalGraphInteractor();
	inline void SetPrimalGraphDrawer(const vtkSmartPointer<PrimalGraphDrawer> & drawer){
		m_Drawer=drawer;
	}
	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}
};
}

#endif /* PRIMALGRAPHINTERACTOR_H_ */
