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
#ifndef VERTEX_LOCATIONS_DRAWER_H
#define VERTEX_LOCATIONS_DRAWER_H

#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>

#include <itkIndex.h>
#include "Drawer.h"
#include <vector>
#include <boost/tuple/tuple.hpp>
#include "tttDescriptionDataTypes.h"
namespace ttt{

class VertexLocationsDrawer : public Drawer {
public:
	typedef boost::tuple<ttt::AdherensJunctionVertex<3>::Pointer, vtkSmartPointer<vtkSphereSource>, vtkSmartPointer<vtkPolyDataMapper>, vtkSmartPointer<vtkActor> > VertexSphereMapperAndActor ;

private:
    typedef itk::FixedArray<float,3> SpacingType;

    ttt::AdherensJunctionVertices<3>::Pointer m_VertexLocations;
    SpacingType m_Spacing;




    std::list<VertexSphereMapperAndActor> m_VertexSphereMapperAndActorList;


public:
	static VertexLocationsDrawer* New(){
		return new VertexLocationsDrawer;
	}

    VertexLocationsDrawer(){

    }

    void Reset(){


    	for(std::list<VertexSphereMapperAndActor>::iterator it= m_VertexSphereMapperAndActorList.begin();it!=m_VertexSphereMapperAndActorList.end();++it){
    		m_Renderer->RemoveActor(it->get<3>());
    	}
    	m_VertexSphereMapperAndActorList.clear();


    }
    inline void SetVertexLocations(const ttt::AdherensJunctionVertices<3>::Pointer & vertexLocations){
    	m_VertexLocations=vertexLocations;
    }
    inline void SetSpacing(const SpacingType & spacing){
    	m_Spacing=spacing;
    }

    AdherensJunctionVertex<3>::Pointer GetVertexFromActor(const vtkSmartPointer<vtkActor> & actor){
    	std::list<VertexSphereMapperAndActor>::iterator it= m_VertexSphereMapperAndActorList.begin();
    	bool found=false;

    	while(!found && it!=m_VertexSphereMapperAndActorList.end()){
    		if(it->get<3>()==actor){
    			found=true;
    		}else{
    			++it;
    		}
    	}
    	assert(found);
    	return it->get<0>();
   	}


    vtkSmartPointer<vtkSphereSource> GetSphereSourceFromActor(const vtkSmartPointer<vtkActor> & actor){
    	std::list<VertexSphereMapperAndActor>::iterator it= m_VertexSphereMapperAndActorList.begin();
    	bool found=false;

    	while(!found && it!=m_VertexSphereMapperAndActorList.end()){
    		if(it->get<3>()==actor){
    			found=true;
    		}else{
    			++it;
    		}
    	}
    	assert(found);
    	return it->get<1>();
    }

    vtkSmartPointer<vtkPolyDataMapper> GetMapperFromActor(const vtkSmartPointer<vtkActor> & actor){
    	std::list<VertexSphereMapperAndActor>::iterator it= m_VertexSphereMapperAndActorList.begin();
    	bool found=false;

    	while(!found && it!=m_VertexSphereMapperAndActorList.end()){
    		if(it->get<3>()==actor){
    			found=true;
    		}else{
    			++it;
    		}
    	}
    	assert(found);
    	return it->get<2>();
    }

    virtual void Show(){
    	for(std::list<VertexSphereMapperAndActor>::iterator it= m_VertexSphereMapperAndActorList.begin();it!=m_VertexSphereMapperAndActorList.end();it++){
    		it->get<3>()->VisibilityOn();
    	}
    }
    virtual void Hide(){
    	for(std::list<VertexSphereMapperAndActor>::iterator it= m_VertexSphereMapperAndActorList.begin();it!=m_VertexSphereMapperAndActorList.end();it++){
    		it->get<3>()->VisibilityOff();
    	}
    }

    VertexSphereMapperAndActor DrawAdherensJunctionVertex(const ttt::AdherensJunctionVertex<3>::Pointer & vertex);

    void EraseAdherensJunctionVertex(const ttt::AdherensJunctionVertex<3>::Pointer & vertex){
    	std::list<VertexSphereMapperAndActor>::iterator it= m_VertexSphereMapperAndActorList.begin();
    	bool found=false;

    	while(!found && it!=m_VertexSphereMapperAndActorList.end()){
    		if(it->get<0>()==vertex){
    			found=true;
    		}else{
    			++it;
    		}
    	}
    	assert(found);
    	m_Renderer->RemoveActor(it->get<3>());
    	m_VertexSphereMapperAndActorList.erase(it);

    }

    virtual void Draw();

	virtual ~VertexLocationsDrawer(){

	}
};
}
#endif
/** @}*/
