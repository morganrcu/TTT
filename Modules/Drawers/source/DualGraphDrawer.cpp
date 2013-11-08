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

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>
#include <boost/graph/iteration_macros.hpp>
#include "DualGraphDrawer.h"

void ttt::DualGraphDrawer::Draw() {
	BGL_FORALL_VERTICES(v,*m_Descriptor->m_CellGraph,ttt::CellGraph){
		itk::Point<double,3> a= boost::get(ttt::CellPropertyTag(),*m_Descriptor->m_CellGraph,v).m_Centroid;
		vtkSmartPointer<vtkSphereSource> newSphere = vtkSmartPointer<vtkSphereSource>::New();
		newSphere->SetCenter(a[0],a[1],a[2]);
		newSphere->SetRadius(0.8);
		vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

		sphereMapper->SetInputConnection(newSphere->GetOutputPort());

		vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
		sphereActor->GetProperty()->SetColor(0,0,0);
		if(boost::degree(v,*m_Descriptor->m_CellGraph) == 1) {
			sphereActor->GetProperty()->SetColor(0,0,1);
		}
		sphereActor->SetMapper(sphereMapper);
		m_Renderer->AddActor(sphereActor);
	}
	BGL_FORALL_EDGES(e,*m_Descriptor->m_CellGraph,ttt::CellGraph){
		vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
		itk::Point<double,3> a= boost::get(ttt::CellPropertyTag(),*m_Descriptor->m_CellGraph,boost::source(e,*m_Descriptor->m_CellGraph)).m_Centroid;
		newLine->SetPoint1(a[0],a[1],a[2]);
		itk::Point<double,3> b= boost::get(ttt::CellPropertyTag(),*m_Descriptor->m_CellGraph,boost::target(e,*m_Descriptor->m_CellGraph)).m_Centroid;

	//IndexType b=m_SkeletonGraph[boost::target(e,m_SkeletonGraph)].position;
		newLine->SetPoint2(b[0],b[1],a[2]);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(newLine->GetOutputPort());

		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(0,0,0);
		actor->GetProperty()->SetLineWidth(3);
		m_Renderer->AddActor(actor);
	}
#ifdef OUT_OF_CONTROL
	std::cout << "in dual cbox" << std::endl;

	typedef itk::Point<double, 3> IndexType;

	BGL_FORALL_VERTICES(v,m_Descriptor->m_CellGraph,ttt::CellGraph){

		itk::Point<double,3> a= boost::get(ttt::CellPropertyTag(),m_Descriptor->m_CellGraph,v).m_Centroid;

	vtkSmartPointer<vtkSphereSource> newSphere = vtkSmartPointer<vtkSphereSource>::New();
	//std::cout <<"loc " << s[0]*a[0] << " " << s[1]*a[1] << std::endl;
	newSphere->SetCenter(m_Spacing[0]*a[0],m_Spacing[1]*a[1],m_Spacing[2]*a[2]);
	newSphere->SetRadius(0.8);

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	sphereMapper->SetInputConnection(newSphere->GetOutputPort());

	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	sphereActor->GetProperty()->SetColor(0,0,0);
	if(boost::degree(v,m_Descriptor->m_CellGraph) == 1) {
		sphereActor->GetProperty()->SetColor(0,0,1);
	}
#if 0
	if(boost::get(ttt::CellPropertyTag(),m_Descriptor->m_CellGraph,v).m_area < 1) {
		sphereActor->GetProperty()->SetColor(1,0,0);
	}
#endif
	sphereActor->SetMapper(sphereMapper);

	/*
	 if(DoneTracking){
	 BGL_FORALL_VERTICES(vt,tracks[m_CurrentFrame]->m_TrackedCellGraph,TrackedCellGraph){
	 //boost::get(ttt::TrackedCellPropertyTag(),tracks[i]->m_TrackedCellGraph,v)
	 itk::Point<double,3> trackedCentroid = boost::get(ttt::TrackedCellPropertyTag(),tracks[m_CurrentFrame]->m_TrackedCellGraph,vt).m_Centroid;

	 if (a[0] == trackedCentroid[0] && a[1] == trackedCentroid[1] && a[2] == trackedCentroid[2])
	 {
	 std::cout<<"Frame "<< m_CurrentFrame << ": " << "Observed: " << v << " Tracked: " << vt << std::endl;
	 sphereActor->GetProperty()->SetColor(r_color[vt],g_color[vt],b_color[vt]);
	 break;
	 }
	 }
	 }*/

	sphereActor->GetProperty()->SetColor(1,1,1);

	int DomainColor = 0;
	int foundCell = 0;
#ifdef OUT_OF_CONTROL
	if(DomainSelected) {
		//std::cout<<"In Dual Graph coloring Domains"<<std::endl;
		int trackID = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).trackID;
		//std::cout<<"Cell ID is: " << trackID << std::endl;
		for(std::vector<Domain>::iterator itr = Domains.begin(); itr != Domains.end(); ++itr) {
			Domain dom = *itr;
			std::vector<int> cells = dom.m_cellNodes;
			for(std::vector<int>::iterator cellitr = cells.begin(); cellitr != cells.end(); ++cellitr) {
				//std::cout<<"Comparing to " << DomainCell.trackID << std::endl;
				if(trackID == *cellitr) {
					//std::cout << "found matching track id " << trackID << " " << *cellitr << std::endl;
					Cell cell = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v);
					std::vector <double> color;
					color.push_back(r_color[DomainColor]);
					color.push_back(g_color[DomainColor]);
					color.push_back(b_color[DomainColor]);
					sphereActor->GetProperty()->SetColor(r_color[DomainColor],g_color[DomainColor],b_color[DomainColor]);
					foundCell = 1;
					break;
				}
			}
			if(foundCell) {
				break;
			}
			DomainColor++;
		}
	}
#endif

	/*else if(DoneTracking){
	 int trackID = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).trackID;
	 for(std::vector<CellVertexType>::iterator cellIterator = selectedCells.begin(); cellIterator != selectedCells.end(); ++cellIterator){
	 Cell trackCell = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,*cellIterator);
	 //std::cout<<"Frame "<< m_CurrentFrame << ": " << "Observed: " << trackID << " Tracked: " << trackCell.trackID << std::endl;
	 if(trackID == trackCell.trackID){

	 Cell cell = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v);
	 SelectCell(cell,sphereActor,cell.m_Color);
	 break;
	 }
	 }
	 }*/
#ifdef OUT_OF_CONTROL
	std::vector<double> col = boost::get(ttt::CellPropertyTag(),m_Descriptor->m_CellGraph,v).m_Color;
#endif
	/*Cell cell = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v);
	 ColorCell(cell,-1,1);
	 if(boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).trackID == 386){
	 ColorCell(cell,0,1);
	 }
	 if(boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).trackID == 387){
	 ColorCell(cell,1,1);
	 }
	 if(boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).trackID == 388){
	 ColorCell(cell,2,1);
	 }
	 if(boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).trackID == 389){
	 ColorCell(cell,5,1);
	 }*/
	m_CurrentRenderer->AddActor(sphereActor);

	int trackID = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).trackID;
	//std::cout << "In Dual Graph: Vertex Pt: " << v << " " << sphereActor << " " << trackID << std::endl;
	//fftw_complex * fft = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).GetFourierTransPolygonPlot(descriptor->m_SkeletonGraph);
	//vtkTogiaaVertex[sphereActor] = v;
	actorTogiaavertexDual[sphereActor] = v;
	giaaTovktVertexToDual[v] = sphereActor;
	vtkSmartPointer<vtkEventQtSlotConnect> connect = vtkSmartPointer<vtkEventQtSlotConnect>::New();
	connect->Connect(renderWindowInteractor,vtkCommand::LeftButtonPressEvent,this,
			SLOT(slot_clicked(vtkObject*, unsigned long, void *, void *)));
	/*std::vector<SkeletonVertexType> firstCorona;
	 dualVertexToEdge[v] = firstCorona;*/

}

	if (DomainSelected) {
		this->PopulateDomainTable();
	}

	BGL_FORALL_EDGES(e,descriptor->m_CellGraph,ttt::CellGraph){

	vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();

	itk::Point<double,3> a= boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,boost::source(e,descriptor->m_CellGraph)).m_Centroid;

	//IndexType a=m_SkeletonGraph[].position;
	newLine->SetPoint1(s[0]*a[0],s[1]*a[1],s[2]*a[2]);

	itk::Point<double,3> b= boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,boost::target(e,descriptor->m_CellGraph)).m_Centroid;

	//IndexType b=m_SkeletonGraph[boost::target(e,m_SkeletonGraph)].position;
	newLine->SetPoint2(s[0]*b[0],s[1]*b[1],s[2]*a[2]);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(newLine->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0,0,0);
	actor->GetProperty()->SetLineWidth(3);
	m_CurrentRenderer->AddActor(actor);

	CellVertexType src = boost::source(e,descriptor->m_CellGraph);
	CellVertexType tgt = boost::target(e,descriptor->m_CellGraph);

	tuple<CellVertexType,CellVertexType> pts (src,tgt);
	svtToactor_edge_dual[pts] = actor;
	//std::cout<< src << " " << tgt << " "<< actor <<std::endl;
	tuple<CellVertexType,CellVertexType> ptt (tgt,src);
	svtToactor_edge_dual[ptt] = actor;

}

	std::cout << "before jx analysis" << std::endl;

	// map(svt - vector of connections)
	std::cout << "In jx analysis" << std::endl;
	std::map<int, std::vector<int> > srctgtmap1;
	std::map<int, std::vector<int> > srctgtmap2;
	std::map<int, int> trackIDmap1;
	std::map<int, int> trackIDmap2;
	if (DoneTracking && m_CurrentFrame < m_Project.GetNumFrames() - 1) {

		BGL_FORALL_EDGES(e,descriptor->m_CellGraph,ttt::CellGraph){
		CellVertexType src = boost::source(e,descriptor->m_CellGraph);
		CellVertexType tgt = boost::target(e,descriptor->m_CellGraph);

		int trackIDsrc = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,src).trackID;
		int trackIDtgt = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,tgt).trackID;

		std::vector<int> assocsrc;
		std::vector<int> assoctgt;

		srctgtmap1[trackIDsrc] = assocsrc;
		srctgtmap1[trackIDtgt] = assoctgt;
	}

	BGL_FORALL_EDGES(e,descriptor->m_CellGraph,ttt::CellGraph) {
		CellVertexType src = boost::source(e,descriptor->m_CellGraph);
		CellVertexType tgt = boost::target(e,descriptor->m_CellGraph);

		int trackIDsrc = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,src).trackID;
		int trackIDtgt = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,tgt).trackID;

		srctgtmap1[trackIDsrc].push_back(trackIDtgt);
		srctgtmap1[trackIDtgt].push_back(trackIDsrc);
	}

	tmpdescriptor = m_Project.GetTissueDescriptor(m_CurrentFrame+1);

	BGL_FORALL_EDGES(e,tmpdescriptor->m_CellGraph,ttt::CellGraph) {
		CellVertexType src = boost::source(e,tmpdescriptor->m_CellGraph);
		CellVertexType tgt = boost::target(e,tmpdescriptor->m_CellGraph);

		int trackIDsrc = boost::get(ttt::CellPropertyTag(),tmpdescriptor->m_CellGraph,src).trackID;
		int trackIDtgt = boost::get(ttt::CellPropertyTag(),tmpdescriptor->m_CellGraph,tgt).trackID;

		std::vector<int> assocsrc;
		std::vector<int> assoctgt;

		srctgtmap2[trackIDsrc] = assocsrc;
		srctgtmap2[trackIDtgt] = assoctgt;
	}

	BGL_FORALL_EDGES(e,tmpdescriptor->m_CellGraph,ttt::CellGraph) {
		CellVertexType src = boost::source(e,tmpdescriptor->m_CellGraph);
		CellVertexType tgt = boost::target(e,tmpdescriptor->m_CellGraph);

		int trackIDsrc = boost::get(ttt::CellPropertyTag(),tmpdescriptor->m_CellGraph,src).trackID;
		int trackIDtgt = boost::get(ttt::CellPropertyTag(),tmpdescriptor->m_CellGraph,tgt).trackID;

		srctgtmap2[trackIDsrc].push_back(trackIDtgt);
		srctgtmap2[trackIDtgt].push_back(trackIDsrc);
	}

	BGL_FORALL_EDGES(e,descriptor->m_CellGraph,ttt::CellGraph) {
		CellVertexType src = boost::source(e,descriptor->m_CellGraph);
		CellVertexType tgt = boost::target(e,descriptor->m_CellGraph);

		int trackIDsrc = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,src).trackID;
		int trackIDtgt = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,tgt).trackID;

		bool found_edge = false;
		for(int i = 0; i < srctgtmap2[trackIDsrc].size(); i++) {
			if (srctgtmap2[trackIDsrc][i] == trackIDtgt) {
				found_edge = true;
				break;
			}
		}

		if(!found_edge) {
			tuple<CellVertexType,CellVertexType> pts (src,tgt);
			svtToactor_edge_dual[pts]->GetProperty()->SetColor(1,0,0);
		}
	}
}

	if (DoneTracking && m_CurrentFrame > 0) {

		tmpdescriptor = m_Project.GetTissueDescriptor(m_CurrentFrame - 1);
		srctgtmap2.clear();

		BGL_FORALL_EDGES(e,tmpdescriptor->m_CellGraph,ttt::CellGraph){
		CellVertexType src = boost::source(e,tmpdescriptor->m_CellGraph);
		CellVertexType tgt = boost::target(e,tmpdescriptor->m_CellGraph);

		int trackIDsrc = boost::get(ttt::CellPropertyTag(),tmpdescriptor->m_CellGraph,src).trackID;
		int trackIDtgt = boost::get(ttt::CellPropertyTag(),tmpdescriptor->m_CellGraph,tgt).trackID;

		std::vector<int> assocsrc;
		std::vector<int> assoctgt;

		srctgtmap2[trackIDsrc] = assocsrc;
		srctgtmap2[trackIDtgt] = assoctgt;
	}

		BGL_FORALL_EDGES(e,tmpdescriptor->m_CellGraph,ttt::CellGraph){
		CellVertexType src = boost::source(e,tmpdescriptor->m_CellGraph);
		CellVertexType tgt = boost::target(e,tmpdescriptor->m_CellGraph);

		int trackIDsrc = boost::get(ttt::CellPropertyTag(),tmpdescriptor->m_CellGraph,src).trackID;
		int trackIDtgt = boost::get(ttt::CellPropertyTag(),tmpdescriptor->m_CellGraph,tgt).trackID;

		srctgtmap2[trackIDsrc].push_back(trackIDtgt);
		srctgtmap2[trackIDtgt].push_back(trackIDsrc);
	}

		BGL_FORALL_EDGES(e,descriptor->m_CellGraph,ttt::CellGraph){
		CellVertexType src = boost::source(e,descriptor->m_CellGraph);
		CellVertexType tgt = boost::target(e,descriptor->m_CellGraph);

		int trackIDsrc = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,src).trackID;
		int trackIDtgt = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,tgt).trackID;
		bool found_edge = false;
		for(int i = 0; i < srctgtmap2[trackIDsrc].size(); i++) {
			if (srctgtmap2[trackIDsrc][i] == trackIDtgt) {
				found_edge = true;
				break;
			}
		}
		if(!found_edge) {
			tuple<CellVertexType,CellVertexType> pts (src,tgt);
			svtToactor_edge_dual[pts]->GetProperty()->SetColor(0,1,0);
		}
	}
}
#endif
}
