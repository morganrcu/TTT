for(std::map<tuple<SkeletonVertexType, SkeletonVertexType>, vtkActor*>::iterator itr = svtToactor_edge.begin(); itr != svtToactor_edge.end(); ++itr){
            tuple<SkeletonVertexType, SkeletonVertexType> pts = itr->first;
            SkeletonVertexType pt1 = boost::get<0>(pts);
            SkeletonVertexType pt2 = boost::get<1>(pts);
            if(boost::degree(pt1,descriptor->m_SkeletonGraph) == 4){
                giaaTovtkVertexToPrimal[pt1]->GetProperty()->SetColor(1,0,0);
                itr->second->GetProperty()->SetColor(1,0,0);
            }
            if(boost::degree(pt1,descriptor->m_SkeletonGraph) >= 5){
                giaaTovtkVertexToPrimal[pt1]->GetProperty()->SetColor(0,0,1);
                itr->second->GetProperty()->SetColor(0,0,1);
            }
            if(boost::degree(pt2,descriptor->m_SkeletonGraph) == 4){
                giaaTovtkVertexToPrimal[pt1]->GetProperty()->SetColor(1,0,0);
                itr->second->GetProperty()->SetColor(1,0,0);
            }
            if(boost::degree(pt2,descriptor->m_SkeletonGraph) >= 5){
                giaaTovtkVertexToPrimal[pt2]->GetProperty()->SetColor(0,0,1);
                itr->second->GetProperty()->SetColor(0,0,1);
            }
        }
