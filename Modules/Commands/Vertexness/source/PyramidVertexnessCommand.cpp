#include "PyramidVertexnessCommand.h"
#include "itkMaximumImageFilter.h"
void ttt::PyramidVertexnessCommand::Do(){

	assert(this->m_SigmaMax >= m_SigmaMin);

	double range = vcl_log(m_SigmaMax) - vcl_log(m_SigmaMin);

	double scaleStepVertexness = range/(this->m_SigmaSteps);

	m_VertexnessImages.resize(this->m_SigmaSteps+1);
	for(unsigned int i=0;i<=m_SigmaSteps;i++){
		//double scale = m_LowerScale + m_ScaleStep*i;
		double scale=vcl_exp( vcl_log (m_SigmaMin) + scaleStepVertexness * i);
		typedef ttt::MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter<ttt::VertexnessMeasurementFunction,VertexnessImageType> VertexnessFilterType;

		typedef ttt::VertexnessMeasurementFunction VertexnessFunctionType;
		VertexnessFunctionType::Pointer vertexnessFunction = VertexnessFunctionType::New();

		VertexnessFilterType::Pointer m_Vertexness=VertexnessFilterType::New();

		m_Vertexness->SetObjectnessMeasurementFunction(vertexnessFunction);
		m_Vertexness->SetSigmaMin(scale);
		m_Vertexness->SetSigmaMax(scale);
		m_Vertexness->SetNumberOfSigmaSteps(1);
		m_Vertexness->SetInput(m_Input);
		m_Vertexness->Update();

		m_VertexnessImages[i]=m_Vertexness->GetOutput();

	}
	typedef itk::MinimumMaximumImageCalculator<VertexnessImageType> MinimumMaximumImageCalculator;

	MinimumMaximumImageCalculator::Pointer  maximumCalculator=MinimumMaximumImageCalculator::New();

	std::vector<float> maximums(m_SigmaSteps+1);
	for(unsigned int i=0;i<=m_SigmaSteps;i++){
		maximumCalculator->SetImage(m_VertexnessImages[i]);
		maximumCalculator->Compute();
		maximums[i]=maximumCalculator->GetMaximum();
	}


	float totalMax = (*std::max_element(maximums.begin(),maximums.end()));


	for(int i=0;i<=m_SigmaSteps;i++){
		typedef typename itk::RescaleIntensityImageFilter<VertexnessImageType,VertexnessImageType> Rescaler01Type;
		Rescaler01Type::Pointer rescaler = Rescaler01Type::New();

		rescaler->SetOutputMinimum(0);
		rescaler->SetOutputMaximum(maximums[i]/totalMax);
		rescaler->SetInput(m_VertexnessImages[i]);
		rescaler->Update();
		m_VertexnessImages[i]=rescaler->GetOutput();

	}

}

ttt::PyramidVertexnessCommand::VertexnessImageType::Pointer ttt::PyramidVertexnessCommand::GetCollapsedPyramid(){
	typedef itk::MaximumImageFilter<VertexnessImageType> MaximumFilterType;

	MaximumFilterType::Pointer maximumFilter = MaximumFilterType::New();
	VertexnessImageType::Pointer maximumImage =m_VertexnessImages[0];
	for(unsigned int i=1;i<m_VertexnessImages.size();i++){
		maximumFilter->SetInput(0,m_VertexnessImages[i]);
		maximumFilter->SetInput(1,maximumImage);
		maximumFilter->Update();
		maximumImage=maximumFilter->GetOutput();
	}
	return maximumImage;

}
