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

/** \addtogroup TTTCellGraph
 *  @{
 */

#ifndef CELLGRAPHCOMMAND_H_
#define CELLGRAPHCOMMAND_H_


#include "tttDescriptionDataTypes.h"
#include "tttPrimalGraphToDualGraphFilter.h"
#include "AppCommand.h"

namespace ttt{

class CellGraphCommand: public AppCommand {
private:
    typedef PrimalGraphToDualGraphFilter<TissueDescriptor> DualFilterType;
    DualFilterType::Pointer m_Dual;

   TissueDescriptor::Pointer m_Graphs;

public:
	CellGraphCommand();
	virtual ~CellGraphCommand();
	virtual void Do();

	inline void SetPrimalGraph(const TissueDescriptor::Pointer & graph){
		m_Graphs=graph;
	}
	inline TissueDescriptor::Pointer GetGraphs(){
		return m_Graphs;
	}
};

#endif /* DUALGRAPHCOMMAND_H_ */
}
/** @}*/
