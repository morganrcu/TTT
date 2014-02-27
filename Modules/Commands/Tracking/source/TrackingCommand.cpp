/*
 * TrackingCommand.cpp
 *
 *  Created on: Oct 2, 2013
 *      Author: morgan
 */

#include <TrackingCommand.h>

ttt::TrackingCommand::TrackingCommand() {
	m_Tracker= ttt::MinCostMaxFlowCellTracker::New();
}

ttt::TrackingCommand::~TrackingCommand() {

}


void ttt::TrackingCommand::Do(){
	m_Tracker->SetObservations(m_Input);
	m_Tracker->Track();
	//m_Tracker->Update();
	m_Output=m_Tracker->GetTracks();
}
