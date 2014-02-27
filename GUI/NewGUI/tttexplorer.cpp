#include "tttexplorer.h"
#include "ui_tttexplorer.h"



TTTExplorer::TTTExplorer(QWidget *parent) : QDialog(parent),  m_pUI(new Ui::TTTExplorer){
	m_pUI->setupUi(this);

	{
	m_CentralRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_CentralRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

	m_CentralRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_CentralRenderWindow->AddRenderer(m_CentralRenderer);
	m_CentralRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_CentralRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_CentralRenderWindowInteractor->SetRenderWindow(m_CentralRenderWindow);
    m_CentralRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_CentralRenderer->AddActor(axes);

    this->m_pUI->centralQVTKView->SetRenderWindow(m_CentralRenderWindow);

	}

	m_OriginalDrawer.SetRenderer(m_CentralRenderer);
	m_EnhancedDrawer.SetRenderer(m_CentralRenderer);
	m_PlatenessDrawer.SetRenderer(m_CentralRenderer);
	m_VertexnessDrawer.SetRenderer(m_CentralRenderer);
	m_VertexLocationsDrawer.SetRenderer(m_CentralRenderer);
	m_PrimalGraphDrawer.SetRenderer(m_CentralRenderer);
	m_DualGraphDrawer.SetRenderer(m_CentralRenderer);


    connect(this->m_pUI->originalGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowOriginal(bool)));
    connect(this->m_pUI->enhancedGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowEnhanced(bool)));
    connect(this->m_pUI->platenessGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowPlateness(bool)));
    connect(this->m_pUI->vertexnessGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowVertexness(bool)));
    connect(this->m_pUI->vertexLocationsGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowVertexLocations(bool)));
    connect(this->m_pUI->primalGraphGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowPrimalGraph(bool)));
    connect(this->m_pUI->dualGraphGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowDualGraph(bool)));
    connect(this->m_pUI->cellRegionsGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowCellRegions(bool)));

    connect(this->m_pUI->frameSlider,SIGNAL(valueChanged(int)),this,SLOT(SetFrame(int)));
}

void TTTExplorer::SetProject(ttt::TissueTrackingProject * project){
			this->m_Project=project;
			this->m_pUI->frameSlider->setMinimum(0);
			this->m_pUI->frameSlider->setMaximum(this->m_Project->GetNumFrames()-1);
			//SetFrame(0);

}
TTTExplorer::~TTTExplorer(){
    delete m_pUI;
}

void TTTExplorer::ExportMovie(){

}
void TTTExplorer::SetFrame(int frame){
	this->m_Project->SetFrame(frame);

	if(this->m_Project->IsRawImageReady()){
		m_OriginalDrawer.SetImage(this->m_Project->GetRawImage());
		m_OriginalDrawer.Draw();
		m_OriginalDrawer.SetVisibility(this->m_pUI->originalGroupBox->isChecked());

	}else{
		m_OriginalDrawer.Reset();
	}
	if(this->m_Project->IsDiffusedImageReady()){
		m_EnhancedDrawer.SetImage(this->m_Project->GetDiffusedImage());
		m_EnhancedDrawer.Draw();
		m_EnhancedDrawer.SetVisibility(this->m_pUI->enhancedGroupBox->isChecked());

	}else{
		m_EnhancedDrawer.Reset();
	}
	if(this->m_Project->IsPlatenessImageReady()){
		m_PlatenessDrawer.SetImage(this->m_Project->GetPlatenessImage());
		m_PlatenessDrawer.Draw();
		m_PlatenessDrawer.SetVisibility(this->m_pUI->platenessGroupBox->isChecked());
	}else{
		m_PlatenessDrawer.Reset();
	}
	if(this->m_Project->IsVertexnessImageReady()){
		m_VertexnessDrawer.SetImage(this->m_Project->GetVertexnessImage());
		m_VertexnessDrawer.Draw();
		m_VertexnessDrawer.SetVisibility(this->m_pUI->vertexnessGroupBox->isChecked());
	}else{
		m_VertexnessDrawer.Reset();
	}
	if(this->m_Project->IsVertexLocationsReady()){
		m_VertexLocationsDrawer.SetVertexLocations(this->m_Project->GetVertexLocations());
		m_VertexLocationsDrawer.SetSpacing(this->m_Project->GetSpacing());
		m_VertexLocationsDrawer.Draw();
		m_VertexLocationsDrawer.SetVisibility(this->m_pUI->vertexLocationsGroupBox->isChecked());
	}else{
		m_VertexLocationsDrawer.Reset();
	}
	this->m_CentralRenderer->Render();
	this->m_CentralRenderWindow->Render();
}

void TTTExplorer::ShowOriginal(bool show){
	m_OriginalDrawer.SetVisibility(show);
}

void TTTExplorer::ShowEnhanced(bool show){
	m_EnhancedDrawer.SetVisibility(show);
}

void TTTExplorer::ShowPlateness(bool show){
	m_PlatenessDrawer.SetVisibility(show);
}

void TTTExplorer::ShowVertexness(bool show){
	m_VertexnessDrawer.SetVisibility(show);
}
void TTTExplorer::ShowVertexLocations(bool show){
	m_VertexLocationsDrawer.SetVisibility(show);
}
void TTTExplorer::ShowPrimalGraph(bool show){
	m_PrimalGraphDrawer.SetVisibility(show);
}
void TTTExplorer::ShowDualGraph(bool show){
	m_DualGraphDrawer.SetVisibility(show);
}
void TTTExplorer::ShowCellRegions(bool show){

}
void TTTExplorer::ShowCellMotionVectors(bool){

}
void TTTExplorer::ShowTectonics(bool){

}
void TTTExplorer::ShowEllipses(bool){

}
