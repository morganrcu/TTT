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
#include "itkMacro.h"

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

template<int dim = 3> class AdherensJunctionVertex : public itk::DataObject{
public:
	/**
	 * Location of the point in pixel coordinates
	 */
	typedef itk::Index<dim> IndexType;
	typedef AdherensJunctionVertex<dim> Self;
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


template<int dim =3 > class AdherensJunctionVertices : public itk::DataObject, public std::vector<typename AdherensJunctionVertex<dim>::Pointer>{
public:
	typedef AdherensJunctionVertices<dim> Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	typedef itk::DataObject Superclass;
	typedef AdherensJunctionVertex<dim> VertexType;
	itkTypeMacro(Self,Superclass);
	itkNewMacro(Self);
protected:
	/**
	 * Default constructor. The attributes are fixed to default values
	 */
	AdherensJunctionVertices() : std::vector<typename VertexType::Pointer>(0){

	}

private:
	AdherensJunctionVertices(const Self &);
};
template<int dim> class TissueDescriptor;
template<class T> struct TissueDescriptorTraits{};
/**
 * \brief Class to represent a point in the adherens junction graph
 */

template<int dim=3> class SkeletonPoint {
public:
	/**
	 * Location of the point in pixel coordinates
	 */
	typedef itk::Point<float, dim> PointType;



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

	SkeletonPoint(const PointType & position) {
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
template<int dim> class SkeletonPointPropertyTag {
public:
	typedef boost::vertex_property_tag kind;
};

/**
 * Auxiliary class to specify the graph storage
 */
typedef boost::property<ttt::SkeletonPointPropertyTag<2>, ttt::SkeletonPoint<2>, boost::property<boost::vertex_index_t, int> > SkeletonPointProperty2D;
typedef boost::property<ttt::SkeletonPointPropertyTag<3>, ttt::SkeletonPoint<3>, boost::property<boost::vertex_index_t, int> > SkeletonPointProperty3D;

//template<int dim> struct SkeletonPointProperty : public boost::property<ttt::SkeletonPointPropertyTag<dim>, ttt::SkeletonPoint<dim>, boost::property<boost::vertex_index_t, int> >{};

/**
 * SkeletonPoint storage. The graph structure represents the intercellular skeleton
 */
#if 0
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		ttt::SkeletonPointProperty, boost::property<boost::edge_index_t, int> > SkeletonGraph;
#endif

//typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,
//		ttt::SkeletonPointProperty, boost::property<boost::edge_index_t, int> > SkeletonGraph;

template<int dim> class SkeletonGraph;

template<> class SkeletonGraph<2> : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,ttt::SkeletonPointProperty2D, boost::property<boost::edge_index_t, int> > {
public:
	const static int NumDimensions=2;
	typedef SkeletonGraph<2> Self;
	typedef typename ttt::SkeletonPoint<2>::PointType PointType;

	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,ttt::SkeletonPointProperty2D, boost::property<boost::edge_index_t, int> > Superclass;

	typedef ttt::SkeletonPointProperty2D  SkeletonPointProperty;

	void AddVertex(const PointType & pt){
		 ttt::SkeletonPoint<2> point(pt);
		boost::add_vertex(point,*this);

	}

protected:

private:


};

template<> class SkeletonGraph<3> : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,ttt::SkeletonPointProperty3D, boost::property<boost::edge_index_t, int> > {
public:
	const static int NumDimensions=3;
	typedef SkeletonGraph<3> Self;
	typedef typename ttt::SkeletonPoint<3>::PointType PointType;

	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,ttt::SkeletonPointProperty3D, boost::property<boost::edge_index_t, int> > Superclass;

	typedef ttt::SkeletonPointProperty3D  SkeletonPointProperty;

	void AddVertex(const PointType & pt){
		 ttt::SkeletonPoint<3> point(pt);
		boost::add_vertex(point,*this);

	}

protected:

private:


};
/**
 * Type of the vertices in the graph_traits
 */
//typedef boost::graph_traits<ttt::SkeletonGraph>::vertex_descriptor SkeletonVertexType;
//template<int dim> struct SkeletonVertexType : boost::graph_traits<typename ttt::SkeletonGraph<dim> >::vertex_descriptor;
//template<int dim> struct SkeletonVertexType : boost::graph_traits<typename ttt::SkeletonGraph<dim> >::vertex_descriptor;

/**
 * Type of the edges in the graph_traits
 */
//typedef boost::graph_traits<ttt::SkeletonGraph>::edge_descriptor SkeletonEdgeType;
//template<int dim> struct SkeletonEdgeType : public boost::graph_traits<ttt::SkeletonGraph<dim> >::edge_descriptor {};


/**
 * Class to represent a Cell in a Tissue
 */
template<int dim> class Cell {
public:

	const static int NumDimensions = dim;
	/**
	 * Position of the centroid of the cell, in Real World? Coordinates
	 */
	typedef itk::Point<double, dim> Point;

	typedef typename boost::graph_traits<ttt::SkeletonGraph<dim> >::vertex_descriptor SkeletonVertexType;
	typedef typename std::vector<SkeletonVertexType>::const_iterator PerimeterIterator;

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

	std::vector< SkeletonVertexType > m_SkeletonNodes;
	/**
	 * Cell centroid location
	 */
	Point m_Centroid;
};

/**
 * Definition of the Cell Properties as vertex properties
 */
template<int dim> struct CellPropertyTag {

	typedef boost::vertex_property_tag kind;
};
/**
 * Cell Property definition
 */
//typedef boost::property<CellPropertyTag, Cell,boost::property<boost::vertex_index_t, int> >  CellProperty;

typedef boost::property<CellPropertyTag<2>, Cell<2>,boost::property<boost::vertex_index_t, int> >  CellProperty2D;
typedef boost::property<CellPropertyTag<3>, Cell<3>,boost::property<boost::vertex_index_t, int> >  CellProperty3D;


//typedef boost::property<ttt::SkeletonPointPropertyTag<2>, ttt::SkeletonPoint<2>, boost::property<boost::vertex_index_t, int> > SkeletonPointProperty2D;
//typedef boost::property<ttt::SkeletonPointPropertyTag<3>, ttt::SkeletonPoint<3>, boost::property<boost::vertex_index_t, int> > SkeletonPointProperty3D;
/**
 * Cell Graph definition
 */
#if 0
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, CellProperty, boost::property<boost::edge_index_t, int> > CellGraph;
#endif

template<int dim> class CellGraph;

//typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, CellProperty, boost::property<boost::edge_index_t, int> > CellGraph;
template<> struct CellGraph<2> : public boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, CellProperty2D, boost::property<boost::edge_index_t, int> >{
	typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, CellProperty2D, boost::property<boost::edge_index_t, int> > GraphType;
	typedef typename boost::graph_traits<GraphType>::vertex_descriptor CellVertexType;

};
template<> struct CellGraph<3> : public boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, CellProperty3D, boost::property<boost::edge_index_t, int> >{
	typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, CellProperty3D, boost::property<boost::edge_index_t, int> > GraphType;
	typedef  typename boost::graph_traits<GraphType>::vertex_descriptor CellVertexType;

};


template<int dim> class TrackedTissueDescriptor;

//typedef boost::graph_traits<CellGraph>::vertex_descriptor CellVertexType;
//template<int dim> struct CellVertexType : public boost::graph_traits<CellGraph<dim> >::vertex_descriptor{};

//template<int dim> struct CellEdgeType : public boost::graph_traits<CellGraph<dim> >::edge_descriptor{};
/**
 * Class extending Cell class to represent tracked Cells
 */

template<int dim> class TrackedCell: public Cell<dim> {
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
	itk::Vector<double, dim> m_Velocity;
	/**
	 *
	 */
	//FIXME
	typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType m_ObservedCell;
	//unsigned int m_ObservedCell;

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
	itk::Vector<double,dim> GetVelocity(){
		return m_Velocity;
	}
	void SetVelocity(const itk::Vector<double,dim> & velocity){
		m_Velocity=velocity;
	}

	void SetObservedCell(int observedCell){
		m_ObservedCell=observedCell;
	}

	int GetObservedCell(){
		return m_ObservedCell;
	}
#if 0

	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<Cell>(*this);
		ar & m_ID;
		ar & m_Velocity;
		ar & m_ParentID;
	}
#endif
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
	TrackedCell(const TrackedCell<dim> & other) : Cell<dim>(other) {
		this->m_Velocity = other.m_Velocity;
		this->m_ID = other.m_ID;
		this->m_ParentID=other.m_ParentID;
		this->m_ObservedCell=other.m_ObservedCell;
	}

	/**
	 * Copy constructor. Copies fields from the other cell
     */
	TrackedCell(const Cell<dim> & other) : Cell<dim>(other) {
		this->m_Velocity.Fill(0);
		this->m_ID = -1;
		this->m_ParentID=-1;
		this->m_ObservedCell=-1;
	}
};

template<int dim> struct TrackedCellPropertyTag {
	typedef boost::vertex_property_tag kind;
};

//typedef boost::property<TrackedCellPropertyTag, TrackedCell, boost::property<boost::vertex_index_t, int> > TrackedCellProperty;
//template<int dim> struct TrackedCellProperty : public boost::property<TrackedCellPropertyTag<dim>, TrackedCell<dim>, boost::property<boost::vertex_index_t, int> > {};
typedef boost::property<TrackedCellPropertyTag<2>, TrackedCell<2>, boost::property<boost::vertex_index_t, int> > TrackedCellProperty2D;
typedef boost::property<TrackedCellPropertyTag<3>, TrackedCell<3>, boost::property<boost::vertex_index_t, int> > TrackedCellProperty3D;

//typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,TrackedCellProperty, boost::property<boost::edge_index_t, int> > TrackedCellGraph;
//template<int dim> struct TrackedCellGraph : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,TrackedCellProperty<dim>, boost::property<boost::edge_index_t, int> >{};

template<int dim> struct TrackedCellGraph;
template<> struct TrackedCellGraph<2> : public boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, TrackedCellProperty2D, boost::property<boost::edge_index_t, int> >{};
template<> struct TrackedCellGraph<3> : public boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, TrackedCellProperty3D, boost::property<boost::edge_index_t, int> >{};

//typedef boost::graph_traits<TrackedCellGraph>::vertex_descriptor TrackedCellVertexType;
//template<int dim> struct TrackedCellVertexType : public boost::graph_traits<TrackedCellGraph<dim> >::vertex_descriptor{};
//template<int dim> struct TrackedCellEdgeType : public  boost::graph_traits<TrackedCellGraph<dim> >::edge_descriptor{};
//typedef boost::graph_traits<TrackedCellGraph>::edge_descriptor TrackedCellEdgeType;



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

//template<int dim> struct  TrackedDomain : public Domain<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType >{};

//template<class T> struct TissueDescriptorTraits{
//typedef void SkeletonVertexType;
//typedef void SkeletonEdgeType;
//};


/**
 * Template Class to represent a Tissue, composed by a primal graph and a dual graph. Primal graph represents intercellular skeleton, Dual graph represents cells
 */
template <class TPrimalGraph, class TDualGraph> class TemplateTissueDescriptor: public itk::DataObject {
public:

	const static int NumDimensions = TPrimalGraph::NumDimensions;

	typedef TemplateTissueDescriptor Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TPrimalGraph PrimalGraphType;
	typedef typename boost::graph_traits<PrimalGraphType>::vertex_descriptor PrimalGraphVertexDescriptorType;

	typedef TDualGraph DualGraphType;
	typedef typename boost::graph_traits<DualGraphType>::vertex_descriptor DualGraphVertexDescriptorType;

	itkNewMacro(TemplateTissueDescriptor);

	TemplateTissueDescriptor(){
		//m_CellGraph=boost::shared_ptr<DualGraphType>(new DualGraphType());
		//m_SkeletonGraph=boost::shared_ptr<PrimalGraphType>(new PrimalGraphType());

	}

	TPrimalGraph & GetAJGraph(){
		return this->m_AJGraph;
	}

	void SetAJGraph(TPrimalGraph & graph){
		this->m_AJGraph=graph;
	}

	TDualGraph & GetCellGraph(){
			return this->m_CellGraph;
	}

	void InvalidateDual(){
		//m_CellGraph=boost::shared_ptr<DualGraphType>(new DualGraphType());
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
		return boost::num_vertices(m_CellGraph);
	}

private:
	/**
	 * Primal Graph data structure -> Intercellular skeleton
	 */
	//boost::shared_ptr<PrimalGraphType>  m_SkeletonGraph;
	PrimalGraphType m_AJGraph;

	/**
	 * Dual Graph data structure -> Cells defined by the intercellular skeleton
	 */
	//boost::shared_ptr<DualGraphType> m_CellGraph;
	DualGraphType m_CellGraph;
public:
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




//typedef TemplateTissueDescriptor<SkeletonGraph,CellGraph> TissueDescriptor;
template<int dim> struct TissueDescriptor : public TemplateTissueDescriptor<SkeletonGraph<dim>,CellGraph<dim> >{};
//typedef TemplateTissueDescriptor<SkeletonGraph,TrackedCellGraph> TrackedTissueDescriptor;
template<int dim> struct TrackedTissueDescriptor : public TemplateTissueDescriptor<SkeletonGraph<dim>,TrackedCellGraph<dim> >{};


template<> struct TissueDescriptorTraits<TissueDescriptor<2> >{
	//typedef ttt::SkeletonVertexType<dim> SkeletonVertexType;
	typedef typename boost::graph_traits<ttt::SkeletonGraph<2> >::vertex_descriptor SkeletonVertexType;
	//typedef ttt::SkeletonPointPropertyTag SkeletonPointPropertyTag;
	typedef typename boost::graph_traits<ttt::SkeletonGraph<2> >::edge_descriptor SkeletonEdgeType;


	typedef typename boost::graph_traits<ttt::CellGraph<2> >::edge_descriptor CellEdgeType;
	typedef typename boost::graph_traits<ttt::CellGraph<2> >::vertex_descriptor CellVertexType;
	//typedef ttt::CellVertexType<2> CellVertexType;
	typedef ttt::CellPropertyTag<2> CellPropertyTagType;
	//typedef ttt::CellEdgeType<2> CellEdgeType;
	//typedef ttt::CellProperty<2> CellPropertyType;
	typedef ttt::Cell<2> CellType;

	typedef ttt::SkeletonPointProperty2D SkeletonPointPropertyType;
};

template<> struct TissueDescriptorTraits<TissueDescriptor<3> >{

	//typedef ttt::SkeletonVertexType<dim> SkeletonVertexType;
	typedef typename boost::graph_traits<ttt::SkeletonGraph<3> >::vertex_descriptor SkeletonVertexType;
	typedef typename boost::graph_traits<ttt::SkeletonGraph<3> >::edge_descriptor SkeletonEdgeType;
	//typedef ttt::SkeletonPointPropertyTag SkeletonPointPropertyTag;
	//typedef ttt::SkeletonEdgeType<3> SkeletonEdgeType;


	typedef typename boost::graph_traits<ttt::CellGraph<3> >::edge_descriptor CellEdgeType;
	typedef typename boost::graph_traits<ttt::CellGraph<3> >::vertex_descriptor CellVertexType;


	typedef ttt::CellPropertyTag<3> CellPropertyTagType;

	typedef ttt::CellProperty3D CellPropertyType;
	typedef ttt::Cell<3> CellType;

	typedef ttt::SkeletonPointProperty3D SkeletonPointPropertyType;
};

template<int dim> struct TissueDescriptorTraits<TrackedTissueDescriptor<dim> >{
	typedef  typename boost::graph_traits<ttt::SkeletonGraph<dim> >::vertex_descriptor SkeletonVertexType;
	//typedef ttt::SkeletonPointPropertyTag SkeletonPointPropertyTag;



	//typedef ttt::TrackedCellVertexType<dim> CellVertexType;

	//typedef ttt::TrackedCellEdgeType<dim> CellEdgeType;
	//typedef ttt::TrackedCellProperty<dim> CellProperty;


};

template<> struct TissueDescriptorTraits<TrackedTissueDescriptor<2> >{
	typedef ttt::SkeletonPointProperty2D SkeletonPointPropertyType;
	typedef typename boost::graph_traits<ttt::TrackedCellGraph<2> >::vertex_descriptor CellVertexType;
	typedef  typename boost::graph_traits<ttt::SkeletonGraph<2> >::edge_descriptor SkeletonEdgeType;

	typedef ttt::TrackedCellPropertyTag<2> CellPropertyTagType;
	typedef ttt::SkeletonPointPropertyTag<2> SkeletonPointPropertyTagType;
};
template<> struct TissueDescriptorTraits<TrackedTissueDescriptor<3> >{
	typedef ttt::SkeletonPointProperty3D SkeletonPointPropertyType;
	typedef ttt::SkeletonPointPropertyTag<3> SkeletonPointPropertyTagType;
	typedef typename boost::graph_traits<ttt::TrackedCellGraph<3> >::vertex_descriptor CellVertexType;
	typedef typename boost::graph_traits<ttt::TrackedCellGraph<3> >::edge_descriptor CellEdgeType;

	typedef typename boost::graph_traits<ttt::SkeletonGraph<3> >::vertex_descriptor SkeletonVertexType;
	typedef  typename boost::graph_traits<ttt::SkeletonGraph<3> >::edge_descriptor SkeletonEdgeType;
	typedef ttt::TrackedCell<3> CellType;
	typedef ttt::TrackedCellPropertyTag<3> CellPropertyTagType;


};

template<int dim> typename TrackedTissueDescriptor<dim>::DualGraphVertexDescriptorType CellID2VertexDescriptor(int ID,const typename TrackedTissueDescriptor<dim>::Pointer & descriptor);

template<int dim> std::pair<typename ttt::TrackedTissueDescriptor<dim>::DualGraphVertexDescriptorType,typename ttt::TrackedTissueDescriptor<dim>::DualGraphVertexDescriptorType> CellParentID2VertexDescriptor(int ID,const typename ttt::TrackedTissueDescriptor<dim>::Pointer & descriptor);


/**
 * Return a duplicate of a TissueDescriptor
 */
template<int dim> typename TissueDescriptor<dim>::Pointer cloneTissueDescriptor(const typename TissueDescriptor<dim>::Pointer & descriptor);



template<class CellType> void CellUnion(const CellType & a, const CellType & b,CellType & result){

	typedef typename CellType::PerimeterIterator PerimeterIterator;

	PerimeterIterator itA = a.PerimeterBegin();

	typename CellType::PerimeterIterator itAEnd = a.PerimeterEnd();

	PerimeterIterator itB = b.PerimeterBegin();
	PerimeterIterator itBEnd = b.PerimeterEnd();

	std::vector<typename CellType::SkeletonVertexType > pointsInA;
	pointsInA.insert(pointsInA.begin(),itA,itAEnd);
	std::sort(pointsInA.begin(),pointsInA.end());

	std::vector<typename CellType::SkeletonVertexType  > pointsInB;
	pointsInB.insert(pointsInB.begin(),itB,itBEnd);
	std::sort(pointsInB.begin(),pointsInB.end());

	std::vector<typename CellType::SkeletonVertexType > common(2);
	std::set_intersection(pointsInA.begin(),pointsInA.end(),pointsInB.begin(),pointsInB.end(),common.begin());

	assert(common.size()==2);

	typename CellType::SkeletonVertexType point1 = common[0];
	typename CellType::SkeletonVertexType point2 = common[1];


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

		typedef typename boost::graph_traits<typename TissueDescriptorType::DualGraphType>::vertex_descriptor VertexType;
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
		for(typename std::set<typename TissueDescriptorType::CellVertexType>::iterator it = domains[i].Begin(); it!=domains[i].End();++it){
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
#include "tttDescriptionDataTypes.hxx"
/** @}*/
