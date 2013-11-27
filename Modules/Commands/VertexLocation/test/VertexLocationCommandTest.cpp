#include "VertexLocationCommand.h"

template <typename TImage>
void CreateImage(const typename TImage::Pointer & image)
{
  // This function creates a 2D image consisting of a black background,
  // a large square of a non-zero pixel value, and a single "erroneous" pixel
  // near the square.
  typename TImage::IndexType corner = {{0,0,0}};

  typename TImage::SizeType size = {{20,20,20}};

  typename TImage::RegionType region(corner, size);


  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);


  typename TImage::IndexType pixelIndex = {{10,10,10}};
  image->SetPixel(pixelIndex, 1);

  typename TImage::IndexType pixelIndex2 = {{5,5,5}};
  image->SetPixel(pixelIndex2, 1);

  typename TImage::IndexType pixelIndex3 = {{4,4,4}};
  image->SetPixel(pixelIndex3, 0.9);

  typename TImage::IndexType pixelIndex4 = {{15,15,15}};
  image->SetPixel(pixelIndex4, 1);
}

int main(int argc,char ** argv){

	ttt::VertexLocationCommand command;

	typedef itk::Image<float,3> InputImageType;
	InputImageType::Pointer inputImage=InputImageType::New();
	CreateImage<InputImageType>(inputImage);
	command.SetVertexnessImage(inputImage);
	command.SetLocalMaxThreshold(0.95);
	command.Do();

}
