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
#ifndef MOTIONVECTORDRAWER_H_
#define MOTIONVECTORDRAWER_H_

#include "tttDescriptionDataTypes.h"
#include "Drawer.h"
namespace ttt{
class MotionVectorDrawer : public Drawer {

private:

	TrackedTissueDescriptor<3>::Pointer m_Descriptor;
	std::vector<vtkSmartPointer<vtkActor> > m_Actors;
public:
	MotionVectorDrawer();
	virtual void Draw();

	inline void SetTrackedDescriptor(const TrackedTissueDescriptor<3>::Pointer & descriptor){
		m_Descriptor=descriptor;
	}
	virtual void Show();
	virtual void Hide();
	virtual void Reset();

};
}

#endif /* MOTIONVECTORDRAWER_H_ */
/** @}*/
