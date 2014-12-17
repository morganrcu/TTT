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
template<int dim> class TrackInitializationCommand : public ttt::AppCommand{

private:

	typename ttt::TissueDescriptor<dim>::Pointer m_TissueDescriptor;
	typename ttt::TrackedTissueDescriptor<dim>::Pointer m_TrackedTissueDescriptor;
	unsigned int m_NextID;
public:
	TrackInitializationCommand(){
		m_NextID=0;
	}
	inline void SetTissueDescriptor(const typename ttt::TissueDescriptor<dim>::Pointer & tissueDescriptor){

		m_TissueDescriptor=tissueDescriptor;
	}

	inline typename ttt::TrackedTissueDescriptor<dim>::Pointer GetTrackedTissueDescriptor(){
		return m_TrackedTissueDescriptor;
	}

	inline unsigned int GetNextID(){
		return m_NextID;
	}

	virtual void Do();

};
}
#include "TrackInitializationCommand.hxx"
#endif /* TRACKINITIALIZATIONCOMMAND_H_ */
