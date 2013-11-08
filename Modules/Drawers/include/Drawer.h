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

/** \addtogroup TTTDrawers
 *  @{
 */
#ifndef DRAWER_H
#define DRAWER_H

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkObjectBase.h>
/**
 * Abstract class to encapsulate image drawing routines.
 */
namespace ttt{
class Drawer : public vtkObjectBase{

protected:
	/**
	 * vtkRenderer object where the different drawers paint.
	 */
	vtkSmartPointer<vtkRenderer> m_Renderer;
	Drawer();
public:

	/**
	 * Draw something in the encapsulated renderer set up by the SetRenderer method
	 */
	virtual void Draw() = 0;

	/**
	 * Set the renderer where Drawer instantiations should draw their result.
	 */
	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}
};
}
#endif
/** @}*/
