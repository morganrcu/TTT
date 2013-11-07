/*
 * VertexLocationCommand.h
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

#ifndef VERTEXLOCATIONCOMMAND_H_
#define VERTEXLOCATIONCOMMAND_H_

#include <AppCommand.h>
#include <itkImage.h>
#include <itkPointSet.h>
#include "itkLocalMaximumImageFilter.h"

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

#endif /* VERTEXLOCATIONCOMMAND_H_ */
/** @}*/
