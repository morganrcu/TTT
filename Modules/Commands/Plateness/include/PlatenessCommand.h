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

/** \addtogroup TTTPlateness
 *  @{
 */

#ifndef PLATENESSCOMMAND_H_
#define PLATENESSCOMMAND_H_
#include <itkRescaleIntensityImageFilter.h>

#include "tttMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "tttPlatenessMeasurementFunction.h"

#include "AppCommand.h"

namespace ttt{
class PlatenessCommand: public AppCommand {

public:
    /**
     * Type of the plateness Image
     */
    typedef itk::Image<float,3> PlatenessImageType;
    /**
     * Type of the orientation Image
     */
    typedef itk::Image<itk::Vector<float,3>,3> OrientationImageType;

private:


    typedef MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter<PlatenessMeasurementFunction,PlatenessImageType> PlatenessFilterType;
    typedef itk::RescaleIntensityImageFilter<PlatenessImageType, PlatenessImageType> PlatenessRescalerType;



	PlatenessFilterType::Pointer m_Plateness;
	PlatenessRescalerType::Pointer m_PlatenessRescaler;
	PlatenessImageType::Pointer m_Input;
	PlatenessImageType::Pointer m_PlatenessImage;
	OrientationImageType::Pointer m_OrientationImage;

	float m_SigmaMax;
	float m_SigmaMin;
	unsigned int m_SigmaSteps;
public:
	PlatenessCommand(){
		m_Plateness=PlatenessFilterType::New();
		m_PlatenessRescaler=PlatenessRescalerType::New();
		m_PlatenessRescaler->SetOutputMinimum(0);
		m_PlatenessRescaler->SetOutputMaximum(1);
		m_Input=0;
		m_PlatenessImage=0;
		m_OrientationImage=0;
		m_SigmaMax=-1;
		m_SigmaMin=-1;
		m_SigmaSteps=0;
	}

	virtual ~PlatenessCommand(){


	}
	inline void SetInput(const PlatenessImageType::Pointer & input){
		m_Input= input;
	}
	inline void SetSigmaMin(float sigmaMin){
		m_SigmaMin=sigmaMin;
	}
	inline void SetSigmaMax(float sigmaMax){
		m_SigmaMax=sigmaMax;
	}
	inline void SetSigmaSteps(float sigmaSteps){
		m_SigmaSteps=sigmaSteps;
	}
	inline OrientationImageType::Pointer GetOrientationImage(){
		return m_OrientationImage;
	}
	inline PlatenessImageType::Pointer GetPlatenessImage(){
		return m_PlatenessImage;
	}
	void Do();
};
}
#endif /* PLATENESSCOMMAND_H_ */
/** @}*/
