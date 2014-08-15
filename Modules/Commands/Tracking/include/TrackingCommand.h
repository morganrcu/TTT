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

#include "tttDescriptionDataTypes.h"
#include "AppCommand.h"
#include "TrackInitializationCommand.h"
#include "MinCostMaxFlowTrackAssociationCommand.h"
namespace ttt{
class TrackingCommand: public AppCommand {
private:

	ttt::TrackInitializationCommand  m_Initializer;
	ttt::MinCostMaxFlowTrackAssociationCommand m_Associator;

	std::vector<TissueDescriptor::Pointer> m_Input;

    std::vector<TrackedTissueDescriptor::Pointer>  m_Output;

public:
	TrackingCommand();

#if 0
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

    inline void SetAspectRatioWeight(double weight){
    	m_Tracker->SetAspectRatioWeight(weight);
    }
    inline void SetOrientationWeight(double weight){
    	m_Tracker->SetOrientationWeight(weight);
    }

    inline void SetEllipseWeight(double weight){
    	m_Tracker->SetEllipseWeight(weight);
    }

    inline void SetDistanceWeight(double weight){
    	m_Tracker->SetDistanceWeight(weight);
    }
#endif
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
