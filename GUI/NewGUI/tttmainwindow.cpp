#include <QtSql/qsqldatabase.h>
#include <iostream>
#include <QtSql/qsqlrelationaltablemodel.h>
#include <QtSql/qsqlerror.h>

#include <QSettings>
#include <QDir>
#include "tttmainwindow.h"
#include "ui_tttmainwindow.h"
#include "tttsqlopenprojectdialog.h"
#include "tttsqlnewprojectdialog.h"

#include "tttexplorer.h"
#include "tttpreferences.h"

#include "VertexnessCommand.h"
#include "PlatenessCommand.h"
#include "DataCastingCommand.h"
#include "AnisotropicDiffusionCommand.h"
#include "VertexLocationCommand.h"
#include "AdherensJunctionSegmentationDijkstraCommand.h"
#include "CellGraphCommand.h"
#include "TrackingCommand.h"
#include "EllipsesCommand.h"
#include "ComputeDomainsCommand.h"
#include "TectonicsCommand.h"
#include <vtkAxesActor.h>
#include <vtkCubeSource.h>

TTTMainWindow::TTTMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUI(new Ui::TTTMainWindow)
{
	m_pUI->setupUi(this);

	{
	m_ProjectRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_ProjectRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_ProjectRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_ProjectRenderWindow->AddRenderer(m_ProjectRenderer);
    m_ProjectRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_ProjectRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_ProjectRenderWindowInteractor->SetRenderWindow(m_ProjectRenderWindow);
    m_ProjectRenderWindowInteractor->Initialize();


    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_ProjectRenderer->AddActor(axes);

    this->m_pUI->projectQVTKView->SetRenderWindow(m_ProjectRenderWindow);

	}
	{
	m_LowestScaleSelectionRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_LowestScaleSelectionRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_LowestScaleSelectionRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_LowestScaleSelectionRenderWindow->AddRenderer(m_LowestScaleSelectionRenderer);
    m_LowestScaleSelectionRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_LowestScaleSelectionRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_LowestScaleSelectionRenderWindowInteractor->SetRenderWindow(m_LowestScaleSelectionRenderWindow);
    m_LowestScaleSelectionRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_LowestScaleSelectionRenderer->AddActor(axes);

    this->m_pUI->lowestScaleSelectionQVTKView->SetRenderWindow(m_LowestScaleSelectionRenderWindow);

	}

	{
	m_HighestScaleSelectionRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_HighestScaleSelectionRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_HighestScaleSelectionRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_HighestScaleSelectionRenderWindow->AddRenderer(m_HighestScaleSelectionRenderer);
    m_HighestScaleSelectionRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_HighestScaleSelectionRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_HighestScaleSelectionRenderWindowInteractor->SetRenderWindow(m_HighestScaleSelectionRenderWindow);
    m_HighestScaleSelectionRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_HighestScaleSelectionRenderer->AddActor(axes);

    this->m_pUI->highestScaleSelectionQVTKView->SetRenderWindow(m_HighestScaleSelectionRenderWindow);

	}

	{
	m_RangeScaleSelectionRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_RangeScaleSelectionRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_RangeScaleSelectionRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_RangeScaleSelectionRenderWindow->AddRenderer(m_RangeScaleSelectionRenderer);
    m_RangeScaleSelectionRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_RangeScaleSelectionRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_RangeScaleSelectionRenderWindowInteractor->SetRenderWindow(m_RangeScaleSelectionRenderWindow);
    m_RangeScaleSelectionRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_RangeScaleSelectionRenderer->AddActor(axes);

    this->m_pUI->rangeScaleSelectionQVTKView->SetRenderWindow(m_RangeScaleSelectionRenderWindow);

	}


	{
	m_BeforeEnhancementRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_BeforeEnhancementRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_BeforeEnhancementRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_BeforeEnhancementRenderWindow->AddRenderer(m_BeforeEnhancementRenderer);
    m_BeforeEnhancementRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_BeforeEnhancementRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_BeforeEnhancementRenderWindowInteractor->SetRenderWindow(m_BeforeEnhancementRenderWindow);
    m_BeforeEnhancementRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_BeforeEnhancementRenderer->AddActor(axes);

    this->m_pUI->beforeEnhancementQVTKView->SetRenderWindow(m_BeforeEnhancementRenderWindow);

	}

	{
	m_AfterEnhancementRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_AfterEnhancementRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_AfterEnhancementRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_AfterEnhancementRenderWindow->AddRenderer(m_AfterEnhancementRenderer);
    m_AfterEnhancementRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_AfterEnhancementRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_AfterEnhancementRenderWindowInteractor->SetRenderWindow(m_AfterEnhancementRenderWindow);
    m_AfterEnhancementRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_AfterEnhancementRenderer->AddActor(axes);

    this->m_pUI->afterEnhancementQVTKView->SetRenderWindow(m_AfterEnhancementRenderWindow);

	}


	{
	m_VertexLocationRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_VertexLocationRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_VertexLocationRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_VertexLocationRenderWindow->AddRenderer(m_VertexLocationRenderer);
    m_VertexLocationRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_VertexLocationRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_VertexLocationRenderWindowInteractor->SetRenderWindow(m_VertexLocationRenderWindow);
    m_VertexLocationRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_VertexLocationRenderer->AddActor(axes);

    this->m_pUI->vertexLocationQVTKView->SetRenderWindow(m_VertexLocationRenderWindow);

	}

	{
	m_CellSegmentationRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_CellSegmentationRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_CellSegmentationRendererWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_CellSegmentationRendererWindow->AddRenderer(m_CellSegmentationRenderer);
    m_CellSegmentationRendererWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_CellSegmentationRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_CellSegmentationRenderWindowInteractor->SetRenderWindow(m_CellSegmentationRendererWindow);


    m_EdgeStandardInteractorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_EdgeStandardInteractorStyle);
    m_CellSegmentationRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_CellSegmentationRenderer->AddActor(axes);

    this->m_pUI->cellSegmentationQVTKView->SetRenderWindow(m_CellSegmentationRendererWindow);

	}

	{
	m_TrackingRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_TrackingRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_TrackingRendererWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_TrackingRendererWindow->AddRenderer(m_TrackingRenderer);
    m_TrackingRendererWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_TrackingRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_TrackingRenderWindowInteractor->SetRenderWindow(m_TrackingRendererWindow);
    m_TrackingRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_TrackingRenderer->AddActor(axes);

    this->m_pUI->trackingQVTKView->SetRenderWindow(m_TrackingRendererWindow);

	}

	{
	m_TectonicsRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_TectonicsRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

	m_TectonicsRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_TectonicsRenderWindow->AddRenderer(m_TectonicsRenderer);
	m_TectonicsRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_TectonicsRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_TectonicsRenderWindowInteractor->SetRenderWindow(m_TectonicsRenderWindow);
    m_TectonicsRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_TectonicsRenderer->AddActor(axes);

    this->m_pUI->tectonicsQVTKView->SetRenderWindow(m_TectonicsRenderWindow);

	}

	{
	m_InspectionRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_InspectionRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

	m_InspectionRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_InspectionRenderWindow->AddRenderer(m_InspectionRenderer);
	m_InspectionRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_InspectionRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_InspectionRenderWindowInteractor->SetRenderWindow(m_InspectionRenderWindow);
    m_InspectionRenderWindowInteractor->Initialize();

    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_InspectionRenderer->AddActor(axes);

    this->m_pUI->inspectionQVTKView->SetRenderWindow(m_InspectionRenderWindow);

	}


    connect(this->m_pUI->actionNew, SIGNAL(triggered()), this, SLOT(New()));
    connect(this->m_pUI->actionOpen, SIGNAL(triggered()), this, SLOT(Open()));
    connect(this->m_pUI->actionClose, SIGNAL(triggered()), this, SLOT(Close()));


    connect(this->m_pUI->actionFrameDown, SIGNAL(triggered()), this, SLOT(FrameDown()));
    connect(this->m_pUI->actionFrameUp, SIGNAL(triggered()), this, SLOT(FrameUp()));
    connect(this->m_pUI->actionAddFrame, SIGNAL(triggered()), this, SLOT(AddFrame()));
    connect(this->m_pUI->actionRemoveFrame, SIGNAL(triggered()), this, SLOT(RemoveFrame()));


    connect(this->m_pUI->lowestScalePreview,SIGNAL(clicked()),this,SLOT(DoLowestScale()));
    connect(this->m_pUI->highestScalePreview,SIGNAL(clicked()),this,SLOT(DoHighestScale()));
    connect(this->m_pUI->rangeScalePreview,SIGNAL(clicked()),this,SLOT(DoRangeScale()));
    connect(this->m_pUI->computeAllMembranessButton,SIGNAL(clicked()),this,SLOT(DoAllScale()));

    connect(this->m_pUI->stepsTabs,SIGNAL(currentChanged(int)),this,SLOT(SetupCurrentTab()));

    connect(this->m_pUI->projectSlider,SIGNAL(valueChanged(int)),this,SLOT(HighlightProjectFrame(int)));
    connect(this->m_pUI->membranessSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupMembranessFrame()));
    connect(this->m_pUI->enhancementSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupEnhancementFrame()));
    connect(this->m_pUI->vertexLocationSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupVertexFrame()));
    connect(this->m_pUI->cellSegmentationSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupSegmentationFrame()));
    connect(this->m_pUI->trackingSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupTrackingFrame()));

    connect(this->m_pUI->tectonicsSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupTectonicsFrame()));

    connect(this->m_pUI->enhanceButton,SIGNAL(clicked()),this,SLOT(EnhanceAndDraw()));
    connect(this->m_pUI->computeAllEnhanceButton,SIGNAL(clicked()),this,SLOT(DoAllEnhance()));


    connect(this->m_pUI->vertexnessButton,SIGNAL(clicked()),this,SLOT(VertexnessAndDraw()));
    connect(this->m_pUI->computeAllVertexLocationButton,SIGNAL(clicked()),this,SLOT(DoAllVertex()));

    connect(this->m_pUI->vertexLocationButton,SIGNAL(clicked()),this,SLOT(VertexLocationAndDraw()));

    connect(this->m_pUI->selectVertexButton,SIGNAL(clicked()),this,SLOT(SelectVertex()));
    connect(this->m_pUI->addVertexButton,SIGNAL(clicked()),this,SLOT(AddVertex()));
    connect(this->m_pUI->deleteVertexButton,SIGNAL(clicked()),this,SLOT(DeleteVertex()));
    connect(this->m_pUI->segmentationButton,SIGNAL(clicked()),this,SLOT(SegmentationAndDraw()));

    connect(this->m_pUI->selectEdgeButton,SIGNAL(clicked()),this,SLOT(SelectEdge()));
    connect(this->m_pUI->addEdgeButton,SIGNAL(clicked()),this,SLOT(AddEdge()));
    connect(this->m_pUI->deleteEdgeButton,SIGNAL(clicked()),this,SLOT(DeleteEdge()));


    connect(this->m_pUI->dualButton,SIGNAL(clicked()),this,SLOT(DualAndDraw()));

    connect(this->m_pUI->trackButton,SIGNAL(clicked()),this,SLOT(DoTracking()));
    connect(this->m_pUI->actionShowProjectExplorer,SIGNAL(triggered()),this,SLOT(LaunchProjectExplorer()));

    connect(this->m_pUI->actionPreferences,SIGNAL(triggered()),this,SLOT(LaunchPreferences()));

    connect(this->m_pUI->lowestScaleDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(LowestScaleChanged(double)));
    connect(this->m_pUI->highestScaleDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(HighestScaleChanged(double)));
    connect(this->m_pUI->stepsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(ScaleStepsChanged(int)));

    connect(this->m_pUI->nameLineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(ProjectNameChanged(const QString&)));
    connect(this->m_pUI->xDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SpacingXChanged(double)));
    connect(this->m_pUI->yDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SpacingYChanged(double)));
    connect(this->m_pUI->zDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SpacingZChanged(double)));

    connect(this->m_pUI->showAdherensJunctionsAtSegmentationCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowPlatenessOnSegmentation(int)));
    connect(this->m_pUI->showAdhrensJunctionsAtVertexLocationsCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowPlatenessOnVertexLocation(int)));
    connect(this->m_pUI->showCellGraphAtSegmentationCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowDualGraphOnCellSegmentation(int)));
    connect(this->m_pUI->showEllipsesCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowTrackedEllipsesAtTectonics(int)));

    connect(this->m_pUI->showCellStrainRateRadioButton,SIGNAL(clicked()),this,SLOT(ShowCellStrainRates()));
    connect(this->m_pUI->showTissueStrainRateRadioButton,SIGNAL(clicked()),this,SLOT(ShowTissueStrainRates()));
    connect(this->m_pUI->showIntercalationStrainRateRadioButton,SIGNAL(clicked()),this,SLOT(ShowIntercalationStrainRates()));

    connect(this->m_pUI->computeEllipse,SIGNAL(clicked()),this,SLOT(DoEllipses()));

    connect(this->m_pUI->computeStrainRates,SIGNAL(clicked()),this,SLOT(DoTectonics()));


    this->SetupVertexSelectionInteractor();
    this->SetupVertexAdditionInteractor();
    this->SetupEdgeSelectionInteractor();
    this->SetupEdgeAdditionInteractor();

    m_PointWidget = vtkSmartPointer<vtkPointWidget>::New();

    m_SettingsFile = QDir::homePath() + QString("/.ttttracker.rc");
    //m_SettingsFile = QApplication::applicationDirPath().left(1) + ":/demosettings.ini";
    std::cout << m_SettingsFile.toStdString() << std::endl;
    this->LoadSettings();
    //m_SettingsFile("~/.ttttracker.rc");
}

void TTTMainWindow::SamplingRateChanged(double rate){
	this->m_Project->SetSamplingRate(rate);
}
void TTTMainWindow::ProjectNameChanged(const QString & name){
	this->m_Project->SetProjectName(name.toStdString());
}
void TTTMainWindow::SpacingXChanged(double value){
	this->m_Project->SetSpacingX(value);
}
void TTTMainWindow::SpacingYChanged(double value){
	this->m_Project->SetSpacingY(value);
}
void TTTMainWindow::SpacingZChanged(double value){
	this->m_Project->SetSpacingZ(value);
}
void TTTMainWindow::LowestScaleChanged(double scale){
	m_Project->SetLowestScale(scale);
}
void TTTMainWindow::HighestScaleChanged(double scale){
	m_Project->SetHighestScale(scale);
}
void TTTMainWindow::ScaleStepsChanged(int steps){
	m_Project->SetScaleSteps(steps);
}

void TTTMainWindow::SetupCurrentTab(){
	int selectedPage= this->m_pUI->stepsTabs->currentIndex();
	std::cout << "Selected: " << selectedPage << std::endl;

	switch(selectedPage){
	case 0:
		break;
	case 1:
		break;
	case 2:
		DrawRangePlateness();
		break;
	case 3:
		DrawEnhancement();
		break;
	case 4:
		DrawVertex();
		break;
	case 5:
		this->DrawSegmentation();
		this->DrawDual();
		break;
	case 6:
		this->DrawTracking();
		break;
	case 7:
		this->DrawEllipses();
		this->DrawTectonics();
		break;
	};
}



void TTTMainWindow::DrawRangePlateness(){
	m_RangePlatenessDrawer.SetImage(m_Project->GetPlatenessImage());
	m_RangePlatenessDrawer.SetRenderer(this->m_RangeScaleSelectionRenderer);
	m_RangePlatenessDrawer.Draw();
	m_RangePlatenessDrawer.SetVisibility(true);
	this->m_RangeScaleSelectionRenderWindow->Render();
}

void TTTMainWindow::DrawEnhancement(){
	if(this->m_Project->IsRawImageReady()){
		m_BeforeEnhancementDrawer.SetImage(m_Project->GetRawImage());
		m_BeforeEnhancementDrawer.SetRenderer(this->m_BeforeEnhancementRenderer);
		m_BeforeEnhancementDrawer.Draw();
		m_BeforeEnhancementDrawer.SetVisibility(true);

	}else{
		//TODO
		m_BeforeEnhancementDrawer.Reset();

	}
	this->m_BeforeEnhancementRenderWindow->Render();

	if(this->m_Project->IsDiffusedImageReady()){
		m_AfterEnhancementDrawer.SetImage(this->m_Project->GetDiffusedImage());
		m_AfterEnhancementDrawer.SetRenderer(this->m_AfterEnhancementRenderer);
		m_AfterEnhancementDrawer.Draw();
		m_AfterEnhancementDrawer.SetVisibility(true);
	}else{
		m_AfterEnhancementDrawer.Reset();
	}
	this->m_AfterEnhancementRenderWindow->Render();
}
void TTTMainWindow::DrawVertex(){
	if(this->m_Project->IsVertexnessImageReady()){

		m_VertexnessImageDrawer.SetImage(this->m_Project->GetVertexnessImage());
		m_VertexnessImageDrawer.SetRenderer(this->m_VertexLocationRenderer);
		m_VertexnessImageDrawer.Draw();
		m_VertexnessImageDrawer.SetVisibility(true);

	}else{
		m_VertexnessImageDrawer.Reset();
	}

	if(this->m_Project->IsVertexLocationsReady()){
		m_VertexLocationsDrawer.SetVertexLocations(this->m_Project->GetVertexLocations());
		m_VertexLocationsDrawer.SetSpacing(m_Project->GetSpacing());
		m_VertexLocationsDrawer.SetRenderer(this->m_VertexLocationRenderer);
		m_VertexLocationsDrawer.Draw();
		m_VertexLocationsDrawer.SetVisibility(true);

	}else{
		m_VertexLocationsDrawer.Reset();
	}

	if(this->m_Project->IsPlatenessImageReady()){
		m_PlatenessDrawerOnVertexLocation.SetImage(this->m_Project->GetPlatenessImage());
		m_PlatenessDrawerOnVertexLocation.SetRenderer(this->m_VertexLocationRenderer);
		m_PlatenessDrawerOnVertexLocation.Draw();
		m_PlatenessDrawerOnVertexLocation.SetVisibility(this->m_pUI->showAdhrensJunctionsAtVertexLocationsCBox->isChecked());
	}else{
		m_PlatenessDrawerOnVertexLocation.Reset();
	}
	this->m_VertexLocationRenderWindow->Render();
}

TTTMainWindow::~TTTMainWindow()
{
    delete m_pUI;
}

void TTTMainWindow::ShowPreferences(){
	//TODO
}

void TTTMainWindow::Open(){
	SQLOpenProjectDialog dialog(m_Host,m_Port,m_DBName,m_User,m_Password);
	dialog.exec();

	if(dialog.IsAccepted()){
		this->m_Project=dialog.GetProject();
		this->m_Project->SetHost(m_Host.toStdString());
		this->m_Project->SetPort(m_Port);
		this->m_Project->SetDBName(m_DBName.toStdString());
		this->m_Project->SetUser(m_User.toStdString());
		this->m_Project->SetPassword(m_Password.toStdString());

		//this->m_Project->Load(path);
		//    m_BasePath = path;
		//  this->m_Project->AddSpacing();
		//  m_Spacing= m_Project->GetSpacing();
		this->m_pUI->stepsTabs->setEnabled(true);
		this->SetupProjectTab();
		this->SetupSliders(this->m_Project->GetNumFrames());
#if 0
		SetupNavigation();
		SetFrame(1);
#endif
	}

}

void TTTMainWindow::New(){

	SQLNewProjectDialog dialog(this);
	dialog.exec();

	if(dialog.IsAccepted()){
		this->m_Project=dialog.GetNewProject();
		this->m_pUI->stepsTabs->setEnabled(true);
#if 0
		SetupNavigation();
		SetFrame(1);
#endif
    }
}

void TTTMainWindow::SetupSliders(int length){
	this->m_pUI->projectSlider->setMinimum(0);
	this->m_pUI->projectSlider->setMaximum(length-1);
	this->m_pUI->cellSegmentationSlider->setMinimum(0);
	this->m_pUI->cellSegmentationSlider->setMaximum(length-1);

	this->m_pUI->membranessSlider->setMinimum(0);
	this->m_pUI->membranessSlider->setMaximum(length-1);

	this->m_pUI->vertexLocationSlider->setMinimum(0);
	this->m_pUI->vertexLocationSlider->setMaximum(length-1);

	this->m_pUI->enhancementSlider->setMinimum(0);
	this->m_pUI->enhancementSlider->setMaximum(length-1);

	this->m_pUI->trackingSlider->setMinimum(0);
	this->m_pUI->trackingSlider->setMaximum(length-1);

	this->m_pUI->tectonicsSlider->setMinimum(0);
	this->m_pUI->tectonicsSlider->setMaximum(length-1);
}
void TTTMainWindow::SetupProjectTab(){

	//1. Setup project view

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(m_Host);
    db.setPort(m_Port);
    db.setDatabaseName(m_DBName);
    db.setUserName(m_User);
    db.setPassword(m_Password);

    bool ok=db.open();

    std::cout << db.lastError().databaseText().toStdString() << std::endl;

    assert(ok);

    ttt::TissueTrackingProject::SpacingType spacing=m_Project->GetSpacing();

    this->m_pUI->xDoubleSpinBox->setValue(spacing[0]);
    this->m_pUI->yDoubleSpinBox->setValue(spacing[1]);
    this->m_pUI->zDoubleSpinBox->setValue(spacing[2]);

    this->m_pUI->incrementDoubleSpinBox->setValue(this->m_Project->GetTemporalScale());

    this->m_pUI->nameLineEdit->setText(QString::fromStdString(this->m_Project->GetProjectName()));


    if(ok){
    	QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this,db);
    	model->setTable("Frame");
    	model->setFilter("idProject=13"); //TODO setup IDProject
    	model->setEditStrategy(QSqlTableModel::OnRowChange);
    	model->select();
    	this->m_pUI->framesTable->setModel(model);
    	this->HighlightProjectFrame(0);

    }
}

void TTTMainWindow::HighlightProjectFrame(int frame){


	RawImageDrawer drawer;
	m_Project->SetFrame(frame);
	drawer.SetImage(m_Project->GetRawImage());
	drawer.SetRenderer(this->m_ProjectRenderer);
	drawer.Draw();
	drawer.SetVisibility(true);
	this->m_ProjectRenderWindow->Render();
	this->m_pUI->framesTable->selectRow(frame);

}


void TTTMainWindow::Close(){
	//TODO
}

void TTTMainWindow::AddFrame(){
	//TODO
}

void TTTMainWindow::RemoveFrame(){
	//TODO
}

void TTTMainWindow::FrameDown(){
	//TODO
}

void TTTMainWindow::FrameUp(){
	//TODO
}


void TTTMainWindow::DoLowestScale(){

	double lowestScale = this->m_pUI->lowestScaleDoubleSpinBox->value();

	PlatenessCommand lowestPlatenessCommand;

	lowestPlatenessCommand.SetSigmaMin(lowestScale);
	lowestPlatenessCommand.SetSigmaMax(lowestScale);
	lowestPlatenessCommand.SetSigmaSteps(1);

	DataCastingCommand caster;

	caster.SetInput(m_Project->GetRawImage());
	caster.Do();

	lowestPlatenessCommand.SetInput(caster.GetOutput());
	lowestPlatenessCommand.Do();


	m_LowestPlatenessDrawer.SetImage(lowestPlatenessCommand.GetPlatenessImage());
	m_LowestPlatenessDrawer.SetRenderer(this->m_LowestScaleSelectionRenderer);
	m_LowestPlatenessDrawer.Draw();
	m_LowestPlatenessDrawer.SetVisibility(true);
}

void TTTMainWindow::DoHighestScale(){
	double highestScale = this->m_pUI->highestScaleDoubleSpinBox->value();

	PlatenessCommand highestPlatenessCommand;

	highestPlatenessCommand.SetSigmaMin(highestScale);
	highestPlatenessCommand.SetSigmaMax(highestScale);
	highestPlatenessCommand.SetSigmaSteps(1);

	DataCastingCommand caster;

	caster.SetInput(m_Project->GetRawImage());
	caster.Do();

	highestPlatenessCommand.SetInput(caster.GetOutput());
	highestPlatenessCommand.Do();


	m_HighestPlatenessDrawer.SetImage(highestPlatenessCommand.GetPlatenessImage());
	m_HighestPlatenessDrawer.SetRenderer(this->m_HighestScaleSelectionRenderer);
	m_HighestPlatenessDrawer.Draw();
	m_HighestPlatenessDrawer.SetVisibility(true);
}

void TTTMainWindow::DoRangeScale(){

	double lowestScale = this->m_pUI->lowestScaleDoubleSpinBox->value();
	double highestScale = this->m_pUI->highestScaleDoubleSpinBox->value();
	int rangeScale = this->m_pUI->stepsSpinBox->value();

	PlatenessCommand rangePlatenessCommand;

	rangePlatenessCommand.SetSigmaMin(lowestScale);
	rangePlatenessCommand.SetSigmaMax(highestScale);
	rangePlatenessCommand.SetSigmaSteps(rangeScale);

	if(this->m_Project->IsDiffusedImageReady()){
		rangePlatenessCommand.SetInput(this->m_Project->GetDiffusedImage());
	}else{
		DataCastingCommand caster;
		caster.SetInput(m_Project->GetRawImage());
		caster.Do();
		rangePlatenessCommand.SetInput(caster.GetOutput());
	}

	rangePlatenessCommand.Do();

	m_Project->SetPlatenessImage(rangePlatenessCommand.GetPlatenessImage());

	this->DrawRangePlateness();
}



void TTTMainWindow::DoAllScale(){
	for(unsigned int i=0;i<m_Project->GetNumFrames();i++){
		m_Project->SetFrame(i);
		this->DoRangeScale();
	}
}



void TTTMainWindow::SetupMembranessFrame(){
	this->m_Project->SetFrame(this->m_pUI->membranessSlider->value());
	this->m_pUI->lowestScaleDoubleSpinBox->setValue(m_Project->GetLowestScale());
	this->m_pUI->highestScaleDoubleSpinBox->setValue(m_Project->GetHighestScale());
	this->m_pUI->stepsSpinBox->setValue(m_Project->GetScaleSteps());
	this->DrawRangePlateness();

}

void TTTMainWindow::SetupEnhancementFrame(){
	this->m_Project->SetFrame(this->m_pUI->enhancementSlider->value());
	this->DrawEnhancement();
}

void TTTMainWindow::SetupVertexFrame(){
	this->m_Project->SetFrame(this->m_pUI->vertexLocationSlider->value());
	this->DrawVertex();
}

void TTTMainWindow::SetupSegmentationFrame(){
	this->m_Project->SetFrame(this->m_pUI->cellSegmentationSlider->value());
	this->DrawSegmentation();
	this->DrawDual();
}
void TTTMainWindow::SetupTrackingFrame(){
	this->m_Project->SetFrame(this->m_pUI->trackingSlider->value());
	this->DrawTracking();
}

void TTTMainWindow::SetupTectonicsFrame(){
	this->m_Project->SetFrame(this->m_pUI->tectonicsSlider->value());
	this->DrawEllipses();
	this->DrawTectonics();
	this->m_TectonicsRenderWindow->Render();

}

void TTTMainWindow::EnhanceAndDraw(){
	this->DoEnhance();
	this->DrawEnhancement();
}

void TTTMainWindow::DoEnhance(){

	double lowestScale = this->m_pUI->lowestScaleDoubleSpinBox->value();
	double highestScale = this->m_pUI->highestScaleDoubleSpinBox->value();
	int rangeScale = this->m_pUI->stepsSpinBox->value();

	int iterations = this->m_pUI->enhanceIterations->value();

	DataCastingCommand caster;

	caster.SetInput(m_Project->GetRawImage());
	caster.Do();

	AnisotropicDiffusionCommand anisotropicDiffusionCommand;

	anisotropicDiffusionCommand.SetDiffusionIterations(iterations);
	anisotropicDiffusionCommand.SetDiffusionSigmaMax(highestScale);
	anisotropicDiffusionCommand.SetDiffusionSigmaMin(lowestScale);
	anisotropicDiffusionCommand.SetDiffusionSigmaSteps(rangeScale);

	anisotropicDiffusionCommand.SetInputImage(caster.GetOutput());
	anisotropicDiffusionCommand.Do();

	this->m_Project->SetDiffusedImage(anisotropicDiffusionCommand.GetOutputImage());

	PlatenessCommand platenessCommand;

	platenessCommand.SetSigmaMin(lowestScale);
	platenessCommand.SetSigmaMax(highestScale);
	platenessCommand.SetSigmaSteps(rangeScale);

	platenessCommand.SetInput(this->m_Project->GetDiffusedImage());
	platenessCommand.Do();

	this->m_Project->SetPlatenessImage(platenessCommand.GetPlatenessImage());


}

void TTTMainWindow::DoAllEnhance(){

	for(unsigned int i=0;i<m_Project->GetNumFrames();i++){
		m_Project->SetFrame(i);
		this->DoEnhance();
	}

}
void TTTMainWindow::VertexnessAndDraw(){
	this->DoVertexness();
	this->DrawVertex();
}
void TTTMainWindow::DoVertexness(){

	double lowestScale = this->m_pUI->lowestScaleDoubleSpinBox->value()*2;
	double highestScale = this->m_pUI->highestScaleDoubleSpinBox->value()*2;
	int rangeScale = this->m_pUI->stepsSpinBox->value();

	DataCastingCommand caster;
	caster.SetInput(m_Project->GetRawImage());
	caster.Do();

	VertexnessCommand vertexnessCommand;

	vertexnessCommand.SetSigmaMax(highestScale);
	vertexnessCommand.SetSigmaMin(lowestScale);
	vertexnessCommand.SetSigmaSteps(rangeScale);
	vertexnessCommand.SetInput(caster.GetOutput());

	if(this->m_Project->IsDiffusedImageReady()){
		vertexnessCommand.SetInput(this->m_Project->GetDiffusedImage());
	}else{
		DataCastingCommand caster;
		caster.SetInput(m_Project->GetRawImage());
		caster.Do();
		vertexnessCommand.SetInput(caster.GetOutput());
	}


	vertexnessCommand.Do();

	m_Project->SetVertexnessImage(vertexnessCommand.GetVertexnessImage());


}
void TTTMainWindow::VertexLocationAndDraw(){
	this->DoVertexLocation();
	this->DrawVertex();
}
void TTTMainWindow::DoVertexLocation(){

	double threshold = this->m_pUI->vertexThresholdSpinBox->value();

	VertexLocationCommand vertexLocationCommand;
	vertexLocationCommand.SetLocalMaxThreshold(threshold);
	vertexLocationCommand.SetVertexnessImage(m_Project->GetVertexnessImage());
	vertexLocationCommand.Do();

	m_Project->SetVertexLocations(vertexLocationCommand.GetLocalMaxima());

}
void TTTMainWindow::VertexSelected(vtkSmartPointer<vtkActor> & actor){
	vtkSmartPointer<vtkCubeSource> cube=vtkSmartPointer<vtkCubeSource>::New();
  	cube->SetBounds(0,100,0,100,0,10);
  	cube->Update();

  	vtkSmartPointer<FollowVertexCallback> myCallback = vtkSmartPointer<FollowVertexCallback>::New();

    vtkSmartPointer<vtkActor> pickedVertexActor=actor;

    myCallback->SetActor(pickedVertexActor);

    vtkSmartPointer<vtkSphereSource> sphereSource=m_VertexLocationsDrawer.GetSphereSourceFromActor(pickedVertexActor);
    myCallback->SetSphereSource(sphereSource);

    ttt::AdherensJunctionVertex::Pointer vertex = m_VertexLocationsDrawer.GetVertexFromActor(pickedVertexActor);


    myCallback->SetVertex(vertex);
    myCallback->SetSpacing(m_Project->GetSpacing());

  	m_PointWidget->AllOn();
  	m_PointWidget->SetInteractor(this->m_VertexLocationRenderWindowInteractor);
  	m_PointWidget->SetInputData(cube->GetOutput());

  	m_PointWidget->PlaceWidget();
  	m_PointWidget->SetPosition(sphereSource->GetCenter());
  	m_PointWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
  	m_PointWidget->On();

  	//m_Project->SetVertexLocationsDirty();

  	this->m_pUI->deleteVertexButton->setEnabled(true);

}

void TTTMainWindow::VertexUnselected(vtkSmartPointer<vtkActor> & actor){
  	m_PointWidget->Off();
  	m_PointWidget->RemoveAllObservers();
  	this->m_pUI->deleteVertexButton->setEnabled(false);
  	this->m_pUI->addVertexButton->setEnabled(true);
  	m_Project->SetVertexLocationsDirty();
}

void TTTMainWindow::VertexAdditionCancelled(){
  	m_PointWidget->Off();
  	m_PointWidget->RemoveAllObservers();
  	this->m_pUI->deleteVertexButton->setEnabled(false);
  	this->m_pUI->addVertexButton->setEnabled(true);
  	m_Project->SetVertexLocationsDirty();
}
void TTTMainWindow::VertexAdded(vtkSmartPointer<vtkActor> & actor){
	vtkSmartPointer<vtkCubeSource> cube=vtkSmartPointer<vtkCubeSource>::New();
	cube->SetBounds(0,100,0,100,0,10);
	cube->Update();

  	vtkSmartPointer<FollowVertexCallback> myCallback = vtkSmartPointer<FollowVertexCallback>::New();

    vtkSmartPointer<vtkActor> pickedVertexActor=actor;

    myCallback->SetActor(pickedVertexActor);

    vtkSmartPointer<vtkSphereSource> sphereSource=m_VertexLocationsDrawer.GetSphereSourceFromActor(pickedVertexActor);
    myCallback->SetSphereSource(sphereSource);

    ttt::AdherensJunctionVertex::Pointer vertex = m_VertexLocationsDrawer.GetVertexFromActor(pickedVertexActor);

    myCallback->SetVertex(vertex);
    myCallback->SetSpacing(m_Project->GetSpacing());

  	m_PointWidget->AllOn();
  	m_PointWidget->SetInteractor(this->m_VertexLocationRenderWindowInteractor);
  	m_PointWidget->SetInputData(cube->GetOutput());

  	m_PointWidget->PlaceWidget();
  	m_PointWidget->SetPosition(sphereSource->GetCenter());
  	m_PointWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
  	m_PointWidget->On();

  	m_Project->SetVertexLocationsDirty();

  	this->m_pUI->deleteVertexButton->setEnabled(true);
}


void TTTMainWindow::SelectVertex(){
	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(m_VertexSelectionInteractor);
	this->m_pUI->addVertexButton->setEnabled(false);
}

void TTTMainWindow::AddVertex(){
	m_VertexAdditionInteractor->SetAdherensJunctionVertices(m_Project->GetVertexLocations());
	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(m_VertexAdditionInteractor);
}

void TTTMainWindow::DeleteVertex(){
	vtkSmartPointer<vtkActor> toRemove=this->m_VertexSelectionInteractor->GetPickedVertexActor();
	if(toRemove){
		m_VertexSelectionInteractor->UnsetSelection();
		ttt::AdherensJunctionVertices::Pointer vertices=this->m_Project->GetVertexLocations();
		ttt::AdherensJunctionVertex::Pointer toRemoveVertex=this->m_VertexLocationsDrawer.GetVertexFromActor(toRemove);
		m_VertexLocationsDrawer.EraseAdherensJunctionVertex(toRemoveVertex);
		ttt::AdherensJunctionVertices::iterator toRemoveIt= std::find(vertices->begin(),vertices->end(),toRemoveVertex);
		vertices->erase(toRemoveIt);
		m_Project->SetVertexLocationsDirty();
		this->m_VertexLocationRenderWindow->Render();
	}

}


void TTTMainWindow::SetupVertexAdditionInteractor(){

	m_VertexAdditionInteractor=vtkSmartPointer<VertexAdditionInteractor>::New();
	m_VertexAdditionInteractor->SetRenderer(this->m_VertexLocationRenderer);


	m_VertexAdditionInteractor->SetVertexLocationsDrawer(&m_VertexLocationsDrawer);

	connect(m_VertexAdditionInteractor,SIGNAL(vertexAdded(vtkSmartPointer<vtkActor> &)),this,SLOT(VertexAdded(vtkSmartPointer<vtkActor>&)));
	connect(m_VertexAdditionInteractor,SIGNAL(additionCancelled()),this,SLOT(VertexAdditionCancelled()));

}

void TTTMainWindow::SetupVertexSelectionInteractor(){

	m_VertexSelectionInteractor=vtkSmartPointer<VertexSelectionInteractor>::New();
	m_VertexSelectionInteractor->SetRenderer(this->m_VertexLocationRenderer);

	connect(m_VertexSelectionInteractor,SIGNAL(vertexSelected(vtkSmartPointer<vtkActor> & )),this,SLOT(VertexSelected(vtkSmartPointer<vtkActor>&)));
	connect(m_VertexSelectionInteractor,SIGNAL(vertexUnselected(vtkSmartPointer<vtkActor> & )),this,SLOT(VertexUnselected(vtkSmartPointer<vtkActor>&)));
}

void TTTMainWindow::SetupEdgeSelectionInteractor(){
	m_EdgeSelectionInteractor=vtkSmartPointer<EdgeSelectionInteractor>::New();
	m_EdgeSelectionInteractor->SetRenderer(this->m_CellSegmentationRenderer);

	connect(m_EdgeSelectionInteractor,SIGNAL(edgeSelected(vtkSmartPointer<vtkActor> &)),this,SLOT(EdgeSelected(vtkSmartPointer<vtkActor> &)));
	connect(m_EdgeSelectionInteractor,SIGNAL(edgeUnselected(vtkSmartPointer<vtkActor> &)),this,SLOT(EdgeUnselected(vtkSmartPointer<vtkActor>&)));
}
void TTTMainWindow::SetupEdgeAdditionInteractor(){
	m_EdgeAdditionInteractor=vtkSmartPointer<EdgeAdditionInteractor>::New();
	m_EdgeAdditionInteractor->SetRenderer(this->m_CellSegmentationRenderer);
	connect(m_EdgeAdditionInteractor,SIGNAL(edgeAdded(vtkSmartPointer<vtkActor> &, vtkSmartPointer<vtkActor> & )),this,SLOT(EdgeAdded(vtkSmartPointer<vtkActor>&,vtkSmartPointer<vtkActor>&)));
	connect(m_EdgeAdditionInteractor,SIGNAL(edgeAdditionCancelled()),this,SLOT(EdgeAdditionCancelled()));
}
void TTTMainWindow::DoAllVertex(){
	for(unsigned int i=0;i<m_Project->GetNumFrames();i++){
		m_Project->SetFrame(i);
		this->DoVertexness();
		this->DoVertexLocation();
	}
}

void TTTMainWindow::SegmentationAndDraw(){
	this->DoSegmentation();
	this->DrawSegmentation();
}
void TTTMainWindow::ShowPlatenessOnSegmentation(int show){
	m_PlatenessDrawerOnSegmentation.SetVisibility(show!=0);
	this->m_CellSegmentationRendererWindow->Render();
}
void TTTMainWindow::ShowPlatenessOnVertexLocation(int show){
	m_PlatenessDrawerOnVertexLocation.SetVisibility(show!=0);
	this->m_VertexLocationRenderWindow->Render();
}
void TTTMainWindow::ShowDualGraphOnCellSegmentation(int show){
	m_DualGraphDrawer.SetVisibility(show!=0);
	this->m_CellSegmentationRendererWindow->Render();
}
void TTTMainWindow::ShowTrackedEllipsesAtTectonics(int show){
	m_EllipseDrawer.SetVisibility(show!=0);
	this->m_TectonicsRenderWindow->Render();
}
void TTTMainWindow::DrawSegmentation(){

	if(this->m_Project->IsTissueDescriptorReady()){
		m_PrimalGraphDrawer.SetRenderer(this->m_CellSegmentationRenderer);
		m_PrimalGraphDrawer.SetVertexColorer(&m_PrimalGraphVertexColorer);
		m_PrimalGraphDrawer.SetEdgeColorer(&m_PrimalGraphEdgeColorer);
		m_PrimalGraphDrawer.SetTissueDescriptor(m_Project->GetTissueDescriptor());
		m_PrimalGraphDrawer.Draw();
		m_PrimalGraphDrawer.SetVisibility(true);

		m_PlatenessDrawerOnSegmentation.SetRenderer(this->m_CellSegmentationRenderer);
		m_PlatenessDrawerOnSegmentation.SetImage(m_Project->GetPlatenessImage());
		m_PlatenessDrawerOnSegmentation.Draw();
		m_PlatenessDrawerOnSegmentation.SetVisibility(this->m_pUI->showAdherensJunctionsAtSegmentationCBox->isChecked());
	}else{
		m_PrimalGraphDrawer.Reset();
		m_PlatenessDrawerOnSegmentation.Reset();
	}

	this->m_CellSegmentationRendererWindow->Render();

}
void TTTMainWindow::DoSegmentation(){

	AdherensJunctionSegmentationDijkstraCommand segmentationCommand;


	segmentationCommand.SetVertexLocations(m_Project->GetVertexLocations());
	segmentationCommand.SetVertexnessImage(m_Project->GetVertexnessImage());
	segmentationCommand.SetPlatenessImage(m_Project->GetPlatenessImage());
	segmentationCommand.Do();

	this->m_Project->SetTissueDescriptor(segmentationCommand.GetTissueDescriptor());
}
void TTTMainWindow::SelectEdge(){
	this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_EdgeSelectionInteractor);
	this->m_pUI->addEdgeButton->setEnabled(false);
	this->m_pUI->selectEdgeButton->setEnabled(false);
}
void TTTMainWindow::AddEdge(){
	m_EdgeAdditionInteractor->SetDrawer(&m_PrimalGraphDrawer);
	this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_EdgeAdditionInteractor);
	this->m_pUI->selectEdgeButton->setEnabled(false);
	this->m_pUI->deleteEdgeButton->setEnabled(false);
}

void TTTMainWindow::DeleteEdge(){

	vtkSmartPointer<vtkActor> selected=this->m_EdgeSelectionInteractor->GetSelectedEdgeActor();
	if(selected){
		ttt::SkeletonEdgeType toDelete = m_PrimalGraphDrawer.GetActorSkeletonEdge(selected);
		ttt::TissueDescriptor::Pointer descriptor=this->m_Project->GetTissueDescriptor();

		boost::remove_edge(toDelete,*descriptor->m_SkeletonGraph);

		m_Project->SetTissueDescriptor(descriptor);

		this->DrawSegmentation();
		this->m_pUI->addEdgeButton->setEnabled(true);
		this->m_pUI->selectEdgeButton->setEnabled(true);
		this->m_pUI->deleteEdgeButton->setEnabled(false);

		this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_EdgeStandardInteractorStyle);
		this->m_CellSegmentationRenderWindowInteractor->Initialize();
	}

}

void TTTMainWindow::EdgeSelected(vtkSmartPointer<vtkActor>&){
	this->m_pUI->deleteEdgeButton->setEnabled(true);


}

void TTTMainWindow::EdgeUnselected(vtkSmartPointer<vtkActor>&){
	this->m_pUI->deleteEdgeButton->setEnabled(false);


}
void TTTMainWindow::EdgeAdded(vtkSmartPointer<vtkActor>& a, vtkSmartPointer<vtkActor> & b){
	//TODO

	ttt::SkeletonVertexType aVertex= m_PrimalGraphDrawer.GetActorSkeletonVertex(a);
	ttt::SkeletonVertexType bVertex= m_PrimalGraphDrawer.GetActorSkeletonVertex(b);

	boost::add_edge(aVertex,bVertex,*this->m_Project->GetTissueDescriptor()->m_SkeletonGraph);

	this->m_Project->SetTissueDescriptorDirty();

	this->DrawSegmentation();

	this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_EdgeStandardInteractorStyle);
	this->m_CellSegmentationRenderWindowInteractor->Initialize();


	this->m_pUI->selectEdgeButton->setEnabled(true);

}
void TTTMainWindow::EdgeAdditionCancelled(){
	//TODO
}
void TTTMainWindow::DualAndDraw(){
	this->DoDual();
	this->DrawDual();
}
void TTTMainWindow::DoDual(){
	CellGraphCommand cellGraphCommand;

	cellGraphCommand.SetPrimalGraph(this->m_Project->GetTissueDescriptor());
	cellGraphCommand.Do();

	this->m_Project->SetTissueDescriptor(cellGraphCommand.GetGraphs());
}

void TTTMainWindow::DrawDual(){

	if(this->m_Project->IsTissueDescriptorReady()){
		m_DualGraphDrawer.SetRenderer(this->m_CellSegmentationRenderer);
		m_DualGraphDrawer.SetVertexColorer(&m_DualGraphVertexColorer);
		m_DualGraphDrawer.SetEdgeColorer(&m_DualGraphEdgeColorer);
		m_DualGraphDrawer.SetTissueDescriptor(this->m_Project->GetTissueDescriptor());
		m_DualGraphDrawer.Draw();
		m_DualGraphDrawer.SetVisibility(this->m_pUI->showCellGraphAtSegmentationCBox->isChecked());
	}

}


void TTTMainWindow::DoTracking(){

	typename TissueTrackingProject::RawImageType::SizeType size=m_Project->GetRawImage()->GetLargestPossibleRegion().GetSize();
	typename TissueTrackingProject::RawImageType::SpacingType spacing = m_Project->GetSpacing();
	double xMax=size[0]*spacing[0];
	double yMax=size[1]*spacing[1];
	double zMax=size[2]*spacing[2];

	TrackingCommand trackingCommand;

	trackingCommand.SetXMin(0);
	trackingCommand.SetXMax(xMax);
	trackingCommand.SetYMin(0);
	trackingCommand.SetYMax(yMax);
	trackingCommand.SetZMin(0);
	trackingCommand.SetZMax(zMax);
	;
	int numFrames= m_Project->GetNumFrames();

	std::vector<TissueDescriptor::Pointer> observations;
	observations.resize(numFrames);

	for(int i=0;i<numFrames;i++){
		m_Project->SetFrame(i);
		observations[i]=m_Project->GetTissueDescriptor();
	}

	trackingCommand.SetObservedTissues(observations);
	trackingCommand.Do();

	std::vector<TrackedTissueDescriptor::Pointer> tracked =trackingCommand.GetTrackedTissue();

	int frame=0;
	for(std::vector<TrackedTissueDescriptor::Pointer>::iterator it = tracked.begin(); it!=tracked.end();it++){

		m_Project->SetFrame(frame++);
		m_Project->SetTrackedTissueDescriptor(*it);
	}
	this->m_Project->SetFrame(0);
	this->DrawTracking();
}


void TTTMainWindow::DrawTracking(){

	m_TrackingDrawer.SetTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	m_TrackingDrawer.SetRenderer(this->m_TrackingRenderer);
	//m_TrackingDrawer.SetEdgeColorer(this->m_TrackingEdgeColorer);

	TrackedCellId::Pointer idFeature=TrackedCellId::New();
	idFeature->SetTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	idFeature->Compute();
	//idFeature-(m_Project->GetTrackedTissueDescriptor());
	//idFeature->Compute();
	m_TrackingVertexColorer.SetFeature(idFeature);

	//m_TrackingDrawer.SetVertexColorer(&m_TrackingVertexColorer);
	//m_TrackingDrawer.SetEdgeColorer(&m_TrackingEdgeColorer);
	m_TrackingDrawer.Draw();

	m_TrackingDrawer.SetVisibility(true);

	m_PrimalGraphTrackingDrawer.SetRenderer(this->m_TrackingRenderer);
	m_PrimalGraphTrackingDrawer.SetTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	m_PrimalGraphTrackingDrawer.SetVertexColorer(&m_PrimalGraphVertexColorer);
	m_PrimalGraphTrackingDrawer.SetEdgeColorer(&m_PrimalGraphEdgeColorer);
	m_PrimalGraphTrackingDrawer.Draw();
	m_PrimalGraphTrackingDrawer.SetVisibility(true);

	this->m_TrackingRendererWindow->Render();

}



void TTTMainWindow::DoEllipses(){
	EllipsesCommand<TrackedTissueDescriptor> command;
	assert(m_Project->GetTrackedTissueDescriptor());
	command.SetTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	command.Do();

	this->m_Project->SetTrackedEllipses(command.GetEllipses());
	this->DrawEllipses();
}
void TTTMainWindow::DrawEllipses(){
#if 0
	ttt::PrimalGraphDrawer<ttt::TrackedTissueDescriptor> primalDrawer;

	primalDrawer.SetTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	primalDrawer.SetVertexColorer(&this->m_PrimalGraphVertexColorer);
	primalDrawer.SetEdgeColorer(&this->m_PrimalGraphEdgeColorer);

	primalDrawer.SetRenderer(this->m_TectonicsRenderer);
	primalDrawer.Draw();
	primalDrawer.SetVisibility(true);
#endif

	if(this->m_Project->IsTrackedEllipsesReady()){
		m_EllipseDrawer.SetEllipses(this->m_Project->GetTrackedEllipses());

		m_EllipseDrawer.SetRenderer(this->m_TectonicsRenderer);
		m_EllipseDrawer.Draw();
		m_EllipseDrawer.SetVisibility(this->m_pUI->showEllipsesCBox->isChecked());

	}else{
		m_EllipseDrawer.Reset();
	}
	this->m_TectonicsRenderWindow->Render();

}

void TTTMainWindow::ShowCellStrainRates(){
	m_DomainStrainRatesDrawer.SetDrawModeToCellSRT();
	m_DomainStrainRatesDrawer.Draw();
	m_DomainStrainRatesDrawer.Show();
	this->m_TectonicsRenderWindow->Render();
}

void TTTMainWindow::ShowTissueStrainRates(){
	m_DomainStrainRatesDrawer.SetDrawModeToTissueSRT();
	m_DomainStrainRatesDrawer.Draw();
	m_DomainStrainRatesDrawer.Show();
	this->m_TectonicsRenderWindow->Render();
}

void TTTMainWindow::ShowIntercalationStrainRates(){
	m_DomainStrainRatesDrawer.SetDrawModeToIntercalationSRT();
	m_DomainStrainRatesDrawer.Draw();
	m_DomainStrainRatesDrawer.Show();
	this->m_TectonicsRenderWindow->Render();
}

void TTTMainWindow::DoTectonics(){

	ComputeDomainsCommand domainsCommand;

	unsigned int order= this->m_pUI->domainOrderSpinBox->value();
	domainsCommand.SetOrder(order);
	domainsCommand.SetTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	domainsCommand.Do();
	m_Project->SetTrackedDomains(domainsCommand.GetDomains());
	TectonicsCommand tectonicsCommand;
	tectonicsCommand.SetDomains(domainsCommand.GetDomains());

	int currentFrame=this->m_Project->GetFrame();


	if(currentFrame==0){
		tectonicsCommand.SetCurrentTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
		tectonicsCommand.SetEllipsesCurrent(m_Project->GetTrackedEllipses());

		m_Project->SetFrame(currentFrame+1);

		tectonicsCommand.SetEllipsesNext(m_Project->GetTrackedEllipses());
		tectonicsCommand.SetNextTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());

	}else if(currentFrame > 0 && currentFrame < m_Project->GetNumFrames()-1){

		tectonicsCommand.SetCurrentTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
		tectonicsCommand.SetEllipsesCurrent(m_Project->GetTrackedEllipses());

		m_Project->SetFrame(currentFrame-1);

		tectonicsCommand.SetEllipsesPrevious(m_Project->GetTrackedEllipses());
		tectonicsCommand.SetPreviousTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());

		m_Project->SetFrame(currentFrame+1);
		tectonicsCommand.SetNextTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
		tectonicsCommand.SetEllipsesNext(m_Project->GetTrackedEllipses());

	}else if(currentFrame == m_Project->GetNumFrames()-1){

		tectonicsCommand.SetCurrentTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
		tectonicsCommand.SetEllipsesCurrent(m_Project->GetTrackedEllipses());
		m_Project->SetFrame(currentFrame-1);
		tectonicsCommand.SetPreviousTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
		tectonicsCommand.SetEllipsesPrevious(m_Project->GetTrackedEllipses());

	}
	m_Project->SetFrame(currentFrame);

	tectonicsCommand.Do();

	m_Project->SetDomainStrainRates(tectonicsCommand.GetDomainStrainRates());
	this->DrawTectonics();

}
void TTTMainWindow::DrawTectonics(){
	if(this->m_Project->IsDomainStrainRatesReady()){
		m_DomainStrainRatesDrawer.SetRenderer(this->m_TectonicsRenderer);
		m_DomainStrainRatesDrawer.SetDomainStrainRates(this->m_Project->GetDomainStrainRates());
		m_DomainStrainRatesDrawer.SetTrackedTissueDescriptor(this->m_Project->GetTrackedTissueDescriptor());
		m_DomainStrainRatesDrawer.Draw();
		m_DomainStrainRatesDrawer.SetVisibility(true);
	}else{
		m_DomainStrainRatesDrawer.Reset();
	}
	this->m_TectonicsRenderWindow->Render();



}
void TTTMainWindow::LaunchProjectExplorer(){
	TTTExplorer explorer;
	explorer.SetProject(this->m_Project);
	explorer.exec();
}
void TTTMainWindow::LaunchPreferences(){
	TTTPreferences preferences;

	preferences.SetHost(m_Host);
	preferences.SetPort(m_Port);
	preferences.SetDBName(m_DBName);
	preferences.SetUser(m_User);
	preferences.SetPassword(m_Password);

	preferences.exec();

	m_Host=preferences.GetHost();
	m_Port=preferences.GetPort();
	m_DBName=preferences.GetDBName();
	m_User=preferences.GetUser();
	m_Password=preferences.GetPassword();
	SaveSettings();
}

void TTTMainWindow::LoadSettings(){

	QSettings settings(m_SettingsFile, QSettings::NativeFormat);

	m_Host = settings.value("host",QString("localhost")).toString();
	m_Port = settings.value("port",3306).toUInt();
	m_DBName = settings.value("dbname",QString("TuftsTissueTracker")).toString();
	m_User = settings.value("user",QString("foo")).toString();
	m_Password =settings.value("password",QString("bar")).toString();
}

void TTTMainWindow::SaveSettings()
{

    QSettings settings(m_SettingsFile, QSettings::NativeFormat);
    settings.setValue("host", m_Host);
    settings.setValue("port",m_Port);
    settings.setValue("dbname",m_DBName);
    settings.setValue("user",m_User);
    settings.setValue("password",m_Password);

}
