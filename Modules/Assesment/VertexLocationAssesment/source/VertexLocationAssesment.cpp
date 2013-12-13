#include <string>

#include <vector>

#include <itkPoint.h>
#include <itkImageFileReader.h>
#include <itkPointSet.h>

#include "itkLocalMaximumImageFilter.h"
#include "readgt.h"
#include "pointsetmapping.h"

#define DRAW_GT

#ifdef DRAW_GT
#include "VertexnessImageDrawer.h"
#include "VertexLocationsDrawer.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#endif

using namespace std;


typedef itk::Image<float,3> ImageType;

typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::Index<3> IndexType;

typedef std::pair<IndexType,float> PointAndValueType;


struct CompareSecond{
	bool operator()(const  PointAndValueType & a, const PointAndValueType & b){
		return a.second < b.second;
	}
};
#ifdef DRAW_GT
void drawPointSet(const typename ImageType::Pointer & vertexness,const ttt::AdherensJunctionVertices::Pointer & gt,ImageType::SpacingType & spacing){

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();

	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren =   vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
#if 0
	vtkSmartPointer<ttt::VertexSelectionInteractor> interactionStyle=vtkSmartPointer<ttt::VertexSelectionInteractor>::New();
	interactionStyle->SetRenderer(renderer);
	iren->SetInteractorStyle(interactionStyle);
#endif





	ttt::VertexLocationsDrawer locationsDrawer;

	locationsDrawer.SetVertexLocations(gt);
	locationsDrawer.SetRenderer(renderer);
	locationsDrawer.SetSpacing(spacing);


	locationsDrawer.Draw();
	ttt::VertexnessImageDrawer vertexnessDrawer;
	vertexnessDrawer.SetRenderer(renderer);
	vertexnessDrawer.SetImage(vertexness);
	//vertexnessDrawer.Draw();
	renWin->Render();
	iren->Start();
}
#endif



int main(int argc, char ** argv){

	if(argc< 3){
		std::cout << "Usage: " << argv[0] << " GTFile VertexnessImage ";
	}

	//1. Leer GT
	ttt::AdherensJunctionVertices::Pointer gt= ttt::AdherensJunctionVertices::New() ;
	readGT(argv[1],gt);

	//2. Leer Imagen
	ReaderType::Pointer reader = ReaderType::New();

	reader->SetFileName(argv[2]);

	reader->Update();

	ImageType::Pointer image = reader->GetOutput();

	ImageType::SpacingType spacing;
	spacing[0]=0.0965251;
	spacing[1]=0.0965251;
	spacing[2]=0.378284;
	image->SetSpacing(spacing);

#ifdef DRAW_GT
	drawPointSet(image,gt,spacing);
#endif
#if 0
	mapPointSet<PointSetType,ImageType::SpacingType>(gt,spacing);

#if 0
	for(typename PointsContainer::Iterator it=gtPoints->Begin();it!=gtPoints->End();it++){

		PointSetType::PointType & point = it->Value();

		point[0]*=spacing[0];
		point[1]*=spacing[1];
	}
#endif

	typedef itk::LocalMaximumImageFilter<ImageType> LocalMaximumFilterType;

	LocalMaximumFilterType::Pointer maximumFilter = LocalMaximumFilterType::New();

	maximumFilter->SetInput(image);
	maximumFilter->SetThreshold(0.0005); //Very low but not zero
	itk::Size<3> radius;
	radius.Fill(3);
	maximumFilter->SetRadius(radius);

	maximumFilter->Update();


	PointSetType::Pointer locatedPoints = maximumFilter->GetOutput();

	PointSetType::PointsContainer::Pointer points = locatedPoints->GetPoints();

	typename PointSetType::PointDataContainer::Pointer pointData = locatedPoints->GetPointData();



	std::vector<PointAndValueType> detections;

	for(int i=0;i<pointData->Size();i++){
		PointAndValueType pointAndValue;
		pointAndValue.first=points->GetElement(i);
		pointAndValue.first[0]*=spacing[0];
		pointAndValue.first[1]*=spacing[1];
		pointAndValue.second=pointData->GetElement(i);
		detections.push_back(pointAndValue);
	}

	std::sort(detections.begin(),detections.end(),CompareSecond());
#if 0
	for(std::vector<PointAndValueType>::iterator it=detections.begin();it!=detections.end();it++){
		std:: cout << it->first << it->second << std::endl;
	}
#endif
	int positive = gt->GetNumberOfPoints();
	int negative = detections.size()-positive;
	int count =0;
	int hit=0;
//	int miss=gt.size();

	double precision=1;
	double recall=0;

	double THRESHOLD=1;

	PointSetType::Pointer matched = PointSetType::New();
	PointsContainer::Pointer matchedPoints = matched->GetPoints();
	PointSetType::PointDataContainer::Pointer matchedData = matched->GetPointData();


	for(vector<PointAndValueType>::reverse_iterator it = detections.rbegin();it!=detections.rend();it++){
		count++;
		//TRY TO MATCH
		//PointsContainer::Iterator min=gtPoints->Begin();
		double minValue=1e128;
		int minLoc=0;

		int i=0;
		for(PointsContainer::Iterator gtIt=gtPoints->Begin();gtIt!=gtPoints->End();gtIt++){

			PointSetType::PointType & gtPoint = gtIt->Value();
			double dist=sqrt(pow(gtPoint[0] - it->first[0],2) +pow(gtPoint[1] - it->first[1],2));
			if(dist<minValue){
				minValue=dist;
				minLoc=i;
			}
			i++;
		}
		if(minValue<=THRESHOLD){
			matchedPoints->InsertElement(hit,gtPoints->GetElement(minLoc));
			matchedData->InsertElement(hit,0);
			gtPoints->DeleteIndex(minLoc);
			hit++;
		}
		precision = (double)hit/count;
		recall = (double)hit/positive;
		std::cout << precision << " " << recall << "; ";
	}
	matched->SetPoints(matchedPoints);
	matched->SetPointData(matchedData);

	unmapPointSet<PointSetType,ImageType::SpacingType>(matched,spacing);
	//drawPointSet(image,matched,spacing);
	//2. Obtener vertex locations

	//PARA CADA

	//4. ROC
#endif
}
