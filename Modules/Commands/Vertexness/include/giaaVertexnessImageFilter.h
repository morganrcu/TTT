/*
 * giaaVertexnessImageFilter.hxx
 *
 *  Created on: 21/10/2011
 *      Author: rod
 */
#pragma once
#include "itkImageToImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "giaaHessian3DToVertexnessMeasureImageFilter.h"

namespace ttt{

template<class TInputImage, class TVertexnessImage> class VertexnessImageFilter : public itk::ImageToImageFilter<TInputImage,TVertexnessImage>{
public:
  /** Standard class typedefs. */
  typedef VertexnessImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage,TVertexnessImage>              Superclass;

  typedef itk::SmartPointer<Self>                                      Pointer;
  typedef itk::SmartPointer<const Self>                                ConstPointer;


  typedef TInputImage                                    InputImageType;
  typedef TVertexnessImage                                VertexnessImageType;
  

  typedef typename TInputImage::PixelType                InputPixelType;
  typedef typename TVertexnessImage::PixelType            VertexnessPixelType;


  /** Update image buffer that holds the best vesselness response */
  typedef VertexnessImageType                              VertexnessBufferType;


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

  

protected:
  VertexnessImageFilter();
  virtual ~VertexnessImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  typedef itk::HessianRecursiveGaussianImageFilter< InputImageType >
                                                        HessianFilterType;

  typedef giaa::Hessian3DToVertexnessMeasureImageFilter<typename HessianFilterType::OutputImageType,VertexnessImageType > VertexnessFilterType;

  /** Generate Data */
  void GenerateData( void );

private:
  void UpdateMaximumResponse();

  double ComputeSigmaValue( int scaleLevel );

  void   AllocateUpdateBuffer();

  //purposely not implemented
  VertexnessImageFilter(const Self&);
  void operator=(const Self&); //purposely not implemented

  double                                            m_SigmaMin;
  double                                            m_SigmaMax;

  int                                               m_NumberOfSigmaSteps;

  typename VertexnessFilterType::Pointer            m_VertexnessFilter;
  typename HessianFilterType::Pointer               m_HessianFilter;


  typename VertexnessBufferType::Pointer			m_VertexnessBuffer;
 

};

}
#include "giaaVertexnessImageFilter.hxx"
