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
#include <QFileDialog>
#include "CellMomentCalculator.h"
#include "tttexplorer.h"
#include "tttpreferences.h"

#include "VertexnessCommand.h"
#include "PyramidVertexnessCommand.h"
#include "PlatenessCommand.h"
#include "DataCastingCommand.h"
#include "AnisotropicDiffusionCommand.h"
#include "VertexLocationCommand.h"
#include "PyramidVertexLocationCommand.h"
#include "AdherensJunctionSegmentationDijkstraCommand.h"
#include "CellGraphCommand.h"
#include "TrackingCommand.h"
#include "TrackInitializationCommand.h"
#include "MinCostMaxFlowTrackAssociationCommand.h"
#include "EllipsesCommand.h"
#include "ComputeDomainsCommand.h"
#include "TectonicsCommand.h"
#include <vtkAxesActor.h>
#include <vtkCubeSource.h>

#include "CellFeatureTableModel.h"
#include "ProjectTableModel.h"
#include "jsontissuetrackingproject2.h"
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
	//m_VertexLocationRenderer->SetBackground(81.0/255,87.0/255,110.0/255);
	m_VertexLocationRenderer->SetBackground(0,0,0);

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
	//m_CellSegmentationRenderer->SetBackground(81.0/255,87.0/255,110.0/255);
	m_CellSegmentationRenderer->SetBackground(0.0,0.0,0.0);

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
		m_PreviousTrackingRenderer = vtkSmartPointer<vtkRenderer>::New();
		m_PreviousTrackingRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

		m_PreviousTrackingRendererWindow = vtkSmartPointer<vtkRenderWindow>::New();
		m_PreviousTrackingRendererWindow->AddRenderer(m_PreviousTrackingRenderer);
		m_PreviousTrackingRendererWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

		m_PreviousTrackingRendererWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
		m_PreviousTrackingRendererWindowInteractor->SetRenderWindow(m_PreviousTrackingRendererWindow);
		m_PreviousTrackingRendererWindowInteractor->Initialize();

		vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    	m_PreviousTrackingRenderer->AddActor(axes);

    	this->m_pUI->previousTrackingQVTKView->SetRenderWindow(m_PreviousTrackingRendererWindow);

	}
	{
		m_CurrentTrackingRenderer = vtkSmartPointer<vtkRenderer>::New();
		m_CurrentTrackingRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

		m_CurrentTrackingRendererWindow = vtkSmartPointer<vtkRenderWindow>::New();
		m_CurrentTrackingRendererWindow->AddRenderer(m_CurrentTrackingRenderer);
		m_CurrentTrackingRendererWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

		m_CurrentTrackingRendererWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
		m_CurrentTrackingRendererWindowInteractor->SetRenderWindow(m_CurrentTrackingRendererWindow);
		m_CurrentTrackingRendererWindowInteractor->Initialize();

		vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

		m_CurrentTrackingRenderer->AddActor(axes);

		this->m_pUI->currentTrackingQVTKView->SetRenderWindow(m_CurrentTrackingRendererWindow);

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
    connect(this->m_pUI->actionOpenFromMYSQL, SIGNAL(triggered()), this, SLOT(OpenMySQL()));
    connect(this->m_pUI->actionOpenFromJSON, SIGNAL(triggered()), this, SLOT(OpenJSON()));
    connect(this->m_pUI->actionClose, SIGNAL(triggered()), this, SLOT(Close()));


    connect(this->m_pUI->actionFrameDown, SIGNAL(triggered()), this, SLOT(FrameDown()));
    connect(this->m_pUI->actionFrameUp, SIGNAL(triggered()), this, SLOT(FrameUp()));
    connect(this->m_pUI->actionAddFrame, SIGNAL(triggered()), this, SLOT(AddFrame()));
    connect(this->m_pUI->actionRemoveFrame, SIGNAL(triggered()), this, SLOT(RemoveFrame()));


    connect(this->m_pUI->platenessLowestScalePreview,SIGNAL(clicked()),this,SLOT(DoLowestScale()));
    connect(this->m_pUI->platenessHighestScalePreview,SIGNAL(clicked()),this,SLOT(DoHighestScale()));
    connect(this->m_pUI->platenessScaleStepsPreview,SIGNAL(clicked()),this,SLOT(DoRangeScale()));
    connect(this->m_pUI->computeAllMembranessButton,SIGNAL(clicked()),this,SLOT(DoAllScale()));

    connect(this->m_pUI->stepsTabs,SIGNAL(currentChanged(int)),this,SLOT(SetupCurrentTab()));

    connect(this->m_pUI->projectSlider,SIGNAL(valueChanged(int)),this,SLOT(HighlightProjectFrame(int)));
    connect(this->m_pUI->membranessSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupMembranessFrame(int)));
    connect(this->m_pUI->enhancementSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupEnhancementFrame(int)));
    connect(this->m_pUI->vertexLocationSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupVertexFrame(int)));
    connect(this->m_pUI->cellSegmentationSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupSegmentationFrame(int)));
    connect(this->m_pUI->trackingSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupTrackingFrame(int)));

    connect(this->m_pUI->tectonicsSlider,SIGNAL(valueChanged(int)),this,SLOT(SetupTectonicsFrame(int)));

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
    connect(this->m_pUI->linkVerticesButton,SIGNAL(clicked()),this,SLOT(AddEdge()));
    connect(this->m_pUI->addVertexToPrimalButton,SIGNAL(clicked()),this,SLOT(AddVertexToPrimal()));
    connect(this->m_pUI->deleteEdgeButton,SIGNAL(clicked()),this,SLOT(DeleteEdge()));


    connect(this->m_pUI->dualButton,SIGNAL(clicked()),this,SLOT(DualAndDraw()));

    connect(this->m_pUI->trackButton,SIGNAL(clicked()),this,SLOT(DoTracking()));
    connect(this->m_pUI->actionShowProjectExplorer,SIGNAL(triggered()),this,SLOT(LaunchProjectExplorer()));

    connect(this->m_pUI->actionPreferences,SIGNAL(triggered()),this,SLOT(LaunchPreferences()));

    connect(this->m_pUI->platenessLowestScaleDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(PlatenessLowestScaleChanged(double)));
    connect(this->m_pUI->platenessHighestScaleDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(PlatenessHighestScaleChanged(double)));
    connect(this->m_pUI->platenessStepsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(PlatenessScaleStepsChanged(int)));

    connect(this->m_pUI->nameLineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(ProjectNameChanged(const QString&)));
    connect(this->m_pUI->xDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SpacingXChanged(double)));
    connect(this->m_pUI->yDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SpacingYChanged(double)));
    connect(this->m_pUI->zDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SpacingZChanged(double)));


    connect(this->m_pUI->showAdherensJunctionsAtSegmentationCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowPlatenessOnSegmentation(int)));
    connect(this->m_pUI->showAdhrensJunctionsAtVertexLocationsCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowPlatenessOnVertexLocation(int)));
    connect(this->m_pUI->showVertexLocationsAtVertexLocationsCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowVertexLocationsOnVertexLocation(int)));
    connect(this->m_pUI->showAJGraphAtSegmentationCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowAJGraphOnCellSegmentation(int)));
    connect(this->m_pUI->showVertexnessAtVertexLocationsCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowVertexnessOnVertexLocation(int)));
    connect(this->m_pUI->showCellGraphAtSegmentationCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowDualGraphOnCellSegmentation(int)));
    connect(this->m_pUI->showEllipsesCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowTrackedEllipsesAtTectonics(int)));

    connect(this->m_pUI->showCellStrainRateRadioButton,SIGNAL(clicked()),this,SLOT(ShowCellStrainRates()));
    connect(this->m_pUI->showTissueStrainRateRadioButton,SIGNAL(clicked()),this,SLOT(ShowTissueStrainRates()));
    connect(this->m_pUI->showIntercalationStrainRateRadioButton,SIGNAL(clicked()),this,SLOT(ShowIntercalationStrainRates()));

    connect(this->m_pUI->computeEllipse,SIGNAL(clicked()),this,SLOT(DoEllipses()));

    connect(this->m_pUI->computeStrainRates,SIGNAL(clicked()),this,SLOT(DoTectonics()));

    connect(this->m_pUI->actionSave,SIGNAL(triggered()),this,SLOT(Save()));

    this->SetupVertexStandardInteractor();
    this->SetupVertexSelectionInteractor();
    this->SetupVertexAdditionInteractor();
    this->SetupVertexAdditionToPrimalInteractor();
    this->SetupEdgeSelectionInteractor();
    this->SetupEdgeAdditionInteractor();

    m_PrimalGraphVertexPointWidget = vtkSmartPointer<vtkPointWidget>::New();
    m_VertexLocationPointWidget = vtkSmartPointer<vtkPointWidget>::New();

    m_SettingsFile = QDir::homePath() + QString("/.ttttracker.rc");
    //m_SettingsFile = QApplication::applicationDirPath().left(1) + ":/demosettings.ini";
    std::cout << m_SettingsFile.toStdString() << std::endl;
    this->LoadSettings();
    //m_SettingsFile("~/.ttttracker.rc");
}

void TTTMainWindow::Save(){
	this->m_Project->Flush();
}

void TTTMainWindow::SamplingRateChanged(double rate){
	this->m_Project->SetSamplingPeriod(rate);
}
void TTTMainWindow::ProjectNameChanged(const QString & name){
	this->m_Project->SetProjectName(name.toStdString());
}
void TTTMainWindow::SpacingXChanged(double value){
	this->m_Project->SetSpacing(0,value);
}
void TTTMainWindow::SpacingYChanged(double value){
	this->m_Project->SetSpacing(1,value);
}
void TTTMainWindow::SpacingZChanged(double value){
	this->m_Project->SetSpacing(2,value);
}
void TTTMainWindow::PlatenessLowestScaleChanged(double scale){
	m_Project->SetLowestPlatenessScale(this->m_CurrentFrame,scale);
}
void TTTMainWindow::PlatenessHighestScaleChanged(double scale){
	m_Project->SetHighestPlatenessScale(this->m_CurrentFrame,scale);
}
void TTTMainWindow::PlatenessScaleStepsChanged(int steps){
	m_Project->SetPlatenessSteps(this->m_CurrentFrame,steps);
}

void TTTMainWindow::VertexnessLowestScaleChanged(double scale){
	m_Project->SetLowestVertexnessScale(this->m_CurrentFrame,scale);
}
void TTTMainWindow::VertexnessHighestScaleChanged(double scale){
	m_Project->SetHighestVertexnessScale(this->m_CurrentFrame,scale);
}
void TTTMainWindow::VertexnessScaleStepsChanged(int steps){
	m_Project->SetVertexnessSteps(this->m_CurrentFrame,steps);
}


void TTTMainWindow::SetupCurrentTab(){
	int selectedPage= this->m_pUI->stepsTabs->currentIndex();
	std::cout << "Selected: " << selectedPage << std::endl;

	switch(selectedPage){
	case 0:
		break;
	case 1:
		DrawRangePlateness();
		break;
	case 2:
		DrawEnhancement();
		break;
	case 3:
		DrawVertex();
		break;
	case 4:
		this->DrawSegmentation();
		//this->DrawDual();
		break;
	case 5:
		this->DrawTracking();
		break;
	case 6:
		this->DrawTrackingInteractive();
		break;
	case 7:
		this->DrawEllipses();
		this->DrawTectonics();
		break;
	case 8:
		this->DrawInspection();
		break;
	};
}

void TTTMainWindow::DrawRangePlateness(){
	if(this->m_Project->IsPlatenessImageAvailable(this->m_CurrentFrame)){
		m_RangePlatenessDrawer.SetImage(m_Project->GetPlatenessImage(this->m_CurrentFrame));
		m_RangePlatenessDrawer.SetRenderer(this->m_RangeScaleSelectionRenderer);
		m_RangePlatenessDrawer.Draw();
		m_RangePlatenessDrawer.SetVisibility(true);
		this->m_RangeScaleSelectionRenderWindow->Render();
	}else{
		m_RangePlatenessDrawer.Reset();
	}
}

void TTTMainWindow::DrawEnhancement(){
	if(this->m_Project->IsRawImageAvailable(m_CurrentFrame)){
		m_BeforeEnhancementDrawer.SetImage(m_Project->GetRawImage(m_CurrentFrame));
		m_BeforeEnhancementDrawer.SetRenderer(this->m_BeforeEnhancementRenderer);
		m_BeforeEnhancementDrawer.Draw();
		m_BeforeEnhancementDrawer.SetVisibility(true);

	}else{
		//TODO
		m_BeforeEnhancementDrawer.Reset();

	}
	this->m_BeforeEnhancementRenderWindow->Render();

	if(this->m_Project->IsDiffusedImageAvailable(m_CurrentFrame)){
		m_AfterEnhancementDrawer.SetImage(this->m_Project->GetDiffusedImage(m_CurrentFrame));
		m_AfterEnhancementDrawer.SetRenderer(this->m_AfterEnhancementRenderer);
		m_AfterEnhancementDrawer.Draw();
		m_AfterEnhancementDrawer.SetVisibility(true);
	}else{
		m_AfterEnhancementDrawer.Reset();
	}
	this->m_AfterEnhancementRenderWindow->Render();
}
void TTTMainWindow::DrawVertex(){
	if(this->m_Project->IsVertexnessImageAvailable(m_CurrentFrame)){

		m_VertexnessImageDrawer.SetImage(this->m_Project->GetVertexnessImage(m_CurrentFrame));
		m_VertexnessImageDrawer.SetRenderer(this->m_VertexLocationRenderer);
		m_VertexnessImageDrawer.Draw();
		m_VertexnessImageDrawer.SetVisibility(this->m_pUI->showVertexnessAtVertexLocationsCBox);

	}else{
		m_VertexnessImageDrawer.Reset();
	}

	if(this->m_Project->IsAdherensJunctionVerticesAvailable(m_CurrentFrame)){
		m_DrawnAJVertices=this->m_Project->GetAdherensJunctionVertices(m_CurrentFrame);
		m_VertexLocationsDrawer.SetVertexLocations(m_DrawnAJVertices);
		m_VertexLocationsDrawer.SetSpacing(m_Project->GetSpacing());
		m_VertexLocationsDrawer.SetRenderer(this->m_VertexLocationRenderer);
		m_VertexLocationsDrawer.Draw();
		m_VertexLocationsDrawer.SetVisibility(this->m_pUI->showVertexLocationsAtVertexLocationsCBox->isChecked());


	}else{
		m_VertexLocationsDrawer.Reset();
	}

	if(this->m_Project->IsPlatenessImageAvailable(m_CurrentFrame)){
		m_PlatenessDrawerOnVertexLocation.SetImage(this->m_Project->GetPlatenessImage(m_CurrentFrame));
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

void TTTMainWindow::OpenMySQL(){
	SQLOpenProjectDialog dialog(m_Host,m_Port,m_DBName,m_User,m_Password);
	dialog.exec();

	if(dialog.IsAccepted()){
		this->m_Project=std::shared_ptr<ttt::TissueTrackingAbstractProject2> (dialog.GetProject());
		//this->m_Project->SetHost(m_Host.toStdString());
		//this->m_Project->SetPort(m_Port);
		//this->m_Project->SetDBName(m_DBName.toStdString());
		//this->m_Project->SetUser(m_User.toStdString());
		//this->m_Project->SetPassword(m_Password.toStdString());

		//this->m_Project->Load(path);
		//    m_BasePath = path;
		//  this->m_Project->AddSpacing();
		//  m_Spacing= m_Project->GetSpacing();
		this->m_pUI->stepsTabs->setEnabled(true);
		this->SetupSliders(this->m_Project->GetNumFrames());
		this->SetupProjectTab();

	}
}
void TTTMainWindow::OpenJSON(){
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setViewMode(QFileDialog::Detail);
	if(dialog.exec()){
		QString dir=dialog.selectedFiles()[0];

		ttt::JSONTissueTrackingProject2 * project = new ttt::JSONTissueTrackingProject2;

		project->SetDirectory(dir.toStdString());
		this->m_Project=std::shared_ptr<ttt::TissueTrackingAbstractProject2> (project);
		this->m_Project->Open();

		this->m_pUI->stepsTabs->setEnabled(true);
 		this->SetupSliders(this->m_Project->GetNumFrames());
		this->SetupProjectTab();

	}
}

void TTTMainWindow::New(){

	SQLNewProjectDialog dialog(this);
	dialog.exec();

	if(dialog.IsAccepted()){
		this->m_Project=std::shared_ptr<ttt::TissueTrackingAbstractProject2>(dialog.GetNewProject());
		this->m_pUI->stepsTabs->setEnabled(true);
    }
}

void TTTMainWindow::SetupSliders(unsigned int length){
	this->m_pUI->projectSlider->blockSignals(true);
	this->m_pUI->projectSlider->setMinimum(0);
	this->m_pUI->projectSlider->setMaximum(length-1);
	this->m_pUI->projectSlider->blockSignals(false);

	this->m_pUI->cellSegmentationSlider->blockSignals(true);
	this->m_pUI->cellSegmentationSlider->setMinimum(0);
	this->m_pUI->cellSegmentationSlider->setMaximum(length-1);
	this->m_pUI->cellSegmentationSlider->blockSignals(false);

	this->m_pUI->membranessSlider->blockSignals(true);
	this->m_pUI->membranessSlider->setMinimum(0);
	this->m_pUI->membranessSlider->setMaximum(length-1);
	this->m_pUI->membranessSlider->blockSignals(false);

	this->m_pUI->vertexLocationSlider->blockSignals(true);
	this->m_pUI->vertexLocationSlider->setMinimum(0);
	this->m_pUI->vertexLocationSlider->setMaximum(length-1);
	this->m_pUI->vertexLocationSlider->blockSignals(false);

	this->m_pUI->enhancementSlider->blockSignals(true);
	this->m_pUI->enhancementSlider->setMinimum(0);
	this->m_pUI->enhancementSlider->setMaximum(length-1);
	this->m_pUI->enhancementSlider->blockSignals(false);

	this->m_pUI->trackingSlider->blockSignals(true);
	this->m_pUI->trackingSlider->setMinimum(0);
	this->m_pUI->trackingSlider->setMaximum(length-1);
	this->m_pUI->trackingSlider->blockSignals(false);

	this->m_pUI->tectonicsSlider->blockSignals(true);
	this->m_pUI->tectonicsSlider->setMinimum(0);
	this->m_pUI->tectonicsSlider->setMaximum(length-1);
	this->m_pUI->tectonicsSlider->blockSignals(false);


}
void TTTMainWindow::SetupProjectTab(){

    typename ttt::TissueTrackingAbstractProject2::SpacingType spacing=m_Project->GetSpacing();

    this->m_pUI->xDoubleSpinBox->blockSignals(true);
    this->m_pUI->xDoubleSpinBox->setValue(spacing[0]);
    this->m_pUI->xDoubleSpinBox->blockSignals(false);
    this->m_pUI->yDoubleSpinBox->blockSignals(true);
    this->m_pUI->yDoubleSpinBox->setValue(spacing[1]);
    this->m_pUI->yDoubleSpinBox->blockSignals(false);
    this->m_pUI->zDoubleSpinBox->blockSignals(true);
    this->m_pUI->zDoubleSpinBox->setValue(spacing[2]);
    this->m_pUI->zDoubleSpinBox->blockSignals(false);
    this->m_pUI->incrementDoubleSpinBox->setValue(this->m_Project->GetSamplingPeriod());
    this->m_pUI->nameLineEdit->setText(QString::fromStdString(this->m_Project->GetProjectName()));

    ProjectTableModel * projectModel = new ProjectTableModel(this->m_Project.get(),this); //TODO Release!
    this->m_pUI->framesTable->setModel(projectModel);
    this->HighlightProjectFrame(0);

}

void TTTMainWindow::HighlightProjectFrame(int frame){

	this->SetupSliderPositions(frame);

	RawImageDrawer drawer;

	itk::Image<float,3>::Pointer rawImage = m_Project->GetRawImage(m_CurrentFrame);
	drawer.SetImage(rawImage);
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

void TTTMainWindow::SetupSliderPositions(int frame){

	m_CurrentFrame=frame;
	this->m_pUI->projectSlider->blockSignals(true);
	this->m_pUI->projectSlider->setValue(frame);
	this->m_pUI->projectSlider->blockSignals(false);

	this->m_pUI->enhancementSlider->blockSignals(true);
	this->m_pUI->enhancementSlider->setValue(frame);
	this->m_pUI->enhancementSlider->blockSignals(false);

	this->m_pUI->vertexLocationSlider->blockSignals(true);
	this->m_pUI->vertexLocationSlider->setValue(frame);
	this->m_pUI->vertexLocationSlider->blockSignals(false);

	this->m_pUI->membranessSlider->blockSignals(true);
	this->m_pUI->membranessSlider->setValue(frame);
	this->m_pUI->membranessSlider->blockSignals(false);

	this->m_pUI->cellSegmentationSlider->blockSignals(true);
	this->m_pUI->cellSegmentationSlider->setValue(frame);
	this->m_pUI->cellSegmentationSlider->blockSignals(false);

	this->m_pUI->trackingSlider->blockSignals(true);
	this->m_pUI->trackingSlider->setValue(frame);
	this->m_pUI->trackingSlider->blockSignals(false);


}
void TTTMainWindow::DoLowestScale(){

	double lowestScale = m_Project->GetLowestPlatenessScale(m_CurrentFrame);

	PlatenessCommand lowestPlatenessCommand;

	lowestPlatenessCommand.SetSigmaMin(lowestScale);
	lowestPlatenessCommand.SetSigmaMax(lowestScale);
	lowestPlatenessCommand.SetSigmaSteps(1);

	DataCastingCommand caster;

	caster.SetInput(m_Project->GetRawImage(m_CurrentFrame));
	caster.Do();

	lowestPlatenessCommand.SetInput(caster.GetOutput());
	lowestPlatenessCommand.Do();


	m_LowestPlatenessDrawer.SetImage(lowestPlatenessCommand.GetPlatenessImage());
	m_LowestPlatenessDrawer.SetRenderer(this->m_LowestScaleSelectionRenderer);
	m_LowestPlatenessDrawer.Draw();
	m_LowestPlatenessDrawer.SetVisibility(true);
}

void TTTMainWindow::DoHighestScale(){
	double highestScale = m_Project->GetHighestPlatenessScale(m_CurrentFrame);

	PlatenessCommand highestPlatenessCommand;

	highestPlatenessCommand.SetSigmaMin(highestScale);
	highestPlatenessCommand.SetSigmaMax(highestScale);
	highestPlatenessCommand.SetSigmaSteps(1);

	DataCastingCommand caster;

	caster.SetInput(m_Project->GetRawImage(m_CurrentFrame));
	caster.Do();

	highestPlatenessCommand.SetInput(caster.GetOutput());
	highestPlatenessCommand.Do();


	m_HighestPlatenessDrawer.SetImage(highestPlatenessCommand.GetPlatenessImage());
	m_HighestPlatenessDrawer.SetRenderer(this->m_HighestScaleSelectionRenderer);
	m_HighestPlatenessDrawer.Draw();
	m_HighestPlatenessDrawer.SetVisibility(true);
}

void TTTMainWindow::DoRangeScale(){

	double lowestScale = m_Project->GetLowestPlatenessScale(m_CurrentFrame);
	double highestScale = m_Project->GetHighestPlatenessScale(m_CurrentFrame);
	int rangeScale = m_Project->GetPlatenessSteps(m_CurrentFrame);

	PlatenessCommand rangePlatenessCommand;

	rangePlatenessCommand.SetSigmaMin(lowestScale);
	rangePlatenessCommand.SetSigmaMax(highestScale);
	rangePlatenessCommand.SetSigmaSteps(rangeScale);

	//if(this->m_Project->IsDiffusedImageReady()){
	//rangePlatenessCommand.SetInput(this->m_Project->GetDiffusedImage());
		//}else{
		DataCastingCommand caster;
		caster.SetInput(m_Project->GetRawImage(m_CurrentFrame));
		caster.Do();
		rangePlatenessCommand.SetInput(caster.GetOutput());
		//}

	rangePlatenessCommand.Do();

	m_Project->SetPlatenessImage(m_CurrentFrame,rangePlatenessCommand.GetPlatenessImage());

	this->DrawRangePlateness();
}



void TTTMainWindow::DoAllScale(){
	for(unsigned int i=0;i<m_Project->GetNumFrames();i++){
		m_CurrentFrame=i; //TODO
		this->DoRangeScale();
	}
}



void TTTMainWindow::SetupMembranessFrame(int frame){

	this->SetupSliderPositions(frame);
	this->m_pUI->platenessLowestScaleDoubleSpinBox->setValue(m_Project->GetLowestPlatenessScale(frame));
	this->m_pUI->platenessHighestScaleDoubleSpinBox->setValue(m_Project->GetHighestPlatenessScale(frame));
	this->m_pUI->platenessStepsSpinBox->setValue(m_Project->GetPlatenessSteps(frame));
	this->DrawRangePlateness();


}

void TTTMainWindow::SetupEnhancementFrame(int frame){
	SetupSliderPositions(frame);
	this->DrawEnhancement();
}

void TTTMainWindow::SetupVertexFrame(int frame){
	SetupSliderPositions(frame);
	this->DrawVertex();
}

void TTTMainWindow::SetupSegmentationFrame(int frame){
	SetupSliderPositions(frame);
	this->DrawSegmentation();
}
void TTTMainWindow::SetupTrackingFrame(int frame){
	this->DrawTracking();
	SetupSliderPositions(frame);
}

void TTTMainWindow::SetupTectonicsFrame(int frame){

	SetupSliderPositions(frame);
	this->DrawEllipses();
	this->DrawTectonics();
	this->m_TectonicsRenderWindow->Render();

}

void TTTMainWindow::EnhanceAndDraw(){
	this->DoEnhance();
	this->DrawEnhancement();
}

void TTTMainWindow::DoEnhance(){

	double lowestScale = this->m_Project->GetLowestPlatenessScale(m_CurrentFrame);
	double highestScale = this->m_Project->GetHighestPlatenessScale(m_CurrentFrame);
	int rangeScale = this->m_Project->GetPlatenessSteps(m_CurrentFrame);

	int iterations = this->m_pUI->enhanceIterations->value();

	DataCastingCommand caster;

	caster.SetInput(m_Project->GetRawImage(m_CurrentFrame));
	caster.Do();

	AnisotropicDiffusionCommand anisotropicDiffusionCommand;

	anisotropicDiffusionCommand.SetDiffusionIterations(iterations);
	anisotropicDiffusionCommand.SetDiffusionSigmaMax(highestScale);
	anisotropicDiffusionCommand.SetDiffusionSigmaMin(lowestScale);
	anisotropicDiffusionCommand.SetDiffusionSigmaSteps(rangeScale);

	anisotropicDiffusionCommand.SetInputImage(caster.GetOutput());
	anisotropicDiffusionCommand.Do();

	this->m_Project->SetDiffusedImage(m_CurrentFrame,anisotropicDiffusionCommand.GetOutputImage());

	PlatenessCommand platenessCommand;

	platenessCommand.SetSigmaMin(lowestScale);
	platenessCommand.SetSigmaMax(highestScale);
	platenessCommand.SetSigmaSteps(rangeScale);

	platenessCommand.SetInput(this->m_Project->GetDiffusedImage(m_CurrentFrame));
	platenessCommand.Do();

	this->m_Project->SetPlatenessImage(m_CurrentFrame,platenessCommand.GetPlatenessImage());



}

void TTTMainWindow::DoAllEnhance(){

	for(unsigned int i=this->m_pUI->enhancementSlider->value();i<m_Project->GetNumFrames();i++){
		m_CurrentFrame=i;
		this->DoEnhance();
	}
	this->SetupEnhancementFrame(m_CurrentFrame);

}
void TTTMainWindow::VertexnessAndDraw(){
	this->DoVertexness();
	this->DrawVertex();
}
void TTTMainWindow::DoVertexness(){

	double lowestScale = this->m_pUI->lowerVertexnessScaleSpinBox->value();
	double highestScale = this->m_pUI->upperVertexnessScaleSpinBox->value();
	int rangeScale = this->m_pUI->stepsVertexnessSpinBox->value();

	PyramidVertexnessCommand vertexnessCommand;

	vertexnessCommand.SetSigmaMax(highestScale);
	vertexnessCommand.SetSigmaMin(lowestScale);
	vertexnessCommand.SetSigmaSteps(rangeScale);

	if(this->m_Project->IsDiffusedImageAvailable(m_CurrentFrame)){
		vertexnessCommand.SetInput(this->m_Project->GetDiffusedImage(m_CurrentFrame));
	}else{
		DataCastingCommand caster;
		caster.SetInput(m_Project->GetRawImage(m_CurrentFrame));
		caster.Do();
		vertexnessCommand.SetInput(caster.GetOutput());
	}

	vertexnessCommand.Do();


	m_Project->SetPyramidVertexnessImage(m_CurrentFrame,vertexnessCommand.GetVertexnessImages());

	#if 0
	DataCastingCommand caster;
	caster.SetInput(m_Project->GetRawImage(m_CurrentFrame));
	caster.Do();

	VertexnessCommand vertexnessCommand;

	vertexnessCommand.SetSigmaMax(highestScale);
	vertexnessCommand.SetSigmaMin(lowestScale);
	vertexnessCommand.SetSigmaSteps(rangeScale);
	vertexnessCommand.SetInput(caster.GetOutput());

	if(this->m_Project->IsDiffusedImageAvailable(m_CurrentFrame)){
		vertexnessCommand.SetInput(this->m_Project->GetDiffusedImage(m_CurrentFrame));
	}else{
		DataCastingCommand caster;
		caster.SetInput(m_Project->GetRawImage(m_CurrentFrame));
		caster.Do();
		vertexnessCommand.SetInput(caster.GetOutput());
	}


	vertexnessCommand.Do();

	m_Project->SetVertexnessImage(m_CurrentFrame,vertexnessCommand.GetVertexnessImage());

#endif
}
void TTTMainWindow::VertexLocationAndDraw(){
	this->DoVertexLocation();
	this->DrawVertex();
}
void TTTMainWindow::DoVertexLocation(){

	double threshold = this->m_pUI->vertexThresholdSpinBox->value();
	PyramidVertexLocationCommand vertexLocationCommand;
	vertexLocationCommand.SetLocalMaxThreshold(threshold);
	vertexLocationCommand.SetInputPyramid(m_Project->GetPyramidVertexnessImage(m_CurrentFrame));

	vertexLocationCommand.Do();

	m_Project->SetAdherensJunctionVertices(m_CurrentFrame,vertexLocationCommand.GetLocalMaxima());

#if 0
	VertexLocationCommand vertexLocationCommand;
	vertexLocationCommand.SetLocalMaxThreshold(threshold);
	vertexLocationCommand.SetVertexnessImage(m_Project->GetVertexnessImage(m_CurrentFrame));
	vertexLocationCommand.SetRadius(0.4);
	vertexLocationCommand.Do();

	m_Project->SetAdherensJunctionVertices(m_CurrentFrame,vertexLocationCommand.GetLocalMaxima());
#endif

}
void TTTMainWindow::VertexSelected(vtkSmartPointer<vtkActor> & actor){
	std::cout << "->VertexSelected" << std::endl;
	vtkSmartPointer<vtkCubeSource> cube=vtkSmartPointer<vtkCubeSource>::New();
	TissueTrackingAbstractProject2::SpacingType spacing=m_Project->GetSpacing();
	TissueTrackingAbstractProject2::SizeType size =m_Project->GetSize();
  	cube->SetBounds(0,spacing[0]*size[0],0,spacing[1]*size[1],0,spacing[2]*size[2]);
  	cube->Update();

  	vtkSmartPointer<FollowVertexCallback> myCallback = vtkSmartPointer<FollowVertexCallback>::New();

    vtkSmartPointer<vtkActor> pickedVertexActor=actor;

    myCallback->SetActor(pickedVertexActor);

    vtkSmartPointer<vtkSphereSource> sphereSource=m_VertexLocationsDrawer.GetSphereSourceFromActor(pickedVertexActor);
    myCallback->SetSphereSource(sphereSource);

    ttt::AdherensJunctionVertex::Pointer vertex = m_VertexLocationsDrawer.GetVertexFromActor(pickedVertexActor);


    myCallback->SetVertex(vertex);
    myCallback->SetSpacing(m_Project->GetSpacing());

    this->m_VertexLocationPointWidget->AllOn();
    this->m_VertexLocationPointWidget->SetInteractor(this->m_VertexLocationRenderWindowInteractor);
    this->m_VertexLocationPointWidget->SetInputData(cube->GetOutput());

    this->m_VertexLocationPointWidget->PlaceWidget();
    this->m_VertexLocationPointWidget->SetPosition(sphereSource->GetCenter());
    this->m_VertexLocationPointWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
    this->m_VertexLocationPointWidget->On();

  	this->m_pUI->deleteVertexButton->setEnabled(true);
  	this->m_pUI->selectVertexButton->setEnabled(false);
  	this->m_pUI->addVertexButton->setEnabled(false);

}

void TTTMainWindow::VertexUnselected(vtkSmartPointer<vtkActor> & actor){
	this->m_VertexLocationPointWidget->Off();
	this->m_VertexLocationPointWidget->RemoveAllObservers();
  	this->m_pUI->deleteVertexButton->setEnabled(false);
  	this->m_pUI->addVertexButton->setEnabled(true);
  	this->m_pUI->selectVertexButton->setEnabled(true);
  	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(this->m_StandardInteractor);
  	this->m_VertexLocationRenderWindowInteractor->ReInitialize();
  	std::cout << "->Vertex Unselected: Standard interactor style" << std::endl;
  	//this->m_VertexLocationRenderWindowInteractor->Start();
  	m_Project->SetAdherensJunctionVertices(m_CurrentFrame,this->m_DrawnAJVertices);
}

void TTTMainWindow::SelectVertex(){
	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(m_VertexSelectionInteractor);
	this->m_VertexLocationRenderWindowInteractor->ReInitialize();
	std::cout << "->Select Vertex: Selection interactor style" << std::endl;
	//this->m_VertexLocationRenderWindowInteractor->Start();
	this->m_pUI->addVertexButton->setEnabled(false);
	this->m_pUI->selectVertexButton->setEnabled(false);
}

void TTTMainWindow::VertexAdded(vtkSmartPointer<vtkActor> & actor){


    vtkSmartPointer<vtkActor> pickedVertexActor=actor;

    vtkSmartPointer<vtkSphereSource> sphereSource=m_VertexLocationsDrawer.GetSphereSourceFromActor(pickedVertexActor);

    ttt::AdherensJunctionVertex::Pointer vertex = m_VertexLocationsDrawer.GetVertexFromActor(pickedVertexActor);

  	m_Project->SetAdherensJunctionVertices(m_CurrentFrame,m_DrawnAJVertices);
  	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(this->m_StandardInteractor);
  	//this->m_VertexLocationRenderWindowInteractor->ReInitialize();
  	std::cout << "->Vertex Added: Standard interactor style" << std::endl;
  	//this->m_VertexLocationRenderWindowInteractor->Start();
  	this->m_pUI->selectVertexButton->setEnabled(true);
  	this->m_pUI->addVertexButton->setEnabled(true);
  	this->m_pUI->deleteVertexButton->setEnabled(true);

  	this->m_VertexLocationRenderWindow->Render();
}

void TTTMainWindow::VertexAddedToPrimal(vtkSmartPointer<vtkActor> & actor){

	m_Project->SetAdherensJunctionVertices(m_CurrentFrame,this->m_DrawnAJVertices);

	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(this->m_EdgeStandardInteractorStyle);
	this->m_CellSegmentationRendererWindow->Render();

#if 0
    vtkSmartPointer<vtkActor> pickedVertexActor=actor;

    vtkSmartPointer<vtkSphereSource> sphereSource=m_VertexLocationsDrawer.GetSphereSourceFromActor(pickedVertexActor);

    ttt::AdherensJunctionVertex::Pointer vertex = m_VertexLocationsDrawer.GetVertexFromActor(pickedVertexActor);

  	m_Project->SetVertexLocationsDirty();
  	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(this->m_StandardInteractor);
  	//this->m_VertexLocationRenderWindowInteractor->ReInitialize();
  	std::cout << "->Vertex Added: Standard interactor style" << std::endl;
  	//this->m_VertexLocationRenderWindowInteractor->Start();
  	this->m_pUI->selectVertexButton->setEnabled(true);
  	this->m_pUI->addVertexButton->setEnabled(true);
  	this->m_pUI->deleteVertexButton->setEnabled(true);

  	this->m_VertexLocationRenderWindow->Render();
#endif
}


void TTTMainWindow::VertexAdditionCancelled(){
	this->m_VertexLocationPointWidget->Off();
	this->m_VertexLocationPointWidget->RemoveAllObservers();

  	this->m_pUI->selectVertexButton->setEnabled(true);
  	this->m_pUI->deleteVertexButton->setEnabled(false);
  	this->m_pUI->addVertexButton->setEnabled(true);

  	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(this->m_StandardInteractor);
  	this->m_VertexLocationRenderWindowInteractor->ReInitialize();
  	std::cout << "->Vertex Addition Cancelled: Standard interactor style" << std::endl;
}




void TTTMainWindow::AddVertex(){
	m_VertexAdditionInteractor->SetSpacing(m_Project->GetSpacing());
	m_VertexAdditionInteractor->SetAdherensJunctionVertices(m_Project->GetAdherensJunctionVertices(m_CurrentFrame));
	this->m_pUI->selectVertexButton->setEnabled(false);
	this->m_pUI->addVertexButton->setEnabled(false);
	this->m_pUI->deleteVertexButton->setEnabled(false);
	this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(m_VertexAdditionInteractor);
	//this->m_VertexLocationRenderWindowInteractor->ReInitialize();
	std::cout << "->Add Vertex: Add vertex interactor style" << std::endl;
}

void TTTMainWindow::AddVertexToPrimal(){
	m_VertexAdditionToPrimalInteractor->SetTissueDescriptor(this->m_DrawnTissueDescriptor);

	this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_VertexAdditionToPrimalInteractor);
}


void TTTMainWindow::DeleteVertex(){
	vtkSmartPointer<vtkActor> toRemove=this->m_VertexSelectionInteractor->GetPickedVertexActor();
	if(toRemove){

		m_VertexSelectionInteractor->UnsetSelection();
		this->m_VertexLocationPointWidget->Off();
		this->m_VertexLocationPointWidget->RemoveAllObservers();

		ttt::AdherensJunctionVertices::Pointer vertices=this->m_DrawnAJVertices;
		ttt::AdherensJunctionVertex::Pointer toRemoveVertex=this->m_VertexLocationsDrawer.GetVertexFromActor(toRemove);
		m_VertexLocationsDrawer.EraseAdherensJunctionVertex(toRemoveVertex);
		ttt::AdherensJunctionVertices::iterator toRemoveIt= std::find(vertices->begin(),vertices->end(),toRemoveVertex);
		vertices->erase(toRemoveIt);
		m_Project->SetAdherensJunctionVertices(m_CurrentFrame,this->m_DrawnAJVertices);
		this->m_VertexLocationRenderWindow->Render();

	  	this->m_pUI->selectVertexButton->setEnabled(true);
	  	this->m_pUI->deleteVertexButton->setEnabled(false);
	  	this->m_pUI->addVertexButton->setEnabled(true);

		this->m_VertexLocationRenderWindowInteractor->SetInteractorStyle(m_StandardInteractor);
		this->m_VertexLocationRenderWindowInteractor->ReInitialize();
		std::cout << "->Delete Vertex: Standard vertex interactor style" << std::endl;
	}

}


void TTTMainWindow::SetupVertexAdditionInteractor(){

	m_VertexAdditionInteractor=vtkSmartPointer<VertexAdditionInteractor>::New();
	m_VertexAdditionInteractor->SetRenderer(this->m_VertexLocationRenderer);


	m_VertexAdditionInteractor->SetVertexLocationsDrawer(&m_VertexLocationsDrawer);

	connect(m_VertexAdditionInteractor,SIGNAL(vertexAdded(vtkSmartPointer<vtkActor> &)),this,SLOT(VertexAdded(vtkSmartPointer<vtkActor>&)));
	connect(m_VertexAdditionInteractor,SIGNAL(additionCancelled()),this,SLOT(VertexAdditionCancelled()));

}

void TTTMainWindow::SetupVertexAdditionToPrimalInteractor(){

	m_VertexAdditionToPrimalInteractor=vtkSmartPointer<VertexAdditionToPrimalInteractor>::New();
	m_VertexAdditionToPrimalInteractor->SetRenderer(this->m_CellSegmentationRenderer);

	m_VertexAdditionToPrimalInteractor->SetPrimalGraphDrawer(&m_PrimalGraphDrawer);

	connect(m_VertexAdditionInteractor,SIGNAL(vertexAdded(vtkSmartPointer<vtkActor> &)),this,SLOT(VertexAddedToPrimal(vtkSmartPointer<vtkActor>&)));
	connect(m_VertexAdditionInteractor,SIGNAL(additionCancelled()),this,SLOT(VertexAdditionToPrimalCancelled()));

}


void TTTMainWindow::SetupVertexStandardInteractor(){
	m_StandardInteractor=vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	m_StandardInteractor->SetDefaultRenderer(this->m_VertexLocationRenderer);
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
void TTTMainWindow::ShowVertexLocationsOnVertexLocation(int show){
	this->m_VertexLocationsDrawer.SetVisibility(show!=0);
	this->m_VertexLocationRenderWindow->Render();
}
void TTTMainWindow::ShowVertexnessOnVertexLocation(int show){
	this->m_VertexnessImageDrawer.SetVisibility(show!=0);
	this->m_VertexLocationRenderWindow->Render();
}

void TTTMainWindow::ShowAJGraphOnCellSegmentation(int show){
	m_PrimalGraphDrawer.SetVisibility(show!=0);
	this->m_CellSegmentationRendererWindow->Render();
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


	if(this->m_Project->IsTissueDescriptorAvailable(m_CurrentFrame)){
		this->m_DrawnTissueDescriptor=m_Project->GetTissueDescriptor(m_CurrentFrame);
		m_PrimalGraphDrawer.SetRenderer(this->m_CellSegmentationRenderer);
		m_PrimalGraphDrawer.SetVertexColorer(&m_PrimalGraphVertexColorer);
		m_PrimalGraphDrawer.SetEdgeColorer(&m_PrimalGraphEdgeColorer);
		m_PrimalGraphDrawer.SetTissueDescriptor(this->m_DrawnTissueDescriptor);
		m_PrimalGraphDrawer.Draw();
		m_PrimalGraphDrawer.SetVisibility(this->m_pUI->showAJGraphAtSegmentationCBox->isChecked());

		m_PlatenessDrawerOnSegmentation.SetRenderer(this->m_CellSegmentationRenderer);
		m_PlatenessDrawerOnSegmentation.SetImage(m_Project->GetPlatenessImage(m_CurrentFrame));
		m_PlatenessDrawerOnSegmentation.Draw();
		m_PlatenessDrawerOnSegmentation.SetVisibility(this->m_pUI->showAdherensJunctionsAtSegmentationCBox->isChecked());
	}else{
		m_PrimalGraphDrawer.Reset();
		m_PlatenessDrawerOnSegmentation.Reset();
	}


	if(this->m_Project->IsTissueDescriptorAvailable(m_CurrentFrame)){
		m_DualGraphDrawer.SetRenderer(this->m_CellSegmentationRenderer);
		m_DualGraphDrawer.SetVertexColorer(&m_DualGraphVertexColorer);
		m_DualGraphDrawer.SetEdgeColorer(&m_DualGraphEdgeColorer);
		m_DualGraphDrawer.SetTissueDescriptor(this->m_Project->GetTissueDescriptor(m_CurrentFrame));
		m_DualGraphDrawer.Draw();
		m_DualGraphDrawer.SetVisibility(this->m_pUI->showCellGraphAtSegmentationCBox->isChecked());
	}else{
		m_DualGraphDrawer.Reset();
	}

	this->m_CellSegmentationRendererWindow->Render();

}
void TTTMainWindow::DoSegmentation(){

	AdherensJunctionSegmentationDijkstraCommand segmentationCommand;

	segmentationCommand.SetLimit(this->m_pUI->horizonDoubleSpinBox->value());
	segmentationCommand.SetVertexLocations(m_Project->GetAdherensJunctionVertices(m_CurrentFrame));
	segmentationCommand.SetVertexnessImage(m_Project->GetVertexnessImage(m_CurrentFrame));
	segmentationCommand.SetPlatenessImage(m_Project->GetPlatenessImage(m_CurrentFrame));
	segmentationCommand.Do();

	this->m_Project->SetTissueDescriptor(m_CurrentFrame,segmentationCommand.GetTissueDescriptor());
}


void TTTMainWindow::SelectEdge(){
	this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_EdgeSelectionInteractor);
	this->m_pUI->linkVerticesButton->setEnabled(false);
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
	ttt::TissueDescriptor::Pointer descriptor=this->m_Project->GetTissueDescriptor(m_CurrentFrame);

	if(selected && m_PrimalGraphDrawer.IsEdge(selected)){

		ttt::SkeletonEdgeType toDelete = m_PrimalGraphDrawer.GetActorSkeletonEdge(selected);

		bool found=false;
		boost::tie(toDelete,found)=boost::edge(boost::source(toDelete,*descriptor->m_SkeletonGraph),boost::target(toDelete,*descriptor->m_SkeletonGraph),*descriptor->m_SkeletonGraph);
		boost::remove_edge(toDelete,*descriptor->m_SkeletonGraph);




	}else if(selected && m_PrimalGraphDrawer.IsVertex(selected)){
		m_PrimalGraphVertexPointWidget->Off();
		m_PrimalGraphVertexPointWidget->RemoveAllObservers();
		ttt::SkeletonVertexType toDelete = m_PrimalGraphDrawer.GetActorSkeletonVertex(selected);
		descriptor->InvalidateDual();

		boost::clear_vertex(toDelete,*descriptor->m_SkeletonGraph);
		boost::remove_vertex(toDelete,*descriptor->m_SkeletonGraph);


	}else{
		assert(false);
	}
	m_Project->SetTissueDescriptor(m_CurrentFrame,descriptor);

	this->DrawSegmentation();
	this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_EdgeStandardInteractorStyle);
	this->m_CellSegmentationRenderWindowInteractor->Initialize();
	this->m_pUI->linkVerticesButton->setEnabled(true);
	this->m_pUI->selectEdgeButton->setEnabled(true);
	this->m_pUI->deleteEdgeButton->setEnabled(false);

}

void TTTMainWindow::EdgeSelected(vtkSmartPointer<vtkActor>& actor){


	if(m_PrimalGraphDrawer.IsVertex(actor)){

		vtkSmartPointer<vtkCubeSource> cube=vtkSmartPointer<vtkCubeSource>::New();
		TissueTrackingAbstractProject2::SpacingType spacing=m_Project->GetSpacing();
		TissueTrackingAbstractProject2::SizeType size =m_Project->GetSize();
	  	cube->SetBounds(0,spacing[0]*size[0],0,spacing[1]*size[1],0,spacing[2]*size[2]);
	  	cube->Update();

	  	vtkSmartPointer<FollowVertexPrimalCallback> myCallback = vtkSmartPointer<FollowVertexPrimalCallback>::New();

	    vtkSmartPointer<vtkActor> pickedVertexActor=actor;

	    myCallback->SetActor(pickedVertexActor);

	    vtkSmartPointer<vtkSphereSource> sphereSource= m_PrimalGraphDrawer.GetSphereSource(actor);
	    myCallback->SetSphereSource(sphereSource);

	    ttt::SkeletonVertexType vertex = m_PrimalGraphDrawer.GetActorSkeletonVertex(actor);


	    myCallback->SetVertex(vertex);
	    myCallback->SetTissueDescriptor(this->m_DrawnTissueDescriptor);

	    m_PrimalGraphVertexPointWidget->AllOn();
	    m_PrimalGraphVertexPointWidget->SetInteractor(this->m_CellSegmentationRenderWindowInteractor);
	    m_PrimalGraphVertexPointWidget->SetInputData(cube->GetOutput());

	    m_PrimalGraphVertexPointWidget->PlaceWidget();
	    m_PrimalGraphVertexPointWidget->SetPosition(sphereSource->GetCenter());
	    m_PrimalGraphVertexPointWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
	    m_PrimalGraphVertexPointWidget->On();



	}else if(m_PrimalGraphDrawer.IsEdge(actor)){
	}
	this->m_pUI->deleteEdgeButton->setEnabled(true);


}

void TTTMainWindow::EdgeUnselected(vtkSmartPointer<vtkActor>& actor){

	if(this->m_PrimalGraphDrawer.IsVertex(actor)){
		m_PrimalGraphVertexPointWidget->Off();
		m_PrimalGraphVertexPointWidget->RemoveAllObservers();

	  	m_Project->SetTissueDescriptor(m_CurrentFrame,this->m_DrawnTissueDescriptor);
	}else if(this->m_PrimalGraphDrawer.IsEdge(actor)){

	}else{

	}

	this->m_pUI->deleteEdgeButton->setEnabled(false);
	this->m_pUI->linkVerticesButton->setEnabled(true);
	this->m_pUI->addVertexToPrimalButton->setEnabled(true);
	this->m_pUI->selectEdgeButton->setEnabled(true);

	this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(this->m_EdgeStandardInteractorStyle);

}
void TTTMainWindow::EdgeAdded(vtkSmartPointer<vtkActor>& a, vtkSmartPointer<vtkActor> & b){
	//TODO

	ttt::SkeletonVertexType aVertex= m_PrimalGraphDrawer.GetActorSkeletonVertex(a);
	ttt::SkeletonVertexType bVertex= m_PrimalGraphDrawer.GetActorSkeletonVertex(b);

	boost::add_edge(aVertex,bVertex,*this->m_DrawnTissueDescriptor->m_SkeletonGraph);

	this->m_Project->SetTissueDescriptor(m_CurrentFrame,this->m_DrawnTissueDescriptor); //TODO

	this->DrawSegmentation();

	this->m_CellSegmentationRenderWindowInteractor->SetInteractorStyle(m_EdgeStandardInteractorStyle);
	this->m_CellSegmentationRenderWindowInteractor->Initialize();


	this->m_pUI->deleteEdgeButton->setEnabled(false);
	this->m_pUI->linkVerticesButton->setEnabled(true);
	this->m_pUI->addVertexToPrimalButton->setEnabled(true);
	this->m_pUI->selectEdgeButton->setEnabled(true);

}




void TTTMainWindow::EdgeAdditionCancelled(){
	//TODO
}
void TTTMainWindow::DualAndDraw(){
	this->DoDual();
	//this->DrawDual();
}
void TTTMainWindow::DoDual(){
	CellGraphCommand cellGraphCommand;

	cellGraphCommand.SetPrimalGraph(this->m_Project->GetTissueDescriptor(m_CurrentFrame));
	cellGraphCommand.Do();

	ttt::TissueDescriptor::Pointer descriptor = cellGraphCommand.GetGraphs();
	this->m_Project->SetTissueDescriptor(m_CurrentFrame,descriptor);


	CellMomentCalculator<ttt::TissueDescriptor> calculator;
	calculator.SetTissueDescriptor(descriptor);
	calculator.Compute();
	this->m_Project->SetCentroids(m_CurrentFrame,calculator.GetCentroid());
	this->m_Project->SetPerimeter(m_CurrentFrame,calculator.GetPerimeter());
	this->m_Project->SetAreas(m_CurrentFrame,calculator.GetAreas());
	this->m_Project->SetXX(m_CurrentFrame,calculator.GetXX());
	this->m_Project->SetXY(m_CurrentFrame,calculator.GetXY());
	this->m_Project->SetYY(m_CurrentFrame,calculator.GetYY());
}

#if 0
void TTTMainWindow::DrawDual(){



}
#endif


void TTTMainWindow::DoTracking(){
#if 0
	typename TissueTrackingProject::RawImageType::SizeType size=m_Project->GetRawImage()->GetLargestPossibleRegion().GetSize();
	typename TissueTrackingProject::RawImageType::SpacingType spacing = m_Project->GetSpacing();
	double xMax=size[0]*spacing[0];
	double yMax=size[1]*spacing[1];
	double zMax=size[2]*spacing[2];



	trackingCommand.SetXMin(0);
	trackingCommand.SetXMax(xMax);
	trackingCommand.SetYMin(0);
	trackingCommand.SetYMax(yMax);
	trackingCommand.SetZMin(0);
	trackingCommand.SetZMax(zMax);
	;
#endif
	TrackingCommand trackingCommand;
	int numFrames= m_Project->GetNumFrames();

	std::vector<TissueDescriptor::Pointer> observations;
	observations.resize(numFrames);

	for(int i=0;i<numFrames;i++){

		observations[i]=m_Project->GetTissueDescriptor(i);
	}

	trackingCommand.SetObservedTissues(observations);
	trackingCommand.Do();

	std::vector<TrackedTissueDescriptor::Pointer> tracked =trackingCommand.GetTrackedTissue();

	int frame=0;
	for(std::vector<TrackedTissueDescriptor::Pointer>::iterator it = tracked.begin(); it!=tracked.end();it++){

		m_Project->SetTrackedTissueDescriptor(frame++,*it);
	}

	this->DrawTracking();
}


void TTTMainWindow::DrawTracking(){
	m_DrawnTrackedTissueDescriptor=m_Project->GetTrackedTissueDescriptor(m_CurrentFrame);
	m_TrackingDrawer.SetTissueDescriptor(m_DrawnTrackedTissueDescriptor);
	m_TrackingDrawer.SetRenderer(this->m_TrackingRenderer);
	//m_TrackingDrawer.SetEdgeColorer(this->m_TrackingEdgeColorer);

	//TrackedCellId::Pointer idFeature=TrackedCellId::New();

	FeatureMap<CellVertexType,unsigned int> idFeature;

	BGL_FORALL_VERTICES_T(v,*m_DrawnTrackedTissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){
		idFeature[v]=boost::get(ttt::TrackedCellPropertyTag(),*m_DrawnTrackedTissueDescriptor->m_CellGraph,v).GetID();
	}

	m_TrackingVertexColorer.SetFeatureMap(idFeature);

	//m_TrackingDrawer.SetVertexColorer(&m_TrackingVertexColorer);
	//m_TrackingDrawer.SetEdgeColorer(&m_TrackingEdgeColorer);
	m_TrackingDrawer.Draw();

	m_TrackingDrawer.SetVisibility(true);

	m_PrimalGraphTrackingDrawer.SetRenderer(this->m_TrackingRenderer);
	m_PrimalGraphTrackingDrawer.SetTissueDescriptor(m_DrawnTrackedTissueDescriptor);
	m_PrimalGraphTrackingDrawer.SetVertexColorer(&m_PrimalGraphVertexColorer);
	m_PrimalGraphTrackingDrawer.SetEdgeColorer(&m_PrimalGraphEdgeColorer);
	m_PrimalGraphTrackingDrawer.Draw();
	m_PrimalGraphTrackingDrawer.SetVisibility(true);

	this->m_TrackingRendererWindow->Render();

}
void TTTMainWindow::DoInitTracking(){
	TrackInitializationCommand initializationCommand;

	initializationCommand.SetTissueDescriptor(m_Project->GetTissueDescriptor(m_CurrentFrame));
	initializationCommand.Do();
	m_Project->SetTrackedTissueDescriptor(m_CurrentFrame,initializationCommand.GetTrackedTissueDescriptor());

}
void TTTMainWindow::DoTrackingInteractive(){

	MinCostMaxFlowTrackAssociationCommand trackingCommand;
	trackingCommand.SetPreviousTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));

	trackingCommand.SetObservedTissueDescriptor(m_Project->GetTissueDescriptor(m_CurrentFrame+1));
	trackingCommand.Do();
	m_Project->SetTrackedTissueDescriptor(m_CurrentFrame+1,trackingCommand.GetCurrentTrackedTissueDescriptor());

}
void TTTMainWindow::DrawTrackingInteractive(){

}


void TTTMainWindow::PopulateFeatureTable(){
	CellFeatureTableModel * featureTableModel = new CellFeatureTableModel(this->m_Project.get(),m_CurrentFrame,this);

	this->m_pUI->featureInspectionTableView->setModel(featureTableModel);

}

void TTTMainWindow::HighlightCellInFeatureTable(){

}

void TTTMainWindow::HighlightCellInRepresentation(){

}

void TTTMainWindow::DoDetectTrackingOutliers(){

}


void TTTMainWindow::DrawInspection(){
	PopulateFeatureTable();
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

void TTTMainWindow::DoEllipses(){
#if 0
	EllipsesCommand<TrackedTissueDescriptor> command;
	assert(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
	command.SetTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
	command.Do();

//	this->m_Project->SetTrackedEllipses(command.GetEllipses());
	this->DrawEllipses();
#endif
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
#if 0
	if(this->m_Project->IsTrackedEllipsesReady()){
		//m_EllipseDrawer.SetEllipses(this->m_Project->GetTrackedEllipses());

		m_EllipseDrawer.SetRenderer(this->m_TectonicsRenderer);
		m_EllipseDrawer.Draw();
		m_EllipseDrawer.SetVisibility(this->m_pUI->showEllipsesCBox->isChecked());

	}else{
		m_EllipseDrawer.Reset();
	}
	this->m_TectonicsRenderWindow->Render();
#endif
}


void TTTMainWindow::DoTectonics(){
#if 0
	ComputeDomainsCommand domainsCommand;

	unsigned int order= this->m_pUI->domainOrderSpinBox->value();
	domainsCommand.SetOrder(order);
	domainsCommand.SetTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
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
#endif
}

void TTTMainWindow::DrawTectonics(){
#if 0
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
#endif
}

void TTTMainWindow::LaunchProjectExplorer(){
	TTTExplorer explorer;
	explorer.SetProject(this->m_Project.get());
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
