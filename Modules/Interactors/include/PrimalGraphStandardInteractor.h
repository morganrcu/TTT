/*
 * PrimalGraphStandardInteractor.h
 *
 *  Created on: Sep 30, 2013
 *      Author: morgan
 */

#ifndef PRIMALGRAPHSTANDARDINTERACTOR_H_
#define PRIMALGRAPHSTANDARDINTERACTOR_H_
#include "PrimalGraphInteractor.h"
namespace ttt{
class PrimalGraphStandardInteractor : public PrimalGraphInteractor {
public:
	class SomethingSelected{
	protected:
		vtkSmartPointer<PrimalGraphStandardInteractor> m_Caller;
	public:
		SomethingSelected(){
			m_Caller=0;
		}
		virtual void Notify()=0;
		void SetCaller(const vtkSmartPointer<PrimalGraphStandardInteractor> & caller){
			m_Caller=caller;
		}
	};
	class SelectionCleared{
	public:
	    	virtual void Notify()=0;

	};
private:
    vtkSmartPointer<vtkActor> m_SelectedEdge;
    vtkSmartPointer<vtkActor> m_SelectedVertex;

    SomethingSelected  * m_SomethingSelected;
    SelectionCleared  * m_SelectionCleared;

public:

	PrimalGraphStandardInteractor();
	virtual ~PrimalGraphStandardInteractor();

    vtkTypeMacro(PrimalGraphStandardInteractor, PrimalGraphInteractor);

    static PrimalGraphStandardInteractor* New(){
    	return new PrimalGraphStandardInteractor;
    }

    inline bool IsSelectedEdge(){
    	return m_SelectedEdge!=0;
    }

    inline bool IsSelectedVertex(){
    	return m_SelectedVertex!=0;
    }
    inline vtkSmartPointer<vtkActor> GetSelectedVertex(){
    	return m_SelectedVertex;
    }
    inline vtkSmartPointer<vtkActor> GetSelectedEdge(){
    	return m_SelectedEdge;
    }
    inline void SetSomethingSelected(SomethingSelected * somethingSelected){
    	m_SomethingSelected=somethingSelected;
    }
    inline void SetSelectionCleared(SelectionCleared * selectionCleared){
    	m_SelectionCleared=selectionCleared;
    }

    virtual void OnRightButtonUp();
    virtual void OnLeftButtonDown();
private:
    inline void HighlightActor(const vtkSmartPointer<vtkActor> & actor);
    inline void UnhighlightActor(const vtkSmartPointer<vtkActor> & actor);
};
}
#endif /* PRIMALGRAPHSTANDARDINTERACTOR_H_ */
