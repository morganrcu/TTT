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

int main(int argc, char** argv ){
	std::vector<std::string> inputFiles;
    typedef float  PixelType;
    // Fixed Image Type
    typedef itk::Image<PixelType,3>               InputImageType;

    // Moving Image Type
    typedef itk::Image<PixelType,3>               OutputImageType;

    typedef InputImageType::SpacingType SpacingType;
    typedef InputImageType::RegionType RegionType;

    SpacingType inputSpacing;
    //inputSpacing[0]=0.3107403;
    //inputSpacing[1]=0.3107403;
    //inputSpacing[2]=0.739833/2;


    inputSpacing[0]=0.1569476;
    inputSpacing[1]=0.1569476;
    inputSpacing[2]=0.6209697;
    // Transform Type

    typedef itk::ImageFileReader<InputImageType> InputImageReaderType;


    InputImageReaderType::Pointer inputImageReader=InputImageReaderType::New();


    inputImageReader->SetFileName(argv[1]);
    inputImageReader->Update();

    InputImageType::Pointer     inputImage    = inputImageReader->GetOutput();
    inputImage->SetSpacing(inputSpacing);


    OutputImageType::Pointer    outputImage   = OutputImageType::New();

    RegionType inputRegion = inputImage->GetLargestPossibleRegion();
    RegionType::SizeType inputSize = inputRegion.GetSize();

    RegionType outputRegion = inputRegion;
    outputRegion.SetSize(2,inputSize[2]+1);

	SpacingType outputSpacing=inputSpacing;
	outputSpacing[2]=inputSpacing[2]/2;

	outputImage->SetRegions(outputRegion);
	outputImage->SetSpacing(inputSpacing);
	outputImage->SetOrigin(inputImage->GetOrigin());
	outputImage->Allocate();

	typedef InputImageType::IndexType IndexType;

	for(unsigned int x=0;x<inputRegion.GetSize(0);x++){
		IndexType inputIndex;
		IndexType outputIndex;
		inputIndex[0]=x;
		outputIndex[0]=x;
		for(unsigned int y=0;y<inputRegion.GetSize(1);y++){
			inputIndex[1]=y;
			outputIndex[1]=y;
			inputIndex[2]=0;
			outputIndex[2]=0;
			outputImage->SetPixel(outputIndex,inputImage->GetPixel(inputIndex));

			inputIndex[2]=inputRegion.GetSize(2)-1;
			outputIndex[2]=outputRegion.GetSize(2)-1;
			outputImage->SetPixel(outputIndex,inputImage->GetPixel(inputIndex));
		}
	}
	IndexType inputIndex;
	IndexType outputIndex;

	for(unsigned int z=1;z<outputRegion.GetSize(2)-1;z++){

		for(unsigned int x=0;x<inputRegion.GetSize(0);x++){
			inputIndex[0]=x;
			outputIndex[0]=x;

			for(unsigned int y=0;y<inputRegion.GetSize(1);y++){

				inputIndex[1]=y;
				outputIndex[1]=y;

				inputIndex[2]=z-1;

				double a = inputImage->GetPixel(inputIndex);

				inputIndex[2]=z;

				double b = inputImage->GetPixel(inputIndex);

//				PixelType c = (PixelType) ((a+b)/2);
				PixelType c = std::min(a,b);

				outputIndex[2]=z;

				outputImage->SetPixel(outputIndex,c);

			}
		}
	}


	typedef itk::ImageFileWriter<OutputImageType>  OutputImageWriterType;
	OutputImageWriterType::Pointer outputImageWriter=OutputImageWriterType::New();
	outputImageWriter->SetFileName(argv[2]);
	outputImageWriter->SetInput(outputImage);
	outputImageWriter->Update();
}



