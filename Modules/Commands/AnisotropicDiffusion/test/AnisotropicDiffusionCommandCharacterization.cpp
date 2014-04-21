#include "AnisotropicDiffusionCommand.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkAbsoluteValueDifferenceImageFilter.h"
int main(int argc,char ** argv){



	typedef itk::Image<float,3> InputImageType;


	typedef itk::ImageFileReader<InputImageType> ImageFileReaderType;

	ImageFileReaderType::Pointer reader = ImageFileReaderType::New();

	reader->SetFileName(argv[1]);

	reader->Update();

	InputImageType::Pointer original = reader->GetOutput();

	InputImageType::SpacingType spacing;
	spacing[0]=0.1395089;
	spacing[1]=0.1395089;
	spacing[2]=0.5000000;
	original->SetSpacing(spacing);

	typedef itk::RescaleIntensityImageFilter<InputImageType,InputImageType> RescalerType;

	RescalerType::Pointer rescaler = RescalerType::New();

	rescaler->SetInput(original);
	rescaler->SetOutputMaximum(1);
	rescaler->SetOutputMinimum(0);
	rescaler->Update();

	InputImageType::Pointer current=rescaler->GetOutput();

	typedef itk::AbsoluteValueDifferenceImageFilter<InputImageType,InputImageType,InputImageType> SubtractorType;

	for(int i=0;i<50;i++){

		ttt::AnisotropicDiffusionCommand command;
		command.SetDiffusionSigmaMin(0.14);
		command.SetDiffusionSigmaMax(0.22);
		command.SetDiffusionSigmaSteps(4);
		command.SetDiffusionIterations(1);

		command.SetInputImage(current);
		command.Do();
		InputImageType::Pointer output = command.GetOutputImage();

		SubtractorType::Pointer subtractor = SubtractorType::New();
		subtractor->SetInput1(output);
		subtractor->SetInput2(current);
		subtractor->Update();


		InputImageType::Pointer abs = subtractor->GetOutput();
		itk::ImageRegionIterator<InputImageType> diffIterator(abs,abs->GetLargestPossibleRegion());
		diffIterator.GoToBegin();
		float sum=0;
		while(!diffIterator.IsAtEnd()){
			sum+= diffIterator.Value();
			++diffIterator;
		}
		std::cout << "Difference " << i << "\t" << sum << std::endl;
		current=output;
	}
}

