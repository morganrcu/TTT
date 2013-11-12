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

/** \addtogroup TTTVertexness
 *  @{
 */

#ifndef VERTEXNESS_COMMAND_H_
#define VERTEXNESS_COMMAND_H_
#include <itkRescaleIntensityImageFilter.h>

#include "tttMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "tttVertexnessMeasurementFunction.h"

#include "AppCommand.h"

namespace ttt{
/**
 * \class VertexnessCommand
 * TODO
 */
class VertexnessCommand : public AppCommand{

public:
    /**
     * Type of the vertexness Image
     */
    typedef itk::Image<float,3> VertexnessImageType;

private:
    /**
     *TODO
     */
    typedef MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter<VertexnessMeasurementFunction,VertexnessImageType> VertexnessFilterType;
    /**
     *TODO
     */
    typedef itk::RescaleIntensityImageFilter<VertexnessImageType, VertexnessImageType> VertexnessRescalerType;

    VertexnessFilterType::Pointer m_Vertexness;
    /**
     *TODO
     */
    VertexnessRescalerType::Pointer m_VertexnessRescaler;
    /**
     *TODO
     */
	VertexnessImageType::Pointer m_Input;
	/**
	 *TODO
	 */
	VertexnessImageType::Pointer m_VertexnessImage;

	/**
	 *TODO
	 */
	float m_SigmaMax;
	/**
	 *TODO
	 */
	float m_SigmaMin;
	/**
	 *TODO
	 */
	unsigned int m_SigmaSteps;
public:
	/**
	 * TODO
	 */
	VertexnessCommand(){
		m_Vertexness=VertexnessFilterType::New();
		m_VertexnessRescaler=VertexnessRescalerType::New();

		m_VertexnessRescaler->SetOutputMinimum(0);
		m_VertexnessRescaler->SetOutputMaximum(1);

		m_SigmaSteps=0;
		m_SigmaMax=0;
		m_SigmaMin=0;
	}

	/**
	 *TODO
	 */
	virtual ~VertexnessCommand(){

	}
	/**
	 *TODO
	 * @param sigmaMin
	 */
	inline void SetSigmaMin(float sigmaMin){
		m_SigmaMin=sigmaMin;
	}
	/**
	 *TODO
	 * @param sigmaMax
	 */
	inline void SetSigmaMax(float sigmaMax){
		m_SigmaMax=sigmaMax;
	}
	/**
	 *TODO
	 * @param sigmaSteps
	 */
	inline void SetSigmaSteps(unsigned int sigmaSteps){
		m_SigmaSteps=sigmaSteps;
	}

	/**
	 *TODO
	 * @param input
	 */
	inline void SetInput(const VertexnessImageType::Pointer & input){
		m_Input= input;
	}

	/**
	 * TODO
	 * @return
	 */
	inline VertexnessImageType::Pointer GetVertexnessImage(){
		return m_VertexnessImage;
	}
	/**
	 * TODO
	 */
	void Do();
};
}

#endif
/** @}*/
