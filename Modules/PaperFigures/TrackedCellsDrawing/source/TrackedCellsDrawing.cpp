#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkPointSet.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkTIFFWriter.h>
#include "AdherensJunctionSegmentationDijkstraCommand.h"
#include "readgt.h"

#include "PrimalGraphDrawer.h"
#include "CellPolygonDrawer.h"
#include "DefaultColorer.h"

#include "itkIndex.h"

#include "qtsqltissuetrackingproject2.h"
#include "jsontissuetrackingproject2.h"

typedef itk::Image<float, 3> PlatenessImageType;
typedef itk::ImageFileReader<PlatenessImageType> PlatenessReaderType;
typedef itk::Image<float, 3> VertexnessImageType;
typedef itk::ImageFileReader<VertexnessImageType> VertexnessReaderType;

typedef std::vector<itk::Index<3> > IndexSetType;
void drawGraph(const ttt::TrackedTissueDescriptor::Pointer & reference,
		std::string & fileName) {

}
int main(int argc, char ** argv) {

	//2. Leer gt
#if 0
	ttt::QTSQLTissueTrackingProject2 m_Project;
	m_Project.SetHost("localhost");
	m_Project.SetDBName("TuftsTissueTracker");
	m_Project.SetUser("root");
	m_Project.SetPassword("ttt1Tracker");
	m_Project.SetProjectID(26);
	m_Project.Open();
#endif
	ttt::JSONTissueTrackingProject2 m_Project;

	m_Project.SetDirectory("/home/morgan/TTTProjects/miniroi/");
	m_Project.Open();

	ttt::CellPolygonDrawer<ttt::TrackedTissueDescriptor> drawer;
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();

	renWin->AddRenderer(renderer);
	drawer.SetRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<	vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	renWin->SetAlphaBitPlanes(1); //enable usage of alpha channel

	ttt::PrimalGraphDrawer<ttt::TrackedTissueDescriptor> primalDrawer;
	DefaultColorer<ttt::SkeletonEdgeType> edgeColorer;
	DefaultColorer<ttt::SkeletonVertexType> vertexColorer;
	primalDrawer.SetVertexColorer(&vertexColorer);
	primalDrawer.SetEdgeColorer(&edgeColorer);
	primalDrawer.SetRenderer(renderer);

	//iren->Start();
	for (unsigned int t = 0; t < m_Project.GetNumFrames(); t++) {
		ttt::TrackedTissueDescriptor::Pointer gt = m_Project.GetTrackedTissueDescriptor(t);
		std::stringstream fileNameStream;
		fileNameStream << "apoptosis-" << t << ".tiff";
		std::string fileName;
		fileNameStream >> fileName;

		primalDrawer.SetTissueDescriptor(gt);
		primalDrawer.Draw();
		primalDrawer.Show();

		drawer.SetTissueDescriptor(gt);
		drawer.Draw();
		drawer.Show();
		renWin->Render();
		renderer->Render();

		vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =vtkSmartPointer<vtkWindowToImageFilter>::New();
		windowToImageFilter->SetInput(renWin);
		windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
		windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
		windowToImageFilter->Update();

		vtkSmartPointer<vtkTIFFWriter> writer =	vtkSmartPointer<vtkTIFFWriter>::New();
		writer->SetFileName(fileName.c_str());

		writer->SetInputConnection(windowToImageFilter->GetOutputPort());
		writer->Write();

	}

}
