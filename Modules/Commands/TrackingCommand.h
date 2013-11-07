/*
 * TrackingCommand.h
 *
 *  Created on: Oct 2, 2013
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

#ifndef TRACKINGCOMMAND_H_
#define TRACKINGCOMMAND_H_
#include <vector>
#include "giaaCellGraph2.h"
#include "giaaCellTracker.h"
#include "AppCommand.h"

class TrackingCommand: public AppCommand {
private:
	giaa::CellTracker::Pointer m_Tracker;

	std::vector<giaa::TissueDescriptor::Pointer> m_Input;

    std::vector<giaa::TrackedTissueDescriptor::Pointer>  m_Output;

public:
	TrackingCommand();
	virtual ~TrackingCommand();

	inline void SetObservedTissues(const std::vector<giaa::TissueDescriptor::Pointer> & input){
		m_Input=input;
	}
	inline std::vector<giaa::TrackedTissueDescriptor::Pointer> GetTrackedTissue(){
		return m_Output;
	}

	virtual void Do();
};

#endif /* TRACKINGCOMMAND_H_ */
/** @}*/
