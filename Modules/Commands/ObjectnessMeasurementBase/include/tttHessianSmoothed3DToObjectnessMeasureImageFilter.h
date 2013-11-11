/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHessianSmoothed3DToVesselnessMeasureImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/06/12 22:59:15 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHessianSmoothed3DToObjectnessMeasureImageFilter_h
#define __itkHessianSmoothed3DToObjectnessMeasureImageFilter_h

#include "itkSymmetricSecondRankTensor.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "itkObjectnessMeasurementFunction.h"
namespace itk
{
/** \class HessianSmoothed3DToVesselnessMeasureImageFilter
 * \brief A filter to enhance 3D vascular structures 
 * 
 * The vesselness measure is based on the analysis of the the Hessian 
 * eigen system. The vesseleness function is a smoothed (continuous) 
 * version of the Frang's vesselness function. The filter takes an 
 * image of a Hessian pixels ( SymmetricSecondRankTensor pixels ) and
 * produces an enhanced image. The Hessian input image can be produced using 
 * itkHessianSmoothedRecursiveGaussianImageFilter. 
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
  
template <class TObjectnessMeasurementFunction,typename  TPixel > class HessianSmoothed3DToObjectnessMeasureImageFilter :
		public ImageToImageFilter< Image< SymmetricSecondRankTensor< double, 3 >, 3 >,
																	Image< TPixel, 3 > >
{
public:
  /** Standard class typedefs. */
  typedef HessianSmoothed3DToObjectnessMeasureImageFilter Self;

  typedef ImageToImageFilter< 
          Image< SymmetricSecondRankTensor< double, 3 >, 3 >, 
          Image< TPixel, 3 > >                 Superclass;

  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>             ConstPointer;
  
  typedef typename Superclass::InputImageType            InputImageType;
  typedef typename Superclass::OutputImageType           OutputImageType;
  typedef typename InputImageType::PixelType             InputPixelType;
  typedef TPixel                                         OutputPixelType;
  
  typedef TObjectnessMeasurementFunction ObjectnessMeasurementFunctionType;

  /** Image dimension = 3. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                    InputImageType::ImageDimension);

  itkStaticConstMacro(InputPixelDimension, unsigned int,
                    InputPixelType::Dimension);

  typedef  FixedArray< double, itkGetStaticConstMacro(InputPixelDimension) >
                                                          EigenValueArrayType;
  typedef  Image< EigenValueArrayType, itkGetStaticConstMacro(ImageDimension) >
                                                          EigenValueImageType;
  typedef   SymmetricEigenAnalysisImageFilter< 
            InputImageType, EigenValueImageType >     EigenAnalysisFilterType;

  typedef Image< TPixel, 3 > OuputImageType;
  typedef typename OutputImageType::RegionType OutputRegionType;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  itkSetObjectMacro(ObjectnessMeasurementFunction,ObjectnessMeasurementFunctionType);
#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(DoubleConvertibleToOutputCheck,
                  (Concept::Convertible<double, OutputPixelType>));
  /** End concept checking */
#endif

protected:
  HessianSmoothed3DToObjectnessMeasureImageFilter();
  ~HessianSmoothed3DToObjectnessMeasureImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  virtual void BeforeThreadedGenerateData();
  /** Generate Data */
  virtual void ThreadedGenerateData(const OutputRegionType& windowRegion, ThreadIdType threadId);
  virtual void AfterThreadedGenerateData();
private:

  //purposely not implemented
  HessianSmoothed3DToObjectnessMeasureImageFilter(const Self&);

  void operator=(const Self&); //purposely not implemented

  typename EigenAnalysisFilterType::Pointer     m_SymmetricEigenValueFilter;
  typename ObjectnessMeasurementFunctionType::Pointer m_ObjectnessMeasurementFunction;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHessianSmoothed3DToObjectnessMeasureImageFilter.hxx"
#endif
  
#endif
