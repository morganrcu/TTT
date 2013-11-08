/*
 * PrimalGraphMoveInteractor.h
 *
 *  Created on: Oct 1, 2013
 *      Author: morgan
 */

#ifndef PRIMALGRAPHMOVEINTERACTOR_H_
#define PRIMALGRAPHMOVEINTERACTOR_H_
#include <vtkActor.h>
#include "tttDescriptionDataTypes.h"
#include "PrimalGraphInteractor.h"
namespace ttt{
class PrimalGraphMoveInteractor : public PrimalGraphInteractor{
public:
	class MoveFinished{
	protected:
		PrimalGraphMoveInteractor * m_Caller;
	public:
		virtual void Notify()=0;
		inline void SetCaller(PrimalGraphMoveInteractor * caller ){
			m_Caller =caller;
		}
		inline PrimalGraphMoveInteractor * GetCaller(){
			return m_Caller;
		}
	};
private:

	vtkSmartPointer<vtkActor> m_TargetActor;
	SkeletonVertexType m_Target;

	TissueDescriptor::Pointer m_TissueDescriptor;
	itk::Point<double,3> m_OriginalPoint;
	MoveFinished * m_MoveFinished;

	bool m_Moved;
public:
	PrimalGraphMoveInteractor();
	virtual ~PrimalGraphMoveInteractor();

	static PrimalGraphMoveInteractor * New(){
		return new PrimalGraphMoveInteractor;
	}
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();

	void SetTarget(const vtkSmartPointer<vtkActor> & targetActor){
		assert(m_Drawer);
		assert(m_TissueDescriptor);
		m_TargetActor=targetActor;
		m_Target=m_Drawer->GetActorSkeletonVertex(targetActor);
		m_OriginalPoint=boost::get(SkeletonPointPropertyTag(),*m_TissueDescriptor->m_SkeletonGraph,m_Target).position;
	}

	inline void SetTissueDescriptor(const TissueDescriptor::Pointer & tissueDescriptor){
		m_TissueDescriptor=tissueDescriptor;
	}

	inline void SetMoveFinished(MoveFinished * moveFinished ){
		m_MoveFinished=moveFinished;
	}
	inline TissueDescriptor::Pointer GetTissueDescriptor(){
		return m_TissueDescriptor;
	}
	inline bool IsMoved(){
		return m_Moved;
	}
};
}
#endif /* PRIMALGRAPHMOVEINTERACTOR_H_ */
