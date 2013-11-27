#include <vtkProperty.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkRenderWindow.h>
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
#include "AdherensJunctionSegmentationCommand.h"
#include "CellGraphCommand.h"
#include "EllipsesCommand.h"
#include "ComputeDomainsCommand.h"

#include "TrackingCommand.h"
#include "TectonicsCommand.h"

#include "GreetingsDrawer.h"
#include "RawImageDrawer.h"
#include "DiffusedImageDrawer.h"
#include "PlatenessImageDrawer.h"
#include "VertexnessImageDrawer.h"
#include "VertexLocationsDrawer.h"
#include "PrimalGraphDrawer.h"
#include "DualGraphDrawer.h"

#include "MotionVectorDrawer.h"
#include "EllipseDrawer.h"
#include "DomainStrainRatesDrawer.h"

using namespace std;


/**
 * Creates a new TissueTracker
 * Initializes all data structures employed in the application and sets up the UI
 */
TissueTracker::TissueTracker(QWidget *parent) :
    QMainWindow(parent),
    m_pUI(new Ui::TissueTracker)
{
    m_pUI->setupUi(this);




    m_CurrentRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_CurrentRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

    m_RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_RenderWindow->AddRenderer(m_CurrentRenderer);

    m_RenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_RenderWindowInteractor->SetRenderWindow(m_RenderWindow);
    m_RenderWindowInteractor->Initialize();


    m_pUI->m_vtkview->SetRenderWindow(m_RenderWindow);

    //SET UP MAIN VTK DISPLAY

    GreetingsDrawer greetingsDrawer;
    greetingsDrawer.SetRenderer(m_CurrentRenderer);

    greetingsDrawer.Draw();


    m_pUI->m_vtkview->show();


    //CONNECT UI SLOTS

    connect(m_pUI->actionOpen, SIGNAL(triggered()), this, SLOT(OpenFile()));
    connect(m_pUI->actionNew,  SIGNAL(triggered()), this, SLOT(NewProject()));




    //connect(m_pUI->actionColor,SIGNAL(triggered()), this, SLOT(ChangeColor()));
	//connect(m_pUI->actionSave, SIGNAL(triggered()), this, SLOT(Save()));

    //connect(m_pUI->actionModify, SIGNAL(triggered()), this, SLOT(DoModification()));
    connect(m_pUI->showOriginalCBox, SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showLHCBox, SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showCLAHECBox, SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showDiffusedCBox, SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showSurfaceSegmentedCBox, SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showPlatenessCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showVertexnessCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showVertexLocationsCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showLevelSetCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showSegmentationCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showSkeletonCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showEllipseCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showPrimalCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
    connect(m_pUI->showDualCBox,SIGNAL(toggled(bool)), this, SLOT(UpdateDisplay()));
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

    connect(m_pUI->ssbutton,SIGNAL(clicked()),this, SLOT(DoSurfaceSegmentation()));
    connect(m_pUI->adbutton,SIGNAL(clicked()),this,SLOT(DoAnisotropicDiffusion()));
    connect(m_pUI->ptnessbutton,SIGNAL(clicked()),this,SLOT(DoPlateness()));
    connect(m_pUI->vtxnessbutton,SIGNAL(clicked()),this,SLOT(DoVertexness()));
    connect(m_pUI->primalButton,SIGNAL(clicked()),this,SLOT(DoPrimalCalculation()));

    connect(m_pUI->lhButton,SIGNAL(clicked()),this,SLOT(DoLateralImageSegmentation()));
    connect(m_pUI->claheButton,SIGNAL(clicked()),this,SLOT(DoCLAHE()));

    connect(m_pUI->dualButton,SIGNAL(clicked()),this,SLOT(DoDualCalculation()));
    connect(m_pUI->trackingButton,SIGNAL(clicked()),this,SLOT(DoTracking()));

    connect(m_pUI->ellipsoidButton,SIGNAL(clicked()),this,SLOT(DoEllipses()));
    connect(m_pUI->domainsButton,SIGNAL(clicked()),this,SLOT(DoDomains()));
    connect(m_pUI->tectonicsButton,SIGNAL(clicked()),this,SLOT(DoTectonics()));

    //connect(m_pUI->finalizeButton,SIGNAL(clicked()),this,SLOT(DoFinalize()));
    connect(m_pUI->vlocatorbutton,SIGNAL(clicked()),this,SLOT(DoVertexLocation()));
    //connect(m_pUI->lvlsetbutton,SIGNAL(clicked()),this,SLOT(DoLevelSet()));
    //connect(m_pUI->lvlsetthreshbutton,SIGNAL(clicked()),this,SLOT(DoThresholding()));
    ///connect(m_pUI->sklbutton,SIGNAL(clicked()),this,SLOT(DoSkeletonization()));
    connect(m_pUI->validateButton,SIGNAL(clicked()),this,SLOT(DoValidation()));

    connect(m_pUI->insertButton,SIGNAL(clicked()),this,SLOT(DoInsert()));
    connect(m_pUI->deleteButton,SIGNAL(clicked()),this,SLOT(DoDeletion()));
    connect(m_pUI->moveButton,SIGNAL(clicked()),this,SLOT(DoMove()));





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

    m_CurrentFrame=0;

    m_DrawOriginal=false;
    m_DrawDiffused=false;
    m_DrawVesselness=false;
    m_DrawVertexness=false;
    m_DrawLevelset=false;
    m_DrawSegmentation=false;
    m_DrawSkeleton=false;

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

    this->m_pUI->adbutton->setEnabled(
                this->m_Project->IsCLAHEDReady(m_CurrentFrame)
                );
    this->m_pUI->adinputgroupBox->setEnabled(
    			this->m_Project->IsDiffusedReady(m_CurrentFrame));

    this->m_pUI->ptnessbutton->setEnabled(
                this->m_Project->IsCLAHEDReady(m_CurrentFrame)
                );
    this->m_pUI->ptnessinputgroupBox->setEnabled(
        			this->m_Project->IsDiffusedReady(m_CurrentFrame));
    this->m_pUI->vtxnessbutton->setEnabled(
                this->m_Project->IsCLAHEDReady(m_CurrentFrame)
                );
    this->m_pUI->vtxnessinputgroupBox->setEnabled(
            			this->m_Project->IsDiffusedReady(m_CurrentFrame));

    this->m_pUI->vlocatorbutton->setEnabled(
                this->m_Project->IsVertexnessReady(m_CurrentFrame)
                );

#ifndef VINODTH_STYLE
    this->m_pUI->lvlsetbutton->setEnabled(
                //this->m_Project->IsVertexLocationsReady(m_CurrentFrame) &
                //this->m_Project->IsPlatenessReady(m_CurrentFrame)
                true
                );
    this->m_pUI->lvlsetthreshbutton->setEnabled(
                //this->m_Project->IsLevelSetReady(m_CurrentFrame)
                true
                );
    this->m_pUI->sklbutton->setEnabled(
                //this->m_Project->IsThresholdedReady(m_CurrentFrame)
                true
                );
#endif
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

#ifdef OUT_OF_CONTROL
void TissueTracker::Save(){
    std::cout<< m_CurrentFrame <<std::endl;
    m_Project->AddTissueDescriptor(m_CurrentFrame,descriptor);
}
#endif

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
    //this->m_pUI->adGroupBox->setEnabled(true);
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
    this->UpdateControls();
    this->UpdateDisplay();
}

void TissueTracker::DoLateralImageSegmentation(){
	LateralImageVolumeSegmentationCommand command;

	float threshold= (float)atof(this->m_pUI->lhLowThresholdtxt->text().toStdString().c_str());

	command.SetRelativeThreshold(threshold);
	command.SetInputImage(m_Project->GetRawImage(m_CurrentFrame));
	command.Do();
	m_Project->AddLateralImageVolumeSegmented(m_CurrentFrame,command.GetOutput());
}

void TissueTracker::DoCLAHE(){
	CLAHECommand command;
	float radius= (float)atof(this->m_pUI->claheRadiustxt->text().toStdString().c_str());
	float maxSlope= (float)atof(this->m_pUI->claheSlopetxt->text().toStdString().c_str());

	command.SetRadius(radius);
	command.SetMaxSlope(maxSlope);
	command.SetInputImage(m_Project->GetLateralImageVolumeSegmentedImage(m_CurrentFrame));
	command.Do();
	m_Project->AddCLAHED(m_CurrentFrame,command.GetOutputImage());
}

void TissueTracker::DoSurfaceSegmentation(){
	float sigmaxy= (float)atof(this->m_pUI->ssxyVartxt->text().toStdString().c_str());
	float sigmaz= (float)atof(this->m_pUI->sszVartxt->text().toStdString().c_str());
	float threshold=(float)atof(this->m_pUI->ssthreshtxt->text().toStdString().c_str());


	SurfaceSegmentationCommand command;

	command.SetVarXY(sigmaxy);
	command.SetVarZ(sigmaz);
	command.SetThres(threshold);
	assert(m_Project->GetRawImage(m_CurrentFrame));
	command.SetInputImage(m_Project->GetRawImage(m_CurrentFrame));

	command.Do();

	command.GetOuputBinary();
	m_Project->AddSurfaceSegmented(m_CurrentFrame,command.GetOutputImage());
    this->m_pUI->showSurfaceSegmentedCBox->setChecked(true);

    this->UpdateControls();
    this->UpdateDisplay();
//    this->m_pUI->ptnessGroupBox->setEnabled(true);
//    this->m_pUI->vtxnessGroupBox->setEnabled(true);
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
		assert(m_Project->GetCLAHEDImage(m_CurrentFrame));
		castingCommand.SetInput(m_Project->GetCLAHEDImage(m_CurrentFrame));
		castingCommand.Do();
		command.SetInputImage(castingCommand.GetOutput());

	}else{
		assert(m_Project->GetDiffusedImage(m_CurrentFrame));
		command.SetInputImage(m_Project->GetDiffusedImage(m_CurrentFrame));

	}
	command.Do();


	m_Project->AddDiffused(m_CurrentFrame,command.GetOutputImage());

	this->m_pUI->showDiffusedCBox->setChecked(true);

	this->UpdateControls();
	this->UpdateDisplay();
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
		assert(m_Project->GetCLAHEDImage(m_CurrentFrame));
		castingCommand.SetInput(m_Project->GetCLAHEDImage(m_CurrentFrame));
		castingCommand.Do();
		command.SetInput(castingCommand.GetOutput());
	}else{
		assert(m_Project->GetDiffusedImage(m_CurrentFrame));
		command.SetInput(m_Project->GetDiffusedImage(m_CurrentFrame));
	}

	command.Do();

	m_Project->AddPlateness(m_CurrentFrame,command.GetPlatenessImage());
	//m_Project->AddOrientation(m_CurrentFrame,command.GetOrientationImage());

	this->m_pUI->showPlatenessCBox->setChecked(true);

	this->UpdateControls();
	this->UpdateDisplay();
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
		assert(m_Project->GetCLAHEDImage(m_CurrentFrame));
		castingCommand.SetInput(m_Project->GetCLAHEDImage(m_CurrentFrame));
		castingCommand.Do();
		command.SetInput(castingCommand.GetOutput());
	}else{
		assert(m_Project->GetDiffusedImage(m_CurrentFrame));
		command.SetInput(m_Project->GetDiffusedImage(m_CurrentFrame));
	}

    command.Do();
    m_Project->AddVertexness(m_CurrentFrame,command.GetVertexnessImage());

    this->m_pUI->showVertexnessCBox->setChecked(true);

//this->m_pUI->vlocatorGroupBox->setEnabled(true);

    this->UpdateControls();
    this->UpdateDisplay();
}


void TissueTracker::DoVertexLocation(){

    int threshold = this->m_pUI->vlocatorslider->value();

    double realThres= (double) threshold/100;

    VertexLocationCommand command;

    command.SetLocalMaxThreshold(realThres);
    assert(m_Project->GetVertexnessImage(m_CurrentFrame));
    command.SetVertexnessImage(m_Project->GetVertexnessImage(m_CurrentFrame));
    command.Do();
    m_Project->AddVertexLocations(m_CurrentFrame,command.GetLocalMaxima());

    this->m_pUI->showVertexLocationsCBox->setChecked(true);

    this->UpdateControls();
    this->UpdateDisplay();
}


void TissueTracker::DoPrimalCalculation(){

	AdherensJunctionSegmentationCommand command;

	assert(m_Project->GetVertexLocations(m_CurrentFrame));
	assert(m_Project->GetPlatenessImage(m_CurrentFrame));

	command.SetVertexLocations(m_Project->GetVertexLocations(m_CurrentFrame));
	command.SetPlatenessImage(m_Project->GetPlatenessImage(m_CurrentFrame));
	command.Do();

	assert(command.GetTissueDescriptor());

	m_Project->AddTissueDescriptor(m_CurrentFrame,command.GetTissueDescriptor());

	this->m_pUI->showPrimalCBox->setChecked(true);
	this->UpdateControls();
	this->UpdateDisplay();

}


void TissueTracker::DoValidation(){


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

}

void TissueTracker::DoInsert(){
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
	assert(m_Project->GetTissueDescriptor(m_CurrentFrame));
	m_PrimalGraphInsertionInteractor->SetTissueDescriptor(m_Project->GetTissueDescriptor(m_CurrentFrame));
	m_PrimalGraphInsertionInteractor->Init();
	m_RenderWindowInteractor->SetInteractorStyle(m_PrimalGraphInsertionInteractor);

	m_RenderWindowInteractor->Start();

}


void TissueTracker::DoMove(){
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
		m_PrimalGraphMoveInteractor->SetTissueDescriptor(m_Project->GetTissueDescriptor(m_CurrentFrame));

		vtkSmartPointer<vtkActor> target=m_PrimalGraphStandardInteractor->GetSelectedVertex();
		assert(target);
		m_PrimalGraphMoveInteractor->SetTarget(target);

		assert(m_Project->GetTissueDescriptor(m_CurrentFrame));



		m_RenderWindowInteractor->SetInteractorStyle(m_PrimalGraphMoveInteractor);

		m_RenderWindowInteractor->Start();
}
#if 0
#if 0
void TissueTracker::DoLevelSet(){

    m_Facade.SetVertexnessImage(m_Project->GetVertexnessImage(m_CurrentFrame));
    m_Facade.SetOrientationImage(m_Project->GetOrientationImage(m_CurrentFrame));
    m_Facade.SetPlatenessImage(m_Project->GetPlatenessImage(m_CurrentFrame));

    m_Facade.SetLevelSetSigma(this->m_pUI->lvlsetsigmatxt->text().toDouble());
    std::cout<<"got here in levelset"<<std::endl;
    m_Facade.DoLevelSet();
    m_Project->AddLevelSet(m_CurrentFrame,m_Facade.GetLevelSetImage());

    this->m_pUI->showLevelSetCBox->setChecked(true);

    this->UpdateControls();
    this->UpdateDisplay();

    this->m_pUI->lvlsetthreshGroupBox->setEnabled(true);
#endif
#ifdef OUT_OF_CONTROL
    m_levelsetdone=true;
#endif

}
#endif



void TissueTracker::DoDeletion(){
	assert(m_PrimalGraphStandardInteractor->IsSelectedEdge() ||m_PrimalGraphStandardInteractor->IsSelectedVertex()  );

	TissueDescriptor::Pointer descriptor = m_Project->GetTissueDescriptor(m_CurrentFrame);

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
}

void TissueTracker::DoDualCalculation(){
	CellGraphCommand command;

	assert(m_Project->GetTissueDescriptor(m_CurrentFrame));
	command.SetPrimalGraph(m_Project->GetTissueDescriptor(m_CurrentFrame));
	command.Do();

	m_Project->AddTissueDescriptor(m_CurrentFrame,command.GetGraphs());
}


#if 0
void TissueTracker::DoThresholding(){

    double value = this->m_pUI->lvlsetthreshslider->value();
    std::cout << "Threshold: " << value <<std::endl;

    m_Facade.SetLevelsetThreshold(value);

    m_Facade.SetLevelSetImage(m_Project->GetLevelSetImage(m_CurrentFrame));
    m_Facade.DoThresholding();

    m_Project->AddThresholded(m_CurrentFrame,m_Facade.GetThresholdedImage());

    this->m_pUI->showSegmentationCBox->setChecked(true);

    this->UpdateControls();
    this->UpdateDisplay();
    this->m_pUI->sklGroupBox->setEnabled(true);
}

void TissueTracker::DoSkeletonization(){

    m_Facade.SetThresholdedImage(m_Project->GetThresholdedImage(m_CurrentFrame));
    m_Facade.DoSkeletonization();
    m_Project->AddSkeleton(m_CurrentFrame,m_Facade.GetSkeletonImage());
    this->m_pUI->showSkeletonCBox->setChecked(true);
    this->UpdateControls();
    this->UpdateDisplay();
}


void TissueTracker::DoPrimalGiaaCalculation(){

#ifdef OUT_OF_CONTROL
    std::cout<<"Clicked primal"<<std::endl;
    m_Facade.SetSkeletonImage(m_Project->GetSkeletonImage(m_CurrentFrame));
    m_Facade.DoPrimalCalculation();
    std::cout << "Just did primal" << std::endl;  
    m_Project->AddTissueDescriptor(m_CurrentFrame,m_Facade.GetTissueDescription());
    descriptor = this->m_Project->GetTissueDescriptor(m_CurrentFrame);
    std::cout << "num edges" << std::endl;
    std::cout << boost::num_edges(descriptor->m_SkeletonGraph_listS) << std::endl;
    std::cout << boost::num_vertices(descriptor->m_SkeletonGraph_listS) << std::endl;  
    this->DoFinalize();
    std::cout << "added desc " << std::endl;
    m_Spacing = m_Project->GetSpacing();

    this->m_pUI->showPrimalCBox->setChecked(true);      
    this->UpdateControls();
    this->UpdateDisplay();
#endif
}
#endif
#if 0

void TissueTracker::DoModSim(){
#if 0
	// Render composite. In default mode. For coverage.
	vtkSmartPointer<ValidationInteractor> style = vtkSmartPointer<ValidationInteractor>::New();
	style->SetDefaultRenderer(m_CurrentRenderer);
    renderWindowInteractor->SetRenderWindow (m_RenderWindow);
    renderWindowInteractor->SetInteractorStyle(style);
    renderWindowInteractor->Initialize();
    std::cout << "Initialized" << std::endl;
    renderWindowInteractor->Start();

#endif
}

#endif
#if 0
void TissueTracker::DoSelection(){

/*vtkSmartPointer<DualInteractor> style = vtkSmartPointer<DualInteractor>::New();
renderWindowInteractor->SetRenderWindow (m_RenderWindow);
renderWindowInteractor->SetInteractorStyle(style);
renderWindowInteractor->Initialize();
std::cout << "Initialized" << std::endl;
renderWindowInteractor->Start();*/
}
#endif






void TissueTracker::DoTracking(){

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

		m_Project->AddTrackedTissueDescriptor(frame++,*it);
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
	assert(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
	command.SetTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
	command.Do();

	m_Project->AddTrackedEllipses(m_CurrentFrame,command.GetEllipses());


}

void TissueTracker::DoDomains(){

	ComputeDomainsCommand command;
	unsigned int order= (unsigned int)atoi(this->m_pUI->ordertxt->text().toStdString().c_str());
	command.SetOrder(order);
	command.SetTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
	command.Do();
	m_Project->AddTrackedDomains(m_CurrentFrame,command.GetDomains());

}

void TissueTracker::DoTectonics(){

	ComputeDomainsCommand domainsCommand;

	unsigned int order= (unsigned int)atoi(this->m_pUI->ordertxt->text().toStdString().c_str());
	domainsCommand.SetOrder(order);
	domainsCommand.SetTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
	domainsCommand.Do();

	TectonicsCommand tectonicsCommand;
	tectonicsCommand.SetDomains(domainsCommand.GetDomains());
	tectonicsCommand.SetCurrentTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
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

}

void TissueTracker::DoCurrent(){

	//this->DoSurfaceSegmentation();
	this->DoLateralImageSegmentation();
	this->DoCLAHE();

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
    for(unsigned int frame=0;frame<m_Project->GetNumFrames();frame++){
  		this->SetFrame(frame+1);
  		DoCurrent();

    }
}

void TissueTracker::UpdateDisplay(){

    vtkSmartPointer<vtkRenderWindow> renWin = this->m_pUI->m_vtkview->GetRenderWindow();

    m_CurrentRenderer->RemoveAllViewProps();

    if(this->m_pUI->showOriginalCBox->isChecked() && m_Project->IsRawReady(m_CurrentFrame)){
    	RawImageDrawer originalDrawer;
    	originalDrawer.SetRenderer(m_CurrentRenderer);
    	originalDrawer.SetRawImage(m_Project->GetRawImage(m_CurrentFrame));
    	originalDrawer.Draw();
    }

    if(this->m_pUI->showLHCBox->isChecked() && m_Project->IsLateralImageVolumeSegmentedReady(m_CurrentFrame)){
        	RawImageDrawer originalDrawer;
        	originalDrawer.SetRenderer(m_CurrentRenderer);
        	originalDrawer.SetRawImage(m_Project->GetLateralImageVolumeSegmentedImage(m_CurrentFrame));
        	originalDrawer.Draw();
        }
    if(this->m_pUI->showCLAHECBox->isChecked() && m_Project->IsCLAHEDReady(m_CurrentFrame)){
            	RawImageDrawer originalDrawer;
            	originalDrawer.SetRenderer(m_CurrentRenderer);
            	originalDrawer.SetRawImage(m_Project->GetCLAHEDImage(m_CurrentFrame));
            	originalDrawer.Draw();
    }

    if(this->m_pUI->showSurfaceSegmentedCBox->isChecked() && m_Project->IsSurfaceSegmentedReady(m_CurrentFrame)){
    	RawImageDrawer originalDrawer;
    	originalDrawer.SetRenderer(m_CurrentRenderer);
    	originalDrawer.SetRawImage(m_Project->GetSurfaceSegmentedImage(m_CurrentFrame));
    	originalDrawer.Draw();
    }

    if(this->m_pUI->showDiffusedCBox->isChecked() && this->m_Project->IsDiffusedReady(m_CurrentFrame)){
    	//DiffusedImageDrawer<giaa::TissueSegmentation::DiffusedImageType> diffusedDrawer(m_CurrentRenderer,m_Project->GetDiffusedImage(m_CurrentFrame));
    	DiffusedImageDrawer diffusedDrawer;
    	diffusedDrawer.SetRenderer(m_CurrentRenderer);
    	diffusedDrawer.SetDiffusedImage(m_Project->GetDiffusedImage(m_CurrentFrame));
    	diffusedDrawer.Draw();

    }

    if(this->m_pUI->showPlatenessCBox->isChecked() && m_Project->IsPlatenessReady(m_CurrentFrame)){
    	PlatenessImageDrawer platenessDrawer;
    	platenessDrawer.SetRenderer(m_CurrentRenderer);
    	platenessDrawer.SetPlatenessImage(m_Project->GetPlatenessImage(m_CurrentFrame));
    	platenessDrawer.Draw();
    }

    if(this->m_pUI->showVertexnessCBox->isChecked() && this->m_Project->IsVertexnessReady(m_CurrentFrame)){
    	VertexnessImageDrawer vertexnessDrawer;

    	vertexnessDrawer.SetRenderer(m_CurrentRenderer);
    	vertexnessDrawer.SetVertexnessImage(m_Project->GetVertexnessImage(m_CurrentFrame));
    	vertexnessDrawer.Draw();
    }

    if(this->m_pUI->showVertexLocationsCBox->isChecked() && m_Project->IsVertexLocationsReady(m_CurrentFrame)){
    	VertexLocationsDrawer vertexLocationsDrawer;
    	vertexLocationsDrawer.SetRenderer(m_CurrentRenderer);
    	vertexLocationsDrawer.SetVertexLocations(m_Project->GetVertexLocations(m_CurrentFrame));
    	vertexLocationsDrawer.SetSpacing(m_Project->GetSpacing());
    	vertexLocationsDrawer.Draw();
    }
#if 0
    if(this->m_pUI->showLevelSetCBox->isChecked() && m_Project->IsLevelSetReady(m_CurrentFrame)){

    }

    if(this->m_pUI->showSegmentationCBox->isChecked() && this->m_Project->IsThresholdedReady(m_CurrentFrame)){

    }
    if(this->m_pUI->showSkeletonCBox && this->m_Project->IsSkeletonReady(m_CurrentFrame)){

    }
#endif

    if(this->m_pUI->showPrimalCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){

    	m_PrimalGraphDrawer = PrimalGraphDrawer::New();

    	m_PrimalGraphDrawer->SetRenderer(m_CurrentRenderer);
    	m_PrimalGraphDrawer->SetTissueDescriptor(m_Project->GetTissueDescriptor(m_CurrentFrame));
    	m_PrimalGraphDrawer->SetSpacing(m_Project->GetSpacing());
    	m_PrimalGraphDrawer->Draw();
    }

    if(this->m_pUI->showSimpleCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){

    }

    if(this->m_pUI->showDualCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){

    	DualGraphDrawer dualGraphDrawer;
    	dualGraphDrawer.SetRenderer(m_CurrentRenderer);
    	dualGraphDrawer.SetTissueDescriptor(m_Project->GetTissueDescriptor(m_CurrentFrame));
    	dualGraphDrawer.SetSpacing(m_Project->GetSpacing());
    	dualGraphDrawer.Draw();
    }
    if(this->m_pUI->showTrackingCBox->isChecked() && this->m_Project->IsTrackedTissueDescriptorReady(m_CurrentFrame)){
    	if(!m_TrackingDrawer) m_TrackingDrawer=TrackingDrawer::New();
    	assert(m_TrackingDrawer);
    	m_TrackingDrawer->SetRenderer(m_CurrentRenderer);
    	m_TrackingDrawer->SetTrackedDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
    	m_TrackingDrawer->Draw();


    }
    if(this->m_pUI->showMotionVectorCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){

    	MotionVectorDrawer motionVectorDrawer;
    	motionVectorDrawer.SetRenderer(m_CurrentRenderer);
    	motionVectorDrawer.SetTrackedDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
    	motionVectorDrawer.Draw();

    }
    if(this->m_pUI->showEllipseCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){
    	EllipseDrawer ellipseDrawer;

    	ellipseDrawer.SetRenderer(m_CurrentRenderer);
    	ellipseDrawer.SetEllipses(m_Project->GetTrackedEllipses(m_CurrentFrame));
    	ellipseDrawer.Draw();

    }
    if(this->m_pUI->showTectonicsCBox->isChecked() && this->m_Project->IsDomainStrainRatesReady(m_CurrentFrame)){
    	DomainStrainRatesDrawer domainStrainRatesDrawer;

    	domainStrainRatesDrawer.SetRenderer(m_CurrentRenderer);
    	domainStrainRatesDrawer.SetTrackedTissueDescriptor(m_Project->GetTrackedTissueDescriptor(m_CurrentFrame));
    	domainStrainRatesDrawer.SetDomainStrainRates(m_Project->GetDomainStrainRates(m_CurrentFrame));
    	domainStrainRatesDrawer.Draw();
    }

    if(this->m_pUI->showAreaMapCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){

    }

    if(this->m_pUI->showPolygonMapCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){

    }

    if(this->m_pUI->showRosettesCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){


    }

    if(this->m_pUI->showEdgeVariationMapCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){
    
    }


    if(this->m_pUI->showClusterMapCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){
        

    }

    if(this->m_pUI->showJxDegMapCBox->isChecked() && this->m_Project->IsTissueDescriptorReady(m_CurrentFrame)){

    }

    if(this->m_pUI->showSecondChannelCBox->isChecked() && m_Project->IsSCReady(m_CurrentFrame)){
        
    }

    //m_CurrentRenderer->ResetCamera();
    // Render composite. In default mode. For coverage.
    renWin->Render();
}
