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
#include "AppCommand.h"
#include "tttAnisotropicDiffusionVesselEnhancementImageFilter.h"
namespace ttt{
/**
 * \class AnisotropicDiffusionCommand Anisotropic Diffusion
 * TODO
 */
class AnisotropicDiffusionCommand : public AppCommand{
public:
	/**
	 * TODO
	 */
	typedef itk::Image<float,3> DiffusedImageType;
	/**
	 * TODO
	 */
	typedef ttt::AnisotropicDiffusionPlateEnhancementImageFilter<DiffusedImageType, DiffusedImageType> DiffusionType;


private:
	/**
	 * TODO
	 */
	DiffusedImageType::Pointer m_Input;
	/**
		 * TODO
		 */
	DiffusedImageType::Pointer m_Output;
	/**
		 * TODO
		 */
	DiffusionType::Pointer m_Diffuser;
	/**
		 * TODO
		 */
	double m_DiffusionSigmaMin;
	/**
		 * TODO
		 */
	double m_DiffusionSigmaMax;
	/**
		 * TODO
		 */
	unsigned int m_DiffusionSigmaSteps;
	/**
		 * TODO
		 */
	unsigned int m_DiffusionIterations;
public:
	/**
		 * TODO
		 */
	AnisotropicDiffusionCommand(){
		m_DiffusionIterations=0;
		m_DiffusionSigmaSteps=0;
		m_DiffusionSigmaMax=-1;
		m_DiffusionSigmaMin=-1;
		m_Diffuser=DiffusionType::New();
	}
	/**
	 * TODO
	 * @param input
	 */
	inline void SetInputImage(const DiffusedImageType::Pointer & input){
		m_Input=input;
	}
	/**
	 * TODO
	 * @param sigmaMin
	 */
	inline void SetDiffusionSigmaMin(float sigmaMin){
		m_DiffusionSigmaMin=sigmaMin;
	}
	/**
	 * TODO
	 * @param sigmaMax
	 */
	inline void SetDiffusionSigmaMax(float sigmaMax){
		m_DiffusionSigmaMax=sigmaMax;
	}
	/**
	 * TODO
	 * @param sigmaSteps
	 */
	inline void SetDiffusionSigmaSteps(unsigned int sigmaSteps){
		m_DiffusionSigmaSteps=sigmaSteps;
	}
	/**
	 * TODO
	 * @param iterations
	 */
	inline void SetDiffusionIterations(unsigned int iterations){
		m_DiffusionIterations=iterations;
	}
	/**
	 * TODO
	 * @return
	 */
	inline DiffusedImageType::Pointer GetOutputImage(){
		return m_Output;
	}
	/**
	 * TODO
	 */
	virtual void Do();
	/**
	 * TODO
	 */
	virtual ~AnisotropicDiffusionCommand(){

	}
};
}

#endif
/** @}*/
