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
/**
 * \class VertexLocationCommand
 * TODO
 */
class VertexLocationCommand: public AppCommand {

public:
	/**
	 * TODO
	 */
	typedef itk::Image<float,3> VertexnessImageType;
	/**
	 * TODO
	 */
    typedef itk::PointSet<VertexnessImageType::PixelType, 3> PointsetType;

private:
    /**
     * TODO
     */
    typedef itk::LocalMaximumImageFilter<VertexnessImageType, PointsetType> LocalMaxFilterType;
    /**
     * TODO
     */
    LocalMaxFilterType::Pointer m_VertexLocator;
    /**
     * TODO
     */
    VertexnessImageType::Pointer m_Input;
    /**
     * TODO
     */
    PointsetType::Pointer m_VertexLocations;
    /**
     * TODO
     */
    float m_LocalMaxThreshold;

public:
    /**
     * TODO
     */
	VertexLocationCommand(){
		m_VertexLocator = LocalMaxFilterType::New();
		m_Input=0;
		m_LocalMaxThreshold=-1;
	}
	/**
	 * TODO
	 * @param input
	 */
	virtual void SetVertexnessImage(const VertexnessImageType::Pointer & input){
		m_Input=input;
	}
	/**
	 * TODO
	 * @param threshold
	 */
	virtual void SetLocalMaxThreshold(float threshold){
		m_LocalMaxThreshold=threshold;
	}
	/**
	 * TODO
	 * @return
	 */
	virtual PointsetType::Pointer GetLocalMaxima(){
		return m_VertexLocations;
	}
	/**
	 * TODO
	 */
	virtual ~VertexLocationCommand(){

	}
	/**
	 * TODO
	 */
	virtual void Do();
};
}
#endif /* VERTEXLOCATIONCOMMAND_H_ */
/** @}*/
