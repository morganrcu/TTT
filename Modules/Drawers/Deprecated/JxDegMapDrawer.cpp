 if(!this->m_pUI->showPrimalCBox->isChecked()){
            this->m_pUI->showPrimalCBox->setChecked(true);
            this->UpdateDisplay();
        }

        BGL_FORALL_EDGES(e,descriptor->m_SkeletonGraph,giaa::SkeletonGraph) {

                typedef itk::Point<double,3> itkpt;
                SkeletonVertexType src = boost::source(e,descriptor->m_SkeletonGraph);
                SkeletonVertexType tgt = boost::target(e,descriptor->m_SkeletonGraph);
                //std::cout<< "In edges " << src << " " << tgt << std::endl;
                itkpt a= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,boost::source(e,descriptor->m_SkeletonGraph)).position;
                //std::cout<< a[0] << " " << a[1] << " " << a[2] << std::endl;
                //IndexType a=m_SkeletonGraph[].position;
                itkpt b= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,boost::target(e,descriptor->m_SkeletonGraph)).position;
                //IndexType b=m_SkeletonGraph[boost::target(e,)].position;
                double slope =  (a[1]-b[1])/(a[0]-b[0]);
                double deg = 180/3.14159*(atan(abs(slope)));
                tuple<SkeletonVertexType,SkeletonVertexType> pts (src,tgt);

                if (slope < 0){
                    deg=180.0-deg;
                }

                std::cout << "deg is " << deg << std::endl;


                if(deg >= 0 && deg < 30){
                    svtToactor_edge[pts]->GetProperty()->SetColor(r_color[0],g_color[0],b_color[0]); //black
                }
                else if(deg >= 30 && deg < 60){
                    svtToactor_edge[pts]->GetProperty()->SetColor(r_color[1],g_color[1],b_color[1]); //grey
                }
                else if(deg >= 60 && deg < 90){
                    svtToactor_edge[pts]->GetProperty()->SetColor(r_color[2],g_color[2],b_color[2]); //red
                }
                else if(deg >= 90 && deg < 120){
                    svtToactor_edge[pts]->GetProperty()->SetColor(r_color[3],g_color[3],b_color[3]); //green
                }
                else if(deg >= 120 && deg < 150){
                    svtToactor_edge[pts]->GetProperty()->SetColor(r_color[4],g_color[4],b_color[4]); // blue
                }
                //int cont;
                else if(deg >= 150 && deg < 180){
                    svtToactor_edge[pts]->GetProperty()->SetColor(r_color[5],g_color[5],b_color[5]); // dark green
                    /*renWin->Render();
                    std::cout<< a[0] << " " << a[1] << " " << b[0] << " " << b[1] << " " << slope << std::endl;
                    scanf("%d",&cont);*/
                }
        }

