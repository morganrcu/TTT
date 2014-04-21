#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <algorithm>    // std::move_backward
#include <vector>        // std::array
//#include <random>       // std::default_random_engine
//#include <chrono>       // std::chrono::system_clock


int main(int argc,char ** argv){

	typedef itk::Image<short,3> ImageType;

	typedef itk::ImageFileReader<ImageType> ReaderType;

	ReaderType::Pointer reader= ReaderType::New();

	reader->SetFileName(argv[1]);
	reader->Update();


	ImageType::Pointer voronoi = reader->GetOutput();

	ReaderType::Pointer reader2= ReaderType::New();
	reader2->SetFileName(argv[2]);
	reader2->Update();

	ImageType::Pointer segmentation = reader2->GetOutput();

	typedef itk::MinimumMaximumImageCalculator< ImageType > MinimumMaximumType;

	MinimumMaximumType::Pointer minimumMaximum = MinimumMaximumType::New();

	minimumMaximum->SetImage(voronoi);

	minimumMaximum->Compute();

	std::vector<int> dict;
#if 0
	std::cout << minimumMaximum->GetMaximum();
	for(int i=0;i<=minimumMaximum->GetMaximum();i++){
		dict.push_back(i);
	}
#endif

	itk::ImageRegionIterator<ImageType> voronoiIterator(voronoi,voronoi->GetLargestPossibleRegion());
	while(!voronoiIterator.IsAtEnd()){
		if(voronoiIterator.Get()!=-1){
			dict.push_back(voronoiIterator.Get());
		}
		++voronoiIterator;
	}
	std::random_shuffle ( dict.begin(), dict.end() );
	voronoiIterator.GoToBegin();
	while(!voronoiIterator.IsAtEnd()){
		if(voronoiIterator.Get()!=-1){
			voronoiIterator.Set(dict[voronoiIterator.Get()]);
		}
		++voronoiIterator;
	}

	itk::ImageRegionIterator<ImageType> segmentationIterator(segmentation,segmentation->GetLargestPossibleRegion());
	while(!segmentationIterator.IsAtEnd()){
		if(segmentationIterator.Get()!=-1){
			segmentationIterator.Set(dict[segmentationIterator.Get()]);
		}
		++segmentationIterator;
	}
	typedef itk::ImageFileWriter<ImageType> WriterType;

	WriterType::Pointer writer = WriterType::New();

	writer->SetFileName(argv[3]);
	writer->SetInput(voronoi);
	writer->Update();

	WriterType::Pointer writer2 = WriterType::New();
	writer2->SetFileName(argv[4]);
	writer2->SetInput(segmentation);
	writer2->Update();


}
