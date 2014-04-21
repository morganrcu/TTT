/*
 * MouseInteractorStyle.h
 *
 *  Created on: Mar 25, 2014
 *      Author: morgan
 */

#ifndef MOUSEINTERACTORSTYLE_H_
#define MOUSEINTERACTORSTYLE_H_
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <QObject>
class MouseInteractorStyle: public QObject, public vtkInteractorStyleTrackballCamera {
Q_OBJECT
private:
	vtkSmartPointer<vtkActor> m_PickedActor;

signals:
		void ActorSelected(vtkSmartPointer<vtkActor> & a);

public:
	static MouseInteractorStyle* New();


	MouseInteractorStyle() {
		m_PickedActor = NULL;
	}

	virtual void OnLeftButtonDown() {
		// Get the location of the click (in window coordinates)
		int* pos = this->GetInteractor()->GetEventPosition();

		vtkSmartPointer<vtkCellPicker> picker =
				vtkSmartPointer<vtkCellPicker>::New();
		picker->SetTolerance(0.0005);

		// Pick from this location.
		picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

		m_PickedActor = picker->GetActor();
		if(m_PickedActor) emit ActorSelected(m_PickedActor);
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}

};



#endif /* MOUSEINTERACTORSTYLE_H_ */
