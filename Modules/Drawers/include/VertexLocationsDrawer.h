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
#ifndef VERTEX_LOCATIONS_DRAWER_H
#define VERTEX_LOCATIONS_DRAWER_H


#include <itkPointSet.h>
#include "Drawer.h"
namespace ttt{
//template<class OriginalImageType> class OriginalImageDrawer : public Drawer {
class VertexLocationsDrawer : public Drawer {

private:
    typedef itk::PointSet<float, 3> PointSetType;
    typedef itk::FixedArray<float,3> SpacingType;

    PointSetType::Pointer m_VertexLocations;
    SpacingType m_Spacing;
public:
    VertexLocationsDrawer(){

    }
    inline void SetVertexLocations(const PointSetType::Pointer & vertexLocations){
    	m_VertexLocations=vertexLocations;
    }
    inline void SetSpacing(const SpacingType & spacing){
    	m_Spacing=spacing;
    }
	virtual void Draw();
	virtual ~VertexLocationsDrawer(){

	}
};
}
#endif
/** @}*/
