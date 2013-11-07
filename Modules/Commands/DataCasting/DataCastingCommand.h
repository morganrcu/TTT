/*
 * DataCastingCommand.h
 *
 *  Created on: Sep 23, 2013
 *      Author: morgan
 */
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

#ifndef DATACASTINGCOMMAND_H_
#define DATACASTINGCOMMAND_H_

#include <itkRescaleIntensityImageFilter.h>
#include <itkCastImageFilter.h>
#include "AppCommand.h"


class DataCastingCommand : public AppCommand{
public:

	typedef itk::Image<unsigned char,3> InputImageType;
	typedef itk::Image<float,3> OutputImageType;
private:

	typedef itk::RescaleIntensityImageFilter<OutputImageType> Rescaler01Type;
	typedef itk::RescaleIntensityImageFilter<InputImageType,OutputImageType> CasterType;

	Rescaler01Type::Pointer m_Rescaler;
	CasterType::Pointer m_Caster;

	InputImageType::Pointer m_Input;
	OutputImageType::Pointer m_Output;

public:
	DataCastingCommand(){
		m_Rescaler=Rescaler01Type::New();
		m_Rescaler->SetOutputMaximum(1);
		m_Rescaler->SetOutputMinimum(0);
		m_Caster=CasterType::New();
		m_Input=0;
		m_Output=0;
	}
	virtual ~DataCastingCommand(){

	}
	virtual void Do();

	inline void SetInput(const InputImageType::Pointer & input){
		m_Input=input;
	}
	inline OutputImageType::Pointer GetOutput(){
		return m_Output;
	}
};

#endif /* DATACASTINGCOMMAND_H_ */
/** @}*/
