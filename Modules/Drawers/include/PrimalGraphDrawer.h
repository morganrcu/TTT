//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

/** \addtogroup TTTDrawer
 *  @{
 */
#ifndef PRIMAL_GRAPH_DRAWER_H_
#define PRIMAL_GRAPH_DRAWER_H_
#include <itkFixedArray.h>
#include "tttDescriptionDataTypes.h"
#include "Drawer.h"
#include "Colorer.h"
namespace ttt{
class PrimalGraphDrawer: public Drawer {
public:
	typedef itk::FixedArray<float,3> SpacingType;


private:
	TissueDescriptor::Pointer m_Descriptor;

	typedef std::map<vtkSmartPointer<vtkActor>, SkeletonVertexType> Actor2SkeletonVertexType;
	Actor2SkeletonVertexType  m_Actor2SkeletonVertex;

	typedef std::map<vtkSmartPointer<vtkActor>, SkeletonEdgeType> Actor2SkeletonEdgeType;
	Actor2SkeletonEdgeType m_Actor2SkeletonEdge;

	Colorer<ttt::SkeletonEdgeType> *  m_pEdgeColorer;
	Colorer<ttt::SkeletonVertexType> *  m_pVertexColorer;

public:
	inline void SetEdgeColorer(Colorer<ttt::SkeletonEdgeType> * colorer){
		m_pEdgeColorer=colorer;
	}

	inline void SetVertexColorer(Colorer<ttt::SkeletonVertexType> * colorer){
		m_pVertexColorer=colorer;
	}

#if 0
	static PrimalGraphDrawer* New(){
		return new PrimalGraphDrawer;
	}
#endif
	PrimalGraphDrawer(){

	}

	inline void SetTissueDescriptor(const TissueDescriptor::Pointer & descriptor){
		m_Descriptor=descriptor;
	}

	inline bool IsVertex(const vtkSmartPointer<vtkActor> & actor){
		return m_Actor2SkeletonVertex.count(actor) >0;
	}

	inline bool IsEdge(const vtkSmartPointer<vtkActor> & actor){
		return m_Actor2SkeletonEdge.count(actor) >0;
	}

	inline SkeletonVertexType GetActorSkeletonVertex(const vtkSmartPointer<vtkActor> & actor){
		assert(IsVertex(actor));
		return m_Actor2SkeletonVertex[actor];
	}

	inline SkeletonEdgeType GetActorSkeletonEdge(const vtkSmartPointer<vtkActor> & actor){
		assert(IsEdge(actor));
		return m_Actor2SkeletonEdge[actor];
	}

	void DrawVertex(const ttt::SkeletonVertexType & vertex);
	inline void DrawEdge(const ttt::SkeletonEdgeType & edge);

	virtual void Reset();
	virtual ~PrimalGraphDrawer(){

	}
	virtual void Draw();
	virtual void Show();
	virtual void Hide();
};
}

#endif /* PRIMALGRAPHDRAWER_H_ */
/** @}*/
