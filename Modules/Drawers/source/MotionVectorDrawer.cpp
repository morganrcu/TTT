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
void ttt::MotionVectorDrawer::Draw(){

	assert(m_Descriptor);
	assert(m_Renderer);
	BGL_FORALL_VERTICES(v,*m_Descriptor->m_CellGraph,ttt::TrackedCellGraph){

		vtkSmartPointer<vtkArrowSource> arrowSource = vtkSmartPointer<vtkArrowSource>::New();
		itk::Point<double,3> midPoint= boost::get(ttt::TrackedCellPropertyTag(),*m_Descriptor->m_CellGraph,v).m_Centroid;
		itk::Vector<double,3> velocity= boost::get(ttt::TrackedCellPropertyTag(),*m_Descriptor->m_CellGraph,v).m_Velocity;


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


#if 0
		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
		transform->Scale(1,1,1);

		vtkSmartPointer<vtkTransformFilter> transformFilter = vtkSmartPointer<vtkTransformFilter>::New();

		transformFilter->SetInputConnection(arrowSource->GetOutputPort());
		transformFilter->SetTransform(transform);

                                  // Visualize
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(transformFilter->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
        actor->SetPosition(posa[0]*spacing[0],posa[1]*spacing[1],0.0);

        double deg = atan2((posb[1] -posa[1]),(posb[0]-posa[0])) * 180/3.14159;
        std::cout<<deg<<std::endl;
        actor->RotateZ(deg);
        m_Renderer->AddActor(actor);
#endif

	}
}

void ttt::MotionVectorDrawer::Show(){
	//TODO
}
void ttt::MotionVectorDrawer::Hide(){
	//TODO
}
