/*
 * DualGraphCommand.h
 *
 *  Created on: Oct 2, 2013
 *      Author: morgan
 */
//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

/** \addtogroup TTTCommand
 *  @{
 */

#ifndef DUALGRAPHCOMMAND_H_
#define DUALGRAPHCOMMAND_H_


#include <giaaCellGraph2.h>
#include <giaaPrimalGraphToDualGraphFilter.h>
#include "AppCommand.h"


class DualGraphCommand: public AppCommand {
private:
    typedef giaa::PrimalGraphToDualGraphFilter<giaa::TissueDescriptor> DualFilterType;
    DualFilterType::Pointer m_Dual;

    giaa::TissueDescriptor::Pointer m_Graphs;

public:
	DualGraphCommand();
	virtual ~DualGraphCommand();
	virtual void Do();

	inline void SetPrimalGraph(const giaa::TissueDescriptor::Pointer & graph){
		m_Graphs=graph;
	}
	inline giaa::TissueDescriptor::Pointer GetGraphs(){
		return m_Graphs;
	}
};

#endif /* DUALGRAPHCOMMAND_H_ */
/** @}*/
