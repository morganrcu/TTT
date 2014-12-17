#include "PlatenessCommand.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkRescaleIntensityImageFilter.h>

int main(int argc, char ** argv){
	typedef itk::Image<float,3> InputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;
	typedef itk::ImageFileWriter<InputImageType> WriterType;

	typedef itk::RescaleIntensityImageFilter<InputImageType> Rescaler01Type;
	Rescaler01Type::Pointer rescaler01=Rescaler01Type::New();

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);
	reader->Update();

	InputImageType::Pointer input =reader->GetOutput();
	InputImageType::SpacingType spacing;
	spacing[0]=0.1904762;//0.0965251;
	spacing[1]=0.1904762;//0.0965251;
	spacing[2]=1;//0.3782843;

	input->SetSpacing(spacing);

	rescaler01->SetOutputMinimum(0);
	rescaler01->SetOutputMaximum(1);
	rescaler01->SetInput(reader->GetOutput());
	rescaler01->Update();
	ttt::PlatenessCommand command;

	command.SetSigmaMin(0.05);
	command.SetSigmaMax(0.15);
	command.SetSigmaSteps(4);
	command.SetInput(rescaler01->GetOutput());

	command.Do();

	WriterType::Pointer writer = WriterType::New();

	writer->SetInput(command.GetPlatenessImage());
	writer->SetFileName(argv[2]);
	writer->Update();
}
