/*
 * VinodthSegmentationCommand.h
 *
 *  Created on: Sep 24, 2013
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

#ifndef VINODTHSEGMENTATIONCOMMAND_H_
#define VINODTHSEGMENTATIONCOMMAND_H_

#include <AppCommand.h>
#include "giaaCellGraph2.h"
#include "itkPointSet.h"
#include "itkImage.h"
class VinodthSegmentationCommand: public AppCommand {

public:
	typedef itk::PointSet<float,3> PointSetType;
	typedef itk::Image<float,3> PlatenessImageType;
private:
	PointSetType::Pointer m_Locations;
	PlatenessImageType::Pointer m_Plateness;

	giaa::TissueDescriptor::Pointer m_Descriptor;
public:
	VinodthSegmentationCommand(){
		m_Descriptor=giaa::TissueDescriptor::New();
	}
	virtual ~VinodthSegmentationCommand(){

	}
	virtual void Do(){
		InitDefGraph();
		DoPrimalComputation();
	}

	inline void SetVertexLocations(const PointSetType::Pointer & locations){
		m_Locations=locations;
	}
	inline void SetPlatenessImage(const PlatenessImageType::Pointer & plateness){
		m_Plateness=plateness;
	}
	inline giaa::TissueDescriptor::Pointer GetTissueDescriptor(){
		return m_Descriptor;
	}
	void InitDefGraph();
	void DoPrimalComputation();
	bool IntersectLine(itk::Point<float ,3> l1posA, itk::Point<float, 3> l1posB, itk::Point<float, 3> l2posA, itk::Point<float, 3> l2posB );
	double angleBetween2Lines(itk::Point<double,3> srcpt, std::vector<itk::Point<double,3> > linepts);
	double distitkpt(itk::Point<float,3> pta, itk::Point<float,3> ptb);
};

#endif /* VINODTHSEGMENTATIONCOMMAND_H_ */
/** @}*/
