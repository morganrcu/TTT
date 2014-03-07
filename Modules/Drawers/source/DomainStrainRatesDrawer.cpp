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


#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include "DomainStrainRatesDrawer.h"
void ttt::DomainStrainRatesDrawer::Reset(){
	for(std::vector<vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		m_Renderer->RemoveActor(*it);
	}
	m_Actors.clear();
}
void ttt::DomainStrainRatesDrawer::Draw(){
	assert(m_DomainStrainRates);
	assert(m_Renderer);

	this->Reset();

	for(DomainStrainRatesMapIteratorType it = m_DomainStrainRates->begin();it!=m_DomainStrainRates->end();it++){
		TrackedTissueDescriptor::DualGraphVertexDescriptorType cell=it->first;
		DomainStrainRates domainStrainRates=it->second;


		vnl_vector<double> majorDir,minorDir;
		double majorLength,minorLength, rotation;

		if(m_DrawMode==INTERCALATION_SRT){
			tensorToPlot(domainStrainRates.Intercalation_SRT,majorDir,&majorLength,minorDir,&minorLength,&rotation);
		}else if(m_DrawMode==TISSUE_SRT){
			tensorToPlot(domainStrainRates.Tissue_SRT,majorDir,&majorLength,minorDir,&minorLength,&rotation);
		}else if((m_DrawMode==CELL_SRT)){
			tensorToPlot(domainStrainRates.CellShape_SRT,majorDir,&majorLength,minorDir,&minorLength,&rotation);
		}
		std::cout << "M: " << majorLength << " m: " << minorLength << " Rotation: " << rotation << std::endl;

		majorLength=majorLength*100;
		minorLength=minorLength*100;
		itk::Point<double,3> center=boost::get(TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,cell).m_Centroid;

		itk::Point<double,3> a,b,c,d;

		a[0]=center[0]+majorDir[0]*majorLength/2;
		a[1]=center[1]+majorDir[1]*majorLength/2;
		a[2]=0;

		b[0]=center[0]-majorDir[0]*majorLength/2;
		b[1]=center[1]-majorDir[1]*majorLength/2;
		b[2]=0;

		c[0]=center[0]+minorDir[0]*minorLength/2;
		c[1]=center[1]+minorDir[1]*minorLength/2;
		c[2]=0;

		d[0]=center[0]-minorDir[0]*minorLength/2;
		d[1]=center[1]-minorDir[1]*minorLength/2;
		d[2]=0;

		vtkSmartPointer<vtkLineSource> major = vtkSmartPointer<vtkLineSource>::New();
		major->SetPoint1(a[0],a[1],a[2]);
		major->SetPoint2(b[0],b[1],b[2]);

		vtkSmartPointer<vtkLineSource> minor = vtkSmartPointer<vtkLineSource>::New();
		minor->SetPoint1(c[0],c[1],c[2]);
		minor->SetPoint2(d[0],d[1],d[2]);

		std::cout << "Major: " << a << " " << b << "Minor: " << c << " " << d << std::endl;

		vtkSmartPointer<vtkPolyDataMapper> majorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkSmartPointer<vtkPolyDataMapper> minorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

		majorMapper->SetInputData(major->GetOutput());
		majorMapper->ImmediateModeRenderingOn();
		majorMapper->SetInputConnection(major->GetOutputPort());

		minorMapper->SetInputData(minor->GetOutput());
		minorMapper->ImmediateModeRenderingOn();
		minorMapper->SetInputConnection(minor->GetOutputPort());

		vtkSmartPointer<vtkActor> majorActor = vtkSmartPointer<vtkActor>::New();
		majorActor->SetMapper(majorMapper);
		if(majorLength>0){
			majorActor->GetProperty()->SetColor(1.0,0.0,0.0);
		}else{
			majorActor->GetProperty()->SetColor(0.0,0.0,1.0);
		}

		vtkSmartPointer<vtkActor> minorActor = vtkSmartPointer<vtkActor>::New();
		minorActor->SetMapper(minorMapper);

		if(minorLength>0){
			minorActor->GetProperty()->SetColor(1.0,0.0,0.0);
		}else{
			minorActor->GetProperty()->SetColor(0.0,0.0,1.0);
		}


		m_Actors.push_back(majorActor);
		m_Actors.push_back(minorActor);
		m_Renderer->AddActor(majorActor);
		m_Renderer->AddActor(minorActor);
	}
}

void ttt::DomainStrainRatesDrawer::Show(){
	for(std::vector<vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		vtkSmartPointer<vtkActor> actor = *it;
		actor->VisibilityOn();
	}
}
void ttt::DomainStrainRatesDrawer::Hide(){
	for(std::vector<vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		vtkSmartPointer<vtkActor> actor = *it;
		actor->VisibilityOff();
	}
}
