/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    $RCSfile: itkHessianSmoothed3DToVesselnessMeasureImageFilter.txx,v $
 Language:  C++
 Date:      $Date: 2007/06/12 20:59:44 $
 Version:   $Revision: 1.12 $

 Copyright (c) Insight Software Consortium. All rights reserved.
 See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#include "giaaHessian3DToPlatenessMeasureImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkSymmetricEigenAnalysis.h"
#include "itkMatrix.h"
#include "vnl/vnl_math.h"

#define EPSILON  1e-03

/**
 * Constructor
 */
template<typename TPixel, typename TPlateness, typename TDirection>
ttt::Hessian3DToPlatenessMeasureImageFilter<TPixel, TPlateness, TDirection>::Hessian3DToPlatenessMeasureImageFilter() {
	m_Alpha = 0.5;
	m_Beta = 0.5;
	m_Gamma = 8.0;

	m_C = 10e-6;

	// By default, scale the vesselness measure by the largest
	// eigen value
	m_ScaleVesselnessMeasure = false;

	m_DirectionImage = DirectionImageType::New();
	m_PlatenessImage = PlatenessImageType::New();



}

template<typename TPixel, typename TPlateness, typename TDirection>
void ttt::Hessian3DToPlatenessMeasureImageFilter<TPixel, TPlateness, TDirection>::GenerateData() {

	itkDebugMacro(
			<< "HessianSmoothed3DToVesselnessMeasureImageFilter generating data ");
#if 0
	m_PlatenessImage->SetRegions(this->GetInput()->GetLargestPossibleRegion());
	m_PlatenessImage->SetSpacing(this->GetInput()->GetSpacing());
	m_PlatenessImage->SetOrigin(this->GetOutput()->GetOrigin());
	m_PlatenessImage->Allocate();
#endif
	this->GetOutput()->SetRegions(this->GetInput()->GetLargestPossibleRegion());
	this->GetOutput()->SetSpacing(this->GetInput()->GetSpacing());
	this->GetOutput()->SetOrigin(this->GetOutput()->GetOrigin());
	this->GetOutput()->Allocate();

	m_DirectionImage->SetRegions(this->GetInput()->GetLargestPossibleRegion());
	m_DirectionImage->SetSpacing(this->GetInput()->GetSpacing());
	m_DirectionImage->SetOrigin(this->GetInput()->GetOrigin());
	m_DirectionImage->Allocate();
	m_DirectionImage->FillBuffer(itk::NumericTraits<DirectionPixelType>::Zero);

	itk::ImageRegionIterator<PlatenessImageType> pit;



	pit = itk::ImageRegionIterator<PlatenessImageType>(this->GetOutput(),
			this->GetOutput()->GetLargestPossibleRegion());

	itk::ImageRegionIterator<DirectionImageType> dit;

	dit = itk::ImageRegionIterator<DirectionImageType>(m_DirectionImage,m_DirectionImage->GetLargestPossibleRegion());

	itk::ImageRegionConstIterator<InputImageType> hit;

	hit = itk::ImageConstIterator<InputImageType>(this->GetInput(),
			this->GetInput()->GetLargestPossibleRegion());


	typedef itk::FixedArray<double, 3> EigvalueType;
	typedef itk::Matrix<double, 3, 3> EigvectorType;

	typedef itk::SymmetricEigenAnalysis<InputPixelType, EigvalueType,
			EigvectorType> EigenCalculatorType;

	EigenCalculatorType calculator;

	calculator.SetOrderEigenValues(true);
	//calculator.SetOrderEigenMagnitudes(true);



	calculator.SetDimension(3);


	while (!hit.IsAtEnd()) {
		EigvectorType eigvectors;
		EigvectorType sortedEigvectors;
		EigvalueType eigvalues;

//		calculator.ComputeEigenValues(hit.Get(),eigvalues);
		calculator.ComputeEigenValuesAndVectors(hit.Get(), eigvalues,eigvectors);
		
		   // Find the smallest eigenvalue
    double smallest = vnl_math_abs( eigvalues[0] );
    double Lambda1 = eigvalues[0];
    
    	sortedEigvectors[0][0]=eigvectors[0][0];
	sortedEigvectors[0][1]=eigvectors[0][1];
	sortedEigvectors[0][2]=eigvectors[0][2];
	
    for ( unsigned int i=1; i <=2; i++ )
      {
      if ( vnl_math_abs( eigvalues[i] ) < smallest )
        {
        Lambda1 = eigvalues[i];
        smallest = vnl_math_abs( eigvalues[i] );
	sortedEigvectors[0][0]=eigvectors[i][0];
	sortedEigvectors[0][1]=eigvectors[i][1];
	sortedEigvectors[0][2]=eigvectors[i][2];
        }
      }

    // Find the largest eigenvalue
    double largest = vnl_math_abs( eigvalues[0] );
    double Lambda3 = eigvalues[0];
    sortedEigvectors[2][0]=eigvectors[0][0];
    sortedEigvectors[2][1]=eigvectors[0][1];
    sortedEigvectors[2][2]=eigvectors[0][2];
	
    for ( unsigned int i=1; i <=2; i++ )
      {
      if (  vnl_math_abs( eigvalues[i] > largest ) )
        {
        Lambda3 = eigvalues[i];
        largest = vnl_math_abs( eigvalues[i] );
      	sortedEigvectors[2][0]=eigvectors[i][0];
	sortedEigvectors[2][1]=eigvectors[i][1];
	sortedEigvectors[2][2]=eigvectors[i][2];
        }

      }


    //  find Lambda2 so that |Lambda1| < |Lambda2| < |Lambda3|
    double Lambda2 = eigvalues[0];

    sortedEigvectors[1][0]=eigvectors[0][0];
    sortedEigvectors[1][1]=eigvectors[0][1];
    sortedEigvectors[1][2]=eigvectors[0][2];
	
    for ( unsigned int i=0; i <=2; i++ )
      {
      if ( eigvalues[i] != Lambda1 && eigvalues[i] != Lambda3 )
        {
        Lambda2 = eigvalues[i];
	sortedEigvectors[1][0]=eigvectors[i][0];
	sortedEigvectors[1][1]=eigvectors[i][1];
	sortedEigvectors[1][2]=eigvectors[i][2];
        break;
        }
      }
      
		DirectionPixelType direction;

  		if (Lambda3 >= 0.0) {

			pit.Set(itk::NumericTraits<PlatenessPixelType>::Zero);
			direction[0]=0;
			direction[1]=0;
			direction[2]=0;
		} else {
			direction[0]=sortedEigvectors[2][0];
			direction[1]=sortedEigvectors[2][1];
			direction[2]=sortedEigvectors[2][2];

		
			double Lambda1Abs = vnl_math_abs(Lambda1);
			double Lambda2Abs = vnl_math_abs(Lambda2);
			double Lambda3Abs = vnl_math_abs(Lambda3);

			double Lambda1Sqr = vnl_math_sqr(Lambda1);
			double Lambda2Sqr = vnl_math_sqr(Lambda2);
			double Lambda3Sqr = vnl_math_sqr(Lambda3);

			double AlphaSqr = vnl_math_sqr(m_Alpha);
			double BetaSqr = vnl_math_sqr(m_Beta);
			double GammaSqr = vnl_math_sqr(m_Gamma);

			double A = Lambda2Abs / Lambda3Abs;
			double B = vcl_sqrt(Lambda1Abs * Lambda2Abs) / Lambda3Abs;
			double S = vcl_sqrt(Lambda1Sqr + Lambda2Sqr + Lambda3Sqr);


		    double vesMeasure_1  =
		      ( vcl_exp(-0.5*(( vnl_math_sqr(A) ) / ( AlphaSqr ) )));

		    double vesMeasure_2  =
		      vcl_exp ( -0.5 * ((vnl_math_sqr( B )) /  ( BetaSqr)));

		    double vesMeasure_3  =
		      ( 1 - vcl_exp( -1.0 * (( vnl_math_sqr( S )) / ( 2.0 * ( GammaSqr)))));

		    double vesMeasure_4  =
		      vcl_exp ( -1.0 * ( 2.0 * vnl_math_sqr( m_C )) / ( Lambda3Sqr) );


			double vesselnessMeasure = vesMeasure_1 * vesMeasure_2	* vesMeasure_3 * vesMeasure_4;

			
			if (m_ScaleVesselnessMeasure) {
			  
				pit.Set(
						static_cast<PlatenessPixelType>(Lambda3Abs
								* vesselnessMeasure));
			} else {
			  
				pit.Set(static_cast<PlatenessPixelType>(vesselnessMeasure));
			}
			
		}
		dit.Set(direction);


		++hit;
		++pit;


		++dit;
	}
}

template<typename TPixel, typename TPlateness, typename TDirection>
void ttt::Hessian3DToPlatenessMeasureImageFilter<TPixel, TPlateness, TDirection>::PrintSelf(
		std::ostream& os, itk::Indent indent) const {
	Superclass::PrintSelf(os, indent);

	os << indent << "Alpha: " << m_Alpha << std::endl;
	os << indent << "Beta:  " << m_Beta << std::endl;
	os << indent << "Gamma: " << m_Gamma << std::endl;
	os << indent << "C: " << m_C << std::endl;
	
}

