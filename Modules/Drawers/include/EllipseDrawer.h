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

#ifndef ELLIPSEDRAWER_H_
#define ELLIPSEDRAWER_H_

#include <Drawer.h>
#include "Ellipse.h"
#include <boost/shared_ptr.hpp>
#include <map>
namespace ttt{
class EllipseDrawer : public Drawer{

private:

	typedef Ellipse<double> EllipseType;
    typedef std::map<unsigned long int,EllipseType >   EllipseMapType;
	typedef boost::shared_ptr<EllipseMapType > EllipseMapTypePointer;
	EllipseMapTypePointer m_Ellipses;

	std::vector<vtkSmartPointer<vtkActor> > m_Actors;

public:

	virtual void Draw();

	inline void SetEllipses(const EllipseMapTypePointer & ellipses){
		m_Ellipses=ellipses;
	}
	virtual void Show();
	virtual void Hide();
	virtual void Reset();
};
}

#endif /* ELLIPSEDRAWER_H_ */
/** @}*/
