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

/** \addtogroup TTTDrawer
 *  @{
 */
#ifndef DUAL_GRAPH_DRAWER_H_
#define DUAL_GRAPH_DRAWER_H_
#include "giaaCellGraph2.h"
#include "Drawer.h"

class DualGraphDrawer: public Drawer {
public:
	typedef itk::FixedArray<float,3> SpacingType;
private:
	giaa::TissueDescriptor::Pointer m_Descriptor;
	SpacingType m_Spacing;

public:
	static DualGraphDrawer* New(){
		return new DualGraphDrawer;
	}
	DualGraphDrawer(){

	}

	inline void SetTissueDescriptor(const giaa::TissueDescriptor::Pointer & descriptor){
		m_Descriptor=descriptor;
	}
	inline void SetSpacing(const SpacingType & spacing ){
		m_Spacing=spacing;
	}
	virtual ~DualGraphDrawer(){

	}
	virtual void Draw();
};



#endif /* DUALGRAPHDRAWER_H_ */
/** @}*/
