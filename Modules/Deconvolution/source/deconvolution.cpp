#include "itkFFTConvolutionImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "tttL1L2RichardsonLucyDeconvolutionImageFilter.h"
//#include "itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter.h"
//#include "itkDeconvolutionIterationCommand.h"
#include "itkSimpleFilterWatcher.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char* argv[])
{
  if ( argc < 5 )
    {
    std::cerr << "Usage: " << argv[0]
              << " <input image> <kernel image> <output image> <iterations> <lambda>"
              << std::endl;
    return EXIT_FAILURE;
    }

  typedef float                              PixelType;
  const unsigned int                         Dimension = 3;
  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::ImageFileWriter< ImageType >  WriterType;

  ReaderType::Pointer inputReader = ReaderType::New();
  inputReader->SetFileName( argv[1] );

  inputReader->Update();


  typedef itk::RescaleIntensityImageFilter<ImageType> Rescaler01FilterType;
  Rescaler01FilterType::Pointer rescaler = Rescaler01FilterType::New();

  rescaler->SetInput(inputReader->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(1);
  rescaler->Update();

  ReaderType::Pointer kernelReader = ReaderType::New();
  kernelReader->SetFileName( argv[2] );
  kernelReader->Update();

    // Test the deconvolution algorithm
  typedef ttt::L1L2RichardsonLucyDeconvolutionImageFilter< ImageType > DeconvolutionFilterType;
  DeconvolutionFilterType::Pointer deconvolutionFilter = DeconvolutionFilterType::New();

  double lambda = static_cast<double>(atof(argv[5]));

  deconvolutionFilter->SetLambda(lambda);
  deconvolutionFilter->SetInput( rescaler->GetOutput() );
  deconvolutionFilter->SetKernelImage( kernelReader->GetOutput() );
  deconvolutionFilter->NormalizeOn();
  unsigned int iterations = static_cast< unsigned int >( atoi( argv[4] ) );
  deconvolutionFilter->SetNumberOfIterations( iterations );


  // Add an observer to report on filter iteration progress
  //typedef itk::DeconvolutionIterationCommand< DeconvolutionFilterType > IterationCommandType;
  //IterationCommandType::Pointer observer = IterationCommandType::New();
  //deconvolutionFilter->AddObserver( itk::IterationEvent(), observer );


  	  itk::SimpleFilterWatcher watcher(deconvolutionFilter);

  // Write the deconvolution result
  try
    {
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( argv[3] );
    writer->SetInput( deconvolutionFilter->GetOutput() );
    writer->Update();
    }
  catch ( itk::ExceptionObject & e )
    {
    std::cerr << "Unexpected exception caught when writing deconvolution image: "
              << e << std::endl;
    return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
  }
