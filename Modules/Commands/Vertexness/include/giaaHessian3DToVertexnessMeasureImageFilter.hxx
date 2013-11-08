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

#include "giaaHessian3DToVertexnessMeasureImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkSymmetricEigenAnalysis.h"
#include "itkMatrix.h"
#include "vnl/vnl_math.h"

#define EPSILON  1e-03

/**
 * Constructor
 */
template<typename TPixel, typename TVertexness>
giaa::Hessian3DToVertexnessMeasureImageFilter<TPixel, TVertexness>::Hessian3DToVertexnessMeasureImageFilter() {
	m_Alpha = 0.5;
	m_Beta = 0.5;
	m_Gamma = 8.0;

	m_C = 10e-6;

	// By default, scale the vesselness measure by the largest
	// eigen value
	m_ScaleVesselnessMeasure = false;

	m_VertexnessImage = VertexnessImageType::New();



}

template<typename TPixel, typename TVertexness>
void giaa::Hessian3DToVertexnessMeasureImageFilter<TPixel, TVertexness>::GenerateData() {

	itkDebugMacro(
			<< "HessianSmoothed3DToVesselnessMeasureImageFilter generating data ");

	this->GetOutput()->SetRegions(this->GetInput()->GetLargestPossibleRegion());
	this->GetOutput()->SetSpacing(this->GetInput()->GetSpacing());
	this->GetOutput()->SetOrigin(this->GetOutput()->GetOrigin());
	this->GetOutput()->Allocate();

	itk::ImageRegionIterator<VertexnessImageType> pit;

	pit = itk::ImageRegionIterator<VertexnessImageType>(this->GetOutput(),
			this->GetOutput()->GetLargestPossibleRegion());
	
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
    

	
    for ( unsigned int i=1; i <=2; i++ )
      {
      if ( vnl_math_abs( eigvalues[i] ) < smallest )
        {
        Lambda1 = eigvalues[i];
        smallest = vnl_math_abs( eigvalues[i] );

        }
      }

    // Find the largest eigenvalue
    double largest = vnl_math_abs( eigvalues[0] );
    double Lambda3 = eigvalues[0];

	
    for ( unsigned int i=1; i <=2; i++ )
      {
      if (  vnl_math_abs( eigvalues[i] > largest ) )
        {
        Lambda3 = eigvalues[i];
        largest = vnl_math_abs( eigvalues[i] );

        }

      }


    //  find Lambda2 so that |Lambda1| < |Lambda2| < |Lambda3|
    double Lambda2 = eigvalues[0];

    for ( unsigned int i=0; i <=2; i++ )
      {
      if ( eigvalues[i] != Lambda1 && eigvalues[i] != Lambda3 )
        {
        Lambda2 = eigvalues[i];
	
        break;
        }
      }
      double value;
      if (Lambda3 < 0 && Lambda2 < 0 && Lambda1 < 0) {
		double S = vcl_sqrt(Lambda1*Lambda1 + Lambda2*Lambda2 * Lambda3*Lambda3);
		double A = Lambda1/Lambda2;
		double B = Lambda2/Lambda3;
		
//			double term1 =  ( 1 - vcl_exp( -1.0 * (( vnl_math_sqr(S)))  / (2.0*GammaSqr)));
//			double term2  = ( 1 - vcl_exp( -1.0 * (( vnl_math_sqr( A )) / (2.0*AlphaSqr))));
//			double term3  = ( 1 - vcl_exp( -1.0 * (( vnl_math_sqr( B )) / (2.0*BetaSqr))));
			double term1 =  ( 1 - vcl_exp( -1.0 * (S)));
			double term2  = ( 1 - vcl_exp( -1.0 * (A)));
			double term3  = ( 1 - vcl_exp( -1.0 * (B)));

			value= term1*term2*term3;
		}else{
			value=0;
		}
		
		pit.Set(value);


		++hit;
		++pit;


		
	}
}

template<typename TPixel, typename TVertexness>
void giaa::Hessian3DToVertexnessMeasureImageFilter<TPixel, TVertexness>::PrintSelf(
		std::ostream& os, itk::Indent indent) const {
	Superclass::PrintSelf(os, indent);

	os << indent << "Alpha: " << m_Alpha << std::endl;
	os << indent << "Beta:  " << m_Beta << std::endl;
	os << indent << "Gamma: " << m_Gamma << std::endl;

	
	
	os << indent << "C: " << m_C << std::endl;
}

