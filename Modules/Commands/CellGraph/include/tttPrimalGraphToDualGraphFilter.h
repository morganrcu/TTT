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

/** \addtogroup TTTCellGraph
 *  @{
 */
#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/planar_face_traversal.hpp>
//#include "itkImageTransformer.h"
#include "itkSmartPointer.h"
#include "itkImageRegionIterator.h"
#include "itkPolygonSpatialObject.h"

#include "tttDescriptionDataTypes.h"
//#include "CellCentroid.h"
#include <queue>
//#include <set>

namespace ttt {

/**
 * \class CentroidGraphCalculator
 * TODO
 */

template<class InputGraph, class OutputGraph, class EdgeIndexMap> class CentroidGraphCalculator: public boost::planar_face_traversal_visitor {

public:

	const static int NumDimensions = InputGraph::NumDimensions;
	/**
	 * TODO
	 */
	typedef typename boost::graph_traits<OutputGraph>::vertex_descriptor vertex_t;
	/**
	 * TODO
	 */
	typedef typename boost::graph_traits<InputGraph>::edge_descriptor edge_t;
	/**
	 * TODO
	 */
	typedef typename std::vector<vertex_t> vertex_vector_t;
	/**
	 * TODO
	 */
	typedef boost::iterator_property_map<typename vertex_vector_t::iterator,
			EdgeIndexMap> edge_to_face_map_t;

	/**
	 * TODO
	 * @param arg_g
	 * @param arg_dual_g
	 * @param arg_em
	 */
	CentroidGraphCalculator(InputGraph & arg_g, OutputGraph & arg_dual_g,
			EdgeIndexMap arg_em) :
			m_Graph(arg_g), m_Dual(arg_dual_g), m_EM(arg_em), m_EdgeToFaceVector(
					boost::num_edges(m_Graph),
					boost::graph_traits<OutputGraph>::null_vertex()) {
		m_Total = -1;
		m_CurrentVertex = -1;
		m_CurrentCentroid.Fill(0);
		m_EdgeToFace = new edge_to_face_map_t(m_EdgeToFaceVector.begin(), m_EM);

	}
	/**
	 * TODO
	 */
	void begin_face() {
		//std::cout << "Comenzando nueva cara ";


			m_CurrentVertex = boost::add_vertex(Cell<NumDimensions>(), m_Dual);

		m_CurrentCentroid.Fill(0);
		//std::cout << m_CurrentVertex << std::endl;
		m_Total = 0;
	}

	/**
	 * TODO
	 */
	void end_face() {
		for(int i=0;i<NumDimensions;i++){
			m_CurrentCentroid[i]/=m_Total;
		}
		boost::get(CellPropertyTag<NumDimensions>(), m_Dual, m_CurrentVertex).SetCentroid(m_CurrentCentroid);
		//std::cout << "Centroide en: " << m_CurrentCentroid	<< std::endl;
	}

	/**
	 * TODO
	 * @param v
	 */
	template<typename Vertex>
	void next_vertex(Vertex v) {

		if(boost::degree(v,m_Graph)>1){
			for(int i=0;i<NumDimensions;i++){
				m_CurrentCentroid[i] +=
									boost::get(SkeletonPointPropertyTag<NumDimensions>(), m_Graph, v).position[i];
			}

			boost::get(CellPropertyTag<NumDimensions>(), m_Dual, m_CurrentVertex).AddVertexToPerimeter(
					v);
			m_Total++;
		}


	}

	/**
	 * TODO
	 * @param e
	 */
	template<typename Edge>
	void next_edge(Edge e) {
		vertex_t existing_face = (*m_EdgeToFace)[e];
		if(existing_face!=m_CurrentVertex){
			if (existing_face == boost::graph_traits<OutputGraph>::null_vertex()) {
				(*m_EdgeToFace)[e] = m_CurrentVertex;
			} else {
				boost::add_edge(existing_face, m_CurrentVertex, m_Dual);
				//std::cout << "Conectando " << existing_face << " con " << m_CurrentVertex << std::endl;
			}
		}
	}

private:

	/**
	 * TODO
	 */
	typename OutputGraph::CellVertexType m_CurrentVertex;

	/**
	 *
	 */
	itk::Point<double,NumDimensions> m_CurrentCentroid;
	/**
	 *
	 */
	int m_Total;

	/**
	 * TODO
	 */
	InputGraph & m_Graph;
	/**
	 * TODO
	 */
	OutputGraph & m_Dual;
	/**
	 * TODO
	 */
	EdgeIndexMap m_EM;
	/**
	 * TODO
	 */
	edge_to_face_map_t * m_EdgeToFace;
	/**
	 * TODO
	 */
	vertex_vector_t m_EdgeToFaceVector;
};

/**
 * TODO
 * @param g
 * @param dual_g
 * @param embedding
 * @param em
 */
template<typename InputGraph, typename OutputGraph, typename PlanarEmbedding,
		typename EdgeIndexMap>
void create_dual_graph(InputGraph& g, OutputGraph& dual_g,
		PlanarEmbedding embedding, EdgeIndexMap em) {
	CentroidGraphCalculator<InputGraph, OutputGraph, EdgeIndexMap> visitor(g,
			dual_g, em);
	boost::planar_face_traversal(g, embedding, visitor, em);

}

/**
 * TODO
 * @param g
 * @param dual_g
 * @param embedding
 */
template<typename InputGraph, typename OutputGraph, typename PlanarEmbedding>
void create_dual_graph(InputGraph& g, OutputGraph& dual_g,
		PlanarEmbedding embedding) {
	create_dual_graph(g, dual_g, embedding, boost::get(boost::edge_index, g));
}

/**
 * \class PrimalGraphToDualGraphFilter
 * TODO
 */
template<class TTissueDescriptor> class PrimalGraphToDualGraphFilter: public itk::ProcessObject {
public:
	static const int NumDimensions = TTissueDescriptor::NumDimensions;
	/**
	 * TODO
	 */
	typedef PrimalGraphToDualGraphFilter Self;
	/**
	 * TODO
	 */
	typedef itk::SmartPointer<Self> Pointer;
	/**
	 * TODO
	 */
	typedef itk::SmartPointer<const Self> ConstPointer;
	/**
	 * TODO
	 */
	typedef TTissueDescriptor TissueDescriptorType;

	/**
	 * TODO
	 */
	void GenerateData();
	/**
	 * TODO
	 */

	typename TissueDescriptorType::Pointer m_TissueDescriptor;
	/**
	 * TODO
	 * @param inputTissueDescriptor
	 */
	virtual void SetInput(
			const typename TissueDescriptorType::Pointer & inputTissueDescriptor) {
		m_TissueDescriptor = inputTissueDescriptor;
	}
	/**
	 * TODO
	 */
	inline typename TissueDescriptorType::Pointer GetOutput() {
		return m_TissueDescriptor;

	}
	/**
	 * TODO
	 */
	itkNewMacro(PrimalGraphToDualGraphFilter)
	;
protected:
	/**
	 * TODO
	 */
	virtual ~PrimalGraphToDualGraphFilter() {
	}
private:
	/**
	 * TODO
	 */
	PrimalGraphToDualGraphFilter() {
	}

};

template<class TTissueDescriptor> void PrimalGraphToDualGraphFilter<TTissueDescriptor>::GenerateData() {
	//dual graph
	{

		typename boost::property_map<typename TTissueDescriptor::PrimalGraphType, boost::edge_index_t>::type e_index =
				boost::get(boost::edge_index,m_TissueDescriptor->GetAJGraph());
		//typename boost::property_map<CellGraph, EllipsePropertyTag>::type name = boost::get(EllipsePropertyTag(), m_TissueDescriptor->m_CellGraph);
		typename boost::graph_traits<typename TTissueDescriptor::PrimalGraphType>::edges_size_type edge_count =
				0;
		typename boost::graph_traits<typename TTissueDescriptor::PrimalGraphType>::edge_iterator ei, ei_end;

		for (boost::tie(ei, ei_end) = boost::edges(
				m_TissueDescriptor->GetAJGraph()); ei != ei_end; ++ei) {
			boost::put(e_index, *ei, edge_count++);
		}

		//std::cout << "Num edges: " << edge_count << std::endl;

		typedef std::vector<
				typename boost::graph_traits<typename TTissueDescriptor::PrimalGraphType>::edge_descriptor> vec_t;

		std::vector<vec_t> embedding(
				boost::num_vertices(m_TissueDescriptor->GetAJGraph()));

		std::vector<vec_t> myembedding(
						boost::num_vertices(m_TissueDescriptor->GetAJGraph()));

		if (boost::boyer_myrvold_planarity_test(
				boost::boyer_myrvold_params::graph =
						m_TissueDescriptor->GetAJGraph(),
				boost::boyer_myrvold_params::embedding = &embedding[0]))
			std::cout << "Input graph is planar" << std::endl;
		else{
			std::cout << "Input graph is not planar" << std::endl;
			return;
		}


		//TODO create my embedding



		BGL_FORALL_VERTICES_T(v,m_TissueDescriptor->GetAJGraph(),TTissueDescriptor::PrimalGraphType){
//			embedding[v].clear();
			if (boost::degree(v,m_TissueDescriptor->GetAJGraph())<3){
				typename boost::graph_traits<typename TTissueDescriptor::PrimalGraphType>::out_edge_iterator oi,  oi_end;
				for (boost::tie(oi, oi_end) = boost::out_edges(v,m_TissueDescriptor->GetAJGraph()); oi != oi_end; ++oi) {
					myembedding[v].push_back(*oi);
				}
			}else if (boost::degree(v,m_TissueDescriptor->GetAJGraph())>=3){
				typedef itk::Point<double,NumDimensions> PointType;
				std::vector<PointType > points,centeredPoints;
				std::vector<PointType> projectedPoints;
				PointType meanPoint;
				meanPoint.Fill(0);


				PointType center=boost::get(SkeletonPointPropertyTag<NumDimensions>(),m_TissueDescriptor->GetAJGraph(),v).position;
				//std::cout << "Center: " << center << std::endl;
				points.push_back(center);

				for(int d=0;d<NumDimensions;d++){
					meanPoint[d]+=center[d];
				}

				int total=1;

				typename boost::graph_traits<typename TTissueDescriptor::PrimalGraphType>::adjacency_iterator ai,  ai_end;
				//std::cout << "Neighbors: " <<  std::endl;

				for (boost::tie(ai, ai_end) = boost::adjacent_vertices(v,m_TissueDescriptor->GetAJGraph()); ai != ai_end; ++ai) {

					itk::Point<double,NumDimensions> position = boost::get(SkeletonPointPropertyTag<NumDimensions>(),m_TissueDescriptor->GetAJGraph(),*ai).position;
					std::cout << position << std::endl;

					points.push_back(position);

					for(int d=0;d<NumDimensions;d++){
						meanPoint[d]+=position[d];
					}


					total++;
				}
				for(int d=0;d<NumDimensions;d++){
						meanPoint[d]/=total;
				}


#if 0
				vnl_matrix<double> A(total,3);
				//std::cout << "Mean: " << meanPoint << std::endl;
				int row=0;

				for(std::vector<PointType>::iterator it = points.begin();it!=points.end();++it){

					PointType diff = *it-meanPoint;
					centeredPoints.push_back(diff);
					A(row,0)=diff[0];
					A(row,1)=diff[1];
					A(row,2)=diff[2];
					row++;
				}

				//std::cout << "A" << std::endl;
				//std::cout << A << std::endl;

				vnl_matrix<double> S= A.transpose()*A;

				//std::cout << "S" << std::endl;
				//std::cout << S << std::endl;

				vnl_symmetric_eigensystem<double > eigensystem(S);
				vnl_vector<double> supportingPlane = eigensystem.get_eigenvector(0);
				//double sign=vnl_determinant(eigensystem.V);
				double sign=supportingPlane(2)/std::abs(supportingPlane(2));



#if 0
				if(supportingPlane(2)>0){
					supportingPlane=-supportingPlane;
					eigensystem.V.set_column(0,supportingPlane);
				}
#endif
				std::cout << supportingPlane << std::endl;


				for(std::vector<PointType>::iterator it = centeredPoints.begin();it!=centeredPoints.end();++it){
					PointType a = *it;
					PointType proj;

					double dot = a[0]*supportingPlane[0] + a[1]*supportingPlane[1] + a[2]*supportingPlane[2];
					proj[0]=a[0]-dot*supportingPlane[0];
					proj[1]=a[1]-dot*supportingPlane[1];
					proj[2]=a[2]-dot*supportingPlane[2];
					projectedPoints.push_back(proj);

				}


				std::vector<vnl_vector<double> > transformedPoints;

				for(std::vector<PointType>::iterator it = projectedPoints.begin();it!=projectedPoints.end();++it){

					PointType a = *it;
					vnl_vector<double> vec(3);
					vec(0)=a[0]; vec(1)=a[1];vec(2)=a[2];

					vnl_vector<double> ap= eigensystem.V.transpose() * vec;
					vnl_vector<double> transformed(2);
					transformed(0)=ap[1];
					transformed(1)=ap[2];
					transformedPoints.push_back(transformed);

				}



				std::vector<vnl_vector<double> >::iterator it =transformedPoints.begin();

				vnl_vector<double> projectedCenter=*it;
				it++;

				vnl_vector<double> reference = *it;
				it++;

				typedef std::pair<double,SkeletonVertexType> AngleAndVertex;

				std::vector<AngleAndVertex> angles;

				boost::tie(ai, ai_end) = boost::adjacent_vertices(v,*m_TissueDescriptor->m_SkeletonGraph);

				angles.push_back(AngleAndVertex(0,*ai));//FIRST NEIGHBOR
				++ai;

				vnl_vector<double> referenceVector=reference-projectedCenter;

				while(it!=transformedPoints.end()){

					vnl_vector<double> currentVector=*it-projectedCenter;


					double angle=atan2(currentVector(1),currentVector(0))-atan2(referenceVector(1),referenceVector(0));
					angles.push_back(AngleAndVertex(sign*angle,*ai));//FIRST NEIGHBOR

					++ai;
					++it;
				}
				assert(it==transformedPoints.end());
				assert(ai==ai_end);
#endif
				typename std::vector<PointType >::iterator it =points.begin();

				PointType projectedCenter=*it;
				it++;

				PointType reference = *it;
				it++;

				typedef std::pair<double,typename ttt::TissueDescriptorTraits<TTissueDescriptor>::SkeletonVertexType> AngleAndVertex;

				std::vector<AngleAndVertex> angles;

				boost::tie(ai, ai_end) = boost::adjacent_vertices(v,m_TissueDescriptor->GetAJGraph());

				angles.push_back(AngleAndVertex(0,*ai));//FIRST NEIGHBOR
				++ai;

				itk::Vector<double,NumDimensions> referenceVector=reference-projectedCenter;

				while(it!=points.end()){

					itk::Vector<double,NumDimensions> currentVector=*it-projectedCenter;


					double angle=atan2(currentVector[1],currentVector[0])-atan2(referenceVector[1],referenceVector[0]);
					angles.push_back(AngleAndVertex(angle,*ai));//FIRST NEIGHBOR

					++ai;
					++it;
				}

				std::sort(angles.begin(),angles.end());

				for(typename std::vector<AngleAndVertex>::iterator it = angles.begin();it!=angles.end();++it){
					//std::cout << v << " To " << it->second << " angle " << it->first << std::endl;
					myembedding[v].push_back(boost::edge(v,it->second,m_TissueDescriptor->GetAJGraph()).first);
				}
			}
			assert(embedding[v].size()==myembedding[v].size());

			typename vec_t::iterator itEmbedding = embedding[v].begin();
			typename vec_t::iterator itMyEmbedding = myembedding[v].begin();

			while(itEmbedding!=embedding[v].end() && itMyEmbedding!=myembedding[v].end()){

				//std::cout << *itEmbedding << *itMyEmbedding << std::endl;
				++itEmbedding;
				++itMyEmbedding;
			}
			//std::cout << std::endl;

		}



		create_dual_graph(m_TissueDescriptor->GetAJGraph(),
				m_TissueDescriptor->GetCellGraph(), &myembedding[0]);

		std::cout << "Num vertex/edges in dual: "
				<< boost::num_vertices(m_TissueDescriptor->GetCellGraph()) << "/ "
				<< boost::num_edges(m_TissueDescriptor->GetCellGraph())
				<< std::endl;
		if(boost::num_vertices(m_TissueDescriptor->GetCellGraph())==0) return;
#if 1
		{
			int maxDegree = -1;
			typename ttt::TissueDescriptorTraits<TTissueDescriptor>::CellVertexType outFace;
			BGL_FORALL_VERTICES_T(v,m_TissueDescriptor->GetCellGraph(),TTissueDescriptor::DualGraphType){
			int currentDegree=boost::degree(v,m_TissueDescriptor->GetCellGraph());
			if (currentDegree>maxDegree) {
				maxDegree=currentDegree;
				outFace=v;
			}

		}

			ttt::Cell<NumDimensions> outerFace = boost::get(ttt::CellPropertyTag<NumDimensions>(),m_TissueDescriptor->GetCellGraph(),outFace);
			m_TissueDescriptor->ClearPerimeter();
			for(typename Cell<NumDimensions>::PerimeterIterator it = outerFace.PerimeterBegin();it!=outerFace.PerimeterEnd();++it){
				m_TissueDescriptor->AddVertexToPerimeter(*it);
			}

			boost::clear_vertex(outFace, m_TissueDescriptor->GetCellGraph());
			boost::remove_vertex(outFace, m_TissueDescriptor->GetCellGraph());
		}




//		std::map<CellVertexType, int> dualEdgeBool;

		typename boost::graph_traits<typename TTissueDescriptor::DualGraphType>::vertex_iterator vi, vi_end, next;
		boost::tie(vi, vi_end) = vertices(m_TissueDescriptor->GetCellGraph());
		for (next = vi; vi != vi_end; vi = next) {
			++next;
			if (boost::degree(*vi, m_TissueDescriptor->GetCellGraph()) == 0) {
				clear_vertex(*vi, m_TissueDescriptor->GetCellGraph());
				remove_vertex(*vi, m_TissueDescriptor->GetCellGraph());
			}
		}
#endif
#if 0
		CellCentroid<ttt::TissueDescriptor>::Pointer centroidCalculator= CellCentroid<ttt::TissueDescriptor>::New();

		centroidCalculator->SetTissueDescriptor(m_TissueDescriptor);
		centroidCalculator->Compute();

		BGL_FORALL_VERTICES_T(v,*m_TissueDescriptor->m_CellGraph,CellGraph){

			assert(boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).GetNumSkeletonPoints()>0);
			std::cout << "Replace: " << boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).GetCentroid() << " By " << (*centroidCalculator)[v] << std::endl;
			boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).SetCentroid((*centroidCalculator)[v]);
		}
#endif
	}

}

}

/** @}*/
