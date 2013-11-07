/*
 * DualGraphCommand.cpp
 *
 *  Created on: Oct 2, 2013
 *      Author: morgan
 */

#include "DualGraphCommand.h"

DualGraphCommand::DualGraphCommand() {
	m_Dual =DualFilterType::New();

}

DualGraphCommand::~DualGraphCommand() {

}

void DualGraphCommand::Do(){

	m_Dual->SetInput(m_Graphs);
	m_Dual->GenerateData();

	this->m_Graphs=m_Dual->GetOutput();
}
