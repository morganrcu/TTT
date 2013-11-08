//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

/** \addtogroup TTTSurfaceSegmentation
 *  @{
 */

#ifndef SURFACESEGMENATIONCOMMAND_H_
#define SURFACESEGMENATIONCOMMAND_H_
#include <itkImage.h>
#include <itkImage.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkAndImageFilter.h>
#include <itkCastImageFilter.h>
#include  "AppCommand.h"
/**
 * Tissue Surface segmentation method based in gaussian blurring + thresholding. Performed employing GPU methods
 */
class SurfaceSegmentationCommand : public AppCommand{
public:
	typedef unsigned char IntegerPixelDepth;
	typedef float SmoothingPixelDepth;

	typedef itk::Image<IntegerPixelDepth,3> IntegerImageType;
	//typedef itk::GPUImage<IntegerPixelDepth,3> GPUIntegerImageType;
	typedef itk::Image<SmoothingPixelDepth,3> SmoothingImageType;


private:

	typedef itk::CastImageFilter<IntegerImageType,SmoothingImageType> Integer2SmoothingFilterType;
	typedef itk::DiscreteGaussianImageFilter< SmoothingImageType, SmoothingImageType> SmoothingFilterType;
	typedef itk::CastImageFilter<SmoothingImageType,IntegerImageType> Smoothing2IntegerFilterType;
	typedef itk::BinaryThresholdImageFilter<IntegerImageType,IntegerImageType> ThresholdingFilterType;
//	typedef itk::CastImageFilter<GPUIntegerImageType,IntegerImageType> GPUInteger2IntegerFilterType;
	typedef itk::AndImageFilter<IntegerImageType,IntegerImageType> AndFilterType;

	IntegerImageType::Pointer m_Input;
	IntegerImageType::Pointer m_OutputBinary;
	IntegerImageType::Pointer m_OutputImage;

	float m_VarXY;
	float m_VarZ;
	unsigned char m_Thres;

	typename Integer2SmoothingFilterType::Pointer m_Integer2SmoothingFilter;
	typename SmoothingFilterType::Pointer m_SmoothingFilter;
	typename Smoothing2IntegerFilterType::Pointer m_Smoothing2IntegerFilter;
	typename ThresholdingFilterType::Pointer m_ThresholdingFilter;
	//typename GPUInteger2IntegerFilterType::Pointer m_GPUInteger2IntegerFilter;
	typename AndFilterType::Pointer m_AndFilter;

public:
	SurfaceSegmentationCommand(): m_VarXY(0),m_VarZ(0),m_Thres(0){
		m_Integer2SmoothingFilter = Integer2SmoothingFilterType::New();
		m_SmoothingFilter= SmoothingFilterType::New();
		m_Smoothing2IntegerFilter=Smoothing2IntegerFilterType::New();
		m_ThresholdingFilter= ThresholdingFilterType::New();
		//m_GPUInteger2IntegerFilter=GPUInteger2IntegerFilterType::New();
		m_AndFilter = AndFilterType::New();
	}

	inline void SetVarXY(float varxy){
		m_VarXY=varxy;
	}
	inline void SetVarZ(float varz){
		m_VarZ=varz;
	}
	inline void SetThres(float thres){
		m_Thres=thres;
	}

	inline void SetInputImage(const IntegerImageType::Pointer & input){
		m_Input=input;
	}

	inline IntegerImageType::Pointer GetOuputBinary(){
		return m_OutputBinary;
	}

	inline IntegerImageType::Pointer GetOutputImage(){
		return m_OutputImage;
	}
	virtual void Do();
	virtual ~SurfaceSegmentationCommand(){

	}
};

#endif
/** @}*/
