#include "ttttrackannotation.h"
#include "ui_ttttrackannotation.h"
#include <vtkAxesActor.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>



TrackAnnotation::TrackAnnotation(QWidget *parent) :
    QMainWindow(parent),
    m_pUI(new Ui::TrackAnnotation)
{
	m_pUI->setupUi(this);

	m_NowRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_NowRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

	m_NowRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_NowRenderWindow->AddRenderer(m_NowRenderer);
	m_NowRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_NowInteractionStyle = vtkSmartPointer<MouseInteractorStyle>::New();

    m_NowInteractionStyle->SetDefaultRenderer(m_NowRenderer);

    m_NowRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_NowRenderWindowInteractor->SetRenderWindow(m_NowRenderWindow);
    m_NowRenderWindowInteractor->SetInteractorStyle(m_NowInteractionStyle);
    m_NowRenderWindowInteractor->Initialize();

    this->m_pUI->nowQVTKWidget->SetRenderWindow(m_NowRenderWindow);
    vtkSmartPointer<vtkAxesActor> nowAxes =  vtkSmartPointer<vtkAxesActor>::New();
    m_NowRenderer->AddActor(nowAxes);

    m_NowPrimalDrawer.SetRenderer(m_NowRenderer);
    m_NowPrimalDrawer.SetEdgeColorer(&m_NowEdgeColorer);
    m_NowPrimalDrawer.SetVertexColorer(&m_NowVertexColorer);

	m_NextRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_NextRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

	m_NextRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_NextRenderWindow->AddRenderer(m_NextRenderer);
	m_NextRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_NextInteractionStyle = vtkSmartPointer<MouseInteractorStyle>::New();

    m_NextInteractionStyle->SetDefaultRenderer(m_NextRenderer);

    m_NextRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_NextRenderWindowInteractor->SetRenderWindow(m_NextRenderWindow);
    m_NextRenderWindowInteractor->SetInteractorStyle(m_NextInteractionStyle);
    m_NextRenderWindowInteractor->Initialize();

    this->m_pUI->nextQVTKWidget->SetRenderWindow(m_NextRenderWindow);


    vtkSmartPointer<vtkAxesActor> nextAxes =  vtkSmartPointer<vtkAxesActor>::New();
    m_NextRenderer->AddActor(nextAxes);

    m_NextPrimalDrawer.SetRenderer(m_NextRenderer);
    m_NextPrimalDrawer.SetEdgeColorer(&m_NextEdgeColorer);
    m_NextPrimalDrawer.SetVertexColorer(&m_NextVertexColorer);



    m_Project.openDB();
    m_Project.OpenProject(2);
    m_Project.SetFrame(0);

    this->m_pUI->frameSlider->setMinimum(0);
    this->m_pUI->frameSlider->setMaximum(this->m_Project.GetNumFrames()-2);

    this->m_pUI->currentTrackSpinBox->setMinimum(0);
    this->m_pUI->currentTrackSpinBox->setMaximum(this->m_Project.GetNumTracks());
    this->m_pUI->nextTrackSpinBox->setMinimum(0);
    this->m_pUI->nextTrackSpinBox->setMaximum(this->m_Project.GetNumTracks());
    this->m_pUI->globalTrackSpinBox->setMaximum(this->m_Project.GetNumTracks());

    connect(this->m_pUI->frameSlider,SIGNAL(valueChanged(int)),this,SLOT(FrameChanged(int)));

    connect(this->m_pUI->currentTrackSpinBox,SIGNAL(valueChanged(int)),this,SLOT(CurrentTrackChanged(int)));
    connect(this->m_pUI->nextTrackSpinBox,SIGNAL(valueChanged(int)),this,SLOT(NextTrackChanged(int)));
    connect(this->m_pUI->globalTrackSpinBox,SIGNAL(valueChanged(int)),this,SLOT(GlobalTrackChanged(int)));

    connect(this->m_NowInteractionStyle,SIGNAL(ActorSelected(vtkSmartPointer<vtkActor>&)),this,SLOT(CurrentCellSelected(vtkSmartPointer<vtkActor>&)));

    connect(this->m_NextInteractionStyle,SIGNAL(ActorSelected(vtkSmartPointer<vtkActor>&)),this,SLOT(NextCellSelected(vtkSmartPointer<vtkActor>&)));

    connect(this->m_pUI->changeButton,SIGNAL(clicked()),this,SLOT(DoCorrection()));

    m_CurrentTrack=0;
    m_NextTrack=0;
    m_CurrentFrame=0;
    m_NowHighlightedActor=NULL;
    m_NextHighlightedActorA=NULL;
    m_NextHighlightedActorB=NULL;

    //DrawPrimalNow();
    FrameChanged(0);
    //DrawPolygonsNow();
    //DrawPolygonsNext();
    //this->ColorPolygons();
}
void TrackAnnotation::DoCorrection(){

	int frame= m_CurrentFrame;
	frame++;

	while(frame<m_Project.GetNumFrames()){
		this->m_Project.SetFrame(frame);
		ttt::TrackedTissueDescriptor::Pointer current = this->m_Project.GetTrackedTissueDescriptor();

		ttt::TrackedCellVertexType a = ttt::CellID2VertexDescriptor(this->m_CurrentTrack,current);
		ttt::TrackedCellVertexType b = ttt::CellID2VertexDescriptor(this->m_NextTrack,current);

		if(a!=-1) boost::get(ttt::TrackedCellPropertyTag(),(*current->m_CellGraph),a).m_ID=this->m_NextTrack;
		if(b!=-1) boost::get(ttt::TrackedCellPropertyTag(),(*current->m_CellGraph),b).m_ID=this->m_CurrentTrack;

		m_Project.SetTrackedTissueDescriptor(current);

		frame++;
	}
	m_Project.SetFrame(m_CurrentFrame);


}
void TrackAnnotation::CurrentCellSelected(vtkSmartPointer<vtkActor>& cell){

	ttt::TrackedCellVertexType selected = m_NowVertexAndActor.right.at(cell);

	int trackID=boost::get(ttt::TrackedCellPropertyTag(),(*m_CurrentTissueDescriptor->m_CellGraph),selected).m_ID;
	CurrentTrackChanged(trackID);
}

void TrackAnnotation::NextCellSelected(vtkSmartPointer<vtkActor>& cell){

	ttt::TrackedCellVertexType selected = m_NextVertexAndActor.right.at(cell);


	int trackID=boost::get(ttt::TrackedCellPropertyTag(),(*m_NextTissueDescriptor->m_CellGraph),selected).m_ID;
	NextTrackChanged(trackID);
}
void TrackAnnotation::GlobalTrackChanged(int track){
	m_GlobalTrack=track;

	this->CurrentTrackChanged(m_GlobalTrack);
	this->NextTrackChanged(m_GlobalTrack);

}

void TrackAnnotation::CurrentTrackChanged(int track){
	m_CurrentTrack=track;
	this->m_pUI->currentTrackSpinBox->setValue(track);
	this->HighlightPolygonNow(m_CurrentTrack);
	m_NowRenderWindow->Render();
}
void TrackAnnotation::NextTrackChanged(int track){
	m_NextTrack=track;
	this->m_pUI->nextTrackSpinBox->setValue(track);
	this->HighlightPolygonNext(m_NextTrack);
	m_NextRenderWindow->Render();
}

void TrackAnnotation::SplitCorrespondence(const ttt::TrackedTissueDescriptor::Pointer & t0,
		ttt::TrackedTissueDescriptor::Pointer & t1,
		std::map<int,int> & associations,
		std::vector<int> & creations,
		std::vector<int> & terminations,
		std::map<int,std::pair<int,int> > & mitosis){

	associations.clear();
	creations.clear();
	terminations.clear();
	mitosis.clear();
	BGL_FORALL_VERTICES(v0,*t0->m_CellGraph,ttt::TrackedCellGraph){

		ttt::TrackedCell cell0=boost::get(ttt::TrackedCellPropertyTag(),*t0->m_CellGraph,v0);
		int trackID=cell0.m_ID;

		ttt::TrackedCellVertexType v1=ttt::CellID2VertexDescriptor(trackID,t1);

		if(v1!=-1){

			associations[v0]=v1;
		}else{
			std::pair<ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType,ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType> children= ttt::CellParentID2VertexDescriptor(trackID,t1);

			if(children.first!=-1 && children.second!=-1){
				mitosis[v0]=std::pair<int,int>(children.first,children.second);
			}else{
				terminations.push_back(v0);
			}
		}
	}

	BGL_FORALL_VERTICES(v1,*t1->m_CellGraph,ttt::TrackedCellGraph){
		ttt::TrackedCell cell1=boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,v1);

		int trackID=cell1.m_ID;

		ttt::TrackedCellVertexType v0=ttt::CellID2VertexDescriptor(trackID,t0);

		if(v0==-1){

			int parentID=cell1.m_ParentID;

			ttt::TrackedCellVertexType v0=ttt::CellID2VertexDescriptor(parentID,t0);

			if(v0==-1){
				creations.push_back(v1);
			}
		}
	}

}

void TrackAnnotation::FrameChanged(int frame){
	m_CurrentFrame=frame;
	m_Project.SetFrame(frame);

	m_CurrentTissueDescriptor=m_Project.GetTrackedTissueDescriptor();
	m_Project.SetFrame(frame+1);
	m_NextTissueDescriptor=m_Project.GetTrackedTissueDescriptor();




	SplitCorrespondence(m_CurrentTissueDescriptor,m_NextTissueDescriptor,m_Associations,m_Creations,m_Terminations,m_Mitosis);


	DrawPrimalNow();
	DrawPrimalNext();

	this->DrawPolygonsNow();
	this->DrawPolygonsNext();
	this->ColorPolygons();

	this->HighlightPolygonNow(m_GlobalTrack);


	this->HighlightPolygonNext(m_GlobalTrack);

	m_NowRenderWindow->Render();
	m_NextRenderWindow->Render();
}

void TrackAnnotation::DrawPrimalNow(){
	m_NowPrimalDrawer.Reset();

	m_NowPrimalDrawer.SetTissueDescriptor(m_CurrentTissueDescriptor);
	m_NowPrimalDrawer.Draw();
	m_NowPrimalDrawer.PickableOff();
	m_NowPrimalDrawer.Show();

}

void TrackAnnotation::DrawPrimalNext(){
	m_NextPrimalDrawer.Reset();
	m_NextPrimalDrawer.SetTissueDescriptor(m_NextTissueDescriptor);
	m_NextPrimalDrawer.Draw();
	m_NextPrimalDrawer.PickableOff();
	m_NextPrimalDrawer.Show();

}



void TrackAnnotation::DrawPolygons(CellVertexAndActorBiMap & storage, ttt::TrackedTissueDescriptor::Pointer & tissue,vtkSmartPointer<vtkRenderer> & renderer){
	for (typename CellVertexAndActorBiMap::const_iterator it =
			storage.begin(); it != storage.end();
			++it) {
		renderer->RemoveActor(it->right);
	}
	storage.clear();

	BGL_FORALL_VERTICES(v,*(tissue->m_CellGraph),ttt::TrackedCellGraph){

		//assert(boost::get(ttt::TrackedCellPropertyTag(),*(tissueDescriptor->m_CellGraph),v).GetNumSkeletonPoints()>0);

		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		int npoints = 0;

		for (std::vector<ttt::SkeletonVertexType>::iterator it = boost::get(ttt::TrackedCellPropertyTag(), *(tissue->m_CellGraph), v).Begin();
				it!= boost::get(ttt::TrackedCellPropertyTag(),*(tissue->m_CellGraph), v).End();
				++it) {

			points->InsertNextPoint(boost::get(ttt::SkeletonPointPropertyTag(),
							*(tissue->m_SkeletonGraph), *it).position[0],
					boost::get(ttt::SkeletonPointPropertyTag(),
							*(tissue->m_SkeletonGraph), *it).position[1],
					boost::get(ttt::SkeletonPointPropertyTag(),
							*(tissue->m_SkeletonGraph), *it).position[2]);

			npoints++;
		}

		vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
		polygon->GetPointIds()->SetNumberOfIds(npoints); //make a quad
		for (int i = 0; i < npoints; i++) {
			polygon->GetPointIds()->SetId(i, i);
		}
		// Add the polygon to a list of polygons
		vtkSmartPointer<vtkCellArray> polygons =
		vtkSmartPointer<vtkCellArray>::New();
		polygons->InsertNextCell(polygon);

		// Create a PolyData
		vtkSmartPointer<vtkPolyData> polygonPolyData =
		vtkSmartPointer<vtkPolyData>::New();
		polygonPolyData->SetPoints(points);
		polygonPolyData->SetPolys(polygons);

		// Create a mapper and actor
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<
		vtkPolyDataMapper>::New();

		mapper->SetInputData(polygonPolyData);

		vtkSmartPointer<vtkActor> polygonActor = vtkSmartPointer<vtkActor>::New();

		polygonActor->SetMapper(mapper);
		polygonActor->GetProperty()->SetColor(1,1,1);
		polygonActor->GetProperty()->SetOpacity(1);
		polygonActor->SetPickable(1);

		renderer->AddActor(polygonActor);

		typedef typename CellVertexAndActorBiMap::value_type value_type;
		//std::cout << v << std::endl;
		storage.insert(value_type(v,polygonActor));
	}
}

void TrackAnnotation::ColorPolygons(){


	for(std::map<int,int>::iterator it=m_Associations.begin();it!=m_Associations.end();it++){
		m_NowVertexAndActor.left.at(it->first)->GetProperty()->SetColor(0.0,1.0,0.0);
		m_NextVertexAndActor.left.at(it->second)->GetProperty()->SetColor(0.0,1.0,0.0);
	}
	for(std::map<int,std::pair<int,int> >::iterator it=m_Mitosis.begin();it!=m_Mitosis.end();it++){
			m_NowVertexAndActor.left.at(it->first)->GetProperty()->SetColor(0.0,0.0,1.0);
			m_NextVertexAndActor.left.at(it->second.first)->GetProperty()->SetColor(0.0,0.0,1.0);
			m_NextVertexAndActor.left.at(it->second.second)->GetProperty()->SetColor(0.0,0.0,1.0);
	}
	for(std::vector<int>::iterator it=m_Creations.begin();it!=m_Creations.end();it++){
		m_NextVertexAndActor.left.at(*it)->GetProperty()->SetColor(1.0,0.0,1.0);
	}

	for(std::vector<int>::iterator it=m_Terminations.begin();it!=m_Terminations.end();it++){
			m_NowVertexAndActor.left.at(*it)->GetProperty()->SetColor(1.0,1.0,0.0);
		}
#if 0
	for (typename CellVertexAndActorBiMap::const_iterator it =m_CurrentVertexAndActor.begin(); it != m_CurrentVertexAndActor.end();++it) {
			if(m_Associations.find(it->left)!=m_Associations.end()){
				it->right->GetProperty()->SetColor(1.0,0.0,0.0);
			}else if(m_Mitosis.find(it->left)!=m_Mitosis.end()){
				it->right->GetProperty()->SetColor(0.0,1.0,0.0);
			}else if(std::find(m_Creations.begin(),m_Creations.end(),it->left)!=m_Creations.end()){
				it->right->GetProperty()->SetColor(0.0,0.0,1.0);
			}
	}
#endif
}

void TrackAnnotation::DrawPolygonsNow() {
	this->DrawPolygons(this->m_NowVertexAndActor,this->m_CurrentTissueDescriptor,this->m_NowRenderer);
}

void TrackAnnotation::DrawPolygonsNext() {
	this->DrawPolygons(this->m_NextVertexAndActor,this->m_NextTissueDescriptor,this->m_NextRenderer);



}

void TrackAnnotation::HighlightPolygon(CellVertexAndActorBiMap & storage, ttt::TrackedTissueDescriptor::Pointer & tissue,int track,vtkSmartPointer<vtkActor> & highlightedActorA,vtkSmartPointer<vtkActor> & highlightedActorB){
	ttt::TrackedCellVertexType v = ttt::CellID2VertexDescriptor(track,tissue);
	if(v!=-1){

		if(highlightedActorA){
			highlightedActorA->GetProperty()->SetColor(1,1,1);
			highlightedActorA->GetProperty()->SetOpacity(0.1);

		}
		if(highlightedActorB){
			highlightedActorB->GetProperty()->SetColor(1,1,1);
			highlightedActorB->GetProperty()->SetOpacity(0.1);

		}
		highlightedActorA=storage.left.at(v);
		highlightedActorA->GetProperty()->SetColor(1.0,1.0,1.0);
		highlightedActorA->GetProperty()->SetOpacity(1.0);
	}else{
		std::pair<ttt::TrackedCellVertexType,ttt::TrackedCellVertexType> mitosis=ttt::CellParentID2VertexDescriptor(track,tissue);
		if(mitosis.first!=-1 && mitosis.second!=-1){
			if(highlightedActorA){
				highlightedActorA->GetProperty()->SetColor(1,1,1);
				highlightedActorA->GetProperty()->SetOpacity(0.1);
			}
			if(highlightedActorB){
				highlightedActorB->GetProperty()->SetColor(1,1,1);
				highlightedActorB->GetProperty()->SetOpacity(0.1);
			}

			highlightedActorA=storage.left.at(mitosis.first);
			highlightedActorB=storage.left.at(mitosis.second);

			highlightedActorA->GetProperty()->SetColor(1.0,1.0,1.0);
			highlightedActorA->GetProperty()->SetOpacity(1.0);

			highlightedActorB->GetProperty()->SetColor(1.0,1.0,1.0);
			highlightedActorB->GetProperty()->SetOpacity(1.0);
		}

	}
}

void TrackAnnotation::HighlightPolygonNow(int track){
	vtkSmartPointer<vtkActor> nullActor=vtkSmartPointer<vtkActor>(NULL);
	this->HighlightPolygon(m_NowVertexAndActor,m_CurrentTissueDescriptor,track,m_NowHighlightedActor,nullActor);

}

void TrackAnnotation::HighlightPolygonNext(int track){
	this->HighlightPolygon(m_NextVertexAndActor,m_NextTissueDescriptor,track,m_NextHighlightedActorA,m_NextHighlightedActorB);
}


void TrackAnnotation::Draw(){




}
TrackAnnotation::~TrackAnnotation()
{
    delete m_pUI;
}
