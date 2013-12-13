/*=========================================================================

  authors: Bryn A. Lloyd, Simon K. Warfield, Computational Radiology Laborotory (CRL), Brigham and Womans
  date: 06/30/2005
  
  Acknowledgements:
This investigation was supported in part by NSF ITR 0426558 and 
NIH grants R21 MH67054 and P41 RR13218.

=========================================================================*/
#ifndef _itkLocalMaximumImageFilter_txx
#define _itkLocalMaximumImageFilter_txx
#include "itkLocalMaximumImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"

#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkConstantBoundaryCondition.h"

#include "itkOffset.h"
#include "itkProgressReporter.h"

#include "itkNumericTraits.h"

#include <iostream>
namespace itk
{

template <class TInputImage> LocalMaximumImageFilter< TInputImage>::LocalMaximumImageFilter()
{

  this->m_Radius.Fill(1);
  this->m_Threshold = 0.005;

  m_LocalMaxima = LocalMaximaContainerType::New();
}




template<class TInputImage> void LocalMaximumImageFilter<TInputImage>::GenerateData() {


	InputImageConstPointer input = this->GetInput();


	unsigned int i;
	ConstantBoundaryCondition<InputImageType> cbc;
	cbc.SetConstant(NumericTraits<InputPixelType>::NonpositiveMin());

	ConstNeighborhoodIterator<InputImageType> bit;

	// Find the data-set boundary "faces"
	typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType faceList;
	NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
	faceList = bC(input, input->GetRequestedRegion(), m_Radius);

	typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

	// Process each of the boundary faces.  These are N-d regions which border
	// the edge of the buffer.
	for (fit = faceList.begin(); fit != faceList.end(); ++fit) {

		bit = ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
		unsigned int neighborhoodSize = bit.Size();

		bit.OverrideBoundaryCondition(&cbc);
		bit.GoToBegin();

		while (!bit.IsAtEnd()) {
			bool isMaximum = true;
			InputPixelType centerValue = bit.GetCenterPixel(); //NumericTraits<InputRealType>::NonpositiveMin();
			if (centerValue > m_Threshold) {
				for (i = 0; i < neighborhoodSize; ++i) {
					InputPixelType tmp = bit.GetPixel(i);

					// if we find a neighbor with a larger value than the center, tthe center is not a maximum...
					if (tmp > centerValue) {
						isMaximum = false;
						break;
					}
				}
				if (isMaximum) {
					ttt::AdherensJunctionVertex::Pointer maxIndex = ttt::AdherensJunctionVertex::New();
					maxIndex->SetPosition(bit.GetIndex());

					m_LocalMaxima->push_back(maxIndex);
				}
			}
			++bit;

		}
	}
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage>
void
LocalMaximumImageFilter< TInputImage>
::PrintSelf(
  std::ostream& os,
  Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Radius: " << m_Radius << std::endl;
  os << indent << "Threshold: " << m_Threshold << std::endl;

}


} // end namespace itk

#endif
