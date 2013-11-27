/*
 * tttCLAHEImageFilter.h
 *
 *  Created on: Nov 23, 2013
 *      Author: morgan
 */

#ifndef TTTCLAHEIMAGEFILTER_H_
#define TTTCLAHEIMAGEFILTER_H_
#include <itkBoxImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>
#include <itkConstNeighborhoodIterator.h>
#include <itkNeighborhoodAlgorithm.h>
#include <itkZeroFluxNeumannBoundaryCondition.h>
#include <itkVector.h>
namespace ttt{
template< class TImage> class CLAHEImageFilter : public itk::BoxImageFilter<TImage,TImage>{

public:
	typedef TImage InputImageType;
	typedef TImage OutputImageType;
	typedef CLAHEImageFilter Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	typedef itk::ImageToImageFilter<InputImageType,InputImageType> Superclass;

	itkTypeMacro(Self,Superclass);
	itkNewMacro(Self);

	itkGetMacro(MaxSlope,float);
	itkSetMacro(MaxSlope,float);

	virtual ~CLAHEImageFilter(){

	}


protected:

	CLAHEImageFilter(){
		 m_MaxSlope=3;
	}
	void GenerateData	();

private:
	CLAHEImageFilter(const Self & other);

	float m_MaxSlope;
};

template<class TImage> void CLAHEImageFilter<TImage>::GenerateData() {
	typename InputImageType::ConstPointer input = this->GetInput();
	typename OutputImageType::Pointer output = this->GetOutput();

	// Allocate the output
	this->AllocateOutputs();

	// Iterator which traverse the input
	itk::ImageRegionConstIterator < InputImageType
			> itInput(input, input->GetRequestedRegion());

	// Setup for processing the image
	//
	itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;
	itk::ConstNeighborhoodIterator<InputImageType> bit;
	// Find the data-set boundary "faces"
	typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType faceList;
	itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator < InputImageType > bC;
	faceList = bC(input, output->GetRequestedRegion(), this->GetRadius());

	typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

	// Map stores (number of pixel)/(window size) for each gray value.
	typedef std::vector<float> MapType;
	MapType count;
	MapType::iterator itMap;

	itk::ProgressReporter progress(this, 0,output->GetRequestedRegion().GetNumberOfPixels());

	// Process each faces.  These are N-d regions which border
	// the edge of the buffer.
	for (fit = faceList.begin(); fit != faceList.end(); ++fit) {
		// Create a neighborhood iterator for the normalized image for the
		// region for this face
		bit = itk::ConstNeighborhoodIterator<InputImageType>(this->GetRadius(),
				input, *fit);
		bit.OverrideBoundaryCondition(&nbc);
		bit.GoToBegin();
		unsigned int neighborhoodSize = bit.Size();

		// iterator for the output for this face
		itk::ImageRegionIterator < InputImageType > itOut(output, *fit);

		// iterate over the region for this face

		float clipPoint =(double)(neighborhoodSize*m_MaxSlope)/256;

		while (!bit.IsAtEnd()) {

			unsigned char f=0;
			unsigned char center=0;
			float sum=0;

			center=bit.GetCenterPixel();

			if(center!=0){
				// "Histogram the window"
				count.resize(256);
				for(int i=0;i< 256;i++){
					count[i]=0;
				}

				for (int i = 0; i < neighborhoodSize; ++i) {
					f = bit.GetPixel(i);
					count[f]++;
				}

				float clippedMass=0;

				for(int i=0;i<256;i++){
					if(count[i]>clipPoint){
						clippedMass=count[i]-clipPoint;
						count[i]=clipPoint;
					}
				}
				float clipPerBin=clippedMass/256;
				for(int i=0;i<256;i++){
					count[i]+=clipPerBin;
				}
				itMap = count.begin();


				MapType cumulative;

				while ( itMap != count.end() ){

					cumulative.push_back((255.0/neighborhoodSize)*sum);
					//std::cout << "(" << itMap->first << "," <<cumulative[itMap->first] << ") ";
					sum = sum+*itMap;
					++itMap;
				}
				//std::cout << std::endl;
				itOut.Set( (unsigned char)(floor(cumulative[center])) );
			}
			++bit;
			++itOut;
		}
	}

}
}

#endif /* TTTCLAHEIMAGEFILTER_H_ */
