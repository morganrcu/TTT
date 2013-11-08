#include "giaaPlatenessImageFilter.h"

template <typename TInputImage, typename TSpeedImage,typename TDirectionImage >
ttt::PlatenessImageFilter
<TInputImage,TSpeedImage,TDirectionImage>
::PlatenessImageFilter()
{
  m_SigmaMin = 0.2;
  m_SigmaMax = 2.0;

  m_NumberOfSigmaSteps = 10;

  m_HessianFilter                = HessianFilterType::New();
  m_PlatenessFilter             = PlatenessFilterType::New();

  //Turn off vesselness measure scaling
  m_PlatenessFilter->SetScaleVesselnessMeasure( false );

  //Instantiate Update buffer
  m_PlatenessBuffer                 = PlatenessImageType::New();
  m_DirectionImage					= DirectionImageType::New();
}

template <typename TInputImage, typename TSpeedImage,typename TDirectionImage >
void
ttt::PlatenessImageFilter
<TInputImage,TSpeedImage,TDirectionImage>
::AllocateUpdateBuffer()
{
  /* The update buffer looks just like the output and holds the best response
     in the  vesselness measure */


  typename TSpeedImage::Pointer output = this->GetOutput();

  m_PlatenessBuffer->SetSpacing(output->GetSpacing());
  m_PlatenessBuffer->SetOrigin(output->GetOrigin());
  m_PlatenessBuffer->SetRegions(output->GetLargestPossibleRegion());
  m_PlatenessBuffer->Allocate();
  m_PlatenessBuffer->FillBuffer(0);
  m_DirectionImage->SetSpacing(output->GetSpacing());
  m_DirectionImage->SetOrigin(output->GetOrigin());
  m_DirectionImage->SetRegions(output->GetLargestPossibleRegion());
  m_DirectionImage->Allocate();
}


template <typename TInputImage, typename TSpeedImage,typename TDirectionImage >
void
ttt::PlatenessImageFilter
<TInputImage,TSpeedImage,TDirectionImage>
::GenerateData()
{

  // Allocate the output
  this->GetOutput()->SetSpacing(this->GetInput()->GetSpacing());
  this->GetOutput()->SetOrigin(this->GetInput()->GetOrigin());
  this->GetOutput()->SetRegions(this->GetOutput()->GetLargestPossibleRegion() );
  this->GetOutput()->Allocate();

  // Allocate the buffer
  AllocateUpdateBuffer();

  typename InputImageType::ConstPointer input = this->GetInput();

  this->m_HessianFilter->SetInput( input );

  this->m_HessianFilter->SetNormalizeAcrossScale( true );

  double sigma = m_SigmaMin;

  int scaleLevel = 1;

  while ( sigma <= m_SigmaMax )
    {
    std::cout << "Computing plateness for scale with sigma= "
              << sigma << std::endl;

    m_HessianFilter->SetSigma( sigma );

    m_HessianFilter->Update();
    m_PlatenessFilter->SetInput ( m_HessianFilter->GetOutput() );

    m_PlatenessFilter->Update();

    this->UpdateMaximumResponse();

    sigma  = this->ComputeSigmaValue( scaleLevel );

    scaleLevel++;
    }

  //Write out the best response to the output image
  itk::ImageRegionIterator<PlatenessBufferType>
               it(m_PlatenessBuffer,m_PlatenessBuffer->GetLargestPossibleRegion());
  it.GoToBegin();

  itk::ImageRegionIterator<PlatenessImageType> oit(this->GetOutput(),
                          this->GetOutput()->GetLargestPossibleRegion());
  oit.GoToBegin();

  while(!oit.IsAtEnd())
    {
    oit.Value() = static_cast< PlatenessPixelType >( it.Get() );
    ++oit;
    ++it;
    }
}

template <typename TInputImage, typename TSpeedImage,typename TDirectionImage >
void
ttt::PlatenessImageFilter
<TInputImage,TSpeedImage,TDirectionImage>
::UpdateMaximumResponse()
{

  itk::ImageRegionIterator<PlatenessBufferType>
            uit(m_PlatenessBuffer,m_PlatenessBuffer->GetLargestPossibleRegion());

  uit.GoToBegin();

  typedef typename PlatenessFilterType::PlatenessImageType
                                         PlatenessOutputImageType;
  typedef typename PlatenessFilterType::DirectionImageType
                                         PlatenessDirectionImageType;

  itk::ImageRegionIterator<PlatenessOutputImageType>
            it(m_PlatenessFilter->GetOutput(),
            this->m_PlatenessFilter->GetOutput()->GetLargestPossibleRegion());

  itk::ImageRegionIterator<PlatenessDirectionImageType> dirit(m_PlatenessFilter->GetDirectionImage(),m_PlatenessFilter->GetDirectionImage()->GetLargestPossibleRegion());

  itk::ImageRegionIterator<DirectionImageType> odirit(m_DirectionImage,m_DirectionImage->GetLargestPossibleRegion());


  it.GoToBegin();

  dirit.GoToBegin();

  while(!uit.IsAtEnd())
    {
    if( uit.Value() < it.Value() )
      {
      uit.Value() = it.Value();
      odirit.Set(dirit.Get());
      }
    ++uit;
    ++it;
    ++dirit;
    ++odirit;

    }
}

template <typename TInputImage, typename TSpeedImage,typename TDirectionImage >
double
ttt::PlatenessImageFilter
<TInputImage,TSpeedImage,TDirectionImage>
::ComputeSigmaValue( int ScaleLevel )
{
  double stepSize =
     ( vcl_log( m_SigmaMax )  - vcl_log( m_SigmaMin) ) / m_NumberOfSigmaSteps;

  if( stepSize <= 1e-10 )
    {
    stepSize = 1e-10;
    }

  return ( vcl_exp( vcl_log (m_SigmaMin) + stepSize * ScaleLevel) );
}

template <typename TInputImage, typename TSpeedImage,typename TDirectionImage >
void
ttt::PlatenessImageFilter
<TInputImage,TSpeedImage,TDirectionImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "SigmaMin:  " << m_SigmaMin << std::endl;
  os << indent << "SigmaMax:  " << m_SigmaMax  << std::endl;
}
