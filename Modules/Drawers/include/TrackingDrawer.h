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
#ifndef TRACKINGDRAWER_H_
#define TRACKINGDRAWER_H_
#include <map>
#include "tttDescriptionDataTypes.h"
#include "Drawer.h"


namespace ttt{
class TrackingDrawer: public Drawer {
public:
	TrackingDrawer();
	virtual ~TrackingDrawer();

	inline void SetTrackedDescriptor(const TrackedTissueDescriptor::Pointer & descriptor){
		m_Descriptor=descriptor;
	}
	virtual void Draw();

	static TrackingDrawer * New(){
		return new TrackingDrawer;
	}
private:
	typedef std::vector<double> Color ;

	std::map<int,Color> m_TrackID2Color;
	TrackedTissueDescriptor::Pointer m_Descriptor;

};
}
#endif /* TRACKINGDRAWER_H_ */
/** @}*/
