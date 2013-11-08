/*
 * PrimalGraphInsertionInteractor.h
 *
 *  Created on: Oct 1, 2013
 *      Author: morgan
 */

#ifndef PRIMALGRAPHINSERTIONINTERACTOR_H_
#define PRIMALGRAPHINSERTIONINTERACTOR_H_
#include <vtkPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkSphereSource.h>
#include "PrimalGraphInteractor.h"
#include "tttDescriptionDataTypes.h"
namespace ttt{
class PrimalGraphInsertionInteractor : public PrimalGraphInteractor {
public:
	class InsertionFinished {
	protected:
		PrimalGraphInsertionInteractor * m_Caller;
	public:
		virtual void Notify()=0;

		inline void SetCaller(PrimalGraphInsertionInteractor * caller) {
			m_Caller = caller;
		}
		inline PrimalGraphInsertionInteractor * GetCaller() {
			return m_Caller;
		}
	};

private:


	vtkSmartPointer<vtkActor> m_OriginActor;
	SkeletonVertexType m_Origin;

	vtkSmartPointer<vtkSphereSource> m_NewVertexSphere;
	vtkSmartPointer<vtkPolyDataMapper> m_NewVertexMapper;
	vtkSmartPointer<vtkActor> m_NewVertexActor;

	vtkSmartPointer<vtkLineSource> m_NewEdgeLine;
	vtkSmartPointer<vtkPolyDataMapper> m_NewEdgeMapper;
	vtkSmartPointer<vtkActor> m_NewEdgeActor;

	TissueDescriptor::Pointer m_TissueDescriptor;

	InsertionFinished * m_InsertionFinished;
public:
	PrimalGraphInsertionInteractor();

	vtkTypeMacro(PrimalGraphInsertionInteractor, PrimalGraphInteractor);

    static PrimalGraphInsertionInteractor* New(){
    	return new PrimalGraphInsertionInteractor;
    }

	virtual ~PrimalGraphInsertionInteractor();
	virtual void Init();
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnRightButtonUp();

	inline void SetOrigin(const vtkSmartPointer<vtkActor> & originActor){
		assert(m_Drawer);
		m_OriginActor=originActor;
		m_Origin=m_Drawer->GetActorSkeletonVertex(originActor);
	}
	inline void SetTissueDescriptor(const TissueDescriptor::Pointer & tissueDescriptor){
		m_TissueDescriptor=tissueDescriptor;
	}
	inline void SetInsertionFinished(InsertionFinished *insertionFinished ){
		m_InsertionFinished=insertionFinished;
	}
	inline TissueDescriptor::Pointer GetTissueDescriptor(){
		return m_TissueDescriptor;
	}

};
}

#endif /* PRIMALGRAPHINSERTIONINTERACTOR_H_ */
