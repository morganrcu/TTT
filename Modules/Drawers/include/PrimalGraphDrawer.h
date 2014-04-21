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
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>
#include "tttDescriptionDataTypes.h"
#include "Drawer.h"
#include "Colorer.h"
namespace ttt{
template<class TissueDescriptor> class PrimalGraphDrawer: public Drawer {
public:
	typedef itk::FixedArray<float,3> SpacingType;

	typedef boost::tuple<ttt::SkeletonVertexType, vtkSmartPointer<vtkSphereSource>, vtkSmartPointer<vtkPolyDataMapper>, vtkSmartPointer<vtkActor> > VertexSphereMapperAndActor ;
private:
	typename TissueDescriptor::Pointer m_Descriptor;

	typedef std::map<vtkSmartPointer<vtkActor>, SkeletonVertexType> Actor2SkeletonVertexType;
	Actor2SkeletonVertexType  m_Actor2SkeletonVertex;

	typedef std::map<vtkSmartPointer<vtkActor>, vtkSmartPointer<vtkSphereSource> > Actor2SphereType;
	Actor2SphereType  m_Actor2Sphere;

	typedef std::map<vtkSmartPointer<vtkActor>, SkeletonEdgeType> Actor2SkeletonEdgeType;
	Actor2SkeletonEdgeType m_Actor2SkeletonEdge;

	Colorer<ttt::SkeletonEdgeType> *  m_pEdgeColorer;
	Colorer<ttt::SkeletonVertexType> *  m_pVertexColorer;



public:

	inline void SetEdgeColorer(Colorer<typename ttt::TissueDescriptorTraits<TissueDescriptor>::SkeletonEdgeType> * colorer){
		m_pEdgeColorer=colorer;
	}

	inline void SetVertexColorer(Colorer<typename ttt::TissueDescriptorTraits<TissueDescriptor>::SkeletonVertexType> * colorer){
		m_pVertexColorer=colorer;
	}

#if 0
	static PrimalGraphDrawer* New(){
		return new PrimalGraphDrawer;
	}
#endif
	PrimalGraphDrawer(){

	}

	inline void SetTissueDescriptor(const typename TissueDescriptor::Pointer & descriptor){
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
	inline vtkSmartPointer<vtkSphereSource> GetSphereSource(const vtkSmartPointer<vtkActor> & actor){
		assert(IsVertex(actor));
		return m_Actor2Sphere[actor];
	}

	inline SkeletonEdgeType GetActorSkeletonEdge(const vtkSmartPointer<vtkActor> & actor){
		assert(IsEdge(actor));
		return m_Actor2SkeletonEdge[actor];
	}

	VertexSphereMapperAndActor DrawVertex(const ttt::SkeletonVertexType & vertex){
		itk::Point<double, 3> a =boost::get(SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,vertex).position;


			vtkSmartPointer<vtkSphereSource> newSphere = vtkSmartPointer<vtkSphereSource>::New();

			newSphere->SetCenter(a[0],a[1],a[2]);

			newSphere->SetRadius(0.15);

			vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			//sphereMapper->ImmediateModeRenderingOn();
			sphereMapper->SetInputConnection(newSphere->GetOutputPort());
			//sphereMapper->ImmediateModeRenderingOn();
			vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
			double  color[3];
			m_pVertexColorer->GetObjectColor(vertex,color);
			sphereActor->GetProperty()->SetColor(color[0],color[1],color[2]);

				/*if(boost::degree(v,descriptor->m_SkeletonGraph) == 1){
		 	 	 sphereActor->GetProperty()->SetColor(1,0,0);
		 	 }
		 	if(boost::degree(v,descriptor->m_SkeletonGraph) == 2){
		 		sphereActor->GetProperty()->SetColor(0,1,0);
		 	 }
		 	 if(boost::degree(v,descriptor->m_SkeletonGraph) == 3){
		 	 	 sphereActor->GetProperty()->SetColor(0,0,1);
		 	 }
		 	 if(boost::degree(v,descriptor->m_SkeletonGraph) > 3){
		 	 	 sphereActor->GetProperty()->SetColor(0,0,0);
		 	 }*/

			sphereActor->SetMapper(sphereMapper);
			sphereActor->VisibilityOff();
			m_Actor2SkeletonVertex[sphereActor] = vertex;
			m_Actor2Sphere[sphereActor]=newSphere;
			m_Renderer->AddActor(sphereActor);
			return VertexSphereMapperAndActor(vertex,newSphere,sphereMapper,sphereActor);
	}
	inline void DrawEdge(const ttt::SkeletonEdgeType & edge){
		typedef itk::Point<double,3> itkpt;
			ttt::SkeletonVertexType src = boost::source(edge,*m_Descriptor->m_SkeletonGraph);
			ttt::SkeletonVertexType tgt = boost::target(edge,*m_Descriptor->m_SkeletonGraph);

			vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
			itkpt a= boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,boost::source(edge,*m_Descriptor->m_SkeletonGraph)).position;

			newLine->SetPoint1(a[0],a[1],a[2]);
			itkpt b= boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,boost::target(edge,*m_Descriptor->m_SkeletonGraph)).position;

			newLine->SetPoint2(b[0],b[1],b[2]);
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper->ImmediateModeRenderingOn();
			mapper->SetInputConnection(newLine->GetOutputPort());
			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			actor->SetMapper(mapper);
			double  color[3];
			m_pEdgeColorer->GetObjectColor(edge,color);
			actor->GetProperty()->SetColor(color[0],color[1],color[2]);


			actor->GetProperty()->SetLineWidth(5);
			actor->VisibilityOff();
			boost::tuple<ttt::SkeletonVertexType,ttt::SkeletonVertexType> pts (src,tgt);

			m_Actor2SkeletonEdge[actor]=edge;

			boost::tuple<ttt::SkeletonVertexType,ttt::SkeletonVertexType> ptt (tgt,src);

			m_Renderer->AddActor(actor);
	}

	virtual void Reset(){
			for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
				m_Renderer->RemoveActor(it->first);
			}
			for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
				m_Renderer->RemoveActor(it->first);
			}

			m_Actor2SkeletonVertex.clear();
			m_Actor2SkeletonEdge.clear();
			m_Actor2Sphere.clear();

	}
	virtual void PickableOn(){
			for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
				it->first->PickableOn();
			}
			for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
				it->first->PickableOn();
			}
	}
	virtual void PickableOff(){
			for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
				it->first->PickableOff();
			}
			for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
				it->first->PickableOff();
			}
	}
	virtual ~PrimalGraphDrawer(){

	}
	virtual void Draw() {

		this->Reset();

		BGL_FORALL_VERTICES(v,*m_Descriptor->m_SkeletonGraph,SkeletonGraph){
			this->DrawVertex(v);
		}

		BGL_FORALL_EDGES(e,*m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
			this->DrawEdge(e);
		}
	}

	virtual void Show(){
		for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
				it->first->VisibilityOn();
			}
			for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
				it->first->VisibilityOn();
			}
	}
	virtual void Hide(){
		for(Actor2SkeletonVertexType::iterator it =m_Actor2SkeletonVertex.begin();it!=m_Actor2SkeletonVertex.end();++it){
				it->first->VisibilityOff();
			}
			for(Actor2SkeletonEdgeType::iterator it =m_Actor2SkeletonEdge.begin();it!=m_Actor2SkeletonEdge.end();++it){
				it->first->VisibilityOff();
			}
	}
};
}

#endif /* PRIMALGRAPHDRAWER_H_ */
/** @}*/
