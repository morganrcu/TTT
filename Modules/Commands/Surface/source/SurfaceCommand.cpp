/*
 * SurfaceCommand.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: morgan
 */

#include "SurfaceCommand.h"
//#include "tttEmptynessMeasurementFunction.h"
#include "tttEmptynessMeasurementFunction.h"
#include "tttMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include <itkImageFileWriter.h>
namespace ttt {

SurfaceCommand::SurfaceCommand() {

	m_MomentsCalculator = ImageMomentsCalculatorType::New();
	m_FastMarching = FastMarchingFilterType::New();
	m_Thresholder = ThresholdingFilterType::New();
	m_Thresholder->SetOutsideValue(0);
	m_Thresholder->SetInsideValue(1);
	m_GradientMagnitude = GradientFilterType::New();
	m_Sigmoid = SigmoidFilterType::New();
	m_Sigmoid->SetOutputMinimum(0.0);
	m_Sigmoid->SetOutputMaximum(1.0);
}

SurfaceCommand::~SurfaceCommand() {
	// TODO Auto-generated destructor stub
}

void SurfaceCommand::Do() {

	typedef itk::ImageFileWriter<InputImageType> FileWriterType;


	typedef MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter<EmptynessMeasurementFunction,InputImageType> EmptynessFilterType;

	EmptynessFilterType::Pointer emptyFilter = EmptynessFilterType::New();

	const float timeThreshold = 20;

	m_Thresholder->SetLowerThreshold(0.0);
	m_Thresholder->SetUpperThreshold(timeThreshold);


	for(int i=-5;i<5;i++){

		const float sigma = pow(2,i);

		emptyFilter->SetSigmaMin(sigma);
		emptyFilter->SetSigmaMax(sigma);
		emptyFilter->SetNumberOfSigmaSteps(1);
		emptyFilter->SetInput(m_InputImage);

		ttt::EmptynessMeasurementFunction::Pointer function = EmptynessMeasurementFunction::New();
		emptyFilter->SetObjectnessMeasurementFunction(function);
		emptyFilter->Update();

		{
			std::stringstream fname;
			fname << "empty-" << i <<".tif";
			FileWriterType::Pointer writer = FileWriterType::New();
			writer->SetInput(emptyFilter->GetOutput());
			writer->SetFileName(fname.str());
			writer->Update();
		}
	}
	exit(-1);
#if 0
	//writer->SetInput( thresholder->GetOutput() );


	float K1 = 1;
	float K2 = 0;
	//  The SigmoidImageFilter class requires two parameters to define the linear
	//  transformation to be applied to the sigmoid argument. These parameters
	//  are passed using the \code{SetAlpha()} and \code{SetBeta()} methods. In
	//  the context of this example, the parameters are used to intensify the
	//  differences between regions of low and high values in the speed image. In
	//  an ideal case, the speed value should be $1.0$ in the homogeneous regions
	//  of anatomical structures and the value should decay rapidly to $0.0$
	//  around the edges of structures. The heuristic for finding the values is
	//  the following. From the gradient magnitude image, let's call $K1$ the
	//  minimum value along the contour of the anatomical structure to be
	//  segmented. Then, let's call $K2$ an average value of the gradient
	//  magnitude in the middle of the structure. These two values indicate the
	//  dynamic range that we want to map to the interval $[0:1]$ in the speed
	//  image.  We want the sigmoid to map $K1$ to $0.0$ and $K2$ to $1.0$. Given
	//  that $K1$ is expected to be higher than $K2$ and we want to map those
	//  values to $0.0$ and $1.0$ respectively, we want to select a negative
	//  value for alpha so that the sigmoid function will also do an inverse
	//  intensity mapping. This mapping will produce a speed image such that the
	//  level set will march rapidly on the homogeneous region and will
	//  definitely stop on the contour. The suggested value for beta is
	//  $(K1+K2)/2$ while the suggested value for alpha is $(K2-K1)/6$, which
	//  must be a negative number.  In our simple example the values are provided
	//  by the user from the command line arguments. The user can estimate these
	//  values by observing the gradient magnitude image.
	//
	//  Software Guide : EndLatex

	const float alpha = (K2 - K1) / 6; //   atof( argv[6] ); //1
	const float beta = (K1 + K2) / 2; //0
	m_Sigmoid->SetInput(m_GradientMagnitude->GetOutput());


	m_Sigmoid->SetAlpha(alpha);
	m_Sigmoid->SetBeta(beta);
	m_Sigmoid->Update();
	{
		FileWriterType::Pointer writer = FileWriterType::New();
		writer->SetInput(m_Sigmoid->GetOutput());
		writer->SetFileName("sigmoid.tif");
		writer->Update();
	}

	m_FastMarching->SetInput(m_Sigmoid->GetOutput());

	m_MomentsCalculator->SetImage(m_InputImage);

	m_MomentsCalculator->Compute();

	ImageMomentsCalculatorType::VectorType center = m_MomentsCalculator->GetCenterOfGravity();


	typedef FastMarchingFilterType::NodeContainer NodeContainer;
	typedef FastMarchingFilterType::NodeType NodeType;
	NodeContainer::Pointer seeds = NodeContainer::New();




	InputImageType::IndexType seedPosition;
	InputImageType::PointType centerPoint;
	centerPoint[0] = center[0];
	centerPoint[1] = center[1];
	centerPoint[2] = center[2];

	m_InputImage->TransformPhysicalPointToIndex(centerPoint, seedPosition);
	NodeType node;
	const double seedValue = 0.0;

	node.SetValue(seedValue);
	node.SetIndex(seedPosition);
	seeds->Initialize();
	seeds->InsertElement(0, node);

	m_FastMarching->SetTrialPoints(seeds);
	m_FastMarching->SetOutputSize(m_InputImage->GetBufferedRegion().GetSize());

	const double stoppingTime = 10000;

	m_FastMarching->SetStoppingValue(stoppingTime);
	m_FastMarching->Update();
	{
		FileWriterType::Pointer writer = FileWriterType::New();
		writer->SetInput(m_FastMarching->GetOutput());
		writer->SetFileName("front.tif");
		writer->Update();
	}

	m_Thresholder->SetInput(m_FastMarching->GetOutput());

	m_Thresholder->Update();
#endif
}

} /* namespace ttt */

