/*
 * TrackInitializationCommand.h
 *
 *  Created on: May 20, 2014
 *      Author: morgan
 */

#ifndef TRACKINITIALIZATIONCOMMAND_H_
#define TRACKINITIALIZATIONCOMMAND_H_

#include "AppCommand.h"
#include "tttDescriptionDataTypes.h"
namespace ttt{
class TrackInitializationCommand : public ttt::AppCommand{

private:

	ttt::TissueDescriptor::Pointer m_TissueDescriptor;
	ttt::TrackedTissueDescriptor::Pointer m_TrackedTissueDescriptor;
	unsigned int m_NextID;
public:
	TrackInitializationCommand(){
		m_NextID=0;
	}
	inline void SetTissueDescriptor(const ttt::TissueDescriptor::Pointer & tissueDescriptor){

		m_TissueDescriptor=tissueDescriptor;
	}

	inline ttt::TrackedTissueDescriptor::Pointer GetTrackedTissueDescriptor(){
		return m_TrackedTissueDescriptor;
	}

	virtual void Do();

};
}

#endif /* TRACKINITIALIZATIONCOMMAND_H_ */
