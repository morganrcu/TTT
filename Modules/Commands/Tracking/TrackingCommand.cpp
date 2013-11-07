/*
 * TrackingCommand.cpp
 *
 *  Created on: Oct 2, 2013
 *      Author: morgan
 */

#include <TrackingCommand.h>

TrackingCommand::TrackingCommand() {
	m_Tracker=giaa::CellTracker::New();
}

TrackingCommand::~TrackingCommand() {

}


void TrackingCommand::Do(){
	m_Tracker->SetObservations(m_Input);
	m_Tracker->Track();
	//m_Tracker->Update();
	m_Output=m_Tracker->GetTracks();
}
