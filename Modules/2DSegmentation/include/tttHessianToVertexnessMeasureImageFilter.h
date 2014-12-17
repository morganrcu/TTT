/*
 * tttHessianToVertexnessMeasureImageFilter.h
 *
 *  Created on: Nov 24, 2014
 *      Author: morgan
 */

#ifndef _TTTHESSIANTOVERTEXNESSMEASUREIMAGEFILTER_H_
#define _TTTHESSIANTOVERTEXNESSMEASUREIMAGEFILTER_H_
#include <itkUnaryFunctorImageFilter.h>
namespace ttt{


template<class TTensor,class TMeasure>
class VertexnessFunctor{
public:
	typedef typename TTensor::ComponentType RealType;
	VertexnessFunctor(){
		m_Gamma=5;
		m_Alpha=0.5;
		m_Beta=0.5;
	};
	  ~VertexnessFunctor() {};
	  bool operator!=( const VertexnessFunctor & other ) const
	    {
	    return !(*this == other);
	    }
	  bool operator==( const VertexnessFunctor & other ) const
	    {
	    return true;
	    }
	TMeasure operator()(const TTensor & tensor){

		typename TTensor::EigenValuesArrayType eigenvalues;
		tensor.ComputeEigenValues( eigenvalues);

		RealType maxEig=std::abs(eigenvalues[1])> std::abs(eigenvalues[0])?eigenvalues[1]:eigenvalues[0];
		RealType minEig=std::abs(eigenvalues[1])< std::abs(eigenvalues[0])?eigenvalues[1]:eigenvalues[0];

		TMeasure value;
		if(minEig == 0 && maxEig==0) return 0;

		if(maxEig>=0 || minEig >=0){
			value=0;
		}else{
			RealType minSquared=std::pow(minEig,2);
			RealType maxSquared=std::pow(maxEig,2);
			double S= std::sqrt(maxSquared+minSquared);
			double A = std::abs(minSquared)/std::abs(maxSquared);
			double B = std::abs(maxSquared)/std::abs(minSquared);

			double a=1-std::exp(-S/(2*m_Gamma));

			double b=std::exp(-A/(2*m_Alpha));
			double c=std::exp(-B/(2*m_Beta));
			value =a*b*c;
		}
		return value;

	}
	RealType m_Gamma;
	RealType m_Alpha;
	RealType m_Beta;

};
template<class TTensorImage,class TMeasureImage>
class HessianVertexnessMeasureImageFilter: public itk::UnaryFunctorImageFilter<TTensorImage,TMeasureImage,VertexnessFunctor<typename TTensorImage::PixelType,typename TMeasureImage::PixelType> >{
public:

	typedef HessianVertexnessMeasureImageFilter<TTensorImage,TMeasureImage> Self;
	typedef itk::UnaryFunctorImageFilter<TTensorImage,TMeasureImage,VertexnessFunctor<typename TTensorImage::PixelType,typename TMeasureImage::PixelType> > Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);
	itkTypeMacro(HessianVertexnessMeasureImageFilter, UnaryFunctorImageFilter);

protected:
	HessianVertexnessMeasureImageFilter(){

	}
	virtual ~HessianVertexnessMeasureImageFilter(){

	}
private:


};


}

#endif /* MODULES_2DSEGMENTATION_INCLUDE_TTTHESSIANTOVERTEXNESSMEASUREIMAGEFILTER_H_ */
