/*
 * MinCostMaxFlowTrackAssociationCommand.h
 *
 *  Created on: May 20, 2014
 *      Author: morgan
 */

#ifndef MINCOSTMAXFLOWTRACKASSOCIATIONCOMMAND_H_
#define MINCOSTMAXFLOWTRACKASSOCIATIONCOMMAND_H_

#include "AppCommand.h"
#include "tttDescriptionDataTypes.h"

namespace ttt {

class MinCostMaxFlowTrackAssociationCommand : AppCommand {

private:
	typename ttt::TissueDescriptor::Pointer m_ObservedTissueDescriptor;

	typename ttt::TrackedTissueDescriptor::Pointer m_PreviousTrackedTissueDescriptor;
	typename ttt::TrackedTissueDescriptor::Pointer m_CurrentTrackedTissueDescriptor;

	double m_TerminationWeight=1;
	double m_CreationWeight=0.5;
	double m_AssociationWeight=0.1;
	double m_ApoptosisWeight=1;
	double m_MitosisWeight=1;

	double m_DistanceWeight=1;
	double m_AreaWeight=0.01;
	double m_PerimeterWeight=0;
	double m_XXWeight=0;
	double m_XYWeight=0;
	double m_YYWeight=0;


	int m_K;
	unsigned int m_NextID;

public:

	MinCostMaxFlowTrackAssociationCommand();
	virtual ~MinCostMaxFlowTrackAssociationCommand();

	inline void SetNextID(unsigned int nextID){
		m_NextID=nextID;
	}
	inline void SetPreviousTrackedTissueDescriptor(const typename ttt::TrackedTissueDescriptor::Pointer & previousTrackedTissueDescriptor){
		m_PreviousTrackedTissueDescriptor=previousTrackedTissueDescriptor;
	}

	inline void SetObservedTissueDescriptor(const typename ttt::TissueDescriptor::Pointer & observedTissueDescriptor ){
		m_ObservedTissueDescriptor=observedTissueDescriptor;
	}

	inline typename ttt::TrackedTissueDescriptor::Pointer GetCurrentTrackedTissueDescriptor(){
		return m_CurrentTrackedTissueDescriptor;
	}

	virtual void Do();

};

} /* namespace ttt */

#endif /* MINCOSTMAXFLOWTRACKASSOCIATIONCOMMAND_H_ */
