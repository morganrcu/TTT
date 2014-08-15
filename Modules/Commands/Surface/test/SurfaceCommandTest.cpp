#include "SurfaceCommand.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkRescaleIntensityImageFilter.h>

int main(int argc, char ** argv){
	typedef itk::Image<float,3> InputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;

	typedef itk::RescaleIntensityImageFilter<InputImageType> Rescaler01Type;
	Rescaler01Type::Pointer rescaler01=Rescaler01Type::New();



	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);
	reader->Update();

	InputImageType::Pointer input =reader->GetOutput();
	InputImageType::SpacingType spacing;
	//spacing[0]=0.1569476;
	//spacing[1]=0.1569476;
	//spacing[2]=0.6209697;

	spacing[0]=0.3107403;
	spacing[1]=0.3107403;
	spacing[2]=0.739833;

	input->SetSpacing(spacing);
	rescaler01->SetOutputMinimum(0);
	rescaler01->SetOutputMaximum(1);
	rescaler01->SetInput(input);
	rescaler01->Update();
	ttt::SurfaceCommand command;


	command.SetInputImage(rescaler01->GetOutput());

	command.Do();
}
