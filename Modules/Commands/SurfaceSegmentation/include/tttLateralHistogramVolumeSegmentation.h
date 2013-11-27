#ifndef tttLATERALHISTOGRAMVOLUMESEGMENTATIONFILTER_H
#define tttLATERALHISTOGRAMVOLUMESEGMENTATIONFILTER_H
#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkAccumulateImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkImageFileWriter.h>
#if 0
template<class TImage> void StoreImage(const typename TImage::Pointer & image,const std::string & filename ){
    typedef itk::ImageFileWriter<TImage> WriterType;

    typename WriterType::Pointer writer =WriterType::New();

    writer->SetFileName(filename);
    writer->SetInput(image);
    writer->Update();
}
#endif

namespace ttt{
  template<class TImage> class LateralHistogramVolumeSegmentationFilter : public itk::ImageToImageFilter<TImage,TImage>{
    
  public:
    typedef LateralHistogramVolumeSegmentationFilter Self;
    typedef itk::SmartPointer<Self> Pointer;
    typedef TImage InputImageType;
    typedef typename TImage::PixelType PixelType;
    typedef itk::Image<double,3> ReducedImageType;
    
    itkNewMacro(Self);

  protected:
    LateralHistogramVolumeSegmentationFilter();
    virtual ~LateralHistogramVolumeSegmentationFilter(){
    }
    void GenerateData();
  private:
    LateralHistogramVolumeSegmentationFilter(const Self &);
    void SetZeroOutputRegion(const typename InputImageType::RegionType & region);
    
    typedef itk::AccumulateImageFilter<InputImageType,ReducedImageType> AccumulateImageFilterType;
    
    typedef itk::BinaryThresholdImageFilter <ReducedImageType, ReducedImageType> ThresholdFilterType;

    typedef itk::RescaleIntensityImageFilter<ReducedImageType,ReducedImageType> RescalerFilterType;

    typename AccumulateImageFilterType::Pointer m_XYAccumulateFilter;
    typename AccumulateImageFilterType::Pointer m_XZAccumulateFilter;
    typename AccumulateImageFilterType::Pointer m_YZAccumulateFilter;

    typename RescalerFilterType::Pointer m_RescalerXY;
    typename RescalerFilterType::Pointer m_RescalerXZ;
    typename RescalerFilterType::Pointer m_RescalerYZ;

    typename ThresholdFilterType::Pointer m_ThresholdFilterXY;
    typename ThresholdFilterType::Pointer m_ThresholdFilterXZ;
    typename ThresholdFilterType::Pointer m_ThresholdFilterYZ;
  };
  
  template<class TImage> LateralHistogramVolumeSegmentationFilter<TImage>::LateralHistogramVolumeSegmentationFilter(){
    m_XYAccumulateFilter = AccumulateImageFilterType::New();
    m_XYAccumulateFilter->SetAccumulateDimension(2);
    m_XYAccumulateFilter->AverageOff();

    m_XZAccumulateFilter = AccumulateImageFilterType::New();
    m_XZAccumulateFilter->SetAccumulateDimension(1);
    m_XZAccumulateFilter->AverageOff();

    m_YZAccumulateFilter = AccumulateImageFilterType::New();
    m_YZAccumulateFilter->SetAccumulateDimension(0);
    m_YZAccumulateFilter->AverageOff();
    
    m_RescalerXY=RescalerFilterType::New();
    m_RescalerXY->SetOutputMinimum(0);
    m_RescalerXY->SetOutputMaximum(100);

    m_RescalerXZ=RescalerFilterType::New();
    m_RescalerXZ->SetOutputMinimum(0);
    m_RescalerXZ->SetOutputMaximum(100);

    m_RescalerYZ=RescalerFilterType::New();
    m_RescalerYZ->SetOutputMinimum(0);
    m_RescalerYZ->SetOutputMaximum(100);

    m_ThresholdFilterXY= ThresholdFilterType::New();
    m_ThresholdFilterXY->SetInsideValue(255);
    m_ThresholdFilterXY->SetOutsideValue(0);
    m_ThresholdFilterXY->SetLowerThreshold(20);
    m_ThresholdFilterXY->SetUpperThreshold(100);
    
    m_ThresholdFilterXZ= ThresholdFilterType::New();
    m_ThresholdFilterXZ->SetInsideValue(255);
    m_ThresholdFilterXZ->SetOutsideValue(0);
    m_ThresholdFilterXZ->SetLowerThreshold(20);
    m_ThresholdFilterXZ->SetUpperThreshold(100);

    m_ThresholdFilterYZ= ThresholdFilterType::New();
    m_ThresholdFilterYZ->SetInsideValue(255);
    m_ThresholdFilterYZ->SetOutsideValue(0);
    m_ThresholdFilterYZ->SetLowerThreshold(20);
    m_ThresholdFilterYZ->SetUpperThreshold(100);

  }
  
  template<class TImage> void LateralHistogramVolumeSegmentationFilter<TImage>::GenerateData(){  
    this->GetOutput()->SetRegions(this->GetInput()->GetLargestPossibleRegion());
    this->GetOutput()->SetSpacing(this->GetInput()->GetSpacing());
    this->GetOutput()->Allocate();
    this->GetOutput()->FillBuffer(255);
    
    this->GetInput()->Print(std::cout,itk::Indent());


    {
      m_XYAccumulateFilter->SetInput(this->GetInput());
      m_XYAccumulateFilter->Update();
      
      ReducedImageType::Pointer projectionXY= m_XYAccumulateFilter->GetOutput();
      //StoreImage<ReducedImageType>(projectionXY,std::string("./projection-xy.mha"));

      m_RescalerXY->SetInput(projectionXY);
      m_ThresholdFilterXY->SetInput(m_RescalerXY->GetOutput());
      m_ThresholdFilterXY->Update();

      //StoreImage<ReducedImageType>(m_ThresholdFilterXY->GetOutput(),std::string("./binary-xy.mha"));
      itk::ImageRegionConstIterator<ReducedImageType> xyIterator (m_ThresholdFilterXY->GetOutput(),m_ThresholdFilterXY->GetOutput()->GetLargestPossibleRegion());
      
      while(!xyIterator.IsAtEnd()){
    	  if(xyIterator.Get()==0){

			  typename InputImageType::IndexType columnIndex;
			  columnIndex[0]=xyIterator.GetIndex()[0];
			  columnIndex[1]=xyIterator.GetIndex()[1];
			  columnIndex[2]=0;

			  typename InputImageType::SizeType columnSize;
			  columnSize[0]=1;
			  columnSize[1]=1;
			  columnSize[2]=this->GetOutput()->GetLargestPossibleRegion().GetSize()[2];

			  typename InputImageType::RegionType column;
			  column.SetIndex(columnIndex);
			  column.SetSize(columnSize);

			  SetZeroOutputRegion(column);
    	  }
    	  ++xyIterator;
      }
    }
    
    {
      m_XZAccumulateFilter->SetInput(this->GetInput());
      m_XZAccumulateFilter->Update();
    
      ReducedImageType::Pointer projectionXZ= m_XZAccumulateFilter->GetOutput();   
      // StoreImage<ReducedImageType>(projectionXZ,std::string("./projection-xz.mha"));

      m_RescalerXZ->SetInput(projectionXZ);
      m_ThresholdFilterXZ->SetInput(m_RescalerXZ->GetOutput());
      m_ThresholdFilterXZ->Update();
      // StoreImage<ReducedImageType>(m_ThresholdFilterXZ->GetOutput(),std::string("./binary-xz.mha"));

      
      itk::ImageRegionConstIterator<ReducedImageType> xzIterator (m_ThresholdFilterXZ->GetOutput(),m_ThresholdFilterXZ->GetOutput()->GetLargestPossibleRegion());
      while(!xzIterator.IsAtEnd()){
	if(xzIterator.Get()==0){
	  
	  typename InputImageType::IndexType columnIndex;
	  columnIndex[0]=xzIterator.GetIndex()[0];
	  columnIndex[1]=0;
	  columnIndex[2]=xzIterator.GetIndex()[2];
	  
	  typename InputImageType::SizeType columnSize;
	  columnSize[0]=1;
	  columnSize[1]=this->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
	  columnSize[2]=1;
	  
	  typename InputImageType::RegionType column;
	  column.SetIndex(columnIndex);
	  column.SetSize(columnSize);
	  
	  SetZeroOutputRegion(column);
	}
	++xzIterator;
      }
    }
    
    {


    	m_YZAccumulateFilter->SetInput(this->GetInput());
    	      m_YZAccumulateFilter->Update();
      ReducedImageType::Pointer projectionYZ= m_YZAccumulateFilter->GetOutput();


      //  StoreImage<ReducedImageType>(projectionYZ,std::string("./projection-yz.mha"));

      m_RescalerYZ->SetInput(projectionYZ);
      m_ThresholdFilterYZ->SetInput(m_RescalerYZ->GetOutput());
      m_ThresholdFilterYZ->Update();

      // StoreImage<ReducedImageType>(m_ThresholdFilterYZ->GetOutput(),std::string("./binary-yz.mha"));
      itk::ImageRegionConstIterator<ReducedImageType> yzIterator (m_ThresholdFilterYZ->GetOutput(),m_ThresholdFilterYZ->GetOutput()->GetLargestPossibleRegion());
      while(!yzIterator.IsAtEnd()){
	if(yzIterator.Get()==0){
	  typename InputImageType::IndexType columnIndex;
	  columnIndex[0]=0;
	  columnIndex[1]=yzIterator.GetIndex()[1];
	  columnIndex[2]=yzIterator.GetIndex()[2];
	  
	  typename InputImageType::SizeType columnSize;
	  columnSize[0]=this->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
	  columnSize[1]=1;
	  columnSize[2]=1;
	  
	  typename InputImageType::RegionType column;
	  column.SetIndex(columnIndex);
	  column.SetSize(columnSize);
	  
	  SetZeroOutputRegion(column);
	}
	++yzIterator;
      }
    }    
  }
  template<class TImage> void LateralHistogramVolumeSegmentationFilter<TImage>::SetZeroOutputRegion(const typename TImage::RegionType & region){
    itk::ImageRegionIterator<TImage> regionIt(this->GetOutput(),region);
    while(!regionIt.IsAtEnd()){
      regionIt.Set(0);
      ++regionIt;
    }
  }
}
#endif
