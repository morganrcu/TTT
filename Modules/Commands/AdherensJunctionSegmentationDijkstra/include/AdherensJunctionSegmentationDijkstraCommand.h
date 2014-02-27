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

static bool operator<(const  itk::Index<3> & a, const itk::Index<3> & b){
	for(int i=0;i<3;i++){
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
class AdherensJunctionSegmentationDijkstraCommand: public AppCommand {

public:
	/**
	 * TODO
	 */

	/**
		 * TODO
		 */
	typedef itk::Image<float,3> PlatenessImageType;
	typedef itk::Image<float,3> VertexnessImageType;
private:

	typedef itk::Image<float,3> SpeedImageType;
	typedef itk::Image<float,3> DistanceImageType;

	typedef itk::Image<int,3> LabelImageType;

	typedef itk::Image<unsigned char,3> ColorImageType;


	typedef itk::Image<double,3> LevelSetImageType;

	typedef LevelSetImageType::IndexType Index;
	typedef LevelSetImageType::OffsetType Offset;
	/**
	 * TODO
	 */
	ttt::AdherensJunctionVertices::Pointer m_Locations;
	/**
	 * TODO
	 */
	PlatenessImageType::Pointer m_Plateness;
	/**
	 * TODO
	 */
	VertexnessImageType::Pointer m_Vertexness;


	/**
	 * TODO
	 */
	SpeedImageType::Pointer m_Speed;

	/**
	 * TODO
	 */
	ttt::TissueDescriptor::Pointer m_Descriptor;

	typedef boost::bimaps::bimap< boost::bimaps::set_of<ttt::AdherensJunctionVertex::Pointer > , boost::bimaps::set_of<SkeletonVertexType> > IndexAndSkeletonVertexBimapType;

	typedef typename IndexAndSkeletonVertexBimapType::value_type IndexAndSkeletonVertexType;

	IndexAndSkeletonVertexBimapType m_IndexToVertex;


	LevelSetImageType::Pointer m_LevelSet;

	LabelImageType::Pointer m_Labels;
	double m_LevelSetThreshold;
public:
	/**
	 * TODO
	 */
	AdherensJunctionSegmentationDijkstraCommand(){

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
	inline void SetVertexLocations(const ttt::AdherensJunctionVertices::Pointer & locations){
		m_Locations=locations;
	}
	/**
	 * TODO
	 * @param plateness
	 */
	inline void SetPlatenessImage(const PlatenessImageType::Pointer & plateness){
		m_Plateness=plateness;
	}
	/**
	 * TODO
	 * @param plateness
	 */
	inline void SetVertexnessImage(const VertexnessImageType::Pointer & vertexness){
		m_Vertexness=vertexness;
	}
	/**
	 * TODO
	 * @return
	 */
	inline ttt::TissueDescriptor::Pointer GetTissueDescriptor(){
		return m_Descriptor;
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
	void GetNeighbors(const itk::Index<3> & index,std::vector<itk::Index<3> > & neighbors);
	double ComputePath(const SkeletonVertexType & a, const SkeletonVertexType & b);
};
}

#endif
/** @}*/
