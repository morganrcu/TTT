/** \addtogroup TTTVertexLocation
 *  @{
 */

#ifndef VERTEXLOCATIONCOMMAND_H_
#define VERTEXLOCATIONCOMMAND_H_

#include <AppCommand.h>
#include <itkImage.h>
#include <itkPointSet.h>
#include "itkLocalMaximumImageFilter.h"
#include "tttDescriptionDataTypes.h"
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


private:
    /**
     * TODO
     */
    typedef itk::LocalMaximumImageFilter<VertexnessImageType> LocalMaxFilterType;
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
    ttt::AdherensJunctionVertices::Pointer m_VertexLocations;
    /**
     * TODO
     */
    float m_LocalMaxThreshold;

    float m_Radius;

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
	virtual ttt::AdherensJunctionVertices::Pointer GetLocalMaxima(){
		return m_VertexLocations;
	}
	/**
	 * TODO
	 */
	virtual ~VertexLocationCommand(){

	}

	virtual void SetRadius(float radius){
		m_Radius=radius;
	}
	/**
	 * TODO
	 */
	virtual void Do();
};
}
#endif /* VERTEXLOCATIONCOMMAND_H_ */
/** @}*/
