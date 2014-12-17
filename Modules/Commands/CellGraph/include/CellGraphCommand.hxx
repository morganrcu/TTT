/*
 * DualGraphCommand.cpp
 *
 *  Created on: Oct 2, 2013
 *      Author: morgan
 */

#include "CellGraphCommand.h"

template<int dims> ttt::CellGraphCommand<dims>::CellGraphCommand() {
	m_Dual =DualFilterType::New();

}

template<int dims> ttt::CellGraphCommand<dims>::~CellGraphCommand() {

}

template<int dims> void ttt::CellGraphCommand<dims>::Do(){

	m_Dual->SetInput(m_Graphs);
	m_Dual->GenerateData();

	this->m_Graphs=m_Dual->GetOutput();
}
