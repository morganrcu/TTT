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

/** \addtogroup TTTTracking
 *  @{
 */

#ifndef TRACKINGCOMMAND_H_
#define TRACKINGCOMMAND_H_
#include <vector>
//#include "giaaCellTracker.h"
#include "tttMinCostMaxFlowCellTracker.h"
#include "tttDescriptionDataTypes.h"
#include "AppCommand.h"
namespace ttt{
class TrackingCommand: public AppCommand {
private:
	ttt::MinCostMaxFlowCellTracker::Pointer m_Tracker;

	std::vector<TissueDescriptor::Pointer> m_Input;

    std::vector<TrackedTissueDescriptor::Pointer>  m_Output;

public:
	TrackingCommand();
#if 0
    inline void SetXMin(double xMin){
    	m_Tracker->SetXMin(xMin);
    }
    inline void SetXMax(double xMax){
    	m_Tracker->SetXMax(xMax);
    }
    inline void SetYMin(double yMin){
    	m_Tracker->SetYMin(yMin);
    }
    inline void SetYMax(double yMax){
    	m_Tracker->SetYMax(yMax);
    }

    inline void SetZMin(double zMin){
    	m_Tracker->SetZMin(zMin);

    }
    inline void SetZMax(double zMax){
    	m_Tracker->SetZMax(zMax);
    }
#endif

    inline void SetMitosisWeight(double weight){
    	m_Tracker->SetMitosisWeight(weight);
    }
    inline void SetAssociationWeight(double weight){
    	m_Tracker->SetAssociationWeight(weight);
    }
    inline void SetCreationWeight(double weight){
    	m_Tracker->SetCreationWeight(weight);
    }
    inline void SetTerminationWeight(double weight){
    	m_Tracker->SetTerminationWeight(weight);
    }

    inline void SetAreaWeight(double weight){
    	m_Tracker->SetAreaWeight(weight);
    }
    inline void SetPerimeterWeight(double perimeter){
    	m_Tracker->SetPerimeterWeight(perimeter);
    }
#if 0
    inline void SetAspectRatioWeight(double weight){
    	m_Tracker->SetAspectRatioWeight(weight);
    }
    inline void SetOrientationWeight(double weight){
    	m_Tracker->SetOrientationWeight(weight);
    }
#endif
    inline void SetEllipseWeight(double weight){
    	m_Tracker->SetEllipseWeight(weight);
    }

    inline void SetDistanceWeight(double weight){
    	m_Tracker->SetDistanceWeight(weight);
    }

	virtual ~TrackingCommand();

	inline void SetObservedTissues(const std::vector<TissueDescriptor::Pointer> & input){
		m_Input=input;
	}
	inline std::vector<TrackedTissueDescriptor::Pointer> GetTrackedTissue(){
		return m_Output;
	}

	virtual void Do();
};
}
#endif /* TRACKINGCOMMAND_H_ */
/** @}*/
