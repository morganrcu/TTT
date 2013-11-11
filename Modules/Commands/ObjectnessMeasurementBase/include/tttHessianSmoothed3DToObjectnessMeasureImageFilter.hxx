/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHessianSmoothed3DToVesselnessMeasureImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2007/06/12 20:59:44 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHessianSmoothed3DToObjectnessMeasureImageFilter_txx
#define __itkHessianSmoothed3DToObjectnessMeasureImageFilter_txx

#include "itkHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "vnl/vnl_math.h"

#define EPSILON  1e-03

#include "itkImageFileWriter.h"
template<class ImageType> void writeImage(
		const typename ImageType::Pointer & img, const std::string & prefix, const std::string & sufix) {
	typedef unsigned char WritePixelType;
	typedef itk::Image<WritePixelType, 3> WriteImageType;
	typedef itk::ImageFileWriter<ImageType> WriterType;
//	typedef itk::VTKImageIO ImageIOWriterType;
#if 0
	typedef itk::RescaleIntensityImageFilter<ImageType, WriteImageType> RescalerType;
#endif
	std::stringstream outputFormat(std::stringstream::in | std::stringstream::out);

	outputFormat << prefix << "-" << sufix << ".mha";
	std::string filename;
	outputFormat >> filename;
	{
#if 0
		typename RescalerType::Pointer rescaler1 = RescalerType::New();
		rescaler1->SetInput(img);
		rescaler1->SetOutputMinimum(0);
		rescaler1->SetOutputMaximum(255);
		rescaler1->Update();
#endif
		typename WriterType::Pointer writer1 = WriterType::New();
//		ImageIOWriterType::Pointer imageIOWriter1 = ImageIOWriterType::New();
		writer1->SetFileName(filename);
//		writer1->SetImageIO(imageIOWriter1);
		writer1->SetInput(img);
		writer1->Update();
	}
}
namespace itk
{

/**
 * Constructor
 */
template <class TObjectnessMeasurementFunction, typename TPixel >
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction, TPixel >
::HessianSmoothed3DToObjectnessMeasureImageFilter()
{
  m_SymmetricEigenValueFilter = EigenAnalysisFilterType::New();
  m_SymmetricEigenValueFilter->SetDimension( ImageDimension );
  m_SymmetricEigenValueFilter->OrderEigenValuesBy(EigenAnalysisFilterType::FunctorType::OrderByValue );

  // By default, scale the vesselness measure by the largest
  // eigen value

}

template<class TObjectnessMeasurementFunction,typename TPixel>
void
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction,TPixel>::BeforeThreadedGenerateData(){
	  m_SymmetricEigenValueFilter->SetInput( this->GetInput() );
	  m_SymmetricEigenValueFilter->Update();
}
template <class TObjectnessMeasurementFunction, typename TPixel >
void 
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction, TPixel >::ThreadedGenerateData(const OutputRegionType& windowRegion, ThreadIdType threadId){
  itkDebugMacro(
      << "HessianSmoothed3DToObjectMeasureImageFilter generating data ");

  typename OutputImageType::Pointer output = this->GetOutput();

  typedef typename EigenAnalysisFilterType::OutputImageType EigenValueImageType;


  
  const typename EigenValueImageType::ConstPointer eigenImage = 
                    m_SymmetricEigenValueFilter->GetOutput();
  
  // walk the region of eigen values and get the vesselness measure
  EigenValueArrayType eigenValue;

  ImageRegionConstIterator< EigenValueImageType > it(eigenImage, windowRegion );
  ImageRegionIterator< OutputImageType > oit( output, windowRegion );


  it.GoToBegin();
  oit.GoToBegin();
  while (!it.IsAtEnd())
    {
	  oit.Set(m_ObjectnessMeasurementFunction->ComputeObjectProperty(it.Get()));
	  ++it;
	  ++oit;
    }
}
template<class TObjectnessMeasurementFunction,typename TPixel>
void
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction,TPixel>::AfterThreadedGenerateData(){
}


template <class TObjectnessMeasurementFunction, typename TPixel >
void
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction, TPixel >::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  
}


} // end namespace itk
  
#endif
