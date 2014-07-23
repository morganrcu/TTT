#include "tttexplorer.h"
#include "ui_tttexplorer.h"

#include <vtkImageCanvasSource2D.h>
#include <vtkWindowToImageFilter.h>
#include <vtkFFMPEGWriter.h>

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
	m_CellMotionVectorDrawer.SetRenderer(m_CentralRenderer);


    connect(this->m_pUI->originalGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowOriginal(bool)));
    connect(this->m_pUI->enhancedGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowEnhanced(bool)));
    connect(this->m_pUI->platenessGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowPlateness(bool)));
    connect(this->m_pUI->vertexnessGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowVertexness(bool)));
    connect(this->m_pUI->vertexLocationsGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowVertexLocations(bool)));
    connect(this->m_pUI->primalGraphGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowPrimalGraph(bool)));
    connect(this->m_pUI->dualGraphGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowDualGraph(bool)));
    connect(this->m_pUI->cellRegionsGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowCellRegions(bool)));
    connect(this->m_pUI->motionVectorsGroupBox,SIGNAL(toggled(bool)),this,SLOT(ShowCellMotionVectors(bool)));
    connect(this->m_pUI->frameSlider,SIGNAL(valueChanged(int)),this,SLOT(SetFrame(int)));

    connect(this->m_pUI->exportMovieButton,SIGNAL(clicked()),this,SLOT(ExportMovie()));
}

void TTTExplorer::SetProject(ttt::TissueTrackingAbstractProject * project){
			this->m_Project=project;
			this->m_pUI->frameSlider->setMinimum(0);
			this->m_pUI->frameSlider->setMaximum(this->m_Project->GetNumFrames()-1);
}
TTTExplorer::~TTTExplorer(){
    delete m_pUI;
}

void TTTExplorer::ExportMovie(){

	vtkSmartPointer<vtkRenderer> fileRenderer = vtkSmartPointer<vtkRenderer>::New();
	fileRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

	vtkSmartPointer<vtkRenderWindow> fileRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	fileRenderWindow->SetOffScreenRendering(1);
	fileRenderWindow->SetSize(1980,1024);
	fileRenderWindow->AddRenderer(m_CentralRenderer);

	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =vtkSmartPointer<vtkWindowToImageFilter>::New();

	windowToImageFilter->SetInput(fileRenderWindow);
	windowToImageFilter->SetMagnification(1); //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel

	windowToImageFilter->Update();

	vtkSmartPointer<vtkFFMPEGWriter> writer =
	      vtkSmartPointer<vtkFFMPEGWriter>::New();

	  writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	  writer->SetFileName("test.avi");
	  writer->Start();

	  for(unsigned int i = 0; i < this->m_Project->GetNumFrames(); i++)    {
		//RENDER EVERYTHING
		  this->SetFrame(i);
		  fileRenderWindow->Render();
		  windowToImageFilter->Update();
		  writer->Write();
	  }
	  writer->End();

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
#if 0
	if(this->m_Project->IsTissueDescriptorReady()){
			m_PrimalGraphDrawer.SetTissueDescriptor(this->m_Project->GetTissueDescriptor());
			m_PrimalGraphDrawer.Draw();
			m_PrimalGraphDrawer.SetVisibility(this->m_pUI->primalGraphGroupBox->isChecked());
			m_DualGraphDrawer.SetTissueDescriptor(this->m_Project->GetTissueDescriptor());
			m_DualGraphDrawer.Draw();
			m_DualGraphDrawer.SetVisibility(this->m_pUI->primalGraphGroupBox->isChecked());

	}else{
			m_PrimalGraphDrawer.Reset();
			m_DualGraphDrawer.Reset();
	}
#endif
	if(this->m_Project->IsTrackedTissueDescriptorReady()){
		m_CellMotionVectorDrawer.SetTrackedDescriptor(this->m_Project->GetTrackedTissueDescriptor());
		m_CellMotionVectorDrawer.Draw();
		m_CellMotionVectorDrawer.SetVisibility(this->m_pUI->motionVectorsGroupBox->isChecked());
	}else{
		m_CellMotionVectorDrawer.Reset();
	}

	this->m_CentralRenderer->Render();
	this->m_CentralRenderWindow->Render();

}

void TTTExplorer::ShowOriginal(bool show){
	m_OriginalDrawer.SetVisibility(show);
	m_CentralRenderWindow->Render();
}

void TTTExplorer::ShowEnhanced(bool show){
	m_EnhancedDrawer.SetVisibility(show);
	m_CentralRenderWindow->Render();
}

void TTTExplorer::ShowPlateness(bool show){
	m_PlatenessDrawer.SetVisibility(show);
	m_CentralRenderWindow->Render();
}

void TTTExplorer::ShowVertexness(bool show){
	m_VertexnessDrawer.SetVisibility(show);
	m_CentralRenderWindow->Render();
}
void TTTExplorer::ShowVertexLocations(bool show){
	m_VertexLocationsDrawer.SetVisibility(show);
	m_CentralRenderWindow->Render();
}
void TTTExplorer::ShowPrimalGraph(bool show){
	m_PrimalGraphDrawer.SetVisibility(show);
	m_CentralRenderWindow->Render();
}
void TTTExplorer::ShowDualGraph(bool show){
	m_DualGraphDrawer.SetVisibility(show);
	m_CentralRenderWindow->Render();
}
void TTTExplorer::ShowCellRegions(bool show){

}
void TTTExplorer::ShowCellMotionVectors(bool show){
	this->m_CellMotionVectorDrawer.SetVisibility(show);
	this->m_CentralRenderWindow->Render();
}
void TTTExplorer::ShowTectonics(bool){

}
void TTTExplorer::ShowEllipses(bool){

}
