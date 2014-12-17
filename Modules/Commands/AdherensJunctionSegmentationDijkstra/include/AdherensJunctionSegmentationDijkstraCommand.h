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

/** \addtogroup TTTAdherensJunctionSegmentation
 *  @{
 */

#ifndef ADHERENSJUNCTIONSEGMENTATIONDIJKSTRACOMMAND_H_
#define ADHERENSJUNCTIONSEGMENTATIONDIJKSTRACOMMAND_H_

#include "AppCommand.h"
#include "tttDescriptionDataTypes.h"
#include "itkPointSet.h"
#include "itkImage.h"
#include "heapplus.h"
#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>
namespace std{

template<int dim> static bool operator<(const  itk::Index<dim> & a, const itk::Index<dim> & b){
	for(int i=0;i<dim;i++){
		if(a[i]<b[i]){
			return true;
		}else if(a[i]>b[i]){
			return false;
		}
	}
	return false;
}

}
namespace ttt{
/**
 * TODO
 * \class AdherensJunctionSegmentationCommand
 */
template<int dim> class AdherensJunctionSegmentationDijkstraCommand: public AppCommand {

public:
	/**
	 * TODO
	 */

	/**
		 * TODO
		 */
	typedef itk::Image<float,dim> PlatenessImageType;
	typedef itk::Image<float,dim> VertexnessImageType;
private:

	typedef itk::Image<float,dim> SpeedImageType;
	typedef itk::Image<float,dim> DistanceImageType;

	typedef itk::Image<int,dim> LabelImageType;

	typedef itk::Image<unsigned char,dim> ColorImageType;


	typedef itk::Image<double,dim> LevelSetImageType;

	typedef typename LevelSetImageType::IndexType Index;
	typedef typename LevelSetImageType::OffsetType Offset;
	/**
	 * TODO
	 */
	typename ttt::AdherensJunctionVertices<dim>::Pointer m_Locations;
	/**
	 * TODO
	 */
	typename PlatenessImageType::Pointer m_Plateness;
	/**
	 * TODO
	 */
	typename VertexnessImageType::Pointer m_Vertexness;


	/**
	 * TODO
	 */
	typename SpeedImageType::Pointer m_Speed;

	typedef typename ttt::TissueDescriptor<dim>::PrimalGraphVertexDescriptorType SkeletonVertexType;
	/**
	 * TODO
	 */
	typename TissueDescriptor<dim>::Pointer m_Descriptor;

	typedef typename boost::bimaps::bimap< boost::bimaps::set_of<typename ttt::AdherensJunctionVertex<dim>::Pointer > , boost::bimaps::set_of< typename ttt::TissueDescriptor<dim>::PrimalGraphVertexDescriptorType> > IndexAndSkeletonVertexBimapType;

	typedef typename IndexAndSkeletonVertexBimapType::value_type IndexAndSkeletonVertexType;

	IndexAndSkeletonVertexBimapType m_IndexToVertex;


	typename LevelSetImageType::Pointer m_LevelSet;

	typename LabelImageType::Pointer m_Labels;
	double m_LevelSetThreshold;

	double m_StoppingValue;
public:
	/**
	 * TODO
	 */
	AdherensJunctionSegmentationDijkstraCommand(){
		m_StoppingValue=100;
	}
	/**
	 * TODO
	 */
	virtual ~AdherensJunctionSegmentationDijkstraCommand(){

	}
	/**
	 * TODO
	 */
	virtual void Do();


	/**
	 * TODO
	 * @param locations
	 */
	inline void SetVertexLocations(const typename ttt::AdherensJunctionVertices<dim>::Pointer & locations){
		m_Locations=locations;
	}
	/**
	 * TODO
	 * @param plateness
	 */
	inline void SetPlatenessImage(const typename PlatenessImageType::Pointer & plateness){
		m_Plateness=plateness;
	}
	/**
	 * TODO
	 * @param vertexness
	 */
	inline void SetVertexnessImage(const typename VertexnessImageType::Pointer & vertexness){
		m_Vertexness=vertexness;
	}
	/**
	 * TODO
	 * @return
	 */
	inline typename ttt::TissueDescriptor<dim>::Pointer GetTissueDescriptor(){
		return m_Descriptor;
	}
	inline void SetLimit(double stoppingValue){
		m_StoppingValue=stoppingValue;
	}

private:
	void PruneLevelSet(double threshold);
	void ComputeLevelsetThreshold(double percentage);
	void ComputSpeedImage();
	void BuildGraph();
	void AllocateLabelsImage();
	void StoreLabels();
	void InitDefGraph();
	void DoFastMarching();
	void DoVertexSegmentation();
	void GetNeighbors(const itk::Index<dim> & index,std::vector<itk::Index<dim> > & neighbors);
	double ComputePath(const SkeletonVertexType & a, const SkeletonVertexType & b);
};
}
#include "AdherensJunctionSegmentationDijkstraCommand.hpp"
#endif
/** @}*/
