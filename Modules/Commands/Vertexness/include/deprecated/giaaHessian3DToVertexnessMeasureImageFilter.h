/*
 * giaaHessian3DToVertexnessMeassureImageFilter.h
 *
 *  Created on: 24/10/2011
 *      Author: rod
 */
#pragma once

#include "itkSymmetricSecondRankTensor.h"
#include "itkSymmetricEigenVectorAnalysisImageFilter.h"
#include "itkImageToImageFilter.h"
namespace giaa{
/** \class HessianSmoothed3DToVertexnessMeasureImageFilter
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

template < class  TInputImage, class TVertexnessImage>
class Hessian3DToVertexnessMeasureImageFilter : public itk::ImageToImageFilter<TInputImage,TVertexnessImage>{

public:
  /** Standard class typedefs. */
  typedef Hessian3DToVertexnessMeasureImageFilter Self;

  typedef itk::ImageToImageFilter<TInputImage,TVertexnessImage>
                 Superclass;

  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  typedef TInputImage            	InputImageType;
  typedef TVertexnessImage           VertexnessImageType;

  typedef typename InputImageType::PixelType             InputPixelType;
  typedef typename VertexnessImageType::PixelType		 VertexnessPixelType;


  /** Image dimension = 3. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                    InputImageType::ImageDimension);

  itkStaticConstMacro(InputPixelDimension, unsigned int,
                    InputPixelType::Dimension);

  typedef  itk::FixedArray< double, itkGetStaticConstMacro(InputPixelDimension) >
                                                          EigenValueArrayType;

  typedef  itk::Image< EigenValueArrayType, itkGetStaticConstMacro(ImageDimension) >
                                                          EigenValueImageType;
#if 0
  typedef itk::SymmetricEigenVectorAnalysisImageFilter<
		  InputPixelType,
		  itk::FixedArray<double,3>,
		  itk::Matrix<double,3,3> > EigenAnalysisFilterType;
#endif
//  typedef  itk::SymmetricEigenAnalysisImageFilter<
//            InputImageType, EigenValueImageType >     EigenAnalysisFilterType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Set/Get macros for Alpha */
  itkSetMacro(Alpha, double);
  itkGetMacro(Alpha, double);

  /** Set/Get macros for Beta */
  itkSetMacro(Beta, double);
  itkGetMacro(Beta, double);

  /** Set/Get macros for Gamma */
  itkSetMacro(Gamma, double);
  itkGetMacro(Gamma, double);

  /** Set/Get macros for C */
  itkSetMacro(C, double);
  itkGetMacro(C, double);

  
  /** Macro to scale the vesselness measure with the
      largest eigenvalue or not */
  itkSetMacro( ScaleVesselnessMeasure, bool );
  itkGetMacro( ScaleVesselnessMeasure, bool );
  itkBooleanMacro(ScaleVesselnessMeasure);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(DoubleConvertibleToOutputCheck,
                  (itk::Concept::Convertible<double, VertexnessPixelType>));
  /** End concept checking */
#endif

protected:
  Hessian3DToVertexnessMeasureImageFilter();
  ~Hessian3DToVertexnessMeasureImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Generate Data */
  void GenerateData( void );

private:

  //purposely not implemented
  Hessian3DToVertexnessMeasureImageFilter(const Self&);

  void operator=(const Self&); //purposely not implemented

//  typename EigenAnalysisFilterType::Pointer     m_SymmetricEigenValueFilter;

  typename VertexnessImageType::Pointer			m_VertexnessImage;

  double                                        m_Alpha;
  double                                        m_Beta;
  double                                        m_Gamma;

  double                                        m_C;

  bool                                          m_ScaleVesselnessMeasure;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "giaaHessian3DToVertexnessMeasureImageFilter.hxx"
#endif
