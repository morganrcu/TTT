/** \addtogroup TTTDrawers
 *  @{
 */
#ifndef DIFFUSED_IMAGE_DRAWER_H
#define DIFFUSED_IMAGE_DRAWER_H


#include "ScalarVolumeDrawer.h"
#include "Drawer.h"
namespace ttt{
/**
 * \brief Draws a \link DiffusedImageDrawer::DiffusedImageType \endlink image into the renderer
 */
class DiffusedImageDrawer : public ScalarVolumeDrawer<itk::Image<float,3> > {

	typedef itk::Image<float,3> DiffusedImageType;

public:
	/**
	 * Does nothing
	 */
	DiffusedImageDrawer();


	/**
	 * Does nothing
	 */
	virtual ~DiffusedImageDrawer(){

	}
protected:
	virtual vtkSmartPointer<vtkVolumeProperty> GetVolumeProperty();
};
}
#endif
/** @}*/
