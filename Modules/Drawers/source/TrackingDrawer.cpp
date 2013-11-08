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


#include <vtkPoints.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <boost/graph/iteration_macros.hpp>
#include "genRandomColor.h"
#include "TrackingDrawer.h"

ttt::TrackingDrawer::TrackingDrawer() {

}

ttt::TrackingDrawer::~TrackingDrawer() {

}

void ttt::TrackingDrawer::Draw(){

	BGL_FORALL_VERTICES(v,*(m_Descriptor->m_CellGraph),TrackedCellGraph){
		vtkSmartPointer<vtkPoints> points =  vtkSmartPointer<vtkPoints>::New();
		int npoints=0;
		for( std::vector<SkeletonVertexType>::iterator it=boost::get(TrackedCellPropertyTag(),*(m_Descriptor->m_CellGraph),v).Begin();
				it!= boost::get(TrackedCellPropertyTag(),*(m_Descriptor->m_CellGraph),v).End();
				++it){

				points->InsertNextPoint(boost::get(SkeletonPointPropertyTag(),*(m_Descriptor->m_SkeletonGraph),*it).position[0],
										boost::get(SkeletonPointPropertyTag(),*(m_Descriptor->m_SkeletonGraph),*it).position[1],
										boost::get(SkeletonPointPropertyTag(),*(m_Descriptor->m_SkeletonGraph),*it).position[2]);

				npoints++;
		}
		vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
		polygon->GetPointIds()->SetNumberOfIds(npoints); //make a quad
		for(int i=0;i<npoints;i++){
			  polygon->GetPointIds()->SetId(i, i);
		}
	  // Add the polygon to a list of polygons
		  vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();
		  polygons->InsertNextCell(polygon);

		  // Create a PolyData
		  vtkSmartPointer<vtkPolyData> polygonPolyData =  vtkSmartPointer<vtkPolyData>::New();
		  polygonPolyData->SetPoints(points);
		  polygonPolyData->SetPolys(polygons);

		  // Create a mapper and actor
		  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

		  mapper->SetInputData(polygonPolyData);

		  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

		  actor->SetMapper(mapper);
		  int trackID=boost::get(TrackedCellPropertyTag(),*(m_Descriptor->m_CellGraph),v).m_ID;

		  std::vector<double> color;
		  if(m_TrackID2Color.count(trackID)){
			  color=m_TrackID2Color[trackID];
		  }else{

			  //color=generator.generateRandomColor(rand());
			  color.push_back(rand() );
			  color.push_back(rand() );
			  color.push_back(rand() );
			  m_TrackID2Color[trackID]=color;
		  }
		  std::cout << trackID << "->" << color[0] << " " <<color[1] << " " << color[2] << std::endl;
		  actor->GetProperty()->SetColor(color[0]/RAND_MAX,color[1]/RAND_MAX,color[2]/RAND_MAX);
		  m_Renderer->AddActor(actor);

	}
}
