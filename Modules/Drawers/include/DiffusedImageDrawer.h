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
#ifndef DIFFUSED_IMAGE_DRAWER_H
#define DIFFUSED_IMAGE_DRAWER_H

#include <itkImage.h>
#include "Drawer.h"
/**
 * \brief Draws a \link DiffusedImageDrawer::DiffusedImageType \endlink image into the renderer
 */
class DiffusedImageDrawer : public Drawer {

private:
	typedef itk::Image<float,3> DiffusedImageType;

	typename DiffusedImageType::Pointer m_DiffusedImage;
public:
	/**
	 * Does nothing
	 */
	DiffusedImageDrawer();
	/**
	 * Sets the pointer to the diffusedImage to be drawn.
	 * You need to call this method before \link DiffusedImageDrawer::Draw() \endlink
	 * @param diffusedImage The volume to draw
	 */
	inline void SetDiffusedImage(const typename DiffusedImageType::Pointer & diffusedImage){
		m_DiffusedImage=diffusedImage;
	}
	/**
	 * Draws the volume set up by \link DiffusedImageDrawer::SetDiffusedImage() \endlink
	 */
	void Draw();
	/**
	 * Does nothing
	 */
	virtual ~DiffusedImageDrawer(){

	}
};
#endif
/** @}*/
