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


/** \addtogroup TTTCore
 *  @{
 */
#pragma once
#include "itkIndex.h"
#include "itkPoint.h"
#include "itkVector.h"
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkDataObject.h"

#include <set>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/shared_ptr.hpp>

#include <numeric>

#ifdef BOOST_SERIALIZATION
namespace itk {
template<class Archive> void serialize(Archive & ar,
		itk::Point<double, 3> & index, const unsigned int version) {

	ar & index[0] & index[1] & index[2];
}

template<class Archive> void serialize(Archive & ar,
		itk::Vector<double, 3> & vector, const unsigned int version) {

	ar & vector[0] & vector[1] & vector[2];
}
}
#endif

namespace ttt {

class AdherensJunctionVertex : public itk::DataObject{
public:
	/**
	 * Location of the point in pixel coordinates
	 */
	typedef itk::Index<3> IndexType;
	typedef AdherensJunctionVertex Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	typedef itk::DataObject Superclass;

	itkTypeMacro(Self,Superclass);
	itkNewMacro(Self);

	itkGetConstReferenceMacro(Position,IndexType);
	itkSetMacro(Position,IndexType);
protected:
	/**
	 * Default constructor. The attributes are fixed to default values
	 */
	AdherensJunctionVertex() {
		m_Position.Fill(-1);
	}
	/**
	 * Position of the point in 3D pixel coordinates
	 */

	IndexType m_Position;

private:
	AdherensJunctionVertex(const Self &);

};


class AdherensJunctionVertices : public itk::DataObject, public std::vector<AdherensJunctionVertex::Pointer>{
public:
	typedef AdherensJunctionVertices Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	typedef itk::DataObject Superclass;

	itkTypeMacro(Self,Superclass);
	itkNewMacro(Self);
protected:
	/**
	 * Default constructor. The attributes are fixed to default values
	 */
	AdherensJunctionVertices() : std::vector<AdherensJunctionVertex::Pointer>(0){

	}

private:
	AdherensJunctionVertices(const Self &);
};

/**
 * \brief Class to represent a point in the adherens junction graph
 */

class SkeletonPoint {
public:
	/**
	 * Location of the point in pixel coordinates
	 */
	typedef itk::Point<float, 3> PointType;

	/**
	 * Default constructor. The attributes are fixed to default values
	 */
	SkeletonPoint() {
		position.Fill(-1);
	}
	/**
	 * Constructor initializating the position of the SkeltonPoint
	 * weight is initialized to weight (represents one true point)
	 * clustered is initialized to false
	 * @param position of the point in the 3D space, in pixel coordinates
	 */

	SkeletonPoint(PointType & position) {
		this->position = position;
	}

	/**
	 * Copy constructors. Attributes are initialized to the values of other
	 * @param other The SkeletonPoint to copy
	 * 
	 */
	SkeletonPoint(const SkeletonPoint & other) {
		this->position = other.position;
	}
	/**
	 * Position of the point in 3D pixel coordinates
	 */

	PointType position;
#ifdef BOOST_SERIALIZATION

	/**
	 * Auxiliary attribute employed in skeleton prunning. The value means the number of points that have been merged into this point.
	 */

	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		//ar & position & weight & clustered & color; // Simply serialize the data members of Obj
		ar & position;// & weight & clustered & color; // Simply serialize the data members of Obj
	}

#endif

}; //class SkeletonPoint


/**
 * Auxiliary class to specify the graph storage
 */
class SkeletonPointPropertyTag {
public:
	typedef boost::vertex_property_tag kind;
};

/**
 * Auxiliary class to specify the graph storage
 */
typedef boost::property<ttt::SkeletonPointPropertyTag, ttt::SkeletonPoint,
		boost::property<boost::vertex_index_t, int> > SkeletonPointProperty;

/**
 * SkeletonPoint storage. The graph structure represents the intercellular skeleton
 */
#if 0
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		ttt::SkeletonPointProperty, boost::property<boost::edge_index_t, int> > SkeletonGraph;
#endif

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,
		ttt::SkeletonPointProperty, boost::property<boost::edge_index_t, int> > SkeletonGraph;
/**
 * Type of the vertices in the graph_traits
 */
typedef boost::graph_traits<ttt::SkeletonGraph>::vertex_descriptor SkeletonVertexType;

/**
 * Type of the edges in the graph_traits
 */
typedef boost::graph_traits<ttt::SkeletonGraph>::edge_descriptor SkeletonEdgeType;


/**
 * Class to represent a Cell in a Tissue
 */
class Cell {
public:
	/**
	 * Position of the centroid of the cell, in Real World? Coordinates
	 */
	typedef itk::Point<double, 3> Point;

	typedef std::vector<SkeletonVertexType>::const_iterator PerimeterIterator;

	/**
	 *  Default constructor. Sets m_Centroid to zero.
	 */


	Cell() {
		m_Centroid.Fill(0);
		m_SkeletonNodes.clear();
	}
	/**
	 * Copy constructor. Initializates the attributes with the values in other
	 */
	Cell(const Cell & other) {
		this->m_SkeletonNodes = other.m_SkeletonNodes;
		this->m_Centroid = other.m_Centroid;
	}
	/**
	 * Returns the number of SkeletonPoints in the border of the cell
	 */
	int GetNumSkeletonPoints() const {
		return m_SkeletonNodes.size();
	}
	/**
	 * Adds a new SkeletonPoint to the border of the cell
	 * @param vertex the descriptor of the point to add, represented by the vertex descriptor of the SkeletonGraph where it is stored
	 */
	inline void AddVertexToPerimeter(const SkeletonVertexType & vertex) {
		if(std::find(m_SkeletonNodes.begin(),m_SkeletonNodes.end(),vertex)==m_SkeletonNodes.end()){
			m_SkeletonNodes.push_back(vertex);
		}
	}
	inline PerimeterIterator PerimeterBegin() const {
		return m_SkeletonNodes.begin();
	}
	/**
	 * Returns an iterator pointing to the last skeletonpoint
	 */
	inline PerimeterIterator PerimeterEnd() const {
		return m_SkeletonNodes.end();
	}
#ifdef BOOST_SERIALIZATION
	/**
	 * Auxiliary function for data serialization with boost
	 */
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & m_SkeletonNodes;  // Simply serialize the data members of Obj
		ar & m_Centroid;
	}
#endif
	inline void SetCentroid(const Point & centroid){
		m_Centroid=centroid;
	}
	inline Point GetCentroid() const{
		return m_Centroid;
	}

private:
	/**
	 * Container with the points in the Cellular Border
	 */

	std::vector<SkeletonVertexType> m_SkeletonNodes;
	/**
	 * Cell centroid location
	 */
	Point m_Centroid;
};

/**
 * Definition of the Cell Properties as vertex properties
 */
struct CellPropertyTag {

	typedef boost::vertex_property_tag kind;
};
/**
 * Cell Property definition
 */
typedef boost::property<CellPropertyTag, Cell,boost::property<boost::vertex_index_t, int> >  CellProperty;


/**
 * Cell Graph definition
 */
#if 0
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, CellProperty, boost::property<boost::edge_index_t, int> > CellGraph;
#endif

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, CellProperty, boost::property<boost::edge_index_t, int> > CellGraph;


typedef boost::graph_traits<CellGraph>::vertex_descriptor CellVertexType;

typedef boost::graph_traits<CellGraph>::edge_descriptor CellEdgeType;
/**
 * Class extending Cell class to represent tracked Cells
 */
class TrackedCell: public Cell {
public:
	/**
	 * Next identifier for new tracks
	 */
	static int m_NextID;
private:
	/**
	 * Track ID
	 */
	int m_ID;
	/**
	 * Parent Track ID
	 */
	int m_ParentID;
	/**
	 * Velocity vector of the cell
	 */
	itk::Vector<double, 3> m_Velocity;
	/**
	 *
	 */
	ttt::CellVertexType m_ObservedCell;

public:

	void SetID(int ID){
		m_ID=ID;
	}
	int GetID(){
		return m_ID;
	}
	int GetParentID(){
		return m_ParentID;
	}
	void SetParentID(int ParentID){
		m_ParentID=ParentID;
	}
	itk::Vector<double,3> GetVelocity(){
		return m_Velocity;
	}
	void SetVelocity(const itk::Vector<double,3> & velocity){
		m_Velocity=velocity;
	}

	void SetObservedCell(int observedCell){
		m_ObservedCell=observedCell;
	}

	int GetObservedCell(){
		return m_ObservedCell;
	}


	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<Cell>(*this);
		ar & m_ID;
		ar & m_Velocity;
		ar & m_ParentID;
	}
	/**
	 * Default constructor initializing fields to default values
	 */
	TrackedCell() {
		m_ParentID=-1;
		m_ID = -1;
		m_Velocity.Fill(0);
		m_ObservedCell=-1;
	}
	/**
	 * Copy constructor. Copies fields from the other cell
     */
	TrackedCell(const TrackedCell & other) : Cell(other) {
		this->m_Velocity = other.m_Velocity;
		this->m_ID = other.m_ID;
		this->m_ParentID=other.m_ParentID;
		this->m_ObservedCell=other.m_ObservedCell;
	}

	/**
	 * Copy constructor. Copies fields from the other cell
     */
	TrackedCell(const Cell & other) : Cell(other) {
		this->m_Velocity.Fill(0);
		this->m_ID = -1;
		this->m_ParentID=-1;
		this->m_ObservedCell=-1;
	}
};

struct TrackedCellPropertyTag {
	typedef boost::vertex_property_tag kind;
};

typedef boost::property<TrackedCellPropertyTag, TrackedCell,
		boost::property<boost::vertex_index_t, int> > TrackedCellProperty;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		TrackedCellProperty, boost::property<boost::edge_index_t, int> > TrackedCellGraph;

typedef boost::graph_traits<TrackedCellGraph>::vertex_descriptor TrackedCellVertexType;
typedef boost::graph_traits<TrackedCellGraph>::edge_descriptor TrackedCellEdgeType;



template<class CellVertexType> class Domain {
public:
	Domain() {
		m_DomainID = -1;
		m_Order=-1;
		m_Nucleus=-1;
		m_Order=-1;

	}

	Domain(const Domain & other) {
		this->m_CellNodes = other.m_CellNodes;
		this->m_DomainID = other.m_DomainID;
		this->m_Nucleus= other.m_Nucleus;
		this->m_Order=other.m_Order;
	}

	inline CellVertexType GetNucleus(){
		return m_Nucleus;
	}
	inline void SetNucleus(const CellVertexType & nucleus){
		m_Nucleus=nucleus;

	}
	inline void AddCell(const CellVertexType & cell) {
		m_CellNodes.insert(cell);
	}
	inline void SetOrder(int order){
		m_Order=order;
	}
	inline int GetOrder(){
		return m_Order;
	}
	/**
	 * Returns an iterator pointing to the first Cell
	 */

	inline typename std::set<CellVertexType>::iterator Begin() {
	//inline std::iterator<CellVertexType> Begin() {
		return m_CellNodes.begin();
	}
	/**
	 * Returns an iterator pointing to the last Cell
	 */
	inline typename std::set<CellVertexType>::iterator End() {
		return m_CellNodes.end();
	}
#if 0
	inline std::vector<int> GetCells() const {
		return m_CellNodes;
	}
#endif

	inline int GetSize() {
		return m_CellNodes.size();
	}
private:
	std::set<CellVertexType> m_CellNodes;
	CellVertexType m_Nucleus;
	int m_DomainID;
	int m_Order;
};

typedef Domain<TrackedCellVertexType> TrackedDomain;

template<class T> struct TissueDescriptorTraits{

};


/**
 * Template Class to represent a Tissue, composed by a primal graph and a dual graph. Primal graph represents intercellular skeleton, Dual graph represents cells
 */
template <class TPrimalGraph, class TDualGraph> class TemplateTissueDescriptor: public itk::Object {
public:
	typedef TemplateTissueDescriptor Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TPrimalGraph PrimalGraphType;
	typedef typename boost::graph_traits<PrimalGraphType>::vertex_descriptor PrimalGraphVertexDescriptorType;
	typedef TDualGraph DualGraphType;
	typedef typename boost::graph_traits<DualGraphType>::vertex_descriptor DualGraphVertexDescriptorType;


	itkNewMacro(TemplateTissueDescriptor);

	TemplateTissueDescriptor(){
		m_CellGraph=boost::shared_ptr<DualGraphType>(new DualGraphType());
		m_SkeletonGraph=boost::shared_ptr<PrimalGraphType>(new PrimalGraphType());

	}

	void InvalidateDual(){
		m_CellGraph=boost::shared_ptr<DualGraphType>(new DualGraphType());
		this->ClearPerimeter();
	}

	/**
	 * Method for class serialization
	 */
#if 0
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & *m_CellGraph & *m_SkeletonGraph;
	}
#endif

	/**
	 * Return the number of cells stored in the tissue
	 */
	inline int GetNumCells() {
		return boost::num_vertices(*m_CellGraph);
	}

	/**
	 * Primal Graph data structure -> Intercellular skeleton
	 */
	boost::shared_ptr<PrimalGraphType>  m_SkeletonGraph;

	/**
	 * Dual Graph data structure -> Cells defined by the intercellular skeleton
	 */
	boost::shared_ptr<DualGraphType> m_CellGraph;

	void AddVertexToPerimeter(const PrimalGraphVertexDescriptorType & vertex){
		m_Perimeter.push_back(vertex);
	}

	typename std::vector<PrimalGraphVertexDescriptorType >::iterator BeginPerimeter(){
		return m_Perimeter.begin();
	}

	typename std::vector<PrimalGraphVertexDescriptorType >::iterator EndPerimeter(){
		return m_Perimeter.end();
	}

	void ClearPerimeter(){
		m_Perimeter.clear();
	}
	int PerimeterSize(){
		return m_Perimeter.size();
	}

private:

	std::vector<PrimalGraphVertexDescriptorType > m_Perimeter;
};




typedef TemplateTissueDescriptor<SkeletonGraph,CellGraph> TissueDescriptor;
typedef TemplateTissueDescriptor<SkeletonGraph,TrackedCellGraph> TrackedTissueDescriptor;


template<> struct TissueDescriptorTraits<TissueDescriptor>{
	typedef ttt::SkeletonVertexType SkeletonVertexType;
	typedef ttt::SkeletonPointPropertyTag SkeletonPointPropertyTag;
	typedef ttt::SkeletonEdgeType SkeletonEdgeType;
	typedef ttt::SkeletonPointProperty SkeletonPointPropertyType;

	typedef ttt::CellVertexType CellVertexType;
	typedef ttt::CellPropertyTag CellPropertyTagType;
	typedef ttt::CellEdgeType CellEdgeType;
	typedef ttt::CellProperty CellPropertyType;
	typedef ttt::Cell CellType;
};

template<> struct TissueDescriptorTraits<TrackedTissueDescriptor>{
	typedef ttt::SkeletonVertexType SkeletonVertexType;
	typedef ttt::SkeletonPointPropertyTag SkeletonPointPropertyTag;
	typedef ttt::SkeletonEdgeType SkeletonEdgeType;
	typedef ttt::SkeletonPointProperty SkeletonPointProperty;

	typedef ttt::TrackedCellVertexType CellVertexType;
	typedef ttt::TrackedCellPropertyTag CellPropertyTagType;
	typedef ttt::TrackedCellEdgeType CellEdgeType;
	typedef ttt::TrackedCellProperty CellProperty;

	typedef ttt::TrackedCell CellType;
};

TrackedTissueDescriptor::DualGraphVertexDescriptorType CellID2VertexDescriptor(int ID,const TrackedTissueDescriptor::Pointer & descriptor);

std::pair<ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType,ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType> CellParentID2VertexDescriptor(int ID,const ttt::TrackedTissueDescriptor::Pointer & descriptor);


/**
 * Return a duplicate of a TissueDescriptor
 */
TissueDescriptor::Pointer cloneTissueDescriptor(const TissueDescriptor::Pointer & descriptor);



template<class CellType> void CellUnion(const CellType & a, const CellType & b,CellType & result){

	typedef typename CellType::PerimeterIterator PerimeterIterator;

	PerimeterIterator itA = a.PerimeterBegin();

	typename CellType::PerimeterIterator itAEnd = a.PerimeterEnd();

	PerimeterIterator itB = b.PerimeterBegin();
	PerimeterIterator itBEnd = b.PerimeterEnd();

	std::vector<ttt::SkeletonVertexType> pointsInA;
	pointsInA.insert(pointsInA.begin(),itA,itAEnd);
	std::sort(pointsInA.begin(),pointsInA.end());

	std::vector<ttt::SkeletonVertexType> pointsInB;
	pointsInB.insert(pointsInB.begin(),itB,itBEnd);
	std::sort(pointsInB.begin(),pointsInB.end());

	std::vector<ttt::SkeletonVertexType> common(2);
	std::set_intersection(pointsInA.begin(),pointsInA.end(),pointsInB.begin(),pointsInB.end(),common.begin());

	assert(common.size()==2);

	ttt::SkeletonVertexType point1 = common[0];
	ttt::SkeletonVertexType point2 = common[1];


	PerimeterIterator loc1A = std::find(itA,itAEnd,point1);
	PerimeterIterator loc2A	=std::find(itA,itAEnd,point2);

	assert(loc1A!=itAEnd);
	assert(loc2A!=itAEnd);

	bool aClockwise;
	PerimeterIterator startA, endA;

	startA=loc2A;
	endA=loc1A;

	if(loc2A==loc1A+1){
		aClockwise=1;
	}else if(loc1A==loc2A+1){
		aClockwise=0;
	}else if(loc1A==itA && loc2A==itAEnd-1){
		aClockwise=0;
	}else if(loc1A==itAEnd-1 && loc2A==itA){
		aClockwise=1;
	}

	PerimeterIterator loc1B = std::find(itB,itBEnd,point1);
	PerimeterIterator loc2B	=std::find(itB,itBEnd,point2);
	PerimeterIterator startB, endB;
	assert(loc1B!=itBEnd);
	assert(loc2B!=itBEnd);

	bool bClockwise;
	startB=loc1B;
	endB=loc2B;
	if(loc2B==loc1B+1){
		bClockwise=0;
	}else if(loc1B==loc2B+1){
		bClockwise=1;
	}else if(loc1B==itB && loc2B==itBEnd-1){
		bClockwise=1;
	}else if(loc1B==itBEnd-1 && loc2B==itB){
		bClockwise=0;
	}



	PerimeterIterator perimIter=startA;
	while(perimIter!=endA){

		result.AddVertexToPerimeter(*perimIter);

		if(aClockwise){
			++perimIter;

			if(perimIter==itAEnd){
				perimIter=itA;
			}
		}else{
			if(perimIter==itA){
				perimIter=itAEnd;
			}
			--perimIter;
		}
	}
	perimIter=startB;
	while(perimIter!=endB){


		result.AddVertexToPerimeter(*perimIter);

		if(bClockwise){
			++perimIter;

			if(perimIter==itBEnd){
				perimIter=itB;
			}
		}else{
			if(perimIter==itB){
				perimIter=itBEnd;
			}
			--perimIter;
		}
	}


}

/**
 * Return all the domains in a tissue
 */

template<class TissueDescriptorType,class DomainType> void GetDomains(const typename TissueDescriptorType::Pointer & descriptor, std::vector<DomainType> & domains, unsigned int order){

	domains.clear();
	domains.resize(descriptor->GetNumCells());

	std::cout << boost::num_edges(*descriptor->m_CellGraph);
	int k=0;
	BGL_FORALL_VERTICES_T(v,*descriptor->m_CellGraph,typename TissueDescriptorType::DualGraphType){
		typedef boost::graph_traits<typename TrackedTissueDescriptor::DualGraphType>::vertex_descriptor VertexType;
		typedef std::set<VertexType > ExplorationSet; //TODO fix to employ VertexType
		ExplorationSet open,next,visited;
		int currentOrder=order;
		domains[k].SetNucleus(v);
		next.insert(v);
		domains[k].SetOrder(order);
		while(currentOrder>0){
			open=next;
			next.clear();
			while(!open.empty()){
				VertexType current=*open.begin();
				open.erase(open.begin());

				if(!visited.count(current)){
					visited.insert(current);
					typename boost::graph_traits < typename TissueDescriptorType::DualGraphType >::adjacency_iterator vi, vi_end;
					for (boost::tie(vi, vi_end) = boost::adjacent_vertices(current, *descriptor->m_CellGraph); vi != vi_end; ++vi){
						next.insert(*vi);
						domains[k].AddCell(*vi);
					}
				}
			}
			currentOrder--;
		}
		k++;
	}

	for(unsigned int i=0;i<domains.size();i++){
		std::cout << domains[i].GetNucleus() << "\t";
		for(std::set<CellVertexType>::iterator it = domains[i].Begin(); it!=domains[i].End();++it){
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}

}

#if 0
class TrackedTissueDescriptor: public itk::Object {
public:
	typedef TrackedTissueDescriptor Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	TrackedCellGraph m_TrackedCellGraph;

	SkeletonGraph m_SkeletonGraph;
	itkNewMacro(TrackedTissueDescriptor)
	;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & m_TrackedCellGraph & m_SkeletonGraph;
	}

	int GetNumCells() {
		return boost::num_vertices(m_TrackedCellGraph);
	}

	TrackedCell GetCell(int i) {

		//TODO
	}
};
#endif

#if 0


x

#if 0
typedef boost::adjacency_list<boost::vecS,
boost::vecS,
boost::undirectedS,
SkeletonPoint>
SkeletonGraph;
#endif

#if 0

typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS,
		ttt::SkeletonPointProperty, boost::property<boost::edge_index_t, int> > SkeletonGraph_listS;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		ttt::SkeletonPointProperty, boost::property<boost::edge_index_t, int> > SkeletonGraph_simple;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		ttt::SkeletonPointProperty, boost::property<boost::edge_index_t, int> > SkeletonGraph_def;
#endif

#if 0
typedef boost::graph_traits<ttt::SkeletonGraph_listS>::vertex_descriptor SkeletonVertexType_lS;
typedef boost::graph_traits<ttt::SkeletonGraph_simple>::vertex_descriptor SkeletonVertexType_sim;
typedef boost::graph_traits<ttt::SkeletonGraph_def>::vertex_descriptor SkeletonVertexType_def;
#endif






#if 0

#endif

struct EllipsePropertyTag {

	typedef boost::vertex_property_tag kind;
};

//typedef boost::property<EllipsePropertyTag,Ellipse, boost::property<boost::vertex_index_t,int> > EllipseProperty;


//typedef boost::property<CellPropertyTag, Cell,boost::property<EllipsePropertyTag, EllipseG,	boost::property<boost::vertex_index_t, int> > > CellProperty;





//typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS,		CellProperty, boost::property<boost::edge_index_t, int> > CellGraph_listS;












TrackedTissueDescriptor::Pointer cloneTrackedTissueDescriptor(const TrackedTissueDescriptor::Pointer & descriptor);

#if 0
void Store(const TissueDescriptor & descriptor, const std::string & fileName) {

	// Save data

	{
		// Create an output archive
		std::ofstream ofs(fileName.c_str());
		boost::archive::text_oarchive ar(ofs);

		// Write data
		ar & descriptor;
	}
}
#endif
}

#endif
}//namespace TTT
/** @}*/
