
            std::cout<<"in simple cbox"<<std::endl;
            int cont;
            descriptor = this->m_Project.GetTissueDescriptor(m_CurrentFrame);
            //std::cout<<"got past desc"<<std::endl;
            typedef itk::Point<double,3> itkpt;
            ProjectType::SpacingType s = m_Project.GetSpacing();
            //std::vector<double> s (0.28,0.28,0.32);
            std::cout<< "size of SG " << boost::num_vertices(descriptor->m_SkeletonGraph)<<std::endl;
            std::cout<< "size of SG edges" << boost::num_edges(descriptor->m_SkeletonGraph)<<std::endl;
            std::cout<<"num V in sim "<< boost::num_vertices(descriptor->m_SkeletonGraph_simple)<<std::endl;
            std::cout<<"num E in sim "<< boost::num_edges(descriptor->m_SkeletonGraph_simple)<<std::endl;
            std::map<vtkActor*,SkeletonVertexType_lS> atovt;
            BGL_FORALL_VERTICES(v,descriptor->m_SkeletonGraph_listS,giaa::SkeletonGraph_listS) {
                itkpt a= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,v).position;
                //itkpt color = boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,v).color;
                vtkSmartPointer<vtkSphereSource> newSphere = vtkSmartPointer<vtkSphereSource>::New();
                newSphere->SetCenter(s[0]*a[0],s[1]*a[1],0.0);
                //std::cout << v << " " << s[0]*a[0] << " " << s[1]*a[1] << " " << s[2]*a[2] << std::endl;
                newSphere->SetRadius(0.20);

                vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
                sphereMapper->SetInputConnection(newSphere->GetOutputPort());

                vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
                //std::cout << v << " " << color[0] << " " << color[1] << " " << color[2] << std::endl;
                if(boost::degree(v,descriptor->m_SkeletonGraph_listS) == 1){
                    sphereActor->GetProperty()->SetColor(0,0,1);
                }
                if(v == chosen1){
                    sphereActor->GetProperty()->SetColor(1,0,0);
                }
                /*if(v == chosen2){
                    sphereActor->GetProperty()->SetColor(0,0,1);
                }*/
                sphereActor->SetMapper(sphereMapper);

                //giaaTovtkVertexToPrimal[v] = sphereActor;
                giaaTovtkVertexToPrimal_ls[v] = sphereActor;
                atovt[sphereActor] = v;
                m_CurrentRenderer->AddActor(sphereActor);

            }


            BGL_FORALL_EDGES(e,descriptor->m_SkeletonGraph_listS,giaa::SkeletonGraph_listS) {

                typedef itk::Point<double,3> itkpt;
                SkeletonVertexType_lS src = boost::source(e,descriptor->m_SkeletonGraph_listS);
                SkeletonVertexType_lS tgt = boost::target(e,descriptor->m_SkeletonGraph_listS);
                //std::cout<< "In edges " << src << " " << tgt << std::endl;
                vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
                itkpt a= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,boost::source(e,descriptor->m_SkeletonGraph_listS)).position;
                //std::cout<< a[0] << " " << a[1] << " " << a[2] << std::endl;
                //IndexType a=m_SkeletonGraph[].position;
                newLine->SetPoint1(a[0]*s[0],a[1]*s[1],0.0);
                itkpt b= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,boost::target(e,descriptor->m_SkeletonGraph_listS)).position;
                //IndexType b=m_SkeletonGraph[boost::target(e,)].position;
                newLine->SetPoint2(b[0]*s[0],b[1]*s[1],0.0);
                vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(newLine->GetOutputPort());

                vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

                actor->SetMapper(mapper);
                actor->GetProperty()->SetLineWidth(2);
                actor->GetProperty()->SetColor(0,0,0);


                m_CurrentRenderer->AddActor(actor);
            }

            /*for(std::map<vtkActor*,SkeletonVertexType_lS>::iterator itr = atovt.begin(); itr != atovt.end(); ++itr){

                itr->first->GetProperty()->SetColor(0,1,0);
                m_RenderWindow->Render();
                SkeletonVertexType_lS svt = itr->second;
                itkpt a= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,svt).position;
                std::cout<< a[0] << " " << a[1] << " " <<a[2] << std::endl;
                scanf("%d",&cont);

            }*/
