/*
 * tttHessianToRidgenessMeasureImageFilter.h
 *
 *  Created on: Nov 24, 2014
 *      Author: morgan
 */

#ifndef _TTTHESSIANTORIDGENESSMEASUREIMAGEFILTER_H_
#define _TTTHESSIANTORIDGENESSMEASUREIMAGEFILTER_H_
#include <itkUnaryFunctorImageFilter.h>
namespace ttt{

template<class TTensor,class TMeasure>
class RidgenessFunctor{
public:
	typedef typename TTensor::ComponentType RealType;
	RidgenessFunctor(){
		m_Gamma=5;
		m_Alpha=0.5;
	};
	  ~RidgenessFunctor() {};
	  bool operator!=( const RidgenessFunctor & other ) const
	    {
	    return !(*this == other);
	    }
	  bool operator==( const RidgenessFunctor & other ) const
	    {
	    return true;
	    }
	TMeasure operator()(const TTensor & tensor){

		typename TTensor::EigenValuesArrayType eigenvalues;
		tensor.ComputeEigenValues( eigenvalues);

		RealType maxEig=std::abs(eigenvalues[1])> std::abs(eigenvalues[0])?eigenvalues[1]:eigenvalues[0];
		RealType minEig=std::abs(eigenvalues[1])< std::abs(eigenvalues[0])?eigenvalues[1]:eigenvalues[0];

		TMeasure value;
		if(maxEig>0 ){
			value=0;
		}else{
			RealType minSquared=std::pow(minEig,2);
			RealType maxSquared=std::pow(maxEig,2);
			double S= std::sqrt(maxSquared+minSquared);
			double A = std::abs(minSquared)/std::abs(maxSquared);

			double a=1-std::exp(-S/(2*m_Gamma));

			double b=std::exp(-A/(2*m_Alpha));
			value =a*b;
		}
		return value;

	}
	RealType m_Gamma;
	RealType m_Alpha;

};
template<class TTensorImage,class TMeasureImage>
class HessianRidgenessMeasureImageFilter: public itk::UnaryFunctorImageFilter<TTensorImage,TMeasureImage,RidgenessFunctor<typename TTensorImage::PixelType,typename TMeasureImage::PixelType> >{
public:

	typedef HessianRidgenessMeasureImageFilter<TTensorImage,TMeasureImage> Self;
	typedef itk::UnaryFunctorImageFilter<TTensorImage,TMeasureImage,RidgenessFunctor<typename TTensorImage::PixelType,typename TMeasureImage::PixelType> > Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);
	itkTypeMacro(HessianRidgenessMeasureImageFilter, itk::UnaryFunctorImageFilter);

protected:
	HessianRidgenessMeasureImageFilter(){

	}
	virtual ~HessianRidgenessMeasureImageFilter(){

	}
private:


};


}




#endif /* MODULES_2DSEGMENTATION_INCLUDE_TTTHESSIANTORIDGENESSMEASUREIMAGEFILTER_H_ */
