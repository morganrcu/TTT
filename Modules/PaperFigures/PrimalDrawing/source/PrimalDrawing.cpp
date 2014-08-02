
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

#include "PlatenessImageDrawer.h"
#include "PrimalGraphDrawer.h"
#include "DefaultColorer.h"

#include "itkIndex.h"

#include "mysqltissuetrackingproject2.h"

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


	ttt::PrimalGraphDrawer<ttt::TissueDescriptor> primalDrawer;

	DefaultColorer<ttt::SkeletonVertexType> vertexColorer;
	DefaultColorer<ttt::SkeletonEdgeType> edgeColorer;
	primalDrawer.SetVertexColorer(&vertexColorer);
	primalDrawer.SetEdgeColorer(&edgeColorer);


	primalDrawer.SetTissueDescriptor(descriptor);
	primalDrawer.SetRenderer(renderer);



	primalDrawer.Draw();
	primalDrawer.Show();
	ttt::PlatenessImageDrawer platenessDrawer;
	platenessDrawer.SetRenderer(renderer);
	platenessDrawer.SetImage(plateness);
	platenessDrawer.Draw();
	platenessDrawer.Show();
	renWin->Render();
	iren->Start();
}

void drawGraph(const ttt::TissueDescriptor::Pointer & reference){
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();

	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren =   vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	BGL_FORALL_EDGES(e,*reference->m_SkeletonGraph,ttt::SkeletonGraph){
		ttt::SkeletonVertexType a = boost::source(e,*reference->m_SkeletonGraph);
		ttt::SkeletonVertexType b = boost::target(e,*reference->m_SkeletonGraph);

		typedef itk::Point<double,3> ItkPT;

		vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
		ItkPT apt= boost::get(ttt::SkeletonPointPropertyTag(),*reference->m_SkeletonGraph,a).position;

		newLine->SetPoint1(apt[0],apt[1],apt[2]);

		ItkPT bpt= boost::get(ttt::SkeletonPointPropertyTag(),*reference->m_SkeletonGraph,b).position;
		newLine->SetPoint2(bpt[0],bpt[1],bpt[2]);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(newLine->GetOutputPort());

		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);


		actor->GetProperty()->SetLineWidth(5);

		renderer->AddActor(actor);


			actor->GetProperty()->SetColor(0.0,1.0,0.0);
	}

	BGL_FORALL_EDGES(e,*reference->m_CellGraph,ttt::CellGraph){
		ttt::SkeletonVertexType a = boost::source(e,*reference->m_CellGraph);
		ttt::SkeletonVertexType b = boost::target(e,*reference->m_CellGraph);

		typedef itk::Point<double,3> Point;

		vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
		Point apt= boost::get(ttt::CellPropertyTag(),*reference->m_CellGraph,a).GetCentroid();

		newLine->SetPoint1(apt[0],apt[1],apt[2]);

		Point bpt= boost::get(ttt::CellPropertyTag(),*reference->m_CellGraph,b).GetCentroid();
		newLine->SetPoint2(bpt[0],bpt[1],bpt[2]);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(newLine->GetOutputPort());

		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);


		actor->GetProperty()->SetLineWidth(5);

		renderer->AddActor(actor);


			actor->GetProperty()->SetColor(1.0,0.0,0.0);
	}

	renWin->SetAlphaBitPlanes(1); //enable usage of alpha channel
	renWin->Render();

	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renWin);
	windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->Update();

	vtkSmartPointer<vtkTIFFWriter> writer = vtkSmartPointer<vtkTIFFWriter>::New();
	writer->SetFileName("primal.tiff");
	writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	writer->Write();

	renderer->Render();

	iren->Start();
}
int main(int argc,char ** argv){

	//2. Leer gt

	ttt::MySQLTissueTrackingProject2 m_Project;
	m_Project.SetHost("localhost");
	m_Project.SetDBName("TuftsTissueTracker");
	m_Project.SetUser("root");
	m_Project.SetPassword("ttt1Tracker");
	m_Project.SetProjectID(2);
	m_Project.Open();


	ttt::TissueDescriptor::Pointer gt = m_Project.GetTissueDescriptor(0);



	drawGraph(gt);

}
