/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMultiScaleHessianSmoothed3DToVesselnessMeasureImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/04/01 23:13:46 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter_h
#define __itkMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkObjectnessMeasurementFunction.h"
namespace itk
{
/**\class MultiScaleHessianSmoothed3DToVesselnessMeasureImageFilter
 * \brief A filter to enhance 3D vascular structures using Hessian 
 *         eigensystem in a multiscale framework
 * 
 * The vesselness measure is based on the analysis of the the Hessian 
 * eigen system. The vesseleness function is a smoothed (continous) 
 * version of the Frang's vesselness function. The filter takes an 
 * image of any pixel type and generates a Hessian image pixels at different
 * scale levels. The vesselness measure is computed from the Hessian image 
 * at each scale level and the best response is selected.  The vesselness 
 * measure is computed using HessianSmoothed3DToVesselnessMeasureImageFilter.
 *
 * Minimum and maximum sigma value can be set using SetMinSigma and SetMaxSigma
 * methods respectively. The number of scale levels is set using 
 * SetNumberOfSigmaSteps method. Exponentially distributed scale levels are 
 * computed within the bound set by the minimum and maximum sigma values 
 *  
 *
 * \par References
 *  Manniesing, R, Viergever, MA, & Niessen, WJ (2006). Vessel Enhancing 
 *  Diffusion: A Scale Space Representation of Vessel Structures. Medical 
 *  Image Analysis, 10(6), 815-825. 
 * 
 * \sa MultiScaleHessianSmoothed3DToVesselnessMeasureImageFilter 
 * \sa Hessian3DToVesselnessMeasureImageFilter
 * \sa HessianSmoothedRecursiveGaussianImageFilter 
 * \sa SymmetricEigenAnalysisImageFilter
 * \sa SymmetricSecondRankTensor
 * 
 * \ingroup IntensityImageFilters TensorObjects
 *
 */
template <class TObjectnessMeasurementFunction,class TInputImage, class TOutputImage = TInputImage >
class ITK_EXPORT MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter
: public
ImageToImageFilter< TInputImage,TOutputImage > 
{
public:
  /** Standard class typedefs. */
  typedef MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>              Superclass;

  typedef SmartPointer<Self>                                      Pointer;
  typedef SmartPointer<const Self>                                ConstPointer;

  
  typedef TInputImage                                    InputImageType;
  typedef TOutputImage                                   OutputImageType;

  typedef typename TInputImage::PixelType                InputPixelType;
  typedef typename TOutputImage::PixelType               OutputPixelType;

  /** Update image buffer that holds the best vesselness response */ 
  typedef Image< double, 3>                              UpdateBufferType;

  typedef TObjectnessMeasurementFunction ObjectnessMeasurementFunctionType;

  typedef HessianSmoothed3DToObjectnessMeasureImageFilter<ObjectnessMeasurementFunctionType, double > ObjectnessFilterType;

  /** Image dimension = 3. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                   InputImageType::ImageDimension);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Set/Get macros for Alpha */
  itkSetMacro(SigmaMin, double);
  itkGetMacro(SigmaMin, double);
  
  /** Set/Get macros for Beta */
  itkSetMacro(SigmaMax, double);
  itkGetMacro(SigmaMax, double);

  /** Set/Get macros for Number of Scales */
  itkSetMacro(NumberOfSigmaSteps, int);
  itkGetMacro(NumberOfSigmaSteps, int);



  void SetObjectnessMeasurementFunction(const typename ObjectnessMeasurementFunctionType::Pointer & objectnessMeasurementFunction){
	  m_ObjectnessFilter->SetObjectnessMeasurementFunction(objectnessMeasurementFunction);
  }


protected:
  MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter();
  ~MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  typedef HessianRecursiveGaussianImageFilter< InputImageType >
                                                        HessianFilterType;



  /** Generate Data */
  void GenerateData( void );

private:
  void UpdateMaximumResponse();

  double ComputeSigmaValue( int scaleLevel );
  
  void   AllocateUpdateBuffer();

  //purposely not implemented
  MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter(const Self&);
  void operator=(const Self&); //purposely not implemented

  double                                            m_SigmaMin;
  double                                            m_SigmaMax;

  int                                               m_NumberOfSigmaSteps;

  typename ObjectnessFilterType::Pointer           	m_ObjectnessFilter;
  typename HessianFilterType::Pointer               m_HessianFilter;

  UpdateBufferType::Pointer                         m_UpdateBuffer;


};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.hxx"
#endif
  
#endif
