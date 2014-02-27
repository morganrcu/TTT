#include <vtkProperty.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkCamera.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkCubeSource.h>

#include "sqlopenprojectdialog.h"

#include "tissuetracker.h"
#include "ui_tissuetracker.h"
#include "sqlnewprojectdialog.h"
#include "LateralImageVolumeSegmentationCommand.h"
#include "CLAHECommand.h"
#include "SurfaceSegmentationCommand.h"
#include "AnisotropicDiffusionCommand.h"

#include "PlatenessCommand.h"
#include "VertexnessCommand.h"
#include "DataCastingCommand.h"
#include "VertexLocationCommand.h"
#include "AdherensJunctionSegmentationDijkstraCommand.h"
#include "CellGraphCommand.h"
#include "EllipsesCommand.h"
#include "ComputeDomainsCommand.h"

#include "TrackingCommand.h"
#include "TectonicsCommand.h"


#include "AdherensJunctionEdgeOrientation.h"
#include "CellOrder.h"
#include "CellArea.h"

#include "FeatureColorer.h"
#include "DefaultColorer.h"


using namespace std;

#include "VertexSelectionInteractor.h"
#include "VertexAdditionInteractor.h"


/**
 * Creates a new TissueTracker
 * Initializes all data structures employed in the application and sets up the UI
 */
TissueTracker::TissueTracker(QWidget *parent) :   QMainWindow(parent),  m_pUI(new Ui::TissueTracker)
{

	m_Project=0;
    m_pUI->setupUi(this);


    m_CurrentRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_CurrentRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_RenderWindow->AddRenderer(m_CurrentRenderer);
    m_RenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_RenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_RenderWindowInteractor->SetRenderWindow(m_RenderWindow);
    m_RenderWindowInteractor->Initialize();


    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_CurrentRenderer->AddActor(axes);

    m_pUI->m_vtkview->SetRenderWindow(m_RenderWindow);
    m_pUI->m_vtkview->show();

    m_GreetingsDrawer.SetRenderer(m_CurrentRenderer);
    m_GreetingsDrawer.Draw();
    m_GreetingsDrawer.Show();

    //CONNECT UI SLOTS

    connect(m_pUI->actionOpen, SIGNAL(triggered()), this, SLOT(OpenFile()));
    connect(m_pUI->actionNew,  SIGNAL(triggered()), this, SLOT(NewProject()));


    //connect(m_pUI->actionColor,SIGNAL(triggered()), this, SLOT(ChangeColor()));
	//connect(m_pUI->actionSave, SIGNAL(triggered()), this, SLOT(Save()));

    //connect(m_pUI->actionModify, SIGNAL(triggered()), this, SLOT(DoModification()));
    connect(m_pUI->showOriginalCBox, SIGNAL(toggled(bool)), this, SLOT(SetShowOriginalImage(bool)));
    connect(m_pUI->showSurfaceSegmentedCBox, SIGNAL(toggled(bool)), this, SLOT(SetShowSurfaceSegmentedImage(bool)));
    connect(m_pUI->showLHCBox, SIGNAL(toggled(bool)), this, SLOT(SetShowLateralImageVolumeSegmentedImage(bool)));
    connect(m_pUI->showCLAHECBox, SIGNAL(toggled(bool)), this, SLOT(SetShowCLAHEDImage(bool)));
    connect(m_pUI->showDiffusedCBox, SIGNAL(toggled(bool)), this, SLOT(SetShowDiffusedImage(bool)));
    connect(m_pUI->showPlatenessCBox,SIGNAL(toggled(bool)), this, SLOT(SetShowPlatenessImage(bool)));
    connect(m_pUI->showVertexnessCBox,SIGNAL(toggled(bool)), this, SLOT(SetShowVertexnessImage(bool)));
    connect(m_pUI->showVertexLocationsCBox,SIGNAL(toggled(bool)), this, SLOT(SetShowVertexLocations(bool)));
    connect(m_pUI->showPrimalCBox,SIGNAL(toggled(bool)), this, SLOT(SetShowPrimalGraph(bool)));
    connect(m_pUI->showDualCBox,SIGNAL(toggled(bool)), this, SLOT(SetShowDualGraph(bool)));
#if 0

    connect(m_pUI->showEllipseCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));


    connect(m_pUI->showSimpleCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showPolygonMapCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showRosettesCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showAreaMapCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));    
    connect(m_pUI->showEdgeVariationMapCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));   
    connect(m_pUI->showSecondChannelCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));   
    connect(m_pUI->showMotionVectorCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showClusterMapCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showJxDegMapCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showTrackingCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showTectonicsCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
#endif
    connect(m_pUI->screenshotButton,SIGNAL(clicked()),this,SLOT(TakeScreenshot()));
    connect(m_pUI->resetViewButton,SIGNAL(clicked()),this,SLOT(ResetView()));



    connect(m_pUI->lhButton,SIGNAL(clicked()),this,SLOT(DoLateralImageSegmentation()));
    connect(m_pUI->claheButton,SIGNAL(clicked()),this,SLOT(DoCLAHE()));
    connect(m_pUI->ssbutton,SIGNAL(clicked()),this, SLOT(DoSurfaceSegmentation()));
    connect(m_pUI->adbutton,SIGNAL(clicked()),this,SLOT(DoAnisotropicDiffusion()));
    connect(m_pUI->ptnessbutton,SIGNAL(clicked()),this,SLOT(DoPlateness()));
    connect(m_pUI->vtxnessbutton,SIGNAL(clicked()),this,SLOT(DoVertexness()));
    connect(m_pUI->vlocatorbutton,SIGNAL(clicked()),this,SLOT(DoVertexLocation()));

    connect(m_pUI->primalButton,SIGNAL(clicked()),this,SLOT(DoPrimalCalculation()));
    connect(m_pUI->dualButton,SIGNAL(clicked()),this,SLOT(DoDualCalculation()));
    connect(m_pUI->trackingButton,SIGNAL(clicked()),this,SLOT(DoTracking()));

    connect(m_pUI->ellipsoidButton,SIGNAL(clicked()),this,SLOT(DoEllipses()));
    connect(m_pUI->domainsButton,SIGNAL(clicked()),this,SLOT(DoDomains()));
    connect(m_pUI->tectonicsButton,SIGNAL(clicked()),this,SLOT(DoTectonics()));

    //connect(m_pUI->finalizeButton,SIGNAL(clicked()),this,SLOT(DoFinalize()));

    //connect(m_pUI->lvlsetbutton,SIGNAL(clicked()),this,SLOT(DoLevelSet()));
    //connect(m_pUI->lvlsetthreshbutton,SIGNAL(clicked()),this,SLOT(DoThresholding()));
    //connect(m_pUI->sklbutton,SIGNAL(clicked()),this,SLOT(DoSkeletonization()));
    //connect(m_pUI->validateButton,SIGNAL(clicked()),this,SLOT(DoValidation()));

    //connect(m_pUI->insertButton,SIGNAL(clicked()),this,SLOT(DoInsert()));
    //connect(m_pUI->deleteButton,SIGNAL(clicked()),this,SLOT(DoDeletion()));
    //connect(m_pUI->moveButton,SIGNAL(clicked()),this,SLOT(DoMove()));





    //connect(m_pUI->primalGiaaButton,SIGNAL(clicked()),this,SLOT(DoPrimalGiaaCalculation()));
    //connect(m_pUI->selectButton,SIGNAL(clicked()),this,SLOT(DoSelection()));

    //connect(m_pUI->reduceButton,SIGNAL(clicked()),this,SLOT(DoReduction()));
    //connect(m_pUI->modsimButton,SIGNAL(clicked()),this,SLOT(DoModSim()));
    //connect(m_pUI->simpleprimalButton,SIGNAL(clicked()),this,SLOT(DoPrimalSimplify()));


    //connect(m_pUI->modifyButton,SIGNAL(clicked()),this,SLOT(DoModification()));

    //connect(m_pUI->clusterButton,SIGNAL(clicked()),this,SLOT(GetTrackedCells()));

    //connect(m_pUI->selectTrackedButton,SIGNAL(clicked()),this,SLOT(DoSelectTracking()));

    connect(m_pUI->DoCurrentButton,SIGNAL(clicked()),this,SLOT(DoCurrent()));
    connect(m_pUI->DoAllButton,SIGNAL(clicked()),this,SLOT(DoAll()));
    //connect(m_pUI->polygonButton,SIGNAL(clicked()),this,SLOT(DoPolygonMap()));
    //connect(m_pUI->domaintableView,SIGNAL(clicked(QModelIndex)),this,SLOT(ondomaintableClick(QModelIndex)));
    //connect(m_pUI->domaintableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(DomainContextMenu(const QPoint &)));
    //connect(m_pUI->celltableView,SIGNAL(clicked(QModelIndex)),this,SLOT(oncelltableClick(QModelIndex)));
    //connect(m_pUI->actionMove, SIGNAL(triggered()), this, SLOT(DoMove()));


    //connect(m_pUI->addCellDomain,SIGNAL(clicked()),this,SLOT(AddCellsToDomain()));
    //connect(m_pUI->junctionButton,SIGNAL(clicked()),this,SLOT(DoJunctions()));
    //connect(m_pUI->celltableView, SIGNAL(selectionChanged()),this, SLOT(onSelectionChanged()));
    connect(m_pUI->timeSlider,SIGNAL(valueChanged(int)),this,SLOT(SetFrame( int))); 
    //connect(m_pUI->timeSlider2,SIGNAL(valueChanged(int)),this,SLOT(SetFrame2( int)));

    connect(m_pUI->selectVertexButton,SIGNAL(clicked()),this,SLOT(SelectVertex()));
    connect(m_pUI->addVertexButton,SIGNAL(clicked()),this,SLOT(AddVertex()));
    connect(m_pUI->deleteVertexButton,SIGNAL(clicked()),this,SLOT(DeleteVertex()));
    m_CurrentFrame=0;


	m_StandardInteractor = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	m_VertexAdditionInteractor= vtkSmartPointer<VertexAdditionInteractor>::New();
	m_VertexSelectionInteractor=vtkSmartPointer<VertexSelectionInteractor>::New();

	m_pVertexAddedCallback= new VertexAddedCallback;
	m_pVertexSelectedCallback = new VertexSelectedCallback;
	m_pVertexUnselectedCallback = new VertexUnselectedCallback;


}

/**
 * Create a NewProjectDialog, exec, store result and update UI
 */

void TissueTracker::NewProject(){

	SQLNewProjectDialog dialog(this);
	dialog.exec();
	if(dialog.IsAccepted()){
		this->m_Project=dialog.GetNewProject();

		SetupNavigation();
		SetFrame(1);

#if 0
    NewProjectDialog dialog(this);
    dialog.exec();

    this->m_Project=dialog.GetProject();

    //this->m_Spacing=this->m_Project->GetSpacing();
    SetupNavigation();
    SetFrame(1);
#endif
    }
}

/**
 * Open a project stored on disc
 */

void TissueTracker::OpenFile(){

	SQLOpenProjectDialog dialog;
	dialog.exec();
	if(dialog.IsAccepted()){
		this->m_Project=dialog.GetProject();
		//this->m_Project->Load(path);
		//    m_BasePath = path;
		//  this->m_Project->AddSpacing();
		//  m_Spacing= m_Project->GetSpacing();
		SetupNavigation();
		SetFrame(1);
	}

}


/**
 * Configure UI Controls according to the available operations
 * TODO Somebody changed it. REVERT!
 */
void TissueTracker::UpdateControls(){

    this->m_pUI->adbutton->setEnabled(this->m_Project->IsLateralImageVolumeSegmentedImageReady());
    this->m_pUI->adinputgroupBox->setEnabled(this->m_Project->IsDiffusedImageReady());

    this->m_pUI->ptnessbutton->setEnabled(this->m_Project->IsLateralImageVolumeSegmentedImageReady());
    this->m_pUI->ptnessinputgroupBox->setEnabled(
        			this->m_Project->IsDiffusedImageReady());
    this->m_pUI->vtxnessbutton->setEnabled(
                this->m_Project->IsLateralImageVolumeSegmentedImageReady()
                );
    this->m_pUI->vtxnessinputgroupBox->setEnabled(
            			this->m_Project->IsDiffusedImageReady());

    this->m_pUI->vlocatorbutton->setEnabled(
                this->m_Project->IsVertexnessImageReady()
                );

    this->m_pUI->primalButton->setEnabled(
                //this->m_Project->IsSkeletonReady(m_CurrentFrame)
                true
                );
    this->m_pUI->dualButton->setEnabled(
                //this->m_Project->IsSkeletonReady(m_CurrentFrame)
                true
                );
}




TissueTracker::~TissueTracker()
{
    delete m_pUI;
}

/**
 * Update navigation information on the UI
 */
void TissueTracker::SetupNavigation(){

    //QString totalFrames( %1");

    //totalFrames=totalFrames.arg(m_Project->GetNumFrames());
    QString totalFrames("/%1");
    totalFrames=totalFrames.arg(m_Project->GetNumFrames());

    std::cout <<"Total Frames: "<< totalFrames.toStdString()<< std::endl;
    this->m_pUI->totalFramesLbl->setText(totalFrames);

    this->m_pUI->timeSlider->setMinimum(1);
    this->m_pUI->timeSlider->setTracking(false);
    this->m_pUI->timeSlider->setMaximum(m_Project->GetNumFrames());
    this->m_pUI->timeSlider->setTickInterval(1);

    this->m_pUI->configurationDock->setEnabled(true);
    this->m_pUI->viewOptionsDock->setEnabled(true);
    this->m_pUI->showSegmentationCBox->setEnabled(true);
}
void TissueTracker::SetFrame( int frame){
    std::cout << "Slot SetFrame activated. Frame: " << frame << std::endl;
    m_CurrentFrame=frame-1;
    std::cout << "m_CurrentFrame is " << m_CurrentFrame << std::endl;
    

    //globalFrame = m_CurrentFrame;
    SetupCurrentFrame();

}


void TissueTracker::SetupCurrentFrame(){
    //this->m_pUI->timeSlider->setValue(m_CurrentFrame+1);

	m_CurrentRenderer->RemoveAllViewProps();

    QString currentFrame("%1");
    currentFrame=currentFrame.arg(m_CurrentFrame+1);
    this->m_pUI->currentFrametxt->setText(currentFrame);
    //descriptor = this->m_Project->GetTissueDescriptor(m_CurrentFrame);

#ifdef OUT_OF_CONTROL
    if(this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){


      this->PopulateCellTable();
      BGL_FORALL_VERTICES(v,descriptor->m_CellGraph,giaa::CellGraph){
        Cell cell = boost::get(giaa::CellPropertyTag(), descriptor->m_CellGraph, v);
        if(cell.trackID != -1){
            DoneTracking = 1;
            break;
        }
      }
    }
#endif
    m_Project->SetFrame(m_CurrentFrame);

    this->UpdateControls();
    this->UpdateDisplay();
    this->UpdateVisibility();

    this->SetupVertexAdditionInteractor();
    this->SetupVertexSelectionInteractor();
    this->SetupStandardInteractor();
}

void TissueTracker::DoLateralImageSegmentation(){
	LateralImageVolumeSegmentationCommand command;

	float threshold= (float)atof(this->m_pUI->lhLowThresholdtxt->text().toStdString().c_str());

	command.SetRelativeThreshold(threshold);
	command.SetInputImage(m_Project->GetRawImage());
	command.Do();
	m_Project->SetLateralImageVolumeSegmentedImage(command.GetOutput());
	this->DrawLateralImageVolumeSegmentedImage();
}

void TissueTracker::DoCLAHE(){
	CLAHECommand command;
	float radius= (float)atof(this->m_pUI->claheRadiustxt->text().toStdString().c_str());
	float maxSlope= (float)atof(this->m_pUI->claheSlopetxt->text().toStdString().c_str());

	command.SetRadius(radius);
	command.SetMaxSlope(maxSlope);
	command.SetInputImage(m_Project->GetLateralImageVolumeSegmentedImage());
	command.Do();
	m_Project->SetCLAHEDImage(command.GetOutputImage());
	this->DrawCLAHEDImage();
}

void TissueTracker::DoSurfaceSegmentation(){
	float sigmaxy= (float)atof(this->m_pUI->ssxyVartxt->text().toStdString().c_str());
	float sigmaz= (float)atof(this->m_pUI->sszVartxt->text().toStdString().c_str());
	float threshold=(float)atof(this->m_pUI->ssthreshtxt->text().toStdString().c_str());


	SurfaceSegmentationCommand command;

	command.SetVarXY(sigmaxy);
	command.SetVarZ(sigmaz);
	command.SetThres(threshold);
	assert(m_Project->GetRawImage());
	command.SetInputImage(m_Project->GetRawImage());

	command.Do();

	command.GetOuputBinary();
	m_Project->SetSurfaceSegmentedImage(command.GetOutputImage());
    this->m_pUI->showSurfaceSegmentedCBox->setChecked(true);

    this->UpdateControls();

    this->DrawSurfaceSegmentedImage();
}


void TissueTracker::DoAnisotropicDiffusion(){
	double sigmamin=(double)atof(this->m_pUI->adsigmamintxt->text().toStdString().c_str());
	double sigmamax=(double)atof(this->m_pUI->adsigmamaxtxt->text().toStdString().c_str());
	int sigmasteps=(int)atoi(this->m_pUI->adstepstxt->text().toStdString().c_str());
	int iterations=(int)atoi(this->m_pUI->aditerationstxt->text().toStdString().c_str());

	bool diffuseSurfaceSegmented=this->m_pUI->adssinputradioButton->isChecked();

	AnisotropicDiffusionCommand command;

	command.SetDiffusionSigmaMin(sigmamin);
	command.SetDiffusionSigmaMax(sigmamax);
	command.SetDiffusionSigmaSteps(sigmasteps);
	command.SetDiffusionIterations(iterations);


	if(diffuseSurfaceSegmented){
		DataCastingCommand castingCommand;
		assert(m_Project->GetLateralImageVolumeSegmentedImage());
		castingCommand.SetInput(m_Project->GetLateralImageVolumeSegmentedImage());
		castingCommand.Do();
		command.SetInputImage(castingCommand.GetOutput());

	}else{
		assert(m_Project->GetDiffusedImage());
		command.SetInputImage(m_Project->GetDiffusedImage());

	}
	command.Do();


	m_Project->SetDiffusedImage(command.GetOutputImage());

	this->m_pUI->showDiffusedCBox->setChecked(true);

	this->UpdateControls();

	this->DrawDiffusedImage();
}

void TissueTracker::DoPlateness(){

	double sigmamin=(double)atof(this->m_pUI->ptnesssigmamintxt->text().toStdString().c_str());
	double sigmamax=(double)atof(this->m_pUI->ptnesssigmamaxtxt->text().toStdString().c_str());
	int sigmasteps=(int)atoi(this->m_pUI->ptnessstepstxt->text().toStdString().c_str());

	bool platenessSurfaceSegmented=this->m_pUI->ptnessssinputradioButton->isChecked();


	PlatenessCommand command;
	command.SetSigmaMin(sigmamin);
	command.SetSigmaMax(sigmamax);
	command.SetSigmaSteps(sigmasteps);
	if(platenessSurfaceSegmented){
		DataCastingCommand castingCommand;
		assert(m_Project->GetLateralImageVolumeSegmentedImage());
		castingCommand.SetInput(m_Project->GetLateralImageVolumeSegmentedImage());
		castingCommand.Do();
		command.SetInput(castingCommand.GetOutput());
	}else{
		assert(m_Project->GetDiffusedImage());
		command.SetInput(m_Project->GetDiffusedImage());
	}

	command.Do();

	m_Project->SetPlatenessImage(command.GetPlatenessImage());
	//m_Project->AddOrientation(m_CurrentFrame,command.GetOrientationImage());

	this->m_pUI->showPlatenessCBox->setChecked(true);

	this->UpdateControls();
	this->DrawPlatenessImage();
}

void TissueTracker::DoVertexness(){

    double sigmamin=(double)atof(this->m_pUI->vtxnesssigmamintxt->text().toStdString().c_str());
    double sigmamax=(double)atof(this->m_pUI->vtxnesssigmamaxtxt->text().toStdString().c_str());
    int sigmasteps=(int)atoi(this->m_pUI->vtxnessstepstxt->text().toStdString().c_str());

	bool vertexnessSurfaceSegmented=this->m_pUI->vtxnessssinputradioButton->isChecked();

    VertexnessCommand command;

    command.SetSigmaMin(sigmamin);
    command.SetSigmaMax(sigmamax);
    command.SetSigmaSteps(sigmasteps);

	if(vertexnessSurfaceSegmented){
		DataCastingCommand castingCommand;
		assert(m_Project->GetLateralImageVolumeSegmentedImage());
		castingCommand.SetInput(m_Project->GetLateralImageVolumeSegmentedImage());
		castingCommand.Do();
		command.SetInput(castingCommand.GetOutput());
	}else{
		assert(m_Project->GetDiffusedImage());
		command.SetInput(m_Project->GetDiffusedImage());
	}

    command.Do();
    m_Project->SetVertexnessImage(command.GetVertexnessImage());

    this->m_pUI->showVertexnessCBox->setChecked(true);

//this->m_pUI->vlocatorGroupBox->setEnabled(true);

    this->UpdateControls();
    this->DrawVertexnessImage();
}


void TissueTracker::DoVertexLocation(){

    int threshold = this->m_pUI->vlocatorslider->value();

    double realThres= (double) threshold/100;

    VertexLocationCommand command;

    command.SetLocalMaxThreshold(realThres);
    assert(m_Project->GetVertexnessImage());
    command.SetVertexnessImage(m_Project->GetVertexnessImage());
    command.Do();
    m_Project->SetVertexLocations(command.GetLocalMaxima());

    this->m_pUI->showVertexLocationsCBox->setChecked(true);

    this->UpdateControls();
    this->DrawVertexLocations();
}

void TissueTracker::SetInteractionModeToStandard(){

	m_RenderWindowInteractor->SetInteractorStyle(m_StandardInteractor);
}
void TissueTracker::SetInteractionModeToAddVertex(){

	m_RenderWindowInteractor->SetInteractorStyle(m_VertexAdditionInteractor);
}

void TissueTracker::SetInteractionModeToVertexSelection(){
	m_RenderWindowInteractor->SetInteractorStyle(m_VertexSelectionInteractor);
}


void TissueTracker::SetupStandardInteractor(){
	m_StandardInteractor= vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
}
void TissueTracker::SetupVertexSelectionInteractor(){

	m_VertexSelectionInteractor=vtkSmartPointer<VertexSelectionInteractor>::New();

	vtkSmartPointer<vtkPointWidget> pointWidget= vtkSmartPointer<vtkPointWidget>::New();
	m_pVertexSelectedCallback=new VertexSelectedCallback;
	m_pVertexSelectedCallback->SetUI(this->m_pUI);
	m_pVertexSelectedCallback->SetTissueTracker(this);
	m_pVertexSelectedCallback->SetPointWidget(pointWidget);
	m_pVertexSelectedCallback->SetVertexLocationsDrawer(&m_VertexLocationsDrawer);

	m_pVertexUnselectedCallback->SetUI(this->m_pUI);
	m_pVertexUnselectedCallback->SetTissueTracker(this);
	m_pVertexUnselectedCallback->SetPointWidget(pointWidget);

	m_VertexSelectionInteractor->SetVertexSelectedCallback(m_pVertexSelectedCallback);
	m_VertexSelectionInteractor->SetVertexUnselectedCallback(m_pVertexUnselectedCallback);
	m_VertexSelectionInteractor->SetRenderer(m_CurrentRenderer);
}

void TissueTracker::SetupVertexAdditionInteractor(){

	m_VertexAdditionInteractor=vtkSmartPointer<VertexAdditionInteractor>::New();

	m_VertexAdditionInteractor->SetAdherensJunctionVertices(m_Project->GetVertexLocations());
	m_VertexAdditionInteractor->SetVertexLocationsDrawer(&m_VertexLocationsDrawer);
	m_VertexAdditionInteractor->SetVertexAddedCallback(m_pVertexAddedCallback);
	this->m_pVertexAddedCallback->SetUI(this->m_pUI);
	this->m_pVertexAddedCallback->SetTissueTracker(this);

	m_VertexAdditionInteractor->SetVertexAddedCallback(m_pVertexAddedCallback);
	m_VertexAdditionInteractor->SetRenderer(m_CurrentRenderer);


}


void TissueTracker::AddVertex(){
	this->SetInteractionModeToAddVertex();
}

void TissueTracker::SelectVertex(){

	this->SetInteractionModeToVertexSelection();

}


void TissueTracker::DeleteVertex(){
	vtkSmartPointer<vtkActor> toRemove=this->m_VertexSelectionInteractor->GetPickedVertexActor();

	m_VertexSelectionInteractor->UnsetSelection();
	if(toRemove){
		ttt::AdherensJunctionVertices::Pointer vertices=this->m_Project->GetVertexLocations();
		ttt::AdherensJunctionVertex::Pointer toRemoveVertex=this->m_VertexLocationsDrawer.GetVertexFromActor(toRemove);
		m_VertexLocationsDrawer.EraseAdherensJunctionVertex(toRemoveVertex);
		ttt::AdherensJunctionVertices::iterator toRemoveIt= std::find(vertices->begin(),vertices->end(),toRemoveVertex);
		vertices->erase(toRemoveIt);
		this->m_RenderWindow->Render();
	}
}

void TissueTracker::FinishVertex(){
	this->SetInteractionModeToStandard();
}

void TissueTracker::DoPrimalCalculation(){

	AdherensJunctionSegmentationDijkstraCommand command;

	//assert(m_Project->GetVertexLocations(m_CurrentFrame));
	assert(m_Project->GetPlatenessImage());

	command.SetVertexLocations(m_Project->GetVertexLocations());
	command.SetPlatenessImage(m_Project->GetPlatenessImage());
	command.SetVertexnessImage(m_Project->GetVertexnessImage());
	command.Do();

	assert(command.GetTissueDescriptor());

	m_Project->SetTissueDescriptor(command.GetTissueDescriptor());

	this->m_pUI->showPrimalCBox->setChecked(true);
	this->DrawPrimalGraph();
	this->UpdateControls();

}


void TissueTracker::DoValidation(){
#if 0

	class PGSISomethingSelected : public PrimalGraphStandardInteractor::SomethingSelected {
	private:
		Ui::TissueTracker * m_pUI;
	public:

		PGSISomethingSelected(Ui::TissueTracker * pUI){
			m_pUI=pUI;
		}
		virtual void Notify(){
			assert(m_Caller);
			if(m_Caller->IsSelectedVertex()){
				m_pUI->insertButton->setEnabled(true);
				m_pUI->moveButton->setEnabled(true);
			}
			m_pUI->deleteButton->setEnabled(true);
		}

		virtual ~PGSISomethingSelected(){

		}
	};

	class PGSISelectionCleared : public PrimalGraphStandardInteractor::SelectionCleared {
	private:
		Ui::TissueTracker * m_pUI;
	public:
		PGSISelectionCleared(Ui::TissueTracker * pUI){
			m_pUI=pUI;
		}
		virtual void Notify(){
			m_pUI->insertButton->setEnabled(false);

			m_pUI->moveButton->setEnabled(false);

			m_pUI->deleteButton->setEnabled(false);
		}
		virtual ~PGSISelectionCleared(){
		}
	};

	PGSISomethingSelected * somethingSelected = new PGSISomethingSelected(this->m_pUI);
	PGSISelectionCleared * selectionCleared = new PGSISelectionCleared(this->m_pUI);


	 m_PrimalGraphStandardInteractor = PrimalGraphStandardInteractor::New();

	 m_PrimalGraphStandardInteractor->SetSomethingSelected(somethingSelected);
	 m_PrimalGraphStandardInteractor->SetSelectionCleared(selectionCleared);

	 m_PrimalGraphStandardInteractor->SetRenderer(m_CurrentRenderer);
	 m_PrimalGraphStandardInteractor->SetPrimalGraphDrawer(m_PrimalGraphDrawer);
	 m_RenderWindowInteractor->SetInteractorStyle(m_PrimalGraphStandardInteractor);

	 m_RenderWindowInteractor->Start();
#endif
}

void TissueTracker::DoInsert(){
#if 0
	m_pUI->insertButton->setEnabled(false);
	m_pUI->moveButton->setEnabled(false);
	m_pUI->deleteButton->setEnabled(false);
	class PGIIInsertionFinished : public PrimalGraphInsertionInteractor::InsertionFinished{
	private:
		Ui::TissueTracker * m_pUI;
		TissueTracker * m_pTissueTracker;
		vtkSmartPointer<PrimalGraphDrawer> m_PrimalGraphDrawer;
	    TissueTrackingProject * m_Project;
		int m_CurrentFrame;
	public:
		PGIIInsertionFinished(Ui::TissueTracker * pUI,
				const vtkSmartPointer<PrimalGraphDrawer> & primalGraphDrawer,
				TissueTracker * pTissueTracker,
				TissueTrackingProject * project,
				int currentFrame){
			m_pUI=pUI;
			m_PrimalGraphDrawer=primalGraphDrawer;
			m_pTissueTracker=pTissueTracker;
			m_Project=project;
			m_CurrentFrame=currentFrame;
		}
		virtual ~PGIIInsertionFinished(){

		}

		virtual void Notify(){
			m_PrimalGraphDrawer->Draw();
			m_Project->AddTissueDescriptor(m_CurrentFrame,m_Caller->GetTissueDescriptor());

			m_pTissueTracker->DoValidation();
		}
	};
	PGIIInsertionFinished * insertionFinished = new PGIIInsertionFinished(this->m_pUI,this->m_PrimalGraphDrawer,this,this->m_Project,m_CurrentFrame);

	m_PrimalGraphInsertionInteractor=vtkSmartPointer<PrimalGraphInsertionInteractor>::New();

	m_PrimalGraphInsertionInteractor->SetInsertionFinished(insertionFinished);

	m_PrimalGraphInsertionInteractor->SetRenderer(m_CurrentRenderer);
	vtkSmartPointer<vtkActor> origin=m_PrimalGraphStandardInteractor->GetSelectedVertex();
	m_PrimalGraphInsertionInteractor->SetPrimalGraphDrawer(m_PrimalGraphDrawer);
	assert(origin);

	m_PrimalGraphInsertionInteractor->SetOrigin(origin);
	assert(m_Project->GetTissueDescriptor());
	m_PrimalGraphInsertionInteractor->SetTissueDescriptor(m_Project->GetTissueDescriptor());
	m_PrimalGraphInsertionInteractor->Init();
	m_RenderWindowInteractor->SetInteractorStyle(m_PrimalGraphInsertionInteractor);

	m_RenderWindowInteractor->Start();
#endif
}

void TissueTracker::ResetView(){
	vtkSmartPointer<vtkCamera> aCamera = m_CurrentRenderer->GetActiveCamera();
	aCamera->SetViewUp (0, 0, -1);
	aCamera->SetPosition (0, 1, 0);
	aCamera->SetFocalPoint (0, 0, 0);
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);

	std::cout << "Reset" << std::endl;
	//vtkSmartPointer<vtkCamera> camera=m_CurrentRenderer->GetActiveCamera();
	//m_CurrentRenderer->ResetCamera();

	//m_CurrentRenderer->ResetCameraClippingRange();
	m_RenderWindow->Render();
}


void TissueTracker::TakeScreenshot(){

	// Screenshot
	  vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	  windowToImageFilter->SetInput(m_RenderWindow);
	  windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
	  windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	  windowToImageFilter->Update();

	  vtkSmartPointer<vtkPNGWriter> writer =  vtkSmartPointer<vtkPNGWriter>::New();
	  QString fileName=QFileDialog::getSaveFileName(this,"Select file...");
	  writer->SetFileName(fileName.toStdString().c_str());
	  writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	  writer->Write();

}

void TissueTracker::DoMove(){
#if 0
	m_pUI->insertButton->setEnabled(false);
	m_pUI->moveButton->setEnabled(false);
	m_pUI->deleteButton->setEnabled(false);
	class PGMIMoveFinished : public PrimalGraphMoveInteractor::MoveFinished{
		private:
			Ui::TissueTracker * m_pUI;
			TissueTracker * m_pTissueTracker;
			vtkSmartPointer<PrimalGraphDrawer> m_PrimalGraphDrawer;
		    TissueTrackingProject * m_Project;
		    int m_CurrentFrame;


		public:
			PGMIMoveFinished(Ui::TissueTracker * pUI,
					const vtkSmartPointer<PrimalGraphDrawer> & primalGraphDrawer,
					TissueTracker * pTissueTracker,
					TissueTrackingProject * project,
					int currentFrame){
				m_pUI=pUI;
				m_PrimalGraphDrawer=primalGraphDrawer;
				m_pTissueTracker=pTissueTracker;
				m_Project=project;
				m_CurrentFrame=currentFrame;

			}
			virtual ~PGMIMoveFinished(){

			}

			virtual void Notify(){
				m_PrimalGraphDrawer->Draw();
				if(m_Caller->IsMoved()){
					m_Project->AddTissueDescriptor(m_CurrentFrame,m_Caller->GetTissueDescriptor());
				}
				m_pTissueTracker->DoValidation();


			}

		};
		PGMIMoveFinished * moveFinished =new PGMIMoveFinished(this->m_pUI,this->m_PrimalGraphDrawer,this,m_Project,m_CurrentFrame);

		vtkSmartPointer<PrimalGraphMoveInteractor> m_PrimalGraphMoveInteractor=vtkSmartPointer<PrimalGraphMoveInteractor>::New();

		m_PrimalGraphMoveInteractor->SetMoveFinished(moveFinished);

		m_PrimalGraphMoveInteractor->SetRenderer(m_CurrentRenderer);

		m_PrimalGraphMoveInteractor->SetPrimalGraphDrawer(m_PrimalGraphDrawer);
		m_PrimalGraphMoveInteractor->SetTissueDescriptor(m_Project->GetTissueDescriptor());

		vtkSmartPointer<vtkActor> target=m_PrimalGraphStandardInteractor->GetSelectedVertex();
		assert(target);
		m_PrimalGraphMoveInteractor->SetTarget(target);

		assert(m_Project->GetTissueDescriptor());



		m_RenderWindowInteractor->SetInteractorStyle(m_PrimalGraphMoveInteractor);

		m_RenderWindowInteractor->Start();
#endif
}

void TissueTracker::DoDeletion(){
#if 0
	assert(m_PrimalGraphStandardInteractor->IsSelectedEdge() ||m_PrimalGraphStandardInteractor->IsSelectedVertex()  );

	TissueDescriptor::Pointer descriptor = m_Project->GetTissueDescriptor();

	if(m_PrimalGraphStandardInteractor->IsSelectedEdge()){
		vtkSmartPointer<vtkActor> edgeActor=m_PrimalGraphStandardInteractor->GetSelectedEdge();
		SkeletonEdgeType toDelete=m_PrimalGraphDrawer->GetActorSkeletonEdge(edgeActor);
		boost::remove_edge(boost::source(toDelete,*descriptor->m_SkeletonGraph),boost::target(toDelete,*descriptor->m_SkeletonGraph),*descriptor->m_SkeletonGraph);
	}else if(m_PrimalGraphStandardInteractor->IsSelectedVertex()){
		vtkSmartPointer<vtkActor> vertexActor=m_PrimalGraphStandardInteractor->GetSelectedVertex();
		SkeletonVertexType toDelete=m_PrimalGraphDrawer->GetActorSkeletonVertex(vertexActor);
		boost::clear_vertex(toDelete,*descriptor->m_SkeletonGraph);
		boost::remove_vertex(toDelete,*descriptor->m_SkeletonGraph);
	}
	m_Project->AddTissueDescriptor(m_CurrentFrame,descriptor);

	this->UpdateDisplay();
	DoValidation();
#endif
}

void TissueTracker::DoDualCalculation(){
	CellGraphCommand command;

	assert(m_Project->GetTissueDescriptor());
	command.SetPrimalGraph(m_Project->GetTissueDescriptor());
	command.Do();

	m_Project->SetTissueDescriptor(command.GetGraphs());
	this->DrawDualGraph();
}





void TissueTracker::DoTracking(){

	TrackingCommand trackingCommand;

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

#if 0

   for(int t=0;t< 10;t++){
    tmpdescriptor = m_Project->GetTissueDescriptor(t);
    BGL_FORALL_VERTICES(v, tmpdescriptor->m_CellGraph, giaa::CellGraph){
        boost::get(giaa::CellPropertyTag(),tmpdescriptor->m_CellGraph,v).trackID = -1;
    }
  }

  for(int t=0;t< 10;t++){
   observations.push_back(m_Project->GetTissueDescriptor(t));
   std::cout << "Number of Observed Cells in " << t << " is " << m_Project->GetTissueDescriptor(t)->GetNumCells() << std::endl;
  }

  m_TrackingFacade.SetObservations(observations);
  std::cout<<"didnt get here"<<std::endl;
  m_TrackingFacade.Track();

  for(int t = 0; t < 10; t++){
    m_Project->AddTissueDescriptor(t,observations[t]);
  }

  /*tracks = m_TrackingFacade.GetTracks();
  for(int i=0;i < 2;i++){

      std::cout << " " << std::endl;
      BGL_FORALL_VERTICES(v,observations[i]->m_CellGraph,giaa::CellGraph){
          //boost::get(giaa::TrackedCellPropertyTag(),tracks[i]->m_TrackedCellGraph,v)
          itk::Point<double,3> observationCentroid = boost::get(giaa::CellPropertyTag(),observations[i]->m_CellGraph,v).m_Centroid;
          std::cout << "Observed in " << i << " " << v << ":" << observationCentroid[0] << " " << observationCentroid[1] << " " << observationCentroid[2] << std::endl;
          std::cout << "Observed ID " << boost::get(giaa::CellPropertyTag(),observations[i]->m_CellGraph,v).trackID << " " << observations[i] << std::endl;
      }

      std::cout << " " << std::endl;
      BGL_FORALL_VERTICES(v,tracks[i]->m_TrackedCellGraph,TrackedCellGraph){
          //boost::get(giaa::TrackedCellPropertyTag(),tracks[i]->m_TrackedCellGraph,v)
          itk::Point<double,3> trackedCentroid = boost::get(giaa::TrackedCellPropertyTag(),tracks[i]->m_TrackedCellGraph,v).m_Centroid;
          std::cout << "Tracked in " << i << " " << v << ":" << trackedCentroid[0] << " " << trackedCentroid[1] << " " << trackedCentroid[2] << std::endl;
          std::cout << "Track ID " <<  boost::get(giaa::TrackedCellPropertyTag(),tracks[i]->m_TrackedCellGraph,v).m_ID << std::endl;
      }

        std::cout << "...CellGraphVertex" << std::endl;
      BGL_FORALL_EDGES(e,descriptor->m_TrackedCellGraph,giaa::TrackedCellGraph){
      boost::add_edge(boost::source(e,descriptor->m_TrackedCellGraph),boost::target(e,descriptor->m_TrackedCellGraph),result->m_TrackedCellGraph);
      }
   }*/

   DoneTracking = 1;
#endif
}
void TissueTracker::DoEllipses(){
	EllipsesCommand<TrackedTissueDescriptor> command;
	assert(m_Project->GetTrackedTissueDescriptor());
	command.SetTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	command.Do();

	m_Project->AddTrackedEllipses(m_CurrentFrame,command.GetEllipses());


}

void TissueTracker::DoDomains(){

	ComputeDomainsCommand command;
	unsigned int order= (unsigned int)atoi(this->m_pUI->ordertxt->text().toStdString().c_str());
	command.SetOrder(order);
	command.SetTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	command.Do();
	m_Project->AddTrackedDomains(m_CurrentFrame,command.GetDomains());

}

void TissueTracker::DoTectonics(){
#if 0
	ComputeDomainsCommand domainsCommand;

	unsigned int order= (unsigned int)atoi(this->m_pUI->ordertxt->text().toStdString().c_str());
	domainsCommand.SetOrder(order);
	domainsCommand.SetTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	domainsCommand.Do();

	TectonicsCommand tectonicsCommand;
	tectonicsCommand.SetDomains(domainsCommand.GetDomains());
	tectonicsCommand.SetCurrentTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
	if(m_CurrentFrame==0){
		tectonicsCommand.SetEllipsesCurrent(m_Project->GetTrackedEllipses(m_CurrentFrame));
		tectonicsCommand.SetEllipsesNext(m_Project->GetTrackedEllipses(m_CurrentFrame+1));

		tectonicsCommand.SetNextTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame+1));

	}else if(m_CurrentFrame > 0 && m_CurrentFrame < m_Project->GetNumFrames()-1){

		tectonicsCommand.SetNextTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame+1));
		tectonicsCommand.SetPreviousTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame-1));

		tectonicsCommand.SetEllipsesPrevious(m_Project->GetTrackedEllipses(m_CurrentFrame-1));
		tectonicsCommand.SetEllipsesNext(m_Project->GetTrackedEllipses(m_CurrentFrame+1));

	}else if(m_CurrentFrame == m_Project->GetNumFrames()-1){

		tectonicsCommand.SetPreviousTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame-1));

		tectonicsCommand.SetEllipsesPrevious(m_Project->GetTrackedEllipses(m_CurrentFrame-1));
		tectonicsCommand.SetEllipsesCurrent(m_Project->GetTrackedEllipses(m_CurrentFrame));
	}


	tectonicsCommand.Do();

	m_Project->AddDomainStrainRates(m_CurrentFrame,tectonicsCommand.GetDomainStrainRates());
#endif
}

void TissueTracker::DoCurrent(){

	//this->DoSurfaceSegmentation();
	this->DoLateralImageSegmentation();
	//this->DoCLAHE();

    this->DoAnisotropicDiffusion();

    this->DoPlateness();

    this->DoVertexness();
    this->DoVertexLocation();
#if 0
    this->DoPrimalCalculation();

    this->DoLevelSet();

    this->DoThresholding();
    this->DoSkeletonization();
#endif
#ifdef OUT_OF_CONTROL
    this->DoPrimalCalculation();
    this->DoDualCalculation();
#endif

}



void TissueTracker::DoAll(){
    for(unsigned int frame=m_CurrentFrame;frame<m_Project->GetNumFrames();frame++){
  		this->SetFrame(frame+1);
  		DoCurrent();

    }
}


void TissueTracker::DrawOriginalImage(){
    if(m_Project->IsRawImageReady()){
    	m_OriginalImageDrawer.SetRenderer(m_CurrentRenderer);
    	m_OriginalImageDrawer.SetImage(m_Project->GetRawImage());
    	m_OriginalImageDrawer.Draw();
    	this->m_pUI->showOriginalCBox->setEnabled(true);
    }else{
    	this->m_pUI->showOriginalCBox->setEnabled(false);
    }

}
void TissueTracker::SetShowOriginalImage(bool state){
	m_OriginalImageDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}

void TissueTracker::DrawLateralImageVolumeSegmentedImage(){
    if(m_Project->IsLateralImageVolumeSegmentedImageReady()){
    	m_LateralImageVolumeSegmentedImageDrawer.SetRenderer(m_CurrentRenderer);
    	m_LateralImageVolumeSegmentedImageDrawer.SetImage(m_Project->GetLateralImageVolumeSegmentedImage());
    	m_LateralImageVolumeSegmentedImageDrawer.Draw();
    	this->m_pUI->showLHCBox->setEnabled(true);
    }else{
    	this->m_pUI->showLHCBox->setEnabled(false);
    }
}
void TissueTracker::SetShowLateralImageVolumeSegmentedImage(bool state){
	m_LateralImageVolumeSegmentedImageDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}
void TissueTracker::DrawCLAHEDImage(){
    if(m_Project->IsCLAHEDImageReady()){
    	m_CLAHEDImageDrawer.SetRenderer(m_CurrentRenderer);
    	m_CLAHEDImageDrawer.SetImage(m_Project->GetCLAHEDImage());
    	m_CLAHEDImageDrawer.Draw();
    	this->m_pUI->showCLAHECBox->setEnabled(true);
    }else{
    	this->m_pUI->showCLAHECBox->setEnabled(false);
    }
}

void TissueTracker::SetShowCLAHEDImage(bool state){
	m_CLAHEDImageDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}

void TissueTracker::DrawSurfaceSegmentedImage(){
    if(m_Project->IsSurfaceSegmentedImageReady()){
    	m_SurfaceSegmentedImageDrawer.SetRenderer(m_CurrentRenderer);
    	m_SurfaceSegmentedImageDrawer.SetImage(m_Project->GetSurfaceSegmentedImage());
    	m_SurfaceSegmentedImageDrawer.Draw();
    	this->m_pUI->showSurfaceSegmentedCBox->setEnabled(true);
    }else{
    	this->m_pUI->showSurfaceSegmentedCBox->setEnabled(false);
    }
}
void TissueTracker::SetShowSurfaceSegmentedImage(bool state){
	m_SurfaceSegmentedImageDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}

void TissueTracker::DrawDiffusedImage(){
    if(this->m_Project->IsDiffusedImageReady()){
    	m_DiffusedImageDrawer.SetRenderer(m_CurrentRenderer);
    	m_DiffusedImageDrawer.SetImage(m_Project->GetDiffusedImage());
    	m_DiffusedImageDrawer.Draw();
    	this->m_pUI->showDiffusedCBox->setEnabled(true);
    }else{
    	this->m_pUI->showDiffusedCBox->setEnabled(false);
    }
}
void TissueTracker::SetShowDiffusedImage(bool state){
	m_DiffusedImageDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}

void TissueTracker::DrawPlatenessImage(){
    if(m_Project->IsPlatenessImageReady()){
    	m_PlatenessImageDrawer.SetRenderer(m_CurrentRenderer);
    	m_PlatenessImageDrawer.SetImage(m_Project->GetPlatenessImage());
    	m_PlatenessImageDrawer.Draw();
    	this->m_pUI->showPlatenessCBox->setEnabled(true);
    }else{
    	this->m_pUI->showPlatenessCBox->setEnabled(false);
    }
}
void TissueTracker::SetShowPlatenessImage(bool state){
	m_PlatenessImageDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}

void TissueTracker::DrawVertexnessImage(){
    if( this->m_Project->IsVertexnessImageReady()){
    	m_VertexnessImageDrawer.SetRenderer(m_CurrentRenderer);
    	m_VertexnessImageDrawer.SetImage(m_Project->GetVertexnessImage());
    	m_VertexnessImageDrawer.Draw();
    	this->m_pUI->showVertexnessCBox->setEnabled(true);
    }else{
    	this->m_pUI->showVertexnessCBox->setEnabled(false);
    }
}
void TissueTracker::SetShowVertexnessImage(bool state){
	m_VertexnessImageDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}

void TissueTracker::DrawVertexLocations(){
    if( m_Project->IsVertexLocationsReady()){
    	m_VertexLocationsDrawer.SetRenderer(m_CurrentRenderer);
    	m_VertexLocationsDrawer.SetVertexLocations(m_Project->GetVertexLocations());
    	m_VertexLocationsDrawer.SetSpacing(m_Project->GetSpacing());
    	m_VertexLocationsDrawer.Draw();
    	this->m_pUI->showVertexLocationsCBox->setEnabled(true);
    }else{
    	this->m_pUI->showVertexLocationsCBox->setEnabled(false);
    }
}
void TissueTracker::SetShowVertexLocations(bool state){
	m_VertexLocationsDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}

void TissueTracker::DrawPrimalGraph(){
    if( m_Project->IsTissueDescriptorReady()){

    	typedef AdherensJunctionEdgeOrientation<ttt::TissueDescriptor> OrientationFeatureType;
    	typename OrientationFeatureType::Pointer orientationFeature = OrientationFeatureType::New();

    	orientationFeature->SetTissueDescriptor(m_Project->GetTissueDescriptor());
    	orientationFeature->Compute();

    	FeatureColorer<OrientationFeatureType> edgeColorer;
    	edgeColorer.SetFeature(orientationFeature);
    	m_PrimalGraphDrawer.SetEdgeColorer(&edgeColorer);

    	DefaultColorer<ttt::SkeletonVertexType> vertexColorer;
    	m_PrimalGraphDrawer.SetVertexColorer(&vertexColorer);


    	m_PrimalGraphDrawer.SetRenderer(m_CurrentRenderer);
    	m_PrimalGraphDrawer.SetTissueDescriptor(m_Project->GetTissueDescriptor());
    	m_PrimalGraphDrawer.Draw();

    	this->m_pUI->showPrimalCBox->setEnabled(true);
    }else{
    	this->m_pUI->showPrimalCBox->setEnabled(false);
    }
}

void TissueTracker::SetShowPrimalGraph(bool state){
	m_PrimalGraphDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}
void TissueTracker::DrawDualGraph(){
    if( m_Project->IsTissueDescriptorReady()){

    	typedef CellArea<ttt::TissueDescriptor> CellOrderFeatureType;
    	typename CellOrderFeatureType::Pointer cellOrderFeature = CellOrderFeatureType::New();

    	cellOrderFeature->SetTissueDescriptor(m_Project->GetTissueDescriptor());
    	cellOrderFeature->Compute();

    	FeatureColorer<CellOrderFeatureType> vertexColorer;
    	vertexColorer.SetFeature(cellOrderFeature);
    	m_DualGraphDrawer.SetVertexColorer(&vertexColorer);
#if 0
    	DefaultColorer<ttt::CellVertexType> vertexColorer;
    	m_DualGraphDrawer.SetVertexColorer(&vertexColorer);
#endif
    	DefaultColorer<ttt::CellEdgeType> edgeColorer;
    	m_DualGraphDrawer.SetEdgeColorer(&edgeColorer);



    	m_DualGraphDrawer.SetRenderer(m_CurrentRenderer);
    	m_DualGraphDrawer.SetTissueDescriptor(m_Project->GetTissueDescriptor());
    	m_DualGraphDrawer.Draw();

    	this->m_pUI->showDualCBox->setEnabled(true);
    }else{
    	this->m_pUI->showDualCBox->setEnabled(false);
    }
}
void TissueTracker::SetShowDualGraph(bool state){
	m_DualGraphDrawer.SetVisibility(state);
	this->m_RenderWindow->Render();
}
void TissueTracker::UpdateVisibility(){
#if 0
	this->m_pUI->showOriginalCBox->setEnabled(m_Project->IsRawImageReady());
	this->m_pUI->showCLAHECBox->setEnabled(m_Project->IsCLAHEDImageReady());
	this->m_pUI->showSurfaceSegmentedCBox->setEnabled(m_Project->IsSurfaceSegmentedImageReady());
	this->m_pUI->showLHCBox->setEnabled(m_Project->IsLateralImageVolumeSegmentedImageReady());
	this->m_pUI->showDiffusedCBox->setEnabled(m_Project->IsDiffusedImageReady());
	this->m_pUI->showPlatenessCBox->setEnabled(m_Project->IsPlatenessImageReady());
	this->m_pUI->showVertexnessCBox->setEnabled(m_Project->IsVertexnessImageReady());
#endif

	this->m_OriginalImageDrawer.SetVisibility(this->m_pUI->showOriginalCBox->isEnabled() & this->m_pUI->showOriginalCBox->isChecked());
	this->m_CLAHEDImageDrawer.SetVisibility(this->m_pUI->showCLAHECBox->isEnabled() & this->m_pUI->showCLAHECBox->isChecked());
	this->m_LateralImageVolumeSegmentedImageDrawer.SetVisibility(this->m_pUI->showLHCBox->isEnabled() & this->m_pUI->showLHCBox->isChecked());
	//this->m_SurfaceSegmentedImageDrawer.SetVisibility(this->m_pUI->showSurfaceSegmentedCBox->isEnabled() & this->m_pUI->showSurfaceSegmentedCBox->isChecked());
	this->m_DiffusedImageDrawer.SetVisibility(this->m_pUI->showDiffusedCBox->isEnabled() & this->m_pUI->showDiffusedCBox->isChecked());
	this->m_PlatenessImageDrawer.SetVisibility(this->m_pUI->showPlatenessCBox->isEnabled() && this->m_pUI->showPlatenessCBox->isChecked());
	this->m_VertexnessImageDrawer.SetVisibility(this->m_pUI->showVertexnessCBox->isEnabled() && this->m_pUI->showVertexnessCBox->isChecked());
	this->m_RenderWindowInteractor->Render();
}
void TissueTracker::UpdateDisplay(){

    vtkSmartPointer<vtkRenderWindow> renWin = this->m_pUI->m_vtkview->GetRenderWindow();

#if 0
    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();


    m_CurrentRenderer->AddActor(axes);
#endif
    this->DrawOriginalImage();
    this->DrawLateralImageVolumeSegmentedImage();
    this->DrawCLAHEDImage();
    this->DrawSurfaceSegmentedImage();
    this->DrawDiffusedImage();
    this->DrawPlatenessImage();
    this->DrawVertexnessImage();
    this->DrawVertexLocations();
    this->DrawPrimalGraph();
    this->DrawDualGraph();

#if 0
    if( this->m_Project->IsTissueDescriptorReady()){


    }

    if( this->m_Project->IsTissueDescriptorReady()){

    }

    if(this->m_Project->IsTissueDescriptorReady()){


    }
    if(this->m_Project->IsTrackedTissueDescriptorReady()){
    	if(!m_TrackingDrawer) m_TrackingDrawer=TrackingDrawer::New();
    	assert(m_TrackingDrawer);
    	m_TrackingDrawer->SetRenderer(m_CurrentRenderer);
    	m_TrackingDrawer->SetTrackedDescriptor(m_Project->GetTrackedTissueDescriptor());
    	m_TrackingDrawer->Draw();


    }
    if( this->m_Project->IsTissueDescriptorReady()){

    	MotionVectorDrawer motionVectorDrawer;
    	motionVectorDrawer.SetRenderer(m_CurrentRenderer);
    	motionVectorDrawer.SetTrackedDescriptor(m_Project->GetTrackedTissueDescriptor());
    	motionVectorDrawer.Draw();

    }
    if( this->m_Project->IsTissueDescriptorReady()){
    	EllipseDrawer ellipseDrawer;

    	ellipseDrawer.SetRenderer(m_CurrentRenderer);
    	ellipseDrawer.SetEllipses(m_Project->GetTrackedEllipses(m_CurrentFrame));
    	ellipseDrawer.Draw();

    }
    if(this->m_Project->IsDomainStrainRatesReady()){
    	DomainStrainRatesDrawer domainStrainRatesDrawer;

    	domainStrainRatesDrawer.SetRenderer(m_CurrentRenderer);
    	domainStrainRatesDrawer.SetTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor());
    	domainStrainRatesDrawer.SetDomainStrainRates(m_Project->GetDomainStrainRates());
    	domainStrainRatesDrawer.Draw();
    }
#endif
    renWin->Render();
}
