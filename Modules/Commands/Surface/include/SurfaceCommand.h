/*
 * SurfaceCommand.h
 *
 *  Created on: Nov 25, 2013
 *      Author: morgan
 */

#ifndef SURFACECOMMAND_H_
#define SURFACECOMMAND_H_

#include "AppCommand.h"
#include <itkImage.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkSigmoidImageFilter.h>
#include <itkFastMarchingImageFilter.h>
#include <itkImageMomentsCalculator.h>
namespace ttt {

class SurfaceCommand: public ttt::AppCommand {
	typedef itk::Image<float,3> InputImageType;
	typedef itk::Image<float,3> OutputImageType;
public:
	SurfaceCommand();
	virtual ~SurfaceCommand();
	virtual void Do();

	virtual void SetInputImage(const InputImageType::Pointer & input){
		m_InputImage=input;
	}
	virtual void GetOutputImage(const InputImageType::Pointer & output){

	}
private:
	typedef itk::ImageMomentsCalculator<InputImageType> ImageMomentsCalculatorType;

	typedef itk::BinaryThresholdImageFilter<InputImageType, OutputImageType> ThresholdingFilterType;

	typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<InputImageType,InputImageType> GradientFilterType;

	typedef itk::SigmoidImageFilter<InputImageType, InputImageType> SigmoidFilterType;
	typedef itk::FastMarchingImageFilter<InputImageType, InputImageType> FastMarchingFilterType;

	GradientFilterType::Pointer m_GradientMagnitude;
	SigmoidFilterType::Pointer m_Sigmoid;

	FastMarchingFilterType::Pointer m_FastMarching;

	ThresholdingFilterType::Pointer m_Thresholder;

	ImageMomentsCalculatorType::Pointer m_MomentsCalculator;

	InputImageType::Pointer m_InputImage;
	OutputImageType::Pointer m_OutputImage;
};

} /* namespace ttt */

#endif /* SURFACECOMMAND_H_ */
