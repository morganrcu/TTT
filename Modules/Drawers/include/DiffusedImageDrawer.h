/** \addtogroup TTTDrawers
 *  @{
 */
#ifndef DIFFUSED_IMAGE_DRAWER_H
#define DIFFUSED_IMAGE_DRAWER_H

#include <itkImage.h>
#include "Drawer.h"
namespace ttt{
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
}
#endif
/** @}*/
