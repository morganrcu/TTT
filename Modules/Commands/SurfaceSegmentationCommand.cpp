/*
 * SurfaceSegmentationCommand.cpp
 *
 *  Created on: Sep 19, 2013
 *      Author: morgan
 */
#include "SurfaceSegmentationCommand.h"


void SurfaceSegmentationCommand::Do(){

	this->m_Integer2SmoothingFilter->SetInput(this->m_Input);
	this->m_Integer2SmoothingFilter->Update();
	this->m_SmoothingFilter->SetInput( this->m_Integer2SmoothingFilter->GetOutput());
	itk::FixedArray<double,3> variances;
	variances[0]=m_VarXY;
	variances[1]=m_VarXY;
	variances[2]=m_VarZ;
	this->m_SmoothingFilter->SetVariance( variances );
	this->m_SmoothingFilter->Update();

	this->m_Smoothing2IntegerFilter->SetInput(this->m_SmoothingFilter->GetOutput());
	this->m_Smoothing2IntegerFilter->Update();

	this->m_ThresholdingFilter->SetInput(m_Smoothing2IntegerFilter->GetOutput());
	this->m_ThresholdingFilter->SetLowerThreshold(m_Thres);
	this->m_ThresholdingFilter->SetUpperThreshold(255);
	this->m_ThresholdingFilter->SetInsideValue(255);
	this->m_ThresholdingFilter->SetOutsideValue(0);
	this->m_ThresholdingFilter->Update();


	this->m_OutputBinary=this->m_ThresholdingFilter->GetOutput();


	m_AndFilter->SetInput1(this->m_Input);
	m_AndFilter->SetInput2(this->m_OutputBinary);
	m_AndFilter->Update();
	this->m_OutputImage=m_AndFilter->GetOutput();
	//this->m_OutputImage

}
