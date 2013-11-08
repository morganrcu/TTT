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

/** \addtogroup TTTAnisotropicDiffusion
 *  @{
 */

#ifndef ANISOTROPICDIFFUSIONCOMMAND_H_
#define ANISOTROPICDIFFUSIONCOMMAND_H_
#include <itkImage.h>
#include <itkImage.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkAndImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include  "AppCommand.h"
#include "itkAnisotropicDiffusionVesselEnhancementImageFilter.h"
namespace ttt{
/**
 * Anisotropic Diffusion
 */
class AnisotropicDiffusionCommand : public AppCommand{
public:
	typedef itk::Image<float,3> DiffusedImageType;
	typedef itk::AnisotropicDiffusionPlateEnhancementImageFilter<DiffusedImageType, DiffusedImageType> DiffusionType;


private:
	DiffusedImageType::Pointer m_Input;
	DiffusedImageType::Pointer m_Output;

	DiffusionType::Pointer m_Diffuser;

	double m_DiffusionSigmaMin;
	double m_DiffusionSigmaMax;
	unsigned int m_DiffusionSigmaSteps;
	unsigned int m_DiffusionIterations;
public:
	AnisotropicDiffusionCommand(){
		m_DiffusionIterations=0;
		m_DiffusionSigmaSteps=0;
		m_DiffusionSigmaMax=-1;
		m_DiffusionSigmaMin=-1;
		m_Diffuser=DiffusionType::New();
	}

	inline void SetInputImage(const DiffusedImageType::Pointer & input){
		m_Input=input;
	}
	inline void SetDiffusionSigmaMin(float sigmaMin){
		m_DiffusionSigmaMin=sigmaMin;
	}
	inline void SetDiffusionSigmaMax(float sigmaMax){
		m_DiffusionSigmaMax=sigmaMax;
	}
	inline void SetDiffusionSigmaSteps(unsigned int sigmaSteps){
		m_DiffusionSigmaSteps=sigmaSteps;
	}
	inline void SetDiffusionIterations(unsigned int iterations){
		m_DiffusionIterations=iterations;
	}
	inline DiffusedImageType::Pointer GetOutputImage(){
		return m_Output;
	}
	virtual void Do();
	virtual ~AnisotropicDiffusionCommand(){

	}
};
}

#endif
/** @}*/
