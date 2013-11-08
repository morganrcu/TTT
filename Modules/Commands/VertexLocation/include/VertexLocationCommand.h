/** \addtogroup TTTVertexLocation
 *  @{
 */

#ifndef VERTEXLOCATIONCOMMAND_H_
#define VERTEXLOCATIONCOMMAND_H_

#include <AppCommand.h>
#include <itkImage.h>
#include <itkPointSet.h>
#include "itkLocalMaximumImageFilter.h"
namespace ttt{
class VertexLocationCommand: public AppCommand {

public:
	typedef itk::Image<float,3> VertexnessImageType;
    typedef itk::PointSet<VertexnessImageType::PixelType, 3> PointsetType;

private:

    typedef itk::LocalMaximumImageFilter<VertexnessImageType, PointsetType> LocalMaxFilterType;
    LocalMaxFilterType::Pointer m_VertexLocator;

    VertexnessImageType::Pointer m_Input;
    PointsetType::Pointer m_VertexLocations;
    float m_LocalMaxThreshold;

public:
	VertexLocationCommand(){
		m_VertexLocator = LocalMaxFilterType::New();
		m_Input=0;
		m_LocalMaxThreshold=-1;
	}
	virtual void SetVertexnessImage(const VertexnessImageType::Pointer & input){
		m_Input=input;
	}
	virtual void SetLocalMaxThreshold(float threshold){
		m_LocalMaxThreshold=threshold;
	}
	virtual PointsetType::Pointer GetLocalMaxima(){
		return m_VertexLocations;
	}
	virtual ~VertexLocationCommand(){

	}
	virtual void Do();
};
}
#endif /* VERTEXLOCATIONCOMMAND_H_ */
/** @}*/
