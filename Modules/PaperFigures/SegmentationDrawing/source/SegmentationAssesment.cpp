
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

#include "mysqltissuetrackingproject.h"

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

void compareGraphs(const ttt::TissueDescriptor::Pointer & reference, const ttt::TissueDescriptor::Pointer & test){
	int hit=0;
	int total = boost::num_edges(*reference->m_SkeletonGraph);
	int retrieved = boost::num_edges(*test->m_SkeletonGraph);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();

	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren =   vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	BGL_FORALL_EDGES(e,*test->m_SkeletonGraph,ttt::SkeletonGraph){
		ttt::SkeletonVertexType a = boost::source(e,*test->m_SkeletonGraph);
		ttt::SkeletonVertexType b = boost::target(e,*test->m_SkeletonGraph);


		typedef itk::Point<double,3> ItkPT;

		vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
		ItkPT apt= boost::get(ttt::SkeletonPointPropertyTag(),*test->m_SkeletonGraph,a).position;

		newLine->SetPoint1(apt[0],apt[1],apt[2]);
		ItkPT bpt= boost::get(ttt::SkeletonPointPropertyTag(),*test->m_SkeletonGraph,b).position;

		newLine->SetPoint2(bpt[0],bpt[1],bpt[2]);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(newLine->GetOutputPort());

		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);


		actor->GetProperty()->SetLineWidth(5);

		renderer->AddActor(actor);

		if(boost::edge(a,b,*reference->m_SkeletonGraph).second){
			actor->GetProperty()->SetColor(0.0,1.0,0.0);
		}else{
			actor->GetProperty()->SetColor(1.0,0.0,0.0);
		}
	}

	BGL_FORALL_EDGES(e,*reference->m_SkeletonGraph,ttt::SkeletonGraph){
		ttt::SkeletonVertexType a = boost::source(e,*reference->m_SkeletonGraph);
		ttt::SkeletonVertexType b = boost::target(e,*reference->m_SkeletonGraph);

		if(!boost::edge(a,b,*test->m_SkeletonGraph).second){

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


			actor->GetProperty()->SetColor(0.0,0.0,1.0);
		}else{

		}
	}


	renWin->SetAlphaBitPlanes(1); //enable usage of alpha channel
	renWin->Render();

	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renWin);
	windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->Update();

	vtkSmartPointer<vtkTIFFWriter> writer = vtkSmartPointer<vtkTIFFWriter>::New();
	writer->SetFileName("edges.tiff");
	writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	writer->Write();

	renderer->Render();

	iren->Start();
}
int main(int argc,char ** argv){

	if(argc< 3){
		std::cout << "Usage: " << argv[0] << " GTFile PlatenessImage ";
	}

	//1. Leer seeds
	ttt::AdherensJunctionVertices::Pointer seeds=ttt::AdherensJunctionVertices::New();
	readGT(argv[1],seeds);


	//2. Leer gt

	ttt::MySQLTissueTrackingProject m_Project;
	m_Project.SetHost("localhost");
	m_Project.SetDBName("TuftsTissueTracker");
	m_Project.SetUser("root");
	m_Project.SetPassword("ttt1Tracker");
	assert(m_Project.openDB());
	m_Project.OpenProject(2);

	m_Project.SetFrame(0);

	ttt::TissueDescriptor::Pointer gt = m_Project.GetTissueDescriptor();

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
	spacing[0]=0.1022727;
	spacing[1]=0.1022727;
	spacing[2]=1.0192918;

	//spacing[0]=0.1395089;
	//spacing[1]=0.1395089;
	//spacing[2]=0.5;

	platenessImage->SetSpacing(spacing);
	vertexnessImage->SetSpacing(spacing);


	std::cout << "Limit,Precision,Recall,F1" << std::endl;
	//double limit=177.828; //leg
	double limit=70.7946;  //notum

	ttt::AdherensJunctionSegmentationDijkstraCommand command;

			//command.SetK(20);
			//command.SetThreshold(0.1);
			command.SetPlatenessImage(platenessImage);
			command.SetVertexnessImage(vertexnessImage);
			command.SetVertexLocations(seeds);
			command.SetLimit(limit);
			command.Do();

			ttt::TissueDescriptor::Pointer result = command.GetTissueDescriptor();

			std::cout << limit << ",";
			compareGraphs(gt,result);
			std::cout << std::endl;
			//limit=limit*prod;

	//drawPrimal(platenessImage,result,spacing);

}
