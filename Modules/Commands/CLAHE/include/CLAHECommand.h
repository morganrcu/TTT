/*
 * CLAHECommand.h
 *
 *  Created on: Nov 24, 2013
 *      Author: morgan
 */

#ifndef CLAHECOMMAND_H_
#define CLAHECOMMAND_H_

#include <itkImage.h>
#include "AppCommand.h"
#include "tttCLAHEImageFilter.h"
namespace ttt {

class CLAHECommand: public ttt::AppCommand {
public:
	typedef itk::Image<unsigned char,3> InputImageType;
	typedef itk::Image<unsigned char,3> OutputImageType;
	CLAHECommand();
	virtual ~CLAHECommand();
	virtual void Do();
	inline void SetMaxSlope(float maxSlope){
		m_MaxSlope=maxSlope;
	}
	inline void SetRadius(float radius){
		m_Radius=radius;
	}
	inline void SetInputImage(const InputImageType::Pointer & image){
		m_InputImage=image;
	}
	inline typename OutputImageType::Pointer GetOutputImage(){
		return m_OutputImage;
	}
private:
	float m_MaxSlope;
	float m_Radius;
	typedef  ttt::CLAHEImageFilter< InputImageType > CLAHEImageFilterType;
	CLAHEImageFilterType::Pointer m_AdaptiveHistogramEqualizationImageFilter;
	typename InputImageType::Pointer m_InputImage;
	typename OutputImageType::Pointer m_OutputImage;
};

} /* namespace ttt */

#endif /* CLAHECOMMAND_H_ */
