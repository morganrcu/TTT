#include "VertexnessCommand.h"
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
	spacing[0]=0.1395089;
	spacing[1]=0.1395089;
	spacing[2]=0.5000000;

	input->SetSpacing(spacing);

	rescaler01->SetOutputMinimum(0);
	rescaler01->SetOutputMaximum(1);
	rescaler01->SetInput(reader->GetOutput());
	rescaler01->Update();
	ttt::VertexnessCommand command;

	command.SetSigmaMin(0.1);
	command.SetSigmaMax(1);
	command.SetSigmaSteps(2);
	command.SetInput(rescaler01->GetOutput());

	command.Do();
}
