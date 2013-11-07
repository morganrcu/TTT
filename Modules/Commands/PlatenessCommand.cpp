/*
 * PlatenessCommand.cpp
 *
 *  Created on: Sep 21, 2013
 *      Author: morgan
 */

#include "PlatenessCommand.h"

void PlatenessCommand::Do(){

	m_Plateness->SetSigmaMin(m_SigmaMin);
	m_Plateness->SetSigmaMax(m_SigmaMax);
	m_Plateness->SetNumberOfSigmaSteps(m_SigmaSteps);
	m_Plateness->SetInput(m_Input);
	m_Plateness->Update();
	m_PlatenessRescaler->SetInput(m_Plateness->GetOutput());
	m_PlatenessRescaler->Update();

	m_PlatenessImage=m_PlatenessRescaler->GetOutput();
	m_OrientationImage=m_Plateness->GetDirectionImage();
}
