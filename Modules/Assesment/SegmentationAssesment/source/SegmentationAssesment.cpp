
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

void compareGraphs(const ttt::TissueDescriptor::Pointer & reference, const ttt::TissueDescriptor::Pointer & test){
	int hit=0;
	int total = boost::num_edges(*reference->m_SkeletonGraph);
	int retrieved = boost::num_edges(*test->m_SkeletonGraph);

	BGL_FORALL_EDGES(e,*test->m_SkeletonGraph,ttt::SkeletonGraph){
		ttt::SkeletonVertexType a = boost::source(e,*test->m_SkeletonGraph);
		ttt::SkeletonVertexType b = boost::target(e,*test->m_SkeletonGraph);

		if(boost::edge(a,b,*reference->m_SkeletonGraph).second){
			hit++;
		}
	}
	double precision=(double)hit/retrieved;
	double recall = (double)hit/total;
	double f1= (2*precision*recall)/(precision+recall);
	std::cout << precision << "," << recall << "," << f1;
}
int main(int argc,char ** argv){

	if(argc< 3){
		std::cout << "Usage: " << argv[0] << " GTFile PlatenessImage ";
	}

	//1. Leer seeds
	ttt::AdherensJunctionVertices::Pointer seeds=ttt::AdherensJunctionVertices::New();
	readGT(argv[1],seeds);


	//2. Leer gt

	ttt::MySQLTissueTrackingProject2 m_Project;
	m_Project.SetHost("localhost");
	m_Project.SetDBName("TuftsTissueTracker");
	m_Project.SetUser("root");
	m_Project.SetPassword("ttt1Tracker");

	m_Project.SetProjectID(16);

	m_Project.Open();
	ttt::TissueDescriptor::Pointer gt = m_Project.GetTissueDescriptor(0);

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
	//spacing[0]=0.1022727;
	//spacing[1]=0.1022727;
	//spacing[2]=1.0192918;

	spacing[0]=0.1395089;
	spacing[1]=0.1395089;
	spacing[2]=0.5;

	platenessImage->SetSpacing(spacing);
	vertexnessImage->SetSpacing(spacing);

	double minLimit=1e0;
	double maxLimit=1e5;

	int nSamples=100;

	double step = (log(maxLimit)/log(10)- log(minLimit)/log(10))/nSamples;

	double prod=10;
	std::cout << "Limit,Precision,Recall,F1" << std::endl;
	double limit= minLimit;
	for(int i=0;i<=nSamples;i++){
	//while(limit<=maxLimit){
		double limit = pow(10,log(minLimit)/log(10) + i*step);
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
	}

	//drawPrimal(platenessImage,result,spacing);

}
