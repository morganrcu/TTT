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
#include <itkIndex.h>

namespace itk{
	static bool operator<(const itk::Index<3> & a, const itk::Index<3> & b ) {
		if(a[0]<b[0]){
			return true;
		}else if(a[0]>b[0]){
			return false;
		}else{
			if(a[1]<b[1]){
				return true;
			}else if(a[1]>b[1]){
				return false;
			}else{
				if(a[2]<b[2]){
					return true;
				}else if(a[2]>b[2]){
					return false;
				}
			}
		}
		return false;
	}
}

namespace ttt{


template< class TImage> class CLAHEImageFilter : public itk::BoxImageFilter<TImage,TImage>{

public:
	typedef TImage InputImageType;
	typedef typename InputImageType::IndexType InputIndexType;
	typedef typename InputImageType::PointType InputPointType;

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

	bool IndexOnCorner(const itk::Index<3> &);
	bool IndexOnEdge(const itk::Index<3> &);
	bool IndexOnFace(const itk::Index<3> &);



	typedef vnl_vector_fixed<double,256> LUT;
	std::map<InputIndexType,LUT> m_LUTables;


	InputPointType m_000;
	InputPointType m_001;
	InputPointType m_010;
	InputPointType m_011;
	InputPointType m_100;
	InputPointType m_101;
	InputPointType m_110;
	InputPointType m_111;

	InputIndexType m_i000;
	InputIndexType m_i001;
	InputIndexType m_i010;
	InputIndexType m_i011;
	InputIndexType m_i100;
	InputIndexType m_i101;
	InputIndexType m_i110;
	InputIndexType m_i111;
};
template<class TImage> bool CLAHEImageFilter<TImage>::IndexOnCorner(const itk::Index<3> & index) {
	if(index[0]<=m_i000[0] && index[1]<=m_i000[1] && index[2]<=m_i000[2]){
		return true;
	}else if(index[0]<=m_i001[0] && index[1]<=m_i001[1] && index[2]>=m_i001[2]){
		return true;
	}else if(index[0]<=m_i010[0] && index[1]>=m_i010[1] && index[2]<=m_i010[2]){
		return true;
	}else if(index[0]<=m_i011[0] && index[1]>=m_i011[1] && index[2]>=m_i011[2]){
		return true;
	}else if(index[0]>=m_i100[0] && index[1]<=m_i100[1] && index[2]<=m_i100[2]){
		return true;
	}else if(index[0]>=m_i101[0] && index[1]<=m_i101[1] && index[2]>=m_i101[2]){
		return true;
	}else if(index[0]>=m_i110[0] && index[1]>=m_i110[1] && index[2]<=m_i110[2]){
		return true;
	}else if(index[0]>=m_i111[0] && index[1]>=m_i111[1] && index[2]>=m_i111[2]){
		return true;
	}
	return false;
}
template<class TImage> bool CLAHEImageFilter<TImage>::IndexOnEdge(const itk::Index<3> & index) {
	if(index[0]<m_i000[0] && index[1]<m_i000[1] && index[2]>m_i000[2] && index[2]<m_i001[2] ){
		return true;
	} else if(index[0]<m_i000[0] && index[1]>m_i000[1] && index[1]<m_i010[1] && index[2]<m_i000[2] ){
		return true;
	} else if(index[0]>m_i000[0] &&  index[0]<m_i100[1] && index[1]<m_i000[1] && index[2]<m_i000[2]){
		return true;
	} else if(index[0]>m_i111[0] && index[1]>m_i111[1] && index[2]>m_i110[2] && index[2]<m_i111[2] ){
		return true;
	} else if(index[0]>m_i111[0] &&  index[1]>m_i101[1] && index[1]<m_i111[1] && index[2]>m_i111[2] ){
		return true;
	} else if(index[0]>m_i011[0] &&  index[0]<m_i111[0] && index[1]>m_i111[1] && index[2]>m_i111[2] ){
		return true;
	} else if(index[0]>m_i010[0] && index[0]<m_i110[0] && index[1]>m_i010[1] && index[2]<m_i010[2] ){
		return true;
	} else if(index[0]<m_i010[0] && index[1]>m_i010[1] && index[2]>m_i010[1] && index[2]<m_i011[2] ){
		return true;
	} else if(index[0]<m_i001[0] && index[1]>m_i001[1] && index[1]<m_i011[1] && index[2]>m_i001[2] ){
		return true;
	} else if(index[0]>m_i001[0] && index[0]<m_i101[0] && index[1]<m_i001[1] && index[2]<m_i001[2] ){
		return true;
	} else if(index[0]>m_i100[0] && index[1]<m_i100[1] && index[2]>m_i100[1] && index[2]<m_i101[2] ){
		return true;
	} else if(index[0]>m_i100[0] && index[1]>m_i100[1] && index[1]<m_i110[1] && index[2]<m_i100[2] ){
		return true;
	}
	return false;
}
template<class TImage> bool CLAHEImageFilter<TImage>::IndexOnFace(const itk::Index<3> & index) {
	if(index[0] < m_i000[0] && index[1]> m_i000[1]&& index[1]< m_i010[1] && index[2]> m_i000[2]&& index[2]< m_i001[2]){
		return true;
	} else if(index[0] > m_i100[0] && index[1]> m_i100[1]&& index[1]< m_i110[1] && index[2]> m_i100[2]&& index[2]< m_i101[2]){
		return true;
	} else if(index[1] < m_i100[1] && index[0]> m_i000[0] && index[0]< m_i100[0] && index[2]> m_i000[2]&& index[2]< m_i001[2]){
		return true;
	} else if(index[1] > m_i110[1] && index[0]> m_i010[0] && index[0]< m_i110[0] && index[2]> m_i110[2]&& index[2]< m_i111[2]){
		return true;
	} else if(index[2] < m_i000[2] && index[0]> m_i000[0] && index[0]< m_i100[0] && index[1]> m_i000[1]&& index[1]< m_010[1]){
		return true;
	} else if(index[2] > m_i001[2] && index[0]> m_i001[0] && index[0]< m_i101[0] && index[1]> m_i001[1]&& index[1]< m_011[1]){
		return true;
	}
	return false;
}

template<class TImage> void CLAHEImageFilter<TImage>::GenerateData() {
	typename InputImageType::ConstPointer input = this->GetInput();


	typename TImage::SizeType radius = this->GetRadius();

	typename InputImageType::SizeType size = input->GetLargestPossibleRegion().GetSize();

	typename InputImageType::SizeType realSize;
	typename InputImageType::SpacingType spacing=input->GetSpacing();




	realSize[0]=spacing[0]*size[0];
	realSize[1]=spacing[1]*size[1];
	realSize[2]=spacing[2]*size[2];

	m_000[0]=radius[0];
	m_000[1]=radius[1];
	m_000[2]=radius[2];
	input->TransformPhysicalPointToIndex(m_000,m_i000);

	m_001[0]=radius[0];
	m_001[1]=radius[1];
	m_001[2]=realSize[2]-radius[2];
	input->TransformPhysicalPointToIndex(m_001,m_i001);

	m_010[0]=radius[0];
	m_010[1]=realSize[1]-radius[1];
	m_010[2]=radius[2];
	input->TransformPhysicalPointToIndex(m_010,m_i010);

	m_011[0]=radius[0];
	m_011[1]=realSize[1]-radius[1];
	m_011[2]=realSize[2]-radius[2];
	input->TransformPhysicalPointToIndex(m_011,m_i011);

	m_100[0]=realSize[0]-radius[0];
	m_100[1]=radius[1];
	m_100[2]=radius[2];
	input->TransformPhysicalPointToIndex(m_100,m_i100);

	m_101[0]=realSize[0]-radius[0];
	m_101[1]=radius[1];
	m_101[2]=realSize[2]-radius[2];
	input->TransformPhysicalPointToIndex(m_101,m_i101);

	m_110[0]=realSize[0]-radius[0];
	m_110[1]=realSize[1]-radius[1];
	m_110[2]=radius[2];
	input->TransformPhysicalPointToIndex(m_110,m_i110);

	m_111[0]=realSize[0]-radius[0];
	m_111[1]=realSize[1]-radius[1];
	m_111[2]=realSize[2]-radius[2];
	input->TransformPhysicalPointToIndex(m_111,m_i111);

	itk::ConstNeighborhoodIterator<InputImageType> bit;
	itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

	for(double r=radius[0];r<=realSize[0]-radius[0];r+=2*radius[0]){
		for(double c=radius[1];c<=realSize[1]-radius[1];c+=2*radius[1]){
			for(double h=radius[2];h<=realSize[2]-radius[2];h+=2*radius[2]){

				//Create neighborhood iterator
				typename InputImageType::PointType point;
				point[0]=r;
				point[1]=c;
				point[2]=h;
				typename  InputImageType::RegionType region;
				InputIndexType index;
				typename InputImageType::SizeType size;
				input->TransformPhysicalPointToIndex(point,index);
				size.Fill(1);
				region.SetSize(size);
				region.SetIndex(index);
				bit = itk::ConstNeighborhoodIterator<InputImageType>(this->GetRadius(),input, region);

				bit.OverrideBoundaryCondition(&nbc);
				bit.GoToBegin();
				assert(!bit.IsAtEnd());

				//Create LUT
				unsigned int neighborhoodSize = bit.Size();
				float clipPoint =(double)(neighborhoodSize*m_MaxSlope)/256;

				unsigned char f=0;
				unsigned char center=0;
				float sum=0;

				center=bit.GetCenterPixel();

				// "Histogram the window"
				LUT count;
				count.fill(0);
				for (int i = 0; i < neighborhoodSize; ++i) {
					f = bit.GetPixel(i);
					count[f]++;
				}

				float clippedMass = 0;

				for (int i = 0; i < 256; i++) {
					if (count[i] > clipPoint) {
						clippedMass = count[i] - clipPoint;
						count[i] = clipPoint;
					}
				}
				float clipPerBin = clippedMass / 256;
				for (int i = 0; i < 256; i++) {
					count[i] += clipPerBin;
				}


				LUT cumulative;

				for(int i=0;i<cumulative.size();i++){
					cumulative[i]=(255.0 / neighborhoodSize) * sum;
					sum = sum + cumulative[i];
				}

				m_LUTables[index]=cumulative;
			}
		}
	}



	typename OutputImageType::Pointer output = this->GetOutput();

	// Allocate the output
	this->AllocateOutputs();

	itk::ImageRegionIteratorWithIndex < OutputImageType > itOut(output, output->GetRequestedRegion());

	while(itOut.IsAtEnd()){

		InputIndexType index = itOut.GetIndex();

		if(this->IndexOnCorner(index)){ //IF INDEX ON CORNERS -> USE LUT

		}else if(this->IndexOnEdge(index)){ //IF INDEX ON CORNERS -> USE  INTERPOLATION

		}else if(this->IndexOnFace(index)){  //IF INDEX ON CORNERS -> USE  BILINEAR INTERPOLATION

		}else{// USE  TRILINEAR INTERPOLATION


		}


		//ELSE IF INDEX ON EDGES -> LINEAR INTERPOLATION

		//ELSE IF INDEX ON FACES -> BILINEAR INTERPOLATION

		//ELSE -> TRILINAR INTERPOLATION


		++itOut;
	}



	// Iterator which traverse the input
	itk::ImageRegionConstIterator < InputImageType
			> itInput(input, input->GetRequestedRegion());

	// Setup for processing the image
	//


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

