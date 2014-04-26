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

/** \addtogroup TTTDrawers
 *  @{
 */
#ifndef DUAL_GRAPH_DRAWER_H_
#define DUAL_GRAPH_DRAWER_H_
#include "tttDescriptionDataTypes.h"
#include "Drawer.h"
#include "Colorer.h"
#include <vtkLineSource.h>
namespace ttt{
template<class TissueDescriptor> class DualGraphDrawer: public Drawer {
public:
	typedef itk::FixedArray<float,3> SpacingType;

private:
	typename TissueDescriptor::Pointer m_Descriptor;
	SpacingType m_Spacing;

	typedef boost::tuple<ttt::CellVertexType, vtkSmartPointer<vtkSphereSource>, vtkSmartPointer<vtkPolyDataMapper>, vtkSmartPointer<vtkActor> > CellVertexSphereMapperAndActor ;
	typedef boost::tuple<ttt::CellEdgeType, vtkSmartPointer<vtkLineSource>, vtkSmartPointer<vtkPolyDataMapper>, vtkSmartPointer<vtkActor> > CellEdgeLineMapperAndActor ;


    std::list<CellVertexSphereMapperAndActor> m_CellVertexSphereMapperAndActorList;
    std::list<CellEdgeLineMapperAndActor> m_CellEdgeLineMapperAndActorList;

	Colorer<ttt::SkeletonEdgeType> *  m_pEdgeColorer;
	Colorer<ttt::SkeletonVertexType> *  m_pVertexColorer;

public:
	inline void SetEdgeColorer(Colorer<typename ttt::TissueDescriptorTraits<TissueDescriptor>::CellEdgeType> * colorer){
		m_pEdgeColorer=colorer;
	}

	inline void SetVertexColorer(Colorer<typename ttt::TissueDescriptorTraits<TissueDescriptor>::CellVertexType> * colorer){
		m_pVertexColorer=colorer;
	}

public:
	static DualGraphDrawer* New(){
		return new DualGraphDrawer;
	}

	DualGraphDrawer(){

	}

	inline void SetTissueDescriptor(const typename TissueDescriptor::Pointer & descriptor){
		m_Descriptor=descriptor;
	}

	virtual ~DualGraphDrawer(){

	}
	virtual void Draw(){
		this->Reset();
			BGL_FORALL_VERTICES_T(v,*m_Descriptor->m_CellGraph,typename TissueDescriptor::DualGraphType){
				typedef typename ttt::TissueDescriptorTraits<TissueDescriptor>::CellPropertyTagType CellPropertyTag;
				itk::Point<double,3> a= boost::get(CellPropertyTag(),*m_Descriptor->m_CellGraph,v).GetCentroid();
				vtkSmartPointer<vtkSphereSource> newSphere = vtkSmartPointer<vtkSphereSource>::New();
				newSphere->SetCenter(a[0],a[1],a[2]);
				newSphere->SetRadius(0.8);
				vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

				sphereMapper->SetInputConnection(newSphere->GetOutputPort());

				vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
				double color[3];

				m_pVertexColorer->GetObjectColor(v,color);

				sphereActor->GetProperty()->SetColor(color);
				sphereActor->SetMapper(sphereMapper);

				m_Renderer->AddActor(sphereActor);
				sphereActor->VisibilityOff();
				CellVertexSphereMapperAndActor tuple = boost::make_tuple(v,newSphere,sphereMapper,sphereActor);
				m_CellVertexSphereMapperAndActorList.push_back(tuple);
			}

			BGL_FORALL_EDGES_T(e,*m_Descriptor->m_CellGraph,typename TissueDescriptor::DualGraphType){
				vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
				typedef typename ttt::TissueDescriptorTraits<TissueDescriptor>::CellPropertyTagType CellPropertyTag;
				itk::Point<double,3> a= boost::get(CellPropertyTag(),*m_Descriptor->m_CellGraph,boost::source(e,*m_Descriptor->m_CellGraph)).GetCentroid();
				newLine->SetPoint1(a[0],a[1],a[2]);
				itk::Point<double,3> b= boost::get(CellPropertyTag(),*m_Descriptor->m_CellGraph,boost::target(e,*m_Descriptor->m_CellGraph)).GetCentroid();

			//IndexType b=m_SkeletonGraph[boost::target(e,m_SkeletonGraph)].position;
				newLine->SetPoint2(b[0],b[1],b[2]);

				vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
				mapper->SetInputConnection(newLine->GetOutputPort());

				vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
				actor->SetMapper(mapper);
				double color[3];

				m_pEdgeColorer->GetObjectColor(e,color);

				actor->GetProperty()->SetColor(color);

				//actor->GetProperty()->SetColor(0,0,0);
				actor->GetProperty()->SetLineWidth(3);
				actor->VisibilityOff();
				m_Renderer->AddActor(actor);
				CellEdgeLineMapperAndActor tuple = boost::make_tuple(e,newLine,mapper,actor);
				m_CellEdgeLineMapperAndActorList.push_back(tuple);
			}
	}
	virtual void Show(){
		for(std::list<CellVertexSphereMapperAndActor>::iterator it= m_CellVertexSphereMapperAndActorList.begin();it!=m_CellVertexSphereMapperAndActorList.end();it++){
			it->get<3>()->VisibilityOn();
		}

		for(std::list<CellEdgeLineMapperAndActor>::iterator it= m_CellEdgeLineMapperAndActorList.begin();it!=m_CellEdgeLineMapperAndActorList.end();it++){
			it->get<3>()->VisibilityOn();
		}
	}
	virtual void Hide(){
		for(std::list<CellVertexSphereMapperAndActor>::iterator it= m_CellVertexSphereMapperAndActorList.begin();it!=m_CellVertexSphereMapperAndActorList.end();it++){
				it->get<3>()->VisibilityOff();
			}
			for(std::list<CellEdgeLineMapperAndActor>::iterator it= m_CellEdgeLineMapperAndActorList.begin();it!=m_CellEdgeLineMapperAndActorList.end();it++){
				it->get<3>()->VisibilityOff();
			}
	}
	virtual void Reset(){
		for(std::list<CellVertexSphereMapperAndActor>::iterator it= m_CellVertexSphereMapperAndActorList.begin();it!=m_CellVertexSphereMapperAndActorList.end();it++){
				m_Renderer->RemoveActor(it->get<3>());
			}
			for(std::list<CellEdgeLineMapperAndActor>::iterator it= m_CellEdgeLineMapperAndActorList.begin();it!=m_CellEdgeLineMapperAndActorList.end();it++){
				m_Renderer->RemoveActor(it->get<3>());
			}
			m_CellVertexSphereMapperAndActorList.clear();
			m_CellEdgeLineMapperAndActorList.clear();
	}
};
}

#endif /* DUALGRAPHDRAWER_H_ */
/** @}*/
