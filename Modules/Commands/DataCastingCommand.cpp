/*
 * DataCastingCommand.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: morgan
 */

#include "DataCastingCommand.h"

void DataCastingCommand::Do(){
	m_Caster->SetInput(m_Input);
	m_Caster->Update();
	m_Rescaler->SetInput(m_Caster->GetOutput());
	m_Rescaler->Update();
	m_Output=m_Rescaler->GetOutput();
}
