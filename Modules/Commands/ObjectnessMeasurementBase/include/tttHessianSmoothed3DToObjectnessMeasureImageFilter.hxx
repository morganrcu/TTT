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

/** \addtogroup TTTObjectnessMeasurementBase
 *  @{
 */
#ifndef __itkHessianSmoothed3DToObjectnessMeasureImageFilter_txx
#define __itkHessianSmoothed3DToObjectnessMeasureImageFilter_txx

#include "tttHessianSmoothed3DToObjectnessMeasureImageFilter.h"
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
namespace ttt
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
/** @}*/
