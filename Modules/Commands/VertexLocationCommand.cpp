/*
 * VertexLocationCommand.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: morgan
 */

#include "VertexLocationCommand.h"

void VertexLocationCommand::Do(){
	itk::Size<3> radius;
	radius.Fill(3);
	m_VertexLocator->SetRadius(radius);
	m_VertexLocator->SetThreshold(m_LocalMaxThreshold);
	m_VertexLocator->SetInput(m_Input);
	m_VertexLocator->Update();

	m_VertexLocations=m_VertexLocator->GetOutput();


}
