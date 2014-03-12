#include <itkImageFileReader.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <vtkAxesActor.h>
#include <QFileDialog>
#include <QString>
#include "scalespaceexplorer.h"
#include "ui_scalespaceexplorer.h"

#include "tttMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "tttVertexnessMeasurementFunction.h"
#include "tttPlatenessMeasurementFunction.h"



ScaleSpaceExplorer::ScaleSpaceExplorer(QWidget *parent) :
    QMainWindow(parent),
    m_pUI(new Ui::ScaleSpaceExplorer)
{
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



	connect(this->m_pUI->actionOPen,SIGNAL(triggered()),this,SLOT(Open()));
	connect(this->m_pUI->computePlatenessButton,SIGNAL(clicked()),this,SLOT(ComputePlateness()));
	connect(this->m_pUI->computeVertexnessButton,SIGNAL(clicked()),this,SLOT(ComputeVertexness()));
	connect(this->m_pUI->platenessScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(DrawPlateness(int)));
	connect(this->m_pUI->vertexnessScaleSlider,SIGNAL(valueChanged(int)),this,SLOT(DrawVertexness(int)));

	connect(this->m_pUI->showPlatenessCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowPlateness(int)));
	connect(this->m_pUI->showVertexnessCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowVertexness(int)));
	connect(this->m_pUI->showVertexLocationsCBox,SIGNAL(stateChanged(int)),this,SLOT(ShowVertexLocations(int)));


	m_VertexnessDrawer.SetRenderer(m_CentralRenderer);
	m_PlatenessDrawer.SetRenderer(m_CentralRenderer);
	m_VertexLocationsDrawer.SetRenderer(m_CentralRenderer);

	m_Spacing[0]=0.1022727;
	m_Spacing[1]=0.1022727;
	m_Spacing[2]=1.0192918;


}

void ScaleSpaceExplorer::Open(){

    QFileDialog dlg(NULL, tr("Open file"),"",tr("Image Files (*.tif)"));
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    QString file;
    if (dlg.exec()){
      file = dlg.selectedFiles().at(0);
    }else{
    	return;
    }


//	QString file = QFileDialog::getOpenFileName(this,"Select file...","", tr("Image Files (*.tif)"));

	typedef typename itk::ImageFileReader<ImageType> ReaderType;

	ReaderType::Pointer reader=ReaderType::New();
	reader->SetFileName(file.toStdString());
	reader->Update();


	typedef typename itk::RescaleIntensityImageFilter<ImageType,ImageType> Rescaler01Type;
	Rescaler01Type::Pointer rescaler = Rescaler01Type::New();

	rescaler->SetOutputMinimum(0);
	rescaler->SetOutputMaximum(1);

	rescaler->SetInput(reader->GetOutput());

	rescaler->Update();

	this->m_ReescaledImage=rescaler->GetOutput();
	m_ReescaledImage->SetSpacing(m_Spacing);

}

void ScaleSpaceExplorer::ComputeVertexness(){

	int steps=this->m_pUI->stepsVertexnessSpinBox->value();
	m_LowerScaleVertexness = this->m_pUI->lowerScaleVertexnessSpinBox->value();
	m_UpperScaleVertexness = this->m_pUI->upperScaleVertexnessSpinBox->value();
	double threshold = this->m_pUI->thesholdVertexnessSpinBox->value();

	assert(m_UpperScaleVertexness > m_LowerScaleVertexness);
	double range = vcl_log(m_UpperScaleVertexness) - vcl_log(m_LowerScaleVertexness);
	this->m_ScaleStepVertexness = range/(steps);
	m_VertexnessImages.resize(steps+1);
	for(int i=0;i<=steps;i++){
		//double scale = m_LowerScale + m_ScaleStep*i;
		double scale=vcl_exp( vcl_log (m_LowerScaleVertexness) + m_ScaleStepVertexness * i);
		typedef ttt::MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter<ttt::VertexnessMeasurementFunction,VertexnessImageType> VertexnessFilterType;

		typedef ttt::VertexnessMeasurementFunction VertexnessFunctionType;
		VertexnessFunctionType::Pointer vertexnessFunction = VertexnessFunctionType::New();

		VertexnessFilterType::Pointer m_Vertexness=VertexnessFilterType::New();

		m_Vertexness->SetObjectnessMeasurementFunction(vertexnessFunction);
		m_Vertexness->SetSigmaMin(scale);
		m_Vertexness->SetSigmaMax(scale);
		m_Vertexness->SetNumberOfSigmaSteps(1);
		m_Vertexness->SetInput(m_ReescaledImage);
		m_Vertexness->Update();

		m_VertexnessImages[i]=m_Vertexness->GetOutput();

	}
	typedef itk::MinimumMaximumImageCalculator<VertexnessImageType> MinimumMaximumImageCalculator;

	MinimumMaximumImageCalculator::Pointer  maximumCalculator=MinimumMaximumImageCalculator::New();

	std::vector<float> maximums(steps+1);
	for(int i=0;i<=steps;i++){
		maximumCalculator->SetImage(m_VertexnessImages[i]);
		maximumCalculator->Compute();
		maximums[i]=maximumCalculator->GetMaximum();
	}


	float totalMax = (*std::max_element(maximums.begin(),maximums.end()));


	for(int i=0;i<=steps;i++){
		typedef typename itk::RescaleIntensityImageFilter<ImageType,ImageType> Rescaler01Type;
		Rescaler01Type::Pointer rescaler = Rescaler01Type::New();

		rescaler->SetOutputMinimum(0);
		rescaler->SetOutputMaximum(maximums[i]/totalMax);
		rescaler->SetInput(m_VertexnessImages[i]);
		rescaler->Update();
		m_VertexnessImages[i]=rescaler->GetOutput();

	}

	typedef ttt::PyramidLocalMaximumImageFilter<VertexnessImageType> LocalMaxFilterType;

	LocalMaxFilterType::Pointer localMaximumFilter=LocalMaxFilterType::New();

	localMaximumFilter->SetInput(m_VertexnessImages);
	localMaximumFilter->SetThreshold(threshold);
	localMaximumFilter->GenerateData();

	m_LocalMaxima=localMaximumFilter->GetOutput();

	this->m_pUI->vertexnessScaleSlider->setMinimum(0);
	this->m_pUI->vertexnessScaleSlider->setMaximum(steps);
	this->m_pUI->vertexnessScaleSlider->setValue(0);
	DrawVertexness(0);
}

void ScaleSpaceExplorer::ComputePlateness(){

	int steps=this->m_pUI->stepsPlatenessSpinBox->value();
	m_LowerScalePlateness = this->m_pUI->lowerScalePlatenessSpinBox->value();
	m_UpperScalePlateness = this->m_pUI->upperScalePlatenessSpinBox->value();

	double range = vcl_log(m_UpperScalePlateness) - vcl_log(m_LowerScalePlateness);
	this->m_ScaleStepPlateness = range/(steps);

	m_PlatenessImages.resize(steps+1);
	for(int i=0;i<=steps;i++){
		//double scale = m_LowerScale + m_ScaleStep*i;
		double scale=vcl_exp( vcl_log (m_LowerScalePlateness) + m_ScaleStepPlateness * i);
		typedef ttt::MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter<ttt::PlatenessMeasurementFunction,PlatenessImageType> PlatenessFilterType;

		typedef ttt::PlatenessMeasurementFunction PlatenessFunctionType;

		PlatenessFunctionType::Pointer platenessFunction = PlatenessFunctionType::New();

		PlatenessFilterType::Pointer m_Plateness=PlatenessFilterType::New();

		m_Plateness->SetObjectnessMeasurementFunction(platenessFunction);

		m_Plateness->SetSigmaMin(scale);
		m_Plateness->SetSigmaMax(scale);
		m_Plateness->SetNumberOfSigmaSteps(1);
		m_Plateness->SetInput(m_ReescaledImage);
		m_Plateness->Update();

		m_PlatenessImages[i]=m_Plateness->GetOutput();

	}
	typedef itk::MinimumMaximumImageCalculator<VertexnessImageType> MinimumMaximumImageCalculator;

	MinimumMaximumImageCalculator::Pointer  maximumCalculator=MinimumMaximumImageCalculator::New();

	std::vector<float> maximums(steps+1);
	for(int i=0;i<=steps;i++){
		maximumCalculator->SetImage(m_PlatenessImages[i]);
		maximumCalculator->Compute();
		maximums[i]=maximumCalculator->GetMaximum();
	}

	float totalMax = (*std::max_element(maximums.begin(),maximums.end()));


	for(int i=0;i<=steps;i++){
		typedef typename itk::RescaleIntensityImageFilter<ImageType,ImageType> Rescaler01Type;
		Rescaler01Type::Pointer rescaler = Rescaler01Type::New();
		rescaler->SetOutputMinimum(0);
		rescaler->SetOutputMaximum(maximums[i]/totalMax);
		rescaler->SetInput(m_PlatenessImages[i]);
		rescaler->Update();
		m_PlatenessImages[i]=rescaler->GetOutput();
	}

	this->m_pUI->platenessScaleSlider->setMinimum(0);
	this->m_pUI->platenessScaleSlider->setMaximum(steps);
	this->m_pUI->platenessScaleSlider->setValue(0);
	DrawPlateness(0);
}

void ScaleSpaceExplorer::ShowVertexness(int mode){
	m_VertexnessDrawer.SetVisibility(mode!=0);
	m_CentralRenderWindow->Render();
}
void ScaleSpaceExplorer::ShowPlateness(int mode){
	m_PlatenessDrawer.SetVisibility(mode!=0);
	m_CentralRenderWindow->Render();
}
void ScaleSpaceExplorer::ShowVertexLocations(int mode){
	m_VertexLocationsDrawer.SetVisibility(mode!=0);
	m_CentralRenderWindow->Render();
}

void ScaleSpaceExplorer::DrawVertexness(int scale){
	m_VertexnessDrawer.SetImage(m_VertexnessImages[scale]);
	m_VertexnessDrawer.Draw();
	m_VertexnessDrawer.SetVisibility(this->m_pUI->showVertexnessCBox->isChecked());

	double shownScale = vcl_exp( vcl_log (m_LowerScaleVertexness) + m_ScaleStepVertexness * scale);


	QString shownScaleString("%1");
	this->m_pUI->vertexnessShownScaleLabel->setText(shownScaleString.arg(shownScale));


	m_VertexLocationsDrawer.SetVertexLocations(m_LocalMaxima[scale]);
	m_VertexLocationsDrawer.SetSpacing(m_Spacing);
	m_VertexLocationsDrawer.Draw();
	m_VertexLocationsDrawer.SetVisibility(this->m_pUI->showVertexLocationsCBox->isChecked());
	this->m_CentralRenderWindow->Render();
}
void ScaleSpaceExplorer::DrawPlateness(int scale){
	m_PlatenessDrawer.SetImage(m_PlatenessImages[scale]);
	m_PlatenessDrawer.Draw();
	m_PlatenessDrawer.SetVisibility(this->m_pUI->showPlatenessCBox->isChecked());

	double shownScale = vcl_exp( vcl_log (m_LowerScalePlateness) + m_ScaleStepPlateness * scale);


	QString shownScaleString("%1");
	this->m_pUI->platenessShownScaleLabel->setText(shownScaleString.arg(shownScale));
	this->m_CentralRenderWindow->Render();
}
ScaleSpaceExplorer::~ScaleSpaceExplorer()
{
    delete m_pUI;
}
