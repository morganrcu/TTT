/*
 * VertexLocationCommand.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: morgan
 */

#include "PyramidVertexLocationCommand.h"
#include "tttPyramidLocalMaximumImageFilter.h"
void ttt::PyramidVertexLocationCommand::Do(){
	typedef ttt::PyramidLocalMaximumImageFilter<SignalImageType> LocalMaxFilterType;

	LocalMaxFilterType::Pointer localMaximumFilter=LocalMaxFilterType::New();

	localMaximumFilter->SetInput(m_Inputs);
	localMaximumFilter->SetThreshold(m_LocalMaxThreshold);
	localMaximumFilter->GenerateData();

	m_PyramidVertexLocations=localMaximumFilter->GetOutput();

	m_VertexLocations=ttt::AdherensJunctionVertices<3>::New();

	for(unsigned int i=0;i<m_PyramidVertexLocations.size();i++){
		m_VertexLocations->insert(m_VertexLocations->end(),m_PyramidVertexLocations[i]->begin(),m_PyramidVertexLocations[i]->end());
	}

}
