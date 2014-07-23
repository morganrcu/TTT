/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2007-01-20 20:05:58 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter_txx
#define __itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter_txx

#include "itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkMultiplyByComplexConjugateImageFilter.h"
#include "itkRelativeChangeCalculator.h"
#include "itkDivideOrZeroOutImageFilter.h"

namespace itk {

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
MaximumEntropyRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::MaximumEntropyRichardsonLucyDeconvolutionImageFilter()
{
  m_Lambda = 0.005;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
MaximumEntropyRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Init()
{
  Superclass::Init();
  
  typedef itk::BinaryFunctorImageFilter< InternalImageType,
                InternalImageType,
                InternalImageType,
                typename Functor::MultiplyByXMinusLambdaXLnX< TInternalPrecision > > MultType;
  typename MultType::Pointer mult = MultType::New();
  mult->SetInput( 0, this->m_Multiplication->GetInput( 0 ) );
  mult->SetInput( 1, this->m_Multiplication->GetInput( 1 ) );
  mult->GetFunctor().m_Lambda = m_Lambda;
  mult->SetNumberOfThreads( this->GetNumberOfThreads() );
  // can't be released, it is required by two filters on next iteration
  // mult->SetReleaseDataFlag( true );
  mult->SetInPlace( true );
  
  this->m_Multiplication = mult;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
MaximumEntropyRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Lambda: "  << m_Lambda << std::endl;
}

}// end namespace itk
#endif
