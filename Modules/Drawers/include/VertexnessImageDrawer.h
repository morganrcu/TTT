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
#ifndef VERTEXNESS_IMAGE_DRAWER_H
#define VERTEXNESS_IMAGE_DRAWER_H

#include <itkImage.h>
#include "Drawer.h"

//template<class OriginalImageType> class OriginalImageDrawer : public Drawer {
class VertexnessImageDrawer : public Drawer {

private:
	typedef itk::Image<float,3> VertexnessImageType;
	typename VertexnessImageType::Pointer m_VertexnessImage;
public:
	//VertexnessImageDrawer(vtkSmartPointer<vtkRenderer> & renderer,);
	VertexnessImageDrawer();
	inline void SetVertexnessImage(const typename VertexnessImageType::Pointer & vertexnessImage){
		m_VertexnessImage=vertexnessImage;
	}
	virtual void Draw();
	virtual ~VertexnessImageDrawer(){

	}
};
#endif
/** @}*/
