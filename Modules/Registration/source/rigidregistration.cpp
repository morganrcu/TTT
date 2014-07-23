#include "itkImageRegistrationMethod.h"
//#include "itkQuaternionRigidTransform.h"
//#include "itkVersor3DTransform.h"

#include "itkMeanSquaresImageToImageMetric.h"
#include "itkMutualInformationImageToImageMetric.h"
#include "itkNormalizedMutualInformationHistogramImageToImageMetric.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"


#include "itkLinearInterpolateImageFunction.h"
//#include "itkQuaternionRigidTransformGradientDescentOptimizer.h"

#include "itkVersorRigid3DTransformOptimizer.h"
#include "itkCenteredTransformInitializer.h"

#include "itkGradientDescentOptimizer.h"
#include "itkGradientDifferenceImageToImageMetric.h"
//#include "itkKullbackLeiblerCompareHistogramImageToImageMetric.h"
#include "itkMutualInformationHistogramImageToImageMetric.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkLandmarkBasedTransformInitializer.h"
#include "itkCompositeTransform.h"
#include "itkTextOutput.h"
#include "itkCommandIterationUpdate.h"
#include "itkTranslationTransform.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"


#if 0


#include "itkCastImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkExtractImageFilter.h"
// The following section of code implements a Command observer
// that will monitor the evolution of the registration process.
//
#include "itkCommand.h"
int main( int argc, char *argv[] )
{
if( argc < 4 )
{
std::cerr << "Missing Parameters " << std::endl;
std::cerr << "Usage: " << argv[0];
std::cerr << " fixedImageFile movingImageFile ";
std::cerr << " outputImagefile [differenceBeforeRegistration] ";
std::cerr << " [differenceAfterRegistration] ";
std::cerr << " [sliceBeforeRegistration] ";
std::cerr << " [sliceDifferenceBeforeRegistration] ";
std::cerr << " [sliceDifferenceAfterRegistration] ";
std::cerr << " [sliceAfterRegistration] " << std::endl;
return EXIT_FAILURE;
}
const unsigned int Dimension = 3;
typedef float PixelType;
typedef itk::Image< PixelType, Dimension > FixedImageType;
typedef itk::Image< PixelType, Dimension > MovingImageType;
// Software Guide : BeginLatex
//
// The Transform class is instantiated using the code below. The only
// template parameter to this class is the representation type of the
// space coordinates.
//
// \index{itk::Versor\-Rigid3D\-Transform!Instantiation}
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet

// Software Guide : EndCodeSnippet


typedef itk:: LinearInterpolateImageFunction< MovingImageType, double > InterpolatorType;
typedef itk::ImageRegistrationMethod< FixedImageType, MovingImageType > RegistrationType;

MetricType::Pointer metric = MetricType::New();
OptimizerType::Pointer optimizer = OptimizerType::New();
InterpolatorType::Pointer interpolator = InterpolatorType::New();
RegistrationType::Pointer registration = RegistrationType::New();
registration->SetMetric( metric );
registration->SetOptimizer( optimizer );
registration->SetInterpolator( interpolator );
// Software Guide : BeginLatex
//
// The transform object is constructed below and passed to the registration
// method.
//
// \index{itk::Versor\-Rigid3D\-Transform!New()}
// \index{itk::Versor\-Rigid3D\-Transform!Pointer}
// \index{itk::Registration\-Method!SetTransform()}
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
TransformType::Pointer transform = TransformType::New();
registration->SetTransform( transform );
// Software Guide : EndCodeSnippet
typedef itk::ImageFileReader< FixedImageType > FixedImageReaderType;
typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
FixedImageReaderType::Pointer fixedImageReader = FixedImageReaderType::New();
MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
fixedImageReader->SetFileName( argv[1] );
movingImageReader->SetFileName( argv[2] );
registration->SetFixedImage( fixedImageReader->GetOutput() );
registration->SetMovingImage( movingImageReader->GetOutput() );
fixedImageReader->Update();
registration->SetFixedImageRegion(
fixedImageReader->GetOutput()->GetBufferedRegion() );
// Software Guide : BeginLatex
//
// The input images are taken from readers. It is not necessary here to
// explicitly call \code{Update()} on the readers since the
// \doxygen{CenteredTransformInitializer} will do it as part of its
// computations. The following code instantiates the type of the
// initializer. This class is templated over the fixed and moving image type
// as well as the transform type. An initializer is then constructed by
// calling the \code{New()} method and assigning the result to a smart
// pointer.
//
// \index{itk::Centered\-Transform\-Initializer!Instantiation}
// \index{itk::Centered\-Transform\-Initializer!New()}
// \index{itk::Centered\-Transform\-Initializer!SmartPointer}
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
// Software Guide : BeginLatex
//
// Let's execute this example over some of the images available in the ftp
// site
//
// \url{ftp://public.kitware.com/pub/itk/Data/BrainWeb}
//
// Note that the images in the ftp site are compressed in \code{.tgz} files.
// You should download these files an uncompress them in your local system.
// After decompressing and extracting the files you could take a pair of
// volumes, for example the pair:
//
// \begin{itemize}
// \item \code{brainweb1e1a10f20.mha}
// \item \code{brainweb1e1a10f20Rot10Tx15.mha}
// \end{itemize}
//
// The second image is the result of intentionally rotating the first image
// by $10$ degrees around the origin and shifting it $15mm$ in $X$. The
// registration takes $24$ iterations and produces:
//
// \begin{center}
// \begin{verbatim}
// [-6.03744e-05, 5.91487e-06, -0.0871932, 2.64659, -17.4637, -0.00232496]
// \end{verbatim}
// \end{center}
//
// That are interpreted as
//
// \begin{itemize}
// \item Versor = $(-6.03744e-05, 5.91487e-06, -0.0871932)$
// \item Translation = $(2.64659, -17.4637, -0.00232496)$ millimeters
// \end{itemize}
//
// This Versor is equivalent to a rotation of $9.98$ degrees around the $Z$
// axis.
//
// Note that the reported translation is not the translation of $(15.0,0.0,0.0)$
// that we may be naively expecting. The reason is that the
// \code{VersorRigid3DTransform} is applying the rotation around the center
// found by the \code{CenteredTransformInitializer} and then adding the
// translation vector shown above.
//
// It is more illustrative in this case to take a look at the actual
// rotation matrix and offset resulting form the $6$ parameters.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
transform->SetParameters( finalParameters );
TransformType::MatrixType matrix = transform->GetMatrix();
TransformType::OffsetType offset = transform->GetOffset();
std::cout << "Matrix = " << std::endl << matrix << std::endl;
std::cout << "Offset = " << std::endl << offset << std::endl;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The output of this print statements is
//
// \begin{center}
// \begin{verbatim}
// Matrix =
// 0.984795 0.173722 2.23132e-05
// -0.173722 0.984795 0.000119257
// -1.25621e-06 -0.00012132 1
//
// Offset =
// [-15.0105, -0.00672343, 0.0110854]
// \end{verbatim}
// \end{center}
//
// From the rotation matrix it is possible to deduce that the rotation is
// happening in the X,Y plane and that the angle is on the order of
// $\arcsin{(0.173722)}$ which is very close to 10 degrees, as we expected.
//
// Software Guide : EndLatex
// Software Guide : BeginLatex
//
// \begin{figure}
// \center
// \includegraphics[width=0.44\textwidth]{BrainProtonDensitySliceBorder20}
// \includegraphics[width=0.44\textwidth]{BrainProtonDensitySliceR10X13Y17}
// \itkcaption[CenteredTransformInitializer input images]{Fixed and moving image
// provided as input to the registration method using
// CenteredTransformInitializer.}
// \label{fig:FixedMovingImageRegistration8}
// \end{figure}
//
//
// \begin{figure}
// \center
// \includegraphics[width=0.32\textwidth]{ImageRegistration8Output}
// \includegraphics[width=0.32\textwidth]{ImageRegistration8DifferenceBefore}
// \includegraphics[width=0.32\textwidth]{ImageRegistration8DifferenceAfter}
// \itkcaption[CenteredTransformInitializer output images]{Resampled moving
// image (left). Differences between fixed and moving images, before (center)
// and after (right) registration with the
// CenteredTransformInitializer.}
// \label{fig:ImageRegistration8Outputs}
// \end{figure}
//
// Figure \ref{fig:ImageRegistration8Outputs} shows the output of the
// registration. The center image in this figure shows the differences
// between the fixed image and the resampled moving image before the
// registration. The image on the right side presents the difference between
// the fixed image and the resampled moving image after the registration has
// been performed. Note that these images are individual slices extracted
// from the actual volumes. For details, look at the source code of this
// example, where the ExtractImageFilter is used to extract a slice from the
// the center of each one of the volumes. One of the main purposes of this
// example is to illustrate that the toolkit can perform registration on
// images of any dimension. The only limitations are, as usual, the amount of
// memory available for the images and the amount of computation time that it
// will take to complete the optimization process.
//
// \begin{figure}
// \center
// \includegraphics[height=0.32\textwidth]{ImageRegistration8TraceMetric}
// \includegraphics[height=0.32\textwidth]{ImageRegistration8TraceAngle}
// \includegraphics[height=0.32\textwidth]{ImageRegistration8TraceTranslations}
// \itkcaption[CenteredTransformInitializer output plots]{Plots of the metric,
// rotation angle, center of rotation and translations during the
// registration using CenteredTransformInitializer.}
// \label{fig:ImageRegistration8Plots}
// \end{figure}
//
// Figure \ref{fig:ImageRegistration8Plots} shows the plots of the main
// output parameters of the registration process. The metric values at every
// iteration. The Z component of the versor is plotted as an indication of
// how the rotation progress. The X,Y translation components of the
// registration are plotted at every iteration too.
//
// Shell and Gnuplot scripts for generating the diagrams in
// Figure~\ref{fig:ImageRegistration8Plots} are available in the directory
//
// \code{InsightDocuments/SoftwareGuide/Art}
//
// You are strongly encouraged to run the example code, since only in this
// way you can gain a first hand experience with the behavior of the
// registration process. Once again, this is a simple reflection of the
// philosophy that we put forward in this book:
//
// \emph{If you can not replicate it, then it does not exist!}.
//
// We have seen enough published papers with pretty pictures, presenting
// results that in practice are impossible to replicate. That is vanity, not
// science.
//
// Software Guide : EndLatex
typedef itk::ResampleImageFilter<
MovingImageType,
FixedImageType > ResampleFilterType;
TransformType::Pointer finalTransform = TransformType::New();
finalTransform->SetCenter( transform->GetCenter() );
finalTransform->SetParameters( finalParameters );
finalTransform->SetFixedParameters( transform->GetFixedParameters() );
ResampleFilterType::Pointer resampler = ResampleFilterType::New();
resampler->SetTransform( finalTransform );
resampler->SetInput( movingImageReader->GetOutput() );
FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
resampler->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
resampler->SetOutputOrigin( fixedImage->GetOrigin() );
resampler->SetOutputSpacing( fixedImage->GetSpacing() );
resampler->SetOutputDirection( fixedImage->GetDirection() );
resampler->SetDefaultPixelValue( 100 );
typedef unsigned char OutputPixelType;
typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
typedef itk::CastImageFilter< FixedImageType, OutputImageType > CastFilterType;
typedef itk::ImageFileWriter< OutputImageType > WriterType;
WriterType::Pointer writer = WriterType::New();
CastFilterType::Pointer caster = CastFilterType::New();
writer->SetFileName( argv[3] );
caster->SetInput( resampler->GetOutput() );
writer->SetInput( caster->GetOutput() );
writer->Update();
typedef itk::SubtractImageFilter<
FixedImageType,
FixedImageType,
FixedImageType > DifferenceFilterType;
DifferenceFilterType::Pointer difference = DifferenceFilterType::New();
typedef itk::RescaleIntensityImageFilter<
FixedImageType,
OutputImageType > RescalerType;
RescalerType::Pointer intensityRescaler = RescalerType::New();
intensityRescaler->SetInput( difference->GetOutput() );
intensityRescaler->SetOutputMinimum( 0 );
intensityRescaler->SetOutputMaximum( 255 );
difference->SetInput1( fixedImageReader->GetOutput() );
difference->SetInput2( resampler->GetOutput() );
resampler->SetDefaultPixelValue( 1 );
WriterType::Pointer writer2 = WriterType::New();
writer2->SetInput( intensityRescaler->GetOutput() );
// Compute the difference image between the
// fixed and resampled moving image.
if( argc > 5 )
{
writer2->SetFileName( argv[5] );
writer2->Update();
}
typedef itk::IdentityTransform< double, Dimension > IdentityTransformType;
IdentityTransformType::Pointer identity = IdentityTransformType::New();
// Compute the difference image between the
// fixed and moving image before registration.
if( argc > 4 )
{
resampler->SetTransform( identity );
writer2->SetFileName( argv[4] );
writer2->Update();
}
//
// Here we extract slices from the input volume, and the difference volumes
// produced before and after the registration. These slices are presented as
// figures in the Software Guide.
//
//
typedef itk::Image< OutputPixelType, 2 > OutputSliceType;
typedef itk::ExtractImageFilter<
OutputImageType,
OutputSliceType > ExtractFilterType;
ExtractFilterType::Pointer extractor = ExtractFilterType::New();
extractor->SetDirectionCollapseToSubmatrix();
extractor->InPlaceOn();
FixedImageType::RegionType inputRegion =
fixedImage->GetLargestPossibleRegion();
FixedImageType::SizeType size = inputRegion.GetSize();
FixedImageType::IndexType start = inputRegion.GetIndex();
// Select one slice as output
size[2] = 0;
start[2] = 90;
FixedImageType::RegionType desiredRegion;
desiredRegion.SetSize( size );
desiredRegion.SetIndex( start );
extractor->SetExtractionRegion( desiredRegion );
typedef itk::ImageFileWriter< OutputSliceType > SliceWriterType;
SliceWriterType::Pointer sliceWriter = SliceWriterType::New();
sliceWriter->SetInput( extractor->GetOutput() );
if( argc > 6 )
{
extractor->SetInput( caster->GetOutput() );
resampler->SetTransform( identity );
sliceWriter->SetFileName( argv[6] );
sliceWriter->Update();
}
if( argc > 7 )
{
extractor->SetInput( intensityRescaler->GetOutput() );
resampler->SetTransform( identity );
sliceWriter->SetFileName( argv[7] );
sliceWriter->Update();
}
if( argc > 8 )
{
resampler->SetTransform( finalTransform );
sliceWriter->SetFileName( argv[8] );
sliceWriter->Update();
}
if( argc > 9 )
{
extractor->SetInput( caster->GetOutput() );
resampler->SetTransform( finalTransform );
sliceWriter->SetFileName( argv[9] );
sliceWriter->Update();
}
return EXIT_SUCCESS;
}


#endif

class CommandIterationUpdate : public itk::Command
{
public:
	typedef CommandIterationUpdate Self;
	typedef itk::Command Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	itkNewMacro( Self );
protected:
	CommandIterationUpdate() {};
public:
	typedef itk::VersorRigid3DTransformOptimizer OptimizerType;
	typedef const OptimizerType * OptimizerPointer;
	void Execute(itk::Object *caller, const itk::EventObject & event)
	{
		Execute( (const itk::Object *)caller, event);
	}
	void Execute(const itk::Object * object, const itk::EventObject & event)
	{
		OptimizerPointer optimizer =
				dynamic_cast< OptimizerPointer >( object );
		if( ! itk::IterationEvent().CheckEvent( &event ) )
		{
			return;
		}
		std::cout << optimizer->GetCurrentIteration() << " ";
		std::cout << optimizer->GetValue() << " ";
		std::cout << optimizer->GetCurrentPosition() << std::endl;
	}
};


template<class FixedImageType,class MovingImageType,class TransformType> void RegisterPair(const typename FixedImageType::Pointer & fixedImage, const typename MovingImageType::Pointer & movingImage, typename TransformType::Pointer & transform ){



	typedef itk::CenteredTransformInitializer< TransformType,FixedImageType,MovingImageType> TransformInitializerType;
	typename TransformInitializerType::Pointer initializer = TransformInitializerType::New();

	initializer->SetTransform( transform );
	initializer->SetFixedImage( fixedImage );
	initializer->SetMovingImage( movingImage);

	initializer->MomentsOn();
	initializer->InitializeTransform();

	  // Optimizer Type
	  typedef itk::VersorRigid3DTransformOptimizer OptimizerType;
	 //typedef itk::ConjugateGradientOptimizer OptimizerType;
	  // Metric Type
	  typedef
	  //itk::NormalizedMutualInformationHistogramImageToImageMetric< FixedImageType, MovingImageType >

	  //itk::MutualInformationImageToImageMetric<FixedImageType,MovingImageType >
			  //itk::GradientDifferenceImageToImageMetric<FixedImageType,MovingImageType>
	//		  itk::NormalizedCorrelationImageToImageMetric< FixedImageType, MovingImageType >
	//		  itk::KullbackLeiblerCompareHistogramImageToImageMetric< FixedImageType, MovingImageType >
			  itk::MattesMutualInformationImageToImageMetric<FixedImageType,MovingImageType>
			  //itk::MutualInformationHistogramImageToImageMetric<FixedImageType,MovingImageType>
			  //itk::MeanSquaresImageToImageMetric< FixedImageType, MovingImageType >
	  MetricType;


	  // Interpolation technique
	  typedef itk:: LinearInterpolateImageFunction<
	                                    MovingImageType,
	                                    double          >    InterpolatorType;

	  // Registration Method
	  typedef itk::ImageRegistrationMethod<
	                                    FixedImageType,
	                                    MovingImageType >    RegistrationType;


	  typename MetricType::Pointer         metric        = MetricType::New();

	  typename OptimizerType::Pointer      optimizer     = OptimizerType::New();
	  typename InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
	  typename RegistrationType::Pointer   registration  = RegistrationType::New();

  /******************************************************************
   * Set up the optimizer.
   ******************************************************************/
	  typedef typename TransformType::VersorType VersorType;
	  typedef typename VersorType::VectorType VectorType;
	  VersorType rotation;
	  VectorType axis;
	  	axis[0] = 0.0;
	  	axis[1] = 0.0;
	  	axis[2] = 1.0;
	  	const double angle = 0;
	  	rotation.Set( axis, angle );
	  	transform->SetRotation( rotation );

	  	registration->SetInitialTransformParameters( transform->GetParameters() );
	  	// Software Guide : EndCodeSnippet
	  	typedef OptimizerType::ScalesType OptimizerScalesType;
	  	OptimizerScalesType optimizerScales( transform->GetNumberOfParameters() );
	  	const double translationScale = 1.0 / 1000.0;
	  	optimizerScales[0] = 1.0;
	  	optimizerScales[1] = 1.0;
	  	optimizerScales[2] = 1.0;
	  	optimizerScales[3] = translationScale;
	  	optimizerScales[4] = translationScale;
	  	optimizerScales[5] = translationScale;
	  	optimizer->SetScales( optimizerScales );
	  	//optimizer->SetMaximumStepLength( 0.1 );
	  	optimizer->SetMaximumStepLength( 1);
	  	optimizer->SetMinimumStepLength( 0.000000001 );
	  	optimizer->SetNumberOfIterations( 150 );
	  	optimizer->SetMinimize(true);



  /******************************************************************
   * Set up the metric.
   ******************************************************************/
  //metric->SetMovingImageStandardDeviation( 1.0 );
  //metric->SetFixedImageStandardDeviation( 1.0 );

  //metric->SetNumberOfSpatialSamples( 50000000 );
  metric->SetUseAllPixels(true);
  //metric->SetFixedImageRegion( fixedImage->GetBufferedRegion() );

  /******************************************************************
   * Set up the registrator.
   ******************************************************************/

  // connect up the components
  registration->SetMetric( metric );
  registration->SetOptimizer( optimizer );
  registration->SetTransform( transform );
  registration->SetFixedImage( fixedImage );
  registration->SetMovingImage( movingImage );
  registration->SetInterpolator( interpolator );

//  transform->SetIdentity();
//  // set initial parameters to identity
//  typename RegistrationType::ParametersType initialParameters(
//    transform->GetNumberOfParameters() );
//
//  initialParameters.Fill( 0.0 );
//  //initialParameters[3] = 1.0;


	CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
	optimizer->AddObserver( itk::IterationEvent(), observer );
	try
	{
	registration->Update();
	std::cout << "Optimizer stop condition: "
	<< registration->GetOptimizer()->GetStopConditionDescription()
	<< std::endl;
	}
	catch( itk::ExceptionObject & err )
	{
	std::cerr << "ExceptionObject caught !" << std::endl;
	std::cerr << err << std::endl;
	//return EXIT_FAILURE;
	}
	OptimizerType::ParametersType finalParameters =
	registration->GetLastTransformParameters();
	const double versorX = finalParameters[0];
	const double versorY = finalParameters[1];
	const double versorZ = finalParameters[2];
	const double finalTranslationX = finalParameters[3];
	const double finalTranslationY = finalParameters[4];
	const double finalTranslationZ = finalParameters[5];
	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();
	const double bestValue = optimizer->GetValue();
	// Print out results
	//
	std::cout << std::endl << std::endl;
	std::cout << "Result = " << std::endl;
	std::cout << " versor X = " << versorX << std::endl;
	std::cout << " versor Y = " << versorY << std::endl;
	std::cout << " versor Z = " << versorZ << std::endl;
	std::cout << " Translation X = " << finalTranslationX << std::endl;
	std::cout << " Translation Y = " << finalTranslationY << std::endl;
	std::cout << " Translation Z = " << finalTranslationZ << std::endl;
	std::cout << " Iterations = " << numberOfIterations << std::endl;
	std::cout << " Metric value = " << bestValue << std::endl;



}

int main(int argc, char** argv )
{
/**
 *   /home/morgan/invaginatingleg/invagination/invagination_T0.ome.tif
 *   /home/morgan/invaginatingleg/invagination/invagination_T1.ome.tif
 *   /home/morgan/invaginatingleg/invagination/invagination_T2.ome.tif
 *   /home/morgan/invaginatingleg/invagination/invagination_T3.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T4.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T5.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T6.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T7.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T8.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T9.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T10.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T11.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T12.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T13.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T14.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T15.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T16.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T17.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T18.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T19.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T20.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T21.ome.tif
 *   /home/morgan/invaginationmasked/invaginated_T22.ome.tif
 *   output
 */
	std::vector<std::string> inputFiles;
    typedef float PixelType;
    // Fixed Image Type
    typedef itk::Image<PixelType,3>               FixedImageType;

    // Moving Image Type
    typedef itk::Image<PixelType,3>               MovingImageType;

    typedef FixedImageType::SpacingType SpacingType;

    SpacingType spacing;
    //spacing[0]=0.3107403;
    //spacing[1]=0.3107403;
    //spacing[2]=0.739833;


    spacing[0]=0.1569476;
    spacing[1]=0.1569476;
    spacing[2]=0.6209697;
    // Transform Type
    typedef itk::VersorRigid3DTransform< double > TransformType;
#if 0


	inputFiles.push_back("/home/morgan/invaginatingleg/invagination/invagination_T0.ome.tif");
    inputFiles.push_back("/home/morgan/invaginatingleg/invagination/invagination_T1.ome.tif");



    typedef   float          VectorComponentType;

    typedef   itk::Vector< VectorComponentType, 3 >    VectorType;
    typedef   itk::Image< VectorType,  3 >   DeformationFieldType;







    typedef itk::ImageFileReader<FixedImageType> FixedImageReaderType;
    typedef itk::ImageFileReader<MovingImageType> MovingImageReaderType;

    FixedImageReaderType::Pointer fixedImageReader=FixedImageReaderType::New();
    MovingImageReaderType::Pointer movingImageReader=MovingImageReaderType::New();

    fixedImageReader->SetFileName(inputFiles[0]);
    movingImageReader->SetFileName(inputFiles[1]);
    fixedImageReader->Update();
    movingImageReader->Update();
    FixedImageType::Pointer     fixedImage    = fixedImageReader->GetOutput();
    MovingImageType::Pointer    movingImage   = movingImageReader->GetOutput();

    fixedImage->SetSpacing(spacing);
    movingImage->SetSpacing(spacing);

    typedef itk::VersorRigid3DTransform< double > Rigid3DTransformType;

    typedef itk::LandmarkBasedTransformInitializer< Rigid3DTransformType, FixedImageType, MovingImageType >
        LandmarkBasedTransformInitializerType;

    LandmarkBasedTransformInitializerType::Pointer landmarkBasedTransformInitializer =
      LandmarkBasedTransformInitializerType::New();
    //  Create source and target landmarks.
    typedef LandmarkBasedTransformInitializerType::LandmarkPointContainer     LandmarkContainerType;
    typedef LandmarkBasedTransformInitializerType::LandmarkPointType          LandmarkPointType;

    LandmarkContainerType fixedLandmarks;
    LandmarkContainerType movingLandmarks;

    LandmarkPointType fixedPoint;
    LandmarkPointType movingPoint;






    fixedPoint[0] = 37.510;
    fixedPoint[1] = 34.685;
    fixedPoint[2] = 52*spacing[2];

    movingPoint[0] = 31.468;
    movingPoint[1] = 27.593;
    movingPoint[2] = 50*spacing[2];

    fixedLandmarks.push_back( fixedPoint );
    movingLandmarks.push_back( movingPoint );



    fixedPoint[0] = 32.645;
    fixedPoint[1] = 33.901;
    fixedPoint[2] = 52*spacing[2];

    movingPoint[0] = 25.888;
    movingPoint[1] = 26.353;
    movingPoint[2] = 50*spacing[2];

    fixedLandmarks.push_back( fixedPoint );
    movingLandmarks.push_back( movingPoint );


    fixedPoint[0] = 35.784;
    fixedPoint[1] = 37.354;
    fixedPoint[2] = 52*spacing[2];

    movingPoint[0] = 29.918;
    movingPoint[1] = 29.918;
    movingPoint[2] = 50*spacing[2];

    fixedLandmarks.push_back( fixedPoint );
    movingLandmarks.push_back( movingPoint );




    fixedPoint[0] = 33.116;
    fixedPoint[1] = 37.040;
    fixedPoint[2] = 52*spacing[2];

    movingPoint[0] = 26.818;
    movingPoint[1] = 29.608;
    movingPoint[2] = 50*spacing[2];

    fixedLandmarks.push_back( fixedPoint );
    movingLandmarks.push_back( movingPoint );

    landmarkBasedTransformInitializer->SetFixedLandmarks( fixedLandmarks );
    landmarkBasedTransformInitializer->SetMovingLandmarks( movingLandmarks );

    Rigid3DTransformType::Pointer transform = Rigid3DTransformType::New();

    transform->SetIdentity();

    landmarkBasedTransformInitializer->SetTransform(transform);
    landmarkBasedTransformInitializer->InitializeTransform();

    typedef itk::ResampleImageFilter<FixedImageType, MovingImageType, double >    ResampleFilterType;
    ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
    resampleFilter->SetInput( movingImage );
    resampleFilter->SetTransform( transform );
    resampleFilter->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
    resampleFilter->SetOutputOrigin(  fixedImage->GetOrigin() );
    resampleFilter->SetOutputSpacing( fixedImage->GetSpacing() );
    resampleFilter->SetOutputDirection( fixedImage->GetDirection() );
    resampleFilter->SetDefaultPixelValue(0);
    resampleFilter->GetOutput();

    // Write the output
    typedef itk::ImageFileWriter<  MovingImageType  > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput (  resampleFilter->GetOutput() );
    writer->SetFileName( "output.ome.tif" );
    writer->Update();

#endif




	for(int i=1;i<argc-1;i++){
		inputFiles.push_back(argv[i]);
	}


	std::string outputPrefix=argv[argc-1];

  int nImages = inputFiles.size();

  std::vector<TransformType::Pointer >transforms(nImages);
  for(int i=0;i<nImages;i++){
	  transforms[i]=TransformType::New();
  }
  transforms[0]->SetIdentity();

  for(int i=1;i<nImages;i++){
	  /*
	   * Read files
	   */
	  std::cout << "Registering " << inputFiles[i] <<" to " <<inputFiles[i-1] << std::endl;
	  typedef itk::ImageFileReader<FixedImageType> FixedImageReaderType;
	  typedef itk::ImageFileReader<MovingImageType> MovingImageReaderType;

	  FixedImageReaderType::Pointer fixedImageReader=FixedImageReaderType::New();
	  MovingImageReaderType::Pointer movingImageReader=MovingImageReaderType::New();

	  fixedImageReader->SetFileName(inputFiles[i-1]);
	  movingImageReader->SetFileName(inputFiles[i]);


	  fixedImageReader->Update();
	  movingImageReader->Update();
	  FixedImageType::Pointer     fixedImage    = fixedImageReader->GetOutput();
	  MovingImageType::Pointer    movingImage   = movingImageReader->GetOutput();

	  fixedImage->SetSpacing(spacing);
	  movingImage->SetSpacing(spacing);
	  FixedImageType::SizeType size= fixedImage->GetLargestPossibleRegion().GetSize();
	  FixedImageType::PointType origin;

	  origin[0]=size[0]*spacing[0]/2;
	  origin[1]=size[1]*spacing[1]/2;
	  origin[2]=size[2]*spacing[2]/2;

	  fixedImage->SetOrigin(origin);
	  movingImage->SetOrigin(origin);

	  RegisterPair<FixedImageType,MovingImageType,TransformType>(fixedImage,movingImage,transforms[i]);
  }

  typedef itk::ResampleImageFilter<MovingImageType,MovingImageType> ResamplerType;
  ResamplerType::Pointer resampler = ResamplerType::New();
  //transform->SetIdentity();
#if 0
  TransformType::ParametersType initialParameters( transform->GetNumberOfParameters() );
  initialParameters.Fill( 0 );
  initialParameters[3] = 1.0;
  initialParameters[4]=10;
  initialParameters[5]=10;
  initialParameters[6]=0;
  transform->SetParameters(initialParameters);
#endif
  for(int i=0;i<nImages;i++){




	  typedef itk::ImageFileReader<MovingImageType> MovingImageReaderType;
	  MovingImageReaderType::Pointer movingImageReader=MovingImageReaderType::New();
	  movingImageReader->SetFileName(inputFiles[i]);
	  movingImageReader->Update();
	  MovingImageType::Pointer    movingImage   = movingImageReader->GetOutput();
	  movingImage->SetSpacing(spacing);


	  MovingImageType::SizeType size = movingImage->GetLargestPossibleRegion().GetSize();



	  MovingImageType::SizeType resampledSize;
	  resampledSize[0]=1280;
	  resampledSize[1]=1280;
	  resampledSize[2]=150;
	  resampler->SetSize(resampledSize );

	  typedef itk::TranslationTransform<double,3> TranslationTransformType;
	  TranslationTransformType::Pointer translationTransform =
	    TranslationTransformType::New();
	  TranslationTransformType::OutputVectorType translation;
	  translation[0] = -20;//-(resampledSize[0]-size[0])/2*spacing[0];
	  translation[1] = -20;// -(resampledSize[1]-size[1])/2*spacing[1];
	  translation[2] = -20; //-(resampledSize[2]-size[2])/2*spacing[2];
	  translationTransform->Translate(translation);

	  typedef itk::CompositeTransform<double,3> CompositeType;
	  CompositeType::Pointer composite = CompositeType::New();


	  composite->AddTransform(translationTransform);

	  for(int k=1;k<=i;k++){
		  composite->AddTransform(transforms[k]);
	  }



	  std::cout << "Origin: " << movingImage->GetOrigin() << std::endl;
	  resampler->SetInput(movingImage);
	  resampler->SetTransform(composite);
	  resampler->SetOutputOrigin(movingImage->GetOrigin());
	  resampler->SetOutputSpacing( movingImage->GetSpacing() );
	  resampler->SetOutputDirection( movingImage->GetDirection() );
	  resampler->SetDefaultPixelValue( 0 );

	  resampler->Update();



	  std::stringstream ss;

	  ss << outputPrefix << "-T" << i << ".ome.tif";
	    std::string filename;
	    ss >> filename;


	  typedef itk::ImageFileWriter<MovingImageType>  MovingImageWriterType;
	  MovingImageWriterType::Pointer movingImageWriter=MovingImageWriterType::New();
	  movingImageWriter->SetFileName(filename);
	  movingImageWriter->SetInput(resampler->GetOutput());
	  movingImageWriter->Update();
	  std::cout << transforms[i] << std::endl;
  }

}

