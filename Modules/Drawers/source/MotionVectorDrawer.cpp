//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.


#include <vtkArrowSource.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkPolyDataMapper.h>
#include <vtkTransformPolyDataFilter.h>
#include <boost/graph/iteration_macros.hpp>
#include "MotionVectorDrawer.h"

ttt::MotionVectorDrawer::MotionVectorDrawer(){

}
void ttt::MotionVectorDrawer::Reset(){
	for(typename std::vector< vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		m_Renderer->RemoveActor(*it);
	}
	m_Actors.clear();
}
void ttt::MotionVectorDrawer::Draw(){

	assert(m_Descriptor);
	assert(m_Renderer);
	this->Reset();
	BGL_FORALL_VERTICES(v,*m_Descriptor->m_CellGraph,ttt::TrackedCellGraph){

		vtkSmartPointer<vtkArrowSource> arrowSource = vtkSmartPointer<vtkArrowSource>::New();
		itk::Point<double,3> midPoint= boost::get(ttt::TrackedCellPropertyTag(),*m_Descriptor->m_CellGraph,v).GetCentroid();
		itk::Vector<double,3> velocity= boost::get(ttt::TrackedCellPropertyTag(),*m_Descriptor->m_CellGraph,v).GetVelocity();


		// Generate a random start and end point
		double startPoint[3], endPoint[3];

		for(int i=0;i<3;i++){
			startPoint[i] =midPoint[i] -velocity[i]/2 ;
		}

		for(int i=0;i<3;i++){
			endPoint[i] =midPoint[i] + velocity[i]/2 ;
		}

		// Compute a basis
		double normalizedX[3];
		double normalizedY[3];
		double normalizedZ[3];

		// The X axis is a vector from start to end
		vtkMath::Subtract(endPoint, startPoint, normalizedX);
		double length = vtkMath::Norm(normalizedX);
		vtkMath::Normalize(normalizedX);

		// The Z axis is an arbitrary vecotr cross X
		double arbitrary[3];
		arbitrary[0] = vtkMath::Random(-10,10);
		arbitrary[1] = vtkMath::Random(-10,10);
		arbitrary[2] = vtkMath::Random(-10,10);
		vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
		vtkMath::Normalize(normalizedZ);


		// The Y axis is Z cross X
		vtkMath::Cross(normalizedZ, normalizedX, normalizedY);
		vtkSmartPointer<vtkMatrix4x4> matrix =
		  vtkSmartPointer<vtkMatrix4x4>::New();

		// Create the direction cosine matrix
		matrix->Identity();
		for (unsigned int i = 0; i < 3; i++)
		  {
		  matrix->SetElement(i, 0, normalizedX[i]);
		  matrix->SetElement(i, 1, normalizedY[i]);
		  matrix->SetElement(i, 2, normalizedZ[i]);
		  }

		// Apply the transforms
		vtkSmartPointer<vtkTransform> transform =
		  vtkSmartPointer<vtkTransform>::New();
		transform->Translate(startPoint);
		transform->Concatenate(matrix);
		transform->Scale(length, length, length);

		// Transform the polydata
		vtkSmartPointer<vtkTransformPolyDataFilter> transformPD =
		  vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformPD->SetTransform(transform);
		transformPD->SetInputConnection(arrowSource->GetOutputPort());

		//Create a mapper and actor for the arrow
		vtkSmartPointer<vtkPolyDataMapper> mapper =
		  vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkSmartPointer<vtkActor> actor =
		  vtkSmartPointer<vtkActor>::New();

		#ifdef USER_MATRIX
		mapper->SetInputConnection(arrowSource->GetOutputPort());
		actor->SetUserMatrix(transform->GetMatrix());
		#else
		mapper->SetInputConnection(transformPD->GetOutputPort());
		#endif
		actor->SetMapper(mapper);

		m_Renderer->AddActor(actor);
		m_Actors.push_back(actor);

	}
}

void ttt::MotionVectorDrawer::Show(){
	for(typename std::vector< vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		(*it)->VisibilityOn();
	}
}
void ttt::MotionVectorDrawer::Hide(){
	for(typename std::vector< vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		(*it)->VisibilityOff();
	}
}
