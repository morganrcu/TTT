/*
 * CellTrackId.h
 *
 *  Created on: Dec 12, 2013
 *      Author: morgan
 */

#ifndef CELLTRACKID_H_
#define CELLTRACKID_H_

#include <tttDescriptionDataTypes.h>
class TrackedCellId : public TissueDescriptorFeature<ttt::TrackedTissueDescriptor,ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor>::CellVertexType, double>  {

private:
	typename ttt::TrackedTissueDescriptor::Pointer m_TissueDescriptor;

public:
	void Compute(){
		BGL_FORALL_VERTICES(v,*m_TissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){
			this->at(v)=boost::get(ttt::TrackedCellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).m_ID;
		}
	}
};



#endif /* CELLTRACKID_H_ */
