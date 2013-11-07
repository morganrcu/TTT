#include <boost/graph/iteration_macros.hpp>
#include "tttDescriptionDataTypes.h"

ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType ttt::CellID2VertexDescriptor(int ID,const ttt::TrackedTissueDescriptor::Pointer & descriptor){
	assert(descriptor);
	assert(descriptor->m_CellGraph);
	BGL_FORALL_VERTICES(v,*descriptor->m_CellGraph,ttt::TrackedTissueDescriptor::DualGraphType){

		if(boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).m_ID==ID){
			return v;
		}
	}
	return -1;
}

ttt::TissueDescriptor::Pointer ttt::cloneTissueDescriptor(const ttt::TissueDescriptor::Pointer & descriptor){
      
      ttt::TissueDescriptor::Pointer result = ttt::TissueDescriptor::New();
      
      
      std::cout << "Duplicando...." << std::endl;
      BGL_FORALL_VERTICES(v,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
    	  boost::add_vertex(boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v),*result->m_SkeletonGraph);
      }
      
      std::cout << "SkeletonGraphVertices" << std::endl;
      BGL_FORALL_EDGES(e,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
    	  boost::add_edge(boost::source(e,*descriptor->m_SkeletonGraph),boost::target(e,*descriptor->m_SkeletonGraph),*result->m_SkeletonGraph);
      }

      std::cout << "...SkeletonGraphEdges" << std::endl;
#if 0
      BGL_FORALL_VERTICES(v,descriptor->m_SkeletonGraph_listS,ttt::SkeletonGraph_listS){
  boost::add_vertex(boost::get(ttt::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,v),result->m_SkeletonGraph_listS);
      }

      std::cout << boost::num_vertices(descriptor->m_SkeletonGraph_listS) << std::endl;
      std::cout << boost::num_vertices(result->m_SkeletonGraph_listS) << std::endl;


      
      std::cout << "SkeletonGraphVertices" << std::endl;
      BGL_FORALL_EDGES(e,descriptor->m_SkeletonGraph_listS,ttt::SkeletonGraph_listS){
  boost::add_edge(boost::source(e,descriptor->m_SkeletonGraph_listS),boost::target(e,descriptor->m_SkeletonGraph_listS),result->m_SkeletonGraph_listS); 
      }

    std::cout << boost::num_edges(descriptor->m_SkeletonGraph_listS) << std::endl;
      std::cout << boost::num_edges(result->m_SkeletonGraph_listS) << std::endl;

      std::cout << "...SkeletonGraphEdges" << std::endl;

      BGL_FORALL_VERTICES(v,descriptor->m_SkeletonGraph_simple,ttt::SkeletonGraph_simple){
  boost::add_vertex(boost::get(ttt::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_simple,v),result->m_SkeletonGraph_simple);
      }
      
      std::cout << "SkeletonGraphVertices" << std::endl;
      BGL_FORALL_EDGES(e,descriptor->m_SkeletonGraph_simple,ttt::SkeletonGraph_simple){
  boost::add_edge(boost::source(e,descriptor->m_SkeletonGraph_simple),boost::target(e,descriptor->m_SkeletonGraph_simple),result->m_SkeletonGraph_simple); 
      }

      std::cout << "...SkeletonGraphEdges" << std::endl;

      
  
#endif
      #if 0
      std::map<SkeletonVertexType,SkeletonVertexType> svt;
      itk::Index<3> pos;
      BGL_FORALL_VERTICES(v,descriptor->m_CellGraph,ttt::CellGraph){
	         CellVertexType vb = boost::add_vertex(boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v),result->m_CellGraph);
           Cell a = boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v);
           Cell b = boost::get(ttt::CellPropertyTag(),result->m_CellGraph,vb);
           
           //std::cout << "NUMBER IN DUPLICATION: " <<  boost::get(ttt::CellPropertyTag(),descriptor->m_CellGraph,v).GetNumSkeletonPoints() << std::endl;
           for(std::set<SkeletonVertexType>::iterator i = a.Begin(); i!=a.End(); ++i){
             std::cout << "Looking at " << *i << std::endl;
             std::map<SkeletonVertexType,SkeletonVertexType>::iterator iter = svt.find(*i);
             
             if(iter == svt.end()){
                  pos = boost::get(ttt::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,*i).position;
                  SkeletonPoint pt = SkeletonPoint(pos);
                  SkeletonVertexType vt = boost::add_vertex(pt,result->m_SkeletonGraph);
                  b.AddSkeletonPoint(vt);
                  svt[*i] = vt;

                 /* 1) obtain position of i
                  2) create new skeleton point with i
                  3) create SkeletonVertexType with new SkeletonPoint
                  4) add to skeletonvertextype to b
                  5) add to map*/
             }
             else{
                   b.AddSkeletonPoint(*i);
             }
           }
      }

      BGL_FORALL_EDGES(e,descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
      boost::add_edge(svt[boost::source(e,descriptor->m_SkeletonGraph)],svt[boost::target(e,descriptor->m_SkeletonGraph)],result->m_SkeletonGraph); 
      }
      #endif

      BGL_FORALL_VERTICES(v,*descriptor->m_CellGraph,ttt::CellGraph){
        boost::add_vertex(boost::get(ttt::CellPropertyTag(),*descriptor->m_CellGraph,v),*result->m_CellGraph);
      } 
      std::cout << "...CellGraphVertex" << std::endl;
      
      BGL_FORALL_EDGES(e,*descriptor->m_CellGraph,ttt::CellGraph){
	       boost::add_edge(boost::source(e,*descriptor->m_CellGraph),boost::target(e,*descriptor->m_CellGraph),*result->m_CellGraph);
      }
      std::cout << "...CellGraphEdges" << std::endl;
      
      return result;
}
#if 0
ttt::TrackedTissueDescriptor::Pointer ttt::cloneTrackedTissueDescriptor(const ttt::TrackedTissueDescriptor::Pointer & descriptor){
      ttt::TrackedTissueDescriptor::Pointer result = ttt::TrackedTissueDescriptor::New();
      
      
      std::cout << "Duplicando...." << std::endl;
      
      BGL_FORALL_VERTICES(v,descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
	boost::add_vertex(boost::get(ttt::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,v),result->m_SkeletonGraph);
      }
      
      std::cout << "SkeletonGraphVertices" << std::endl;
      BGL_FORALL_EDGES(e,descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
	boost::add_edge(boost::source(e,descriptor->m_SkeletonGraph),boost::target(e,descriptor->m_SkeletonGraph),result->m_SkeletonGraph);
	
      }
      std::cout << "...SkeletonGraphEdges" << std::endl;
      BGL_FORALL_VERTICES(v,descriptor->m_TrackedCellGraph,ttt::TrackedCellGraph){
	boost::add_vertex(boost::get(ttt::TrackedCellPropertyTag(),descriptor->m_TrackedCellGraph,v),result->m_TrackedCellGraph);
      }
      std::cout << "...CellGraphVertex" << std::endl;
      BGL_FORALL_EDGES(e,descriptor->m_TrackedCellGraph,ttt::TrackedCellGraph){
	boost::add_edge(boost::source(e,descriptor->m_TrackedCellGraph),boost::target(e,descriptor->m_TrackedCellGraph),result->m_TrackedCellGraph);
      }
      std::cout << "...CellGraphEdges" << std::endl;
      
      return result;
}
#endif
