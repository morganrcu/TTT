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
//#include <boost/tuple/tuple.hpp>
//#include <boost/tuple/tuple_comparison.hpp>
#include "Drawer.h"
#include "giaaCellGraph2.h"
class PrimalGraphDrawer: public Drawer {
public:
	typedef itk::FixedArray<float,3> SpacingType;
private:
	giaa::TissueDescriptor::Pointer m_Descriptor;
	SpacingType m_Spacing;

	typedef std::map<vtkSmartPointer<vtkActor>, giaa::SkeletonVertexType> Actor2SkeletonVertexType;
	Actor2SkeletonVertexType  m_Actor2SkeletonVertex;//vtkTogiaaVertex; // map from actor to svt in listS graph used for validation of primal

	typedef std::map<vtkSmartPointer<vtkActor>, giaa::SkeletonEdgeType> Actor2SkeletonEdgeType;
	Actor2SkeletonEdgeType m_Actor2SkeletonEdge;// vtkTowgiaaEdge;

#if 0
	//typedef boost::tuple<giaa::SkeletonVertexType, giaa::SkeletonVertexType> SkeletonEdgeType;

	typedef std::map<vtkActor*, giaa::SkeletonVertexType> Actor2SkeletonVertexType;
	Actor2SkeletonVertexType m_Actor2SkeletonVertex; //actorTogiaavertex;// map for actor to svt in vecS graph used for Primal

	typedef boost::tuple<giaa::SkeletonVertexType, giaa::SkeletonVertexType> SkeletonEdgeType;
	typedef std::map<SkeletonEdgeType, vtkActor*> SkeletonEdge2ActorType;

	SkeletonEdge2ActorType m_SkeletonEdge2Actor;// svtToactor_edge;

	typedef std::map<giaa::SkeletonVertexType, vtkActor*> SkeletonVertex2ActorType;
	SkeletonVertex2ActorType m_SkeletonVertex2Actor;// giaaTovtkVertexToPrimal;

	typedef std::map<SkeletonEdgeLsType, vtkActor*> SkeletonEdgeLs2ActorType;
	SkeletonEdgeLs2ActorType m_SkeletonEdgeLs2Actor; //  giaaTovtkEdge;

#endif
public:
	static PrimalGraphDrawer* New(){
		return new PrimalGraphDrawer;
	}
	PrimalGraphDrawer(){

	}

	inline void SetTissueDescriptor(const giaa::TissueDescriptor::Pointer & descriptor){
		m_Descriptor=descriptor;
	}
	inline void SetSpacing(const SpacingType & spacing ){
		m_Spacing=spacing;
	}
	inline bool IsVertex(const vtkSmartPointer<vtkActor> & actor){
		return m_Actor2SkeletonVertex.count(actor) >0;
	}
	inline bool IsEdge(const vtkSmartPointer<vtkActor> & actor){
		return m_Actor2SkeletonEdge.count(actor) >0;
	}
	inline giaa::SkeletonVertexType GetActorSkeletonVertex(const vtkSmartPointer<vtkActor> & actor){
		assert(IsVertex(actor));
		return m_Actor2SkeletonVertex[actor];
	}
	inline giaa::SkeletonEdgeType GetActorSkeletonEdge(const vtkSmartPointer<vtkActor> & actor){
		assert(IsEdge(actor));
		return m_Actor2SkeletonEdge[actor];
	}
	virtual ~PrimalGraphDrawer(){

	}
	virtual void Draw();
};


#endif /* PRIMALGRAPHDRAWER_H_ */
/** @}*/
