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

/** \addtogroup TTTCommand
 *  @{
 */

#ifndef VERTEXNESS_COMMAND_H_
#define VERTEXNESS_COMMAND_H_
#include <itkRescaleIntensityImageFilter.h>

#include "AppCommand.h"
#include "giaaVertexnessImageFilter.h"

class VertexnessCommand : public AppCommand{

public:
    /**
     * Type of the vertexness Image
     */
    typedef itk::Image<float,3> VertexnessImageType;

private:

	typedef giaa::VertexnessImageFilter<VertexnessImageType,VertexnessImageType> VertexnessFilterType;
    typedef itk::RescaleIntensityImageFilter<VertexnessImageType, VertexnessImageType> VertexnessRescalerType;

    VertexnessFilterType::Pointer m_Vertexness;
    VertexnessRescalerType::Pointer m_VertexnessRescaler;
	VertexnessImageType::Pointer m_Input;
	VertexnessImageType::Pointer m_VertexnessImage;


	float m_SigmaMax;
	float m_SigmaMin;
	unsigned int m_SigmaSteps;
public:
	VertexnessCommand(){
		m_Vertexness=VertexnessFilterType::New();
		m_VertexnessRescaler=VertexnessRescalerType::New();

		m_VertexnessRescaler->SetOutputMinimum(0);
		m_VertexnessRescaler->SetOutputMaximum(1);

		m_SigmaSteps=0;
		m_SigmaMax=0;
		m_SigmaMin=0;
	}

	virtual ~VertexnessCommand(){

	}
	inline void SetSigmaMin(float sigmaMin){
		m_SigmaMin=sigmaMin;
	}
	inline void SetSigmaMax(float sigmaMax){
		m_SigmaMax=sigmaMax;
	}
	inline void SetSigmaSteps(unsigned int sigmaSteps){
		m_SigmaSteps=sigmaSteps;
	}

	inline void SetInput(const VertexnessImageType::Pointer & input){
		m_Input= input;
	}

	inline VertexnessImageType::Pointer GetVertexnessImage(){
		return m_VertexnessImage;
	}
	void Do();
};



#endif
/** @}*/
