#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkSCIFIOImageIO.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <vtkAxesActor.h>
#include <QFileDialog>
#include <QString>
#include <vtkWindowToImageFilter.h>
#include <vtkTIFFWriter.h>
#include <vtkCamera.h>
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

	//m_CentralRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

	m_CentralRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_CentralRenderWindow->AddRenderer(m_CentralRenderer);



    m_CentralRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_CentralRenderWindowInteractor->SetRenderWindow(m_CentralRenderWindow);
    m_CentralRenderWindowInteractor->Initialize();

    //vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    //m_CentralRenderer->AddActor(axes);

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

	connect(this->m_pUI->saveScreenshotButton,SIGNAL(clicked()),this,SLOT(SaveScreenshot()));


	connect(this->m_pUI->savePlatenessButton,SIGNAL(clicked()),this,SLOT(SavePlateness()));
	connect(this->m_pUI->saveVertexnessButton,SIGNAL(clicked()),this,SLOT(SaveVertexness()));

	m_VertexnessDrawer.SetRenderer(m_CentralRenderer);
	m_PlatenessDrawer.SetRenderer(m_CentralRenderer);
	m_VertexLocationsDrawer.SetRenderer(m_CentralRenderer);

	//m_Spacing[0]=0.1395089;
	//m_Spacing[1]=0.1395089;
	//m_Spacing[2]=0.5000000;


	//m_Spacing[0]=0.1395089;
	//m_Spacing[1]=0.1395089;
	//m_Spacing[2]=0.5000000;
	//m_Spacing[0]=0.1022727;
	//m_Spacing[1]=0.1022727;
	//m_Spacing[2]=1.0192918;

	//m_Spacing[0]=0.0949848;
	//m_Spacing[1]=0.0949848;
	//m_Spacing[2]=1.2420000;
	//m_Spacing[0]=0.1569476;
	//m_Spacing[1]=0.1569476;
	//m_Spacing[2]=0.6209697;

	//m_Spacing[0]=0.1569476;
	//m_Spacing[1]=0.1569476;
	//m_Spacing[2]=0.6209697;

	//m_Spacing[0]=0.0965251;
	//m_Spacing[1]=0.0965251;
	//m_Spacing[2]=0.3782843;


	m_Spacing[0]=0.3107403;
	m_Spacing[1]=0.3107403;
	m_Spacing[2]=0.739833;
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
	ImageType::DirectionType directionType = m_ReescaledImage->GetDirection();
//	directionType[0]= -directionType[0];
	//	directionType[1]= -directionType[1];
	//directionType[2]= -directionType[2];
	std::cout << directionType << std::endl;
	directionType[2][2]=-1;
	m_ReescaledImage->SetDirection(directionType);
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



		m_ReescaledImage.Print(std::cout);
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
	m_CurrentVertexness=scale;
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
	m_CurrentPlateness=scale;
	m_PlatenessDrawer.SetImage(m_PlatenessImages[scale]);
	m_PlatenessDrawer.Draw();
	m_PlatenessDrawer.SetVisibility(this->m_pUI->showPlatenessCBox->isChecked());

	double shownScale = vcl_exp( vcl_log (m_LowerScalePlateness) + m_ScaleStepPlateness * scale);


	QString shownScaleString("%1");
	this->m_pUI->platenessShownScaleLabel->setText(shownScaleString.arg(shownScale));


	/*
	VertexnessImageType::PointType origin;
	origin=m_PlatenessImages[scale]->GetOrigin();

	vtkSmartPointer<vtkCamera> camera = m_CentralRenderer->GetActiveCamera();
	 camera->ParallelProjectionOn();

	VertexnessImageType::RegionType region = m_PlatenessImages[scale]->GetLargestPossibleRegion();
	float xc = origin[0] + 0.5*(region.GetIndex(0) + region.GetSize(0))*m_Spacing[0];
	float yc = origin[1] + 0.5*(region.GetIndex(1) + region.GetSize(1))*m_Spacing[1];
	float zc = origin[2] + (region.GetIndex(2) + region.GetSize(2))*m_Spacing[2];
	//
	//  float xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
	//  float yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
	//  float xd = (extent[1] - extent[0] + 1)*spacing[0]; // not used
	  float yd = (region.GetSize(1) - region.GetIndex(1) + 1)*m_Spacing[1];

	  float d = camera->GetDistance();
	  camera->SetParallelScale(0.5f*static_cast<float>(yd));
	  camera->SetFocalPoint(xc,yc,zc);
	  camera->SetPosition(xc,yc,-d-10);



	  m_CentralRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel
*/
	this->m_CentralRenderWindow->Render();
}
ScaleSpaceExplorer::~ScaleSpaceExplorer()
{
    delete m_pUI;
}

void ScaleSpaceExplorer::SaveScreenshot(){
	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter("Image files (*.tif)");
	dialog.setFileMode(QFileDialog::AnyFile);
	if(dialog.exec()){
		QString fileName = dialog.selectedFiles().first();
		vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =vtkSmartPointer<vtkWindowToImageFilter>::New();
		windowToImageFilter->SetInput(this->m_CentralRenderWindow);
		windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
		windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
		windowToImageFilter->Update();

		vtkSmartPointer<vtkTIFFWriter> writer = vtkSmartPointer<vtkTIFFWriter>::New();
		writer->SetFileName(fileName.toStdString().c_str());
		writer->SetInputConnection(windowToImageFilter->GetOutputPort());
		writer->Write();
}


}

void ScaleSpaceExplorer::SavePlateness(){
	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter("Image files (*.ome.tif)");
	dialog.setFileMode(QFileDialog::AnyFile);
	if(dialog.exec()){
			QString fileName = dialog.selectedFiles().first();

			typedef itk::ImageFileWriter<VertexnessImageType> WriterType;

			WriterType::Pointer writer = WriterType::New();
			itk::SCIFIOImageIO::Pointer iodriver = itk::SCIFIOImageIO::New();
			writer->SetImageIO(iodriver);
			writer->SetFileName(fileName.toStdString());
			writer->SetInput(m_PlatenessImages[m_CurrentPlateness]);
			writer->Update();

	}
}

void ScaleSpaceExplorer::SaveVertexness(){
	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter("Image files (*.ome.tif)");
	dialog.setFileMode(QFileDialog::AnyFile);
	if(dialog.exec()){
			QString fileName = dialog.selectedFiles().first();

			typedef itk::ImageFileWriter<VertexnessImageType> WriterType;


			WriterType::Pointer writer = WriterType::New();
			itk::SCIFIOImageIO::Pointer iodriver = itk::SCIFIOImageIO::New();
			writer->SetImageIO(iodriver);
			writer->SetFileName(fileName.toStdString());
			writer->SetInput(m_VertexnessImages[m_CurrentVertexness]);
			writer->Update();

	}
}



