  BGL_FORALL_VERTICES(v,descriptor->m_CellGraph,giaa::CellGraph){
             Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v);
             ColorCell(a,a.m_SkeletonNodes.size(),1);
        }
        this->SetUpPolygonPlot();
