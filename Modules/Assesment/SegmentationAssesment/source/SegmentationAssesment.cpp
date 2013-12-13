
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkPointSet.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include "AdherensJunctionSegmentationDijkstraCommand.h"
#include "readgt.h"

#include "PlatenessImageDrawer.h"
#include "PrimalGraphDrawer.h"

#include "itkIndex.h"


typedef itk::Image<float,3> PlatenessImageType;
typedef itk::ImageFileReader<PlatenessImageType> PlatenessReaderType;
typedef itk::Image<float,3> VertexnessImageType;
typedef itk::ImageFileReader<VertexnessImageType> VertexnessReaderType;

typedef std::vector<itk::Index<3> > IndexSetType;

void drawPrimal(PlatenessImageType::Pointer & plateness, const ttt::TissueDescriptor::Pointer & descriptor,PlatenessImageType::SpacingType & spacing){

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();

	renWin->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);


	ttt::PrimalGraphDrawer primalDrawer;



	primalDrawer.SetTissueDescriptor(descriptor);
	primalDrawer.SetRenderer(renderer);


	primalDrawer.Draw();
	ttt::PlatenessImageDrawer platenessDrawer;
	platenessDrawer.SetRenderer(renderer);
	platenessDrawer.SetImage(plateness);
	platenessDrawer.Draw();
	renWin->Render();
	iren->Start();
}
int main(int argc,char ** argv){

	if(argc< 3){
		std::cout << "Usage: " << argv[0] << " GTFile PlatenessImage ";
	}

	//1. Leer GT
	ttt::AdherensJunctionVertices::Pointer gt=ttt::AdherensJunctionVertices::New();
	readGT(argv[1],gt);

	//2. Leer Plateness
	PlatenessReaderType::Pointer platenessReader = PlatenessReaderType::New();

	platenessReader->SetFileName(argv[2]);

	platenessReader->Update();

	PlatenessImageType::Pointer platenessImage = platenessReader->GetOutput();


	VertexnessReaderType::Pointer vertexnessReader = PlatenessReaderType::New();

	vertexnessReader->SetFileName(argv[3]);

	vertexnessReader->Update();

	VertexnessImageType::Pointer vertexnessImage = vertexnessReader->GetOutput();


	PlatenessImageType::SpacingType spacing;
	spacing[0]=0.0965251;
	spacing[1]=0.0965251;
	spacing[2]=0.378284;
	platenessImage->SetSpacing(spacing);
	vertexnessImage->SetSpacing(spacing);

	ttt::AdherensJunctionSegmentationDijkstraCommand command;

	//command.SetK(20);
	//command.SetThreshold(0.1);
	command.SetPlatenessImage(platenessImage);
	command.SetVertexnessImage(vertexnessImage);
	command.SetVertexLocations(gt);
	command.Do();

	drawPrimal(platenessImage,command.GetTissueDescriptor(),spacing);

}
