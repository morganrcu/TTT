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

	double m_TerminationWeight;
	double m_DistanceWeight;
	double m_CreationWeight;
	double m_AssociationWeight;
	double m_AreaWeight;
	double m_EllipseWeight;
	double m_PerimeterWeight;
	double m_MitosisWeight;
	int m_K;
	int m_NextID;
public:

	MinCostMaxFlowTrackAssociationCommand();
	virtual ~MinCostMaxFlowTrackAssociationCommand();

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
