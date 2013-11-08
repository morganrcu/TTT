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

#include <queue>
//#include <set>

namespace ttt {

  
  
template<class InputGraph, class OutputGraph, class EdgeIndexMap> class CentroidGraphCalculator: public boost::planar_face_traversal_visitor {

public:

    typedef typename boost::graph_traits<OutputGraph>::vertex_descriptor vertex_t;
    typedef typename boost::graph_traits<InputGraph>::edge_descriptor edge_t;
    typedef typename std::vector<vertex_t> vertex_vector_t;
    typedef boost::iterator_property_map<typename vertex_vector_t::iterator, EdgeIndexMap> edge_to_face_map_t;


    CentroidGraphCalculator(InputGraph & arg_g,
                            OutputGraph & arg_dual_g,
                            EdgeIndexMap arg_em
                           ) :
            m_Graph(arg_g),
            m_Dual(arg_dual_g),
            m_EM(arg_em),
            m_EdgeToFaceVector(boost::num_edges(m_Graph),boost::graph_traits<OutputGraph>::null_vertex())
    {
    	m_Total=-1;
    	m_CurrentVertex=-1;

        m_EdgeToFace = new edge_to_face_map_t(m_EdgeToFaceVector.begin(), m_EM);

    }

    void begin_face() {
        std::cout << "Comenzando nueva cara " ;

        m_CurrentVertex= boost::add_vertex(CellProperty(),m_Dual);
        std::cout << m_CurrentVertex << std::endl;
        m_Total = 0;
    }


    void end_face() {
        boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).m_Centroid[0]/=m_Total;
        boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).m_Centroid[1]/=m_Total;
        boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).m_Centroid[2]/=m_Total;
        std::cout << "Centroide en: " << boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).m_Centroid << std::endl;
    }

    template<typename Vertex>
    void next_vertex(Vertex v) {
        boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).m_Centroid[0]+= boost::get(SkeletonPointPropertyTag(),m_Graph,v).position[0];
        boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).m_Centroid[1]+= boost::get(SkeletonPointPropertyTag(),m_Graph,v).position[1];
        boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).m_Centroid[2]+= boost::get(SkeletonPointPropertyTag(),m_Graph,v).position[2];
        boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).AddSkeletonPoint(v); 
        //std::cout << "Adding SkeletonPoint to: " << m_CurrentVertex << " " << v << ": Total is: " <<  boost::get(CellPropertyTag(),m_Dual,m_CurrentVertex).GetNumSkeletonPoints() << std::endl;
        BGL_FORALL_VERTICES(vt,m_Dual,CellGraph)
        {
            std::cout << "Number of Cells in " << vt << " is " << boost::get(CellPropertyTag(),m_Dual,vt).GetNumSkeletonPoints() << std::endl;
        }

        m_Total++;
    }

    template<typename Edge>
    void next_edge(Edge e) {
        vertex_t existing_face = (*m_EdgeToFace)[e];
        if (existing_face == boost::graph_traits<OutputGraph>::null_vertex()) {
            (*m_EdgeToFace)[e] = m_CurrentVertex;
        } else {
            boost::add_edge(existing_face, m_CurrentVertex, m_Dual);
            std::cout << "Conectando " << existing_face << " con " << m_CurrentVertex << std::endl;
        }

    }

private:
    //giaa::Cell m_CurrentCell;

    //CellVertexType m_CurrentVertex;
    CellVertexType m_CurrentVertex;
    int m_Total;

    InputGraph & m_Graph;
    OutputGraph & m_Dual;
    EdgeIndexMap m_EM;

    edge_to_face_map_t * m_EdgeToFace;
    vertex_vector_t m_EdgeToFaceVector;
};


template<typename InputGraph, typename OutputGraph, typename PlanarEmbedding,
typename EdgeIndexMap>
void create_dual_graph(InputGraph& g, OutputGraph& dual_g,PlanarEmbedding embedding, EdgeIndexMap em) {
    std::cout << "B" << std::endl;
    CentroidGraphCalculator<InputGraph, OutputGraph, EdgeIndexMap> visitor(g, dual_g,em);
    boost::planar_face_traversal(g, embedding, visitor, em);

}

template<typename InputGraph, typename OutputGraph, typename PlanarEmbedding>
void create_dual_graph(InputGraph& g, OutputGraph& dual_g,PlanarEmbedding embedding) {
    std::cout << "A" << std::endl;
    create_dual_graph(g,dual_g, embedding, boost::get(boost::edge_index,g));
}

template<class TCellGraph> class PrimalGraphToDualGraphFilter : public itk::ProcessObject{
public:

  typedef PrimalGraphToDualGraphFilter Self;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer< const Self> ConstPointer;

  typedef TCellGraph TissueDescriptorType;  
  void GenerateData();
  
  typename TissueDescriptorType::Pointer m_TissueDescriptor;

    virtual void SetInput(const typename TissueDescriptorType::Pointer & inputTissueDescriptor){
        m_TissueDescriptor = inputTissueDescriptor;
    }

    inline typename TissueDescriptorType::Pointer GetOutput(){
      return m_TissueDescriptor;
    
    }

    itkNewMacro(PrimalGraphToDualGraphFilter);
protected:
    virtual ~PrimalGraphToDualGraphFilter() {
    }
private:
  
  PrimalGraphToDualGraphFilter(){
  }
  
};

//template<class TSkeletonImage> long CellBoundaryDescriptor2<TSkeletonImage>::SkeletonPoint::nextID=0;

template<class TCellGraph> void
PrimalGraphToDualGraphFilter<TCellGraph>::GenerateData() {
    //dual graph
    {

        
        typename boost::property_map<SkeletonGraph, boost::edge_index_t>::type e_index = boost::get(boost::edge_index, *m_TissueDescriptor->m_SkeletonGraph);
        //typename boost::property_map<CellGraph, EllipsePropertyTag>::type name = boost::get(EllipsePropertyTag(), m_TissueDescriptor->m_CellGraph);
        typename boost::graph_traits<SkeletonGraph>::edges_size_type edge_count = 0;
        typename boost::graph_traits<SkeletonGraph>::edge_iterator ei, ei_end;

        for (boost::tie(ei, ei_end) = boost::edges(*m_TissueDescriptor->m_SkeletonGraph); ei != ei_end; ++ei)
        {
            boost::put(e_index, *ei, edge_count++);
        }

        std::cout << "Num edges: " << edge_count << std::endl;


        typedef std::vector<typename boost::graph_traits<SkeletonGraph>::edge_descriptor> vec_t;

        std::vector<vec_t> embedding(boost::num_vertices(*m_TissueDescriptor->m_SkeletonGraph));

        if (boost::boyer_myrvold_planarity_test(boost::boyer_myrvold_params::graph =*m_TissueDescriptor->m_SkeletonGraph, boost::boyer_myrvold_params::embedding =&embedding[0]))
//if (boost::boyer_myrvold_planarity_test(m_SkeletonGraph)) //, boost::boyer_myrvold_params::embedding =&embedding[0]))
            std::cout << "Input graph is planar" << std::endl;
        else
            std::cout << "Input graph is not planar" << std::endl;

        m_TissueDescriptor->m_CellGraph=boost::shared_ptr<CellGraph>(new CellGraph());

        create_dual_graph(*m_TissueDescriptor->m_SkeletonGraph,*m_TissueDescriptor->m_CellGraph,&embedding[0]);
        


        std::cout << "Num vertex/edges in dual: " << boost::num_vertices(*m_TissueDescriptor->m_CellGraph) << "/ " << boost::num_edges(*m_TissueDescriptor->m_CellGraph) << std::endl;
        {
            int maxDegree=-1;
            CellVertexType outFace;
            BGL_FORALL_VERTICES_T(v,*m_TissueDescriptor->m_CellGraph,CellGraph) {
                int currentDegree=boost::degree(v,*m_TissueDescriptor->m_CellGraph);
                if (currentDegree>maxDegree) {
                    maxDegree=currentDegree;
                    outFace=v;
                }

            }

            boost::clear_vertex(outFace,*m_TissueDescriptor->m_CellGraph);
            boost::remove_vertex(outFace,*m_TissueDescriptor->m_CellGraph);
        }

        std::map<CellVertexType, int> dualEdgeBool;

        boost::graph_traits<CellGraph>::vertex_iterator vi, vi_end, next;
        boost::tie(vi, vi_end) = vertices(*m_TissueDescriptor->m_CellGraph);
            for (next = vi; vi != vi_end; vi = next) {
                 ++next;
                 if(boost::degree(*vi,*m_TissueDescriptor->m_CellGraph) == 0){
                        clear_vertex(*vi, *m_TissueDescriptor->m_CellGraph);
                        remove_vertex(*vi,* m_TissueDescriptor->m_CellGraph);
                 }
            }
            
    }


}

}

