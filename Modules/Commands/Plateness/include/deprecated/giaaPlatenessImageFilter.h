/*
 * giaaPlatenessImageFilter.hxx
 *
 *  Created on: 21/10/2011
 *      Author: rod
 */
#pragma once
#include "itkImageToImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "giaaHessian3DToPlatenessMeasureImageFilter.h"
namespace ttt{

template<class TInputImage, class TPlatenessImage,class TDirectionImage> class PlatenessImageFilter : public itk::ImageToImageFilter<TInputImage,TPlatenessImage>{
public:
  /** Standard class typedefs. */
  typedef PlatenessImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage,TPlatenessImage>              Superclass;

  typedef itk::SmartPointer<Self>                                      Pointer;
  typedef itk::SmartPointer<const Self>                                ConstPointer;


  typedef TInputImage                                    InputImageType;
  typedef TPlatenessImage                                PlatenessImageType;
  typedef TDirectionImage								 DirectionImageType;

  typedef typename TInputImage::PixelType                InputPixelType;
  typedef typename TPlatenessImage::PixelType            PlatenessPixelType;
  typedef typename TDirectionImage::PixelType			 DirectionPixelType;


  /** Update image buffer that holds the best vesselness response */
  typedef PlatenessImageType                              PlatenessBufferType;


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

  itkGetObjectMacro(DirectionImage,DirectionImageType);

protected:
  PlatenessImageFilter();
  virtual ~PlatenessImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  typedef itk::HessianRecursiveGaussianImageFilter< InputImageType >
                                                        HessianFilterType;

  typedef ttt::Hessian3DToPlatenessMeasureImageFilter<typename HessianFilterType::OutputImageType,PlatenessImageType,DirectionImageType > PlatenessFilterType;

  /** Generate Data */
  void GenerateData( void );

private:
  void UpdateMaximumResponse();

  double ComputeSigmaValue( int scaleLevel );

  void   AllocateUpdateBuffer();

  //purposely not implemented
  PlatenessImageFilter(const Self&);
  void operator=(const Self&); //purposely not implemented

  double                                            m_SigmaMin;
  double                                            m_SigmaMax;

  int                                               m_NumberOfSigmaSteps;

  typename PlatenessFilterType::Pointer            	m_PlatenessFilter;
  typename HessianFilterType::Pointer               m_HessianFilter;


  typename PlatenessBufferType::Pointer                         m_PlatenessBuffer;

  typename DirectionImageType::Pointer m_DirectionImage;

};

}
#include "giaaPlatenessImageFilter.hxx"
