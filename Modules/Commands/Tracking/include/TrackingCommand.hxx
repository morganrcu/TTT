/*
 * TrackingCommand.cpp
 *
 *  Created on: Oct 2, 2013
 *      Author: morgan
 */

#include <TrackingCommand.h>

template<int dim> ttt::TrackingCommand<dim>::TrackingCommand() {

}

template<int dim>  ttt::TrackingCommand<dim>::~TrackingCommand() {

}


template<int dim> void ttt::TrackingCommand<dim>::Do(){
	int numFrames=m_Input.size();

	m_Output.resize(numFrames);

	m_Initializer.SetTissueDescriptor(m_Input[0]);
	m_Initializer.Do();

	m_Output[0]=m_Initializer.GetTrackedTissueDescriptor();

	m_Associator.SetNextID(m_Initializer.GetNextID());
	for(int i=1;i<numFrames;i++){

		m_Associator.SetObservedTissueDescriptor(m_Input[i]);
		m_Associator.SetPreviousTrackedTissueDescriptor(m_Output[i-1]);
		m_Associator.Do();
		m_Output[i]=m_Associator.GetCurrentTrackedTissueDescriptor();
	}

}
