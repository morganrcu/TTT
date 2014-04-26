/*
 * CellTrackId.h
 *
 *  Created on: Dec 12, 2013
 *      Author: morgan
 */

#ifndef CELLTRACKID_H_
#define CELLTRACKID_H_

#include <tttDescriptionDataTypes.h>
#include "TissueDescriptorFeature.h"
class TrackedCellId : public TissueDescriptorFeature<ttt::TrackedTissueDescriptor,ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor>::CellVertexType, double>  {

public:
	typedef TrackedCellId Self;
	typedef TissueDescriptorFeature<ttt::TrackedTissueDescriptor,ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor>::CellVertexType, double> Superclass;

	typedef boost::shared_ptr<Self> Pointer;
	TrackedCellId(){

	}
	virtual ~TrackedCellId(){

	}
	static Pointer New(){
		return Pointer(new Self);
	}
	virtual void Compute(){
		BGL_FORALL_VERTICES(v,*m_TissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){

			std::cout << boost::get(ttt::TrackedCellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).GetID() << std::endl;
			this->Insert(v,boost::get(ttt::TrackedCellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).GetID());
			//this->at(v)=boost::get(ttt::TrackedCellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).m_ID;
		}
	}
};



#endif /* CELLTRACKID_H_ */
