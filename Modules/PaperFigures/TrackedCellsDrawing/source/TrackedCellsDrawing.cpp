#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkPointSet.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkCamera.h>
#include <vtkTransform.h>
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
void drawGraph(const ttt::TrackedTissueDescriptor<3>::Pointer & reference,
		std::string & fileName) {

}
int main(int argc, char ** argv) {

	//2. Leer gt

	ttt::QTSQLTissueTrackingProject2<3> m_Project;
	m_Project.SetHost("localhost");
	m_Project.SetDBName("TuftsTissueTracker");
	m_Project.SetUser("root");
	m_Project.SetPassword("ttt1Tracker");
	m_Project.SetProjectID(16);
	m_Project.Open();
#if 0
	ttt::JSONTissueTrackingProject2<3> m_Project;

	m_Project.SetDirectory("/home/morgan/TTTProjects/miniroi/");
	m_Project.Open();
#endif
	ttt::CellPolygonDrawer<ttt::TrackedTissueDescriptor<3> > drawer;
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();


	//camera->SetPosition(0, 0, 2);
	///camera->SetFocalPoint(0, 0, 0);

	  // Create a renderer, render window, and interactor

	//renderer->SetActiveCamera(camera);
	//renderer->GetActiveCamera()->Print(std::cout);
	renWin->AddRenderer(renderer);
	drawer.SetRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<	vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	renWin->SetAlphaBitPlanes(1); //enable usage of alpha channel

	ttt::PrimalGraphDrawer<ttt::TrackedTissueDescriptor<3> > primalDrawer;
	DefaultColorer<ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<3> >::SkeletonEdgeType > edgeColorer;
	DefaultColorer<ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<3> >::SkeletonVertexType> vertexColorer;
	primalDrawer.SetVertexColorer(&vertexColorer);
	primalDrawer.SetEdgeColorer(&edgeColorer);
	primalDrawer.SetRenderer(renderer);
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	transform->RotateX(0);
	transform->RotateY(0);
	transform->RotateZ(0);


	//iren->Start();
	for (unsigned int t = 0; t < m_Project.GetNumFrames(); t++) {


		ttt::TrackedTissueDescriptor<3>::Pointer gt = m_Project.GetTrackedTissueDescriptor(t);

		double x0=0;
		double y0=0;
		double z0=0;
		int total=0;
		BGL_FORALL_VERTICES_T(v,gt->GetAJGraph(),ttt::SkeletonGraph<3>){
			x0=x0+boost::get(ttt::SkeletonPointPropertyTag<3>(),gt->GetAJGraph(),v).position[0];
			y0=y0+boost::get(ttt::SkeletonPointPropertyTag<3>(),gt->GetAJGraph(),v).position[1];
			z0=z0+boost::get(ttt::SkeletonPointPropertyTag<3>(),gt->GetAJGraph(),v).position[2];
			total++;
		}
		x0=x0/total;
		y0=y0/total;
		z0=z0/total;
		std::cout << "MEAN POINT " << x0 << " "<< y0 << " "<< z0 << std::endl;
		std::stringstream fileNameStream;
		fileNameStream << "leg-" << t << ".tiff";
		std::string fileName;
		fileNameStream >> fileName;

		primalDrawer.SetTissueDescriptor(gt);
		primalDrawer.Draw();
		primalDrawer.Show();

		drawer.SetTissueDescriptor(gt);
		drawer.Draw();
		drawer.Show();


		//renderer->GetActiveCamera()->SetFocalPoint(x0,y0,z0);
		//renderer->GetActiveCamera()->SetPosition(x0,y0,200);

		renderer->GetActiveCamera()->SetFocalPoint(x0,y0,z0);
		renderer->GetActiveCamera()->SetPosition(x0+150,y0+100,z0-100);
		renderer->GetActiveCamera()->Roll(90);
		//renderer->GetActiveCamera()->SetEyeAngle(180);
		//renderer->GetActiveCamera()->SetFocalPoint(35,35,10);
		//renderer->GetActiveCamera()->SetPosition(50,-100,10);


		renWin->Render();


		renderer->Render();

		renderer->GetActiveCamera()->Print(std::cout);

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
