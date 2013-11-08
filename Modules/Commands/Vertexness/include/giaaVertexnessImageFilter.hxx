#include "giaaVertexnessImageFilter.h"

template <typename TInputImage, typename TSpeedImage>
ttt::VertexnessImageFilter
<TInputImage,TSpeedImage>
::VertexnessImageFilter()
{
  m_SigmaMin = 0.2;
  m_SigmaMax = 2.0;

  m_NumberOfSigmaSteps = 10;

  m_HessianFilter                = HessianFilterType::New();
  m_VertexnessFilter             = VertexnessFilterType::New();

  //Turn off vesselness measure scaling
  m_VertexnessFilter->SetScaleVesselnessMeasure( false );

  //Instantiate Update buffer
  m_VertexnessBuffer                 = VertexnessImageType::New();
  
}

template <typename TInputImage, typename TSpeedImage >
void
ttt::VertexnessImageFilter
<TInputImage,TSpeedImage>
::AllocateUpdateBuffer()
{
  /* The update buffer looks just like the output and holds the best response
     in the  vesselness measure */


  typename TSpeedImage::Pointer output = this->GetOutput();

  m_VertexnessBuffer->SetSpacing(output->GetSpacing());
  m_VertexnessBuffer->SetOrigin(output->GetOrigin());
  m_VertexnessBuffer->SetRegions(output->GetLargestPossibleRegion());
  m_VertexnessBuffer->Allocate();
  m_VertexnessBuffer->FillBuffer(0);
  
}


template <typename TInputImage, typename TSpeedImage >
void
ttt::VertexnessImageFilter
<TInputImage,TSpeedImage>
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
    std::cout << "Computing vertexness for scale with sigma= "
              << sigma << std::endl;

    m_HessianFilter->SetSigma( sigma );

    m_HessianFilter->Update();
    m_VertexnessFilter->SetInput ( m_HessianFilter->GetOutput() );

    m_VertexnessFilter->Update();

    this->UpdateMaximumResponse();

    sigma  = this->ComputeSigmaValue( scaleLevel );

    scaleLevel++;
    }

  //Write out the best response to the output image
  itk::ImageRegionIterator<VertexnessBufferType>
               it(m_VertexnessBuffer,m_VertexnessBuffer->GetLargestPossibleRegion());
  it.GoToBegin();

  itk::ImageRegionIterator<VertexnessImageType> oit(this->GetOutput(),
                          this->GetOutput()->GetLargestPossibleRegion());
  oit.GoToBegin();

  while(!oit.IsAtEnd())
    {
    oit.Value() = static_cast< VertexnessPixelType >( it.Get() );
    ++oit;
    ++it;
    }
}

template <typename TInputImage, typename TSpeedImage>
void
ttt::VertexnessImageFilter
<TInputImage,TSpeedImage>
::UpdateMaximumResponse()
{

  itk::ImageRegionIterator<VertexnessBufferType>
            uit(m_VertexnessBuffer,m_VertexnessBuffer->GetLargestPossibleRegion());

  uit.GoToBegin();

  typedef typename VertexnessFilterType::VertexnessImageType
                                         VertexnessOutputImageType;

  itk::ImageRegionIterator<VertexnessOutputImageType>
            it(m_VertexnessFilter->GetOutput(),
            this->m_VertexnessFilter->GetOutput()->GetLargestPossibleRegion());

    
  it.GoToBegin();

  
  while(!uit.IsAtEnd())
    {
    if( uit.Value() < it.Value() )
      {
      uit.Value() = it.Value();
      
      }
    ++uit;
    ++it;
    
    

    }
}

template <typename TInputImage, typename TSpeedImage>
double
ttt::VertexnessImageFilter
<TInputImage,TSpeedImage>
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

template <typename TInputImage, typename TSpeedImage>
void
ttt::VertexnessImageFilter
<TInputImage,TSpeedImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "SigmaMin:  " << m_SigmaMin << std::endl;
  os << indent << "SigmaMax:  " << m_SigmaMax  << std::endl;
}
