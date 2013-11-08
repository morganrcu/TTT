/*
 * DualGraphCommand.cpp
 *
 *  Created on: Oct 2, 2013
 *      Author: morgan
 */

#include "CellGraphCommand.h"

ttt::CellGraphCommand::CellGraphCommand() {
	m_Dual =DualFilterType::New();

}

ttt::CellGraphCommand::~CellGraphCommand() {

}

void ttt::CellGraphCommand::Do(){

	m_Dual->SetInput(m_Graphs);
	m_Dual->GenerateData();

	this->m_Graphs=m_Dual->GetOutput();
}
