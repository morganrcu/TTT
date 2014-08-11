/** \addtogroup TTTVertexLocation
 *  @{
 */

#ifndef PYRAMIDVERTEXLOCATIONCOMMAND_H_
#define PYRAMIDVERTEXLOCATIONCOMMAND_H_

#include <AppCommand.h>
#include <itkImage.h>
#include <itkPointSet.h>
#include "itkLocalMaximumImageFilter.h"
#include "tttDescriptionDataTypes.h"
namespace ttt{
/**
 * \class PyramidVertexLocationCommand
 * TODO
 */
class PyramidVertexLocationCommand: public AppCommand {

public:
	/**
	 * TODO
	 */
	typedef itk::Image<float,3> SignalImageType;


private:

    /**
     * TODO
     */
    std::vector<SignalImageType::Pointer> m_Inputs;
    /**
     * TODO
     */
    std::vector<ttt::AdherensJunctionVertices::Pointer> m_PyramidVertexLocations;
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
    PyramidVertexLocationCommand(){

		m_LocalMaxThreshold=-1;
	}
	/**
	 * TODO
	 * @param input
	 */
	virtual void SetInputPyramid(const std::vector<SignalImageType::Pointer> & input){
		m_Inputs=input;
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
	virtual ~PyramidVertexLocationCommand(){

	}


	/**
	 * TODO
	 */
	virtual void Do();
};
}
#endif /* VERTEXLOCATIONCOMMAND_H_ */
/** @}*/
