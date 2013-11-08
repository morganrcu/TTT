/*
 * PrimalGraphDrawerInteractor.h
 *
 *  Created on: Sep 24, 2013
 *      Author: morgan
 */

#ifndef PRIMALGRAPHDRAWERINTERACTOR_H_
#define PRIMALGRAPHDRAWERINTERACTOR_H_
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vector>
#include "PrimalGraphDrawer.h"
namespace ttt{
class PrimalGraphDrawerInteractor : public PrimalGraphDrawer, public vtkInteractorStyleTrackballCamera{
private:
    vtkSmartPointer<vtkActor> m_SelectedEdge;
    vtkSmartPointer<vtkActor> m_SelectedVertex;

    vtkSmartPointer<vtkActor> pt1;
    vtkSmartPointer<vtkActor> pt2;
    vtkSmartPointer<vtkActor> pt1prev;
    vtkSmartPointer<vtkActor> pt2prev;
    //std::vector<giaa::SkeletonVertexType_lS> selected_svt;

    int chosen;
    int interpolationSwitch = 0;
    double interpolationValue = 0.0;

    vtkSmartPointer<vtkActorCollection> interpolationSet = vtkSmartPointer<vtkActorCollection>::New();
    vtkSmartPointer<vtkActor> firstInterpolationPoint = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> secondInterpolationPoint = vtkSmartPointer<vtkActor>::New();

    PrimalGraphDrawerInteractor(){
    	pt1= vtkSmartPointer<vtkActor>::New();
    	pt2= vtkSmartPointer<vtkActor>::New();
    	pt1prev= vtkSmartPointer<vtkActor>::New();
    	pt2prev= vtkSmartPointer<vtkActor>::New();
    	chosen=1;
    }
public:
    static PrimalGraphDrawerInteractor* New(){
    	return new PrimalGraphDrawerInteractor;
    }

    vtkTypeMacro(PrimalGraphDrawerInteractor, vtkInteractorStyle);
    virtual void OnRightButtonUp();
    virtual void OnLeftButtonDown();
    virtual void OnKeyPress();
private:
    inline void HighlightActor(const vtkSmartPointer<vtkActor> & actor);
    inline void UnhighlightActor(const vtkSmartPointer<vtkActor> & actor);
};
//vtkStandardNewMacro(PrimalGraphDrawerInteractor);

}
#endif /* PRIMALGRAPHDRAWERINTERACTOR_H_ */
