/*
 * PrimalInteractor.h
 *
 *  Created on: Sep 24, 2013
 *      Author: morgan
 */

#ifndef PRIMALINTERACTOR_H_
#define PRIMALINTERACTOR_H_


#include "tttDescriptionDataTypes.h"
namespace ttt{
class PrimalInteractor : public vtkInteractorStyle{
private:
    vtkSmartPointer<vtkActor> pt1 = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> pt2 = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> pt1prev = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> pt2prev = vtkSmartPointer<vtkActor>::New();
    std::vector<SkeletonVertexType_lS> selected_svt;
    int chosen = 1;
    PrimalInteractor(){

    }
public:
    static PrimalInteractor* New();
    vtkTypeMacro(PrimalInteractor, vtkInteractorStyle);
    virtual void OnRightButtonUp();
    virtual void OnLeftButtonDown();
    virtual void OnKeyPress();
};
}

#endif /* PRIMALINTERACTOR_H_ */
