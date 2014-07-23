#ifndef _tttLocalMaximumImageFilter_txx
#define _tttLocalMaximumImageFilter_txx
#include "tttPyramidLocalMaximumImageFilter.h"

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
namespace ttt {

template<class TInputImage> PyramidLocalMaximumImageFilter<TInputImage>::PyramidLocalMaximumImageFilter() {


	this->m_Threshold = 0.005;
}

template<class TInputImage> void PyramidLocalMaximumImageFilter<TInputImage>::GenerateData() {

	std::cout << "Here we are" << std::endl;
	int numLevels = m_Inputs.size();
	typename InputImageType::SizeType size =
			m_Inputs[0]->GetLargestPossibleRegion().GetSize();

	m_LocalMaxima.resize(numLevels);
	for (int level = 0; level < numLevels; level++) {
		m_LocalMaxima[level] = LocalMaximaContainerType::New();
		typename InputImageType::IndexType position;
		for (int r = 0; r < size[0]; r++) {
			position[0] = r;
			for (int c = 0; c < size[1]; c++) {
				position[1] = c;
				for (int h = 0; h < size[2]; h++) {
					position[2] = h;

					double centerValue = m_Inputs[level]->GetPixel(position);
					if(centerValue < m_Threshold) continue;
					bool max = true;
					typename InputImageType::OffsetType offset;

					for (int checkLevel=0;checkLevel<numLevels;checkLevel++){
						for (int i = -1; i <= 1 && max; i++) {
							offset[0] = i;
							for (int j = -1; j <= 1 && max; j++) {
								offset[1] = j;
								for (int k = -1; k <= 1 && max; k++) {
									if (i == 0 && j == 0 && k == 0 && checkLevel==level)
										continue;
									offset[2] = k;

									typename InputImageType::IndexType neigh = position
											+ offset;

									if (neigh[0] < 0 || neigh[1] < 0 || neigh[2] < 0
											|| neigh[0] >= size[0]
											|| neigh[1] >= size[1]
											|| neigh[2] >= size[2])
										continue;

									if (m_Inputs[checkLevel]->GetPixel(neigh)
											> centerValue) {
										max = false;
									}

								}
							}
						}
					}
#if 0
					for (int i = -1; i <= 1 && max; i++) {
						offset[0] = i;
						for (int j = -1; j <= 1 && max; j++) {
							offset[1] = j;
							for (int k = -1; k <= 1 && max; k++) {
								if (i == 0 && j == 0 && k == 0)
									continue;
								offset[2] = k;

								typename InputImageType::IndexType neigh = position
										+ offset;

								if (neigh[0] < 0 || neigh[1] < 0 || neigh[2] < 0
										|| neigh[0] >= size[0]
										|| neigh[1] >= size[1]
										|| neigh[2] >= size[2])
									continue;

								if (m_Inputs[level]->GetPixel(neigh)
										> centerValue) {
									max = false;
								}

							}
						}
					}

					if (max && level > 0) {
						for (int i = -1; i <= 1 && max; i++) {
							offset[0] = i;
							for (int j = -1; j <= 1 && max; j++) {
								offset[1] = j;
								for (int k = -1; k <= 1 && max; k++) {
									offset[2] = k;
									typename InputImageType::IndexType neigh = position
											+ offset;

									if (neigh[0] < 0 || neigh[1] < 0
											|| neigh[2] < 0
											|| neigh[0] >= size[0]
											|| neigh[1] >= size[1]
											|| neigh[2] >= size[2])
										continue;
									if (m_Inputs[level - 1]->GetPixel(neigh)
											> centerValue) {
										max = false;
									}

								}
							}
						}
					}
					if (max && level < numLevels - 1) {
						for (int i = -1; i <= 1 && max; i++) {
							offset[0] = i;
							for (int j = -1; j <= 1 && max; j++) {
								offset[1] = j;
								for (int k = -1; k <= 1 && max; k++) {
									offset[2] = k;
									typename InputImageType::IndexType neigh = position
											+ offset;

									if (neigh[0] < 0 || neigh[1] < 0
											|| neigh[2] < 0
											|| neigh[0] >= size[0]
											|| neigh[1] >= size[1]
											|| neigh[2] >= size[2])
										continue;
									if (m_Inputs[level + 1]->GetPixel(neigh)
											> centerValue) {
										max = false;
									}

								}
							}
						}
					}
#endif
					if (max) {
						ttt::AdherensJunctionVertex::Pointer maxIndex =
								ttt::AdherensJunctionVertex::New();
						maxIndex->SetPosition(position);
						m_LocalMaxima[level]->push_back(maxIndex);
					}
				}
			}
		}
	}
}

/**
 * Standard "PrintSelf" method
 */
template<class TInputImage>
void PyramidLocalMaximumImageFilter<TInputImage>::PrintSelf(std::ostream& os,
		itk::Indent indent) const {
	Superclass::PrintSelf(os, indent);

	os << indent << "Threshold: " << m_Threshold << std::endl;

}

} // end namespace itk

#endif
