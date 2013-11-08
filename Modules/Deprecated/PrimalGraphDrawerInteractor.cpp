#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkCellPicker.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>

#include "PrimalGraphDrawerInteractor.h"



void PrimalGraphDrawerInteractor::OnRightButtonUp(){
    vtkInteractorStyle::OnRightButtonUp();

    if(m_SelectedVertex){
    	UnhighlightActor(m_SelectedVertex);
    	m_SelectedVertex=0;
    }
    if(m_SelectedEdge){
    	UnhighlightActor(m_SelectedEdge);
    	m_SelectedEdge=0;
    }

    assert(m_SelectedVertex == 0 && m_SelectedEdge==0);


#if 0
    // Forward events
    vtkInteractorStyle::OnRightButtonUp();

    double picked[3];
    typedef itk::Index<3> IndexType;
    IndexType index;
    vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
    picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_Renderer);
    picker->GetPickPosition(picked);

    std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << " " << m_Spacing[0] << " " << m_Spacing[1] << std::endl;
    vtkSmartPointer<vtkSphereSource> newSphere = vtkSmartPointer<vtkSphereSource>::New();
    newSphere->SetCenter(picked[0],picked[1],0.0);
    newSphere->SetRadius(0.45);

    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(newSphere->GetOutputPort());

    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    m_Renderer->AddActor(sphereActor);
    index[0]=picked[0]/m_Spacing[0];
    index[1]=picked[1]/m_Spacing[1];

    double mindist = 10000000;
    giaa::SkeletonVertexType closestpt;
    itk::Point<double,3> itkpt;


    //LOOK FOR NEAREST VERTEX TO SET UP Z
    BGL_FORALL_VERTICES(v,m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph){

        itk::Point<double,3> pos = boost::get(giaa::SkeletonPointPropertyTag(),m_Descriptor->m_SkeletonGraph,v).position;
        double dist = sqrt(pow(pos[0]-index[0],2)+ pow(pos[1]-index[1],2));
        if(dist < mindist){
            mindist = dist;
            itkpt[2] = pos[2];
        }
    }

    itkpt[0] = index[0];
    itkpt[1] = index[1];

    giaa::SkeletonPoint pt(itkpt);
    std::cout<<"num V in SListS before "<< boost::num_vertices(m_Descriptor->m_SkeletonGraph)<<std::endl;
    std::cout<<"num E in SListS before adding "<< boost::num_edges(m_Descriptor->m_SkeletonGraph)<<std::endl;

    giaa::SkeletonVertexType vt = boost::add_vertex(pt,m_Descriptor->m_SkeletonGraph);
    std::cout<<"num V in SListS after adding "<< boost::num_vertices(m_Descriptor->m_SkeletonGraph)<<std::endl;
    std::cout<<"num E in SListS after adding "<< boost::num_edges(m_Descriptor->m_SkeletonGraph)<<std::endl;

    m_Actor2SkeletonVertexLs[sphereActor] = vt;

    if(interpolationSwitch){
        interpolationSet->AddItem(sphereActor);
    }

    /*
    actor_pos[sphereActor] = index;
    pos_svt[index] = vt;
    pos_svt_edge[index] = vt;*/

    //vtkTogiaaVertex[sphereActor] = vt;

    //skeletonVertexTypeSpacing[vt] = 0;

    std::cout << "Just added: " << vt << " " << sphereActor << std::endl;
    std::cout << index[0] << " " << index[1] << " " << index[2] << std::endl;
    m_Renderer->GetRenderWindow()->Render();
#endif

  }
void PrimalGraphDrawerInteractor::UnhighlightActor(const vtkSmartPointer<vtkActor> & actor){
	actor->GetProperty()->SetColor(1,1,1);
}
void PrimalGraphDrawerInteractor::HighlightActor(const vtkSmartPointer<vtkActor> & actor){
	actor->GetProperty()->SetColor(1,0,0);
}

void PrimalGraphDrawerInteractor::OnLeftButtonDown(){

    vtkInteractorStyle::OnLeftButtonDown();

    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();

    picker->SetTolerance(0.01);
    bool somethingPicked = picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_Renderer);

    if(!somethingPicked) return;


    vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();
    assert(pickedActor);

    HighlightActor(pickedActor);

    if(m_Actor2SkeletonVertex.count(pickedActor)){
    	//pickedActor is a Vertex
    	m_SelectedVertex=pickedActor;
    	UnhighlightActor(m_SelectedEdge);
    	m_SelectedEdge=0;

    }else if(m_Actor2SkeletonEdge.count(pickedActor)){

    	//pickedActor is an Edge

    	m_SelectedEdge=pickedActor;

    	UnhighlightActor(m_SelectedVertex);
    	m_SelectedVertex=0;
    }
    assert (m_SelectedVertex!=0 || m_SelectedEdge!=0);
#if 0


    itk::Point<double,3> index;

    if(!pickedActor) return;

    double pickedPosition[3];
    picker->GetPickPosition(pickedPosition);

    std::cout << "Picked actor: " << picker->GetActor() << std::endl;

    if(chosen) {
       pt1 = pickedActor;

       if(m_Actor2SkeletonVertex.count(pt1)){
    	   std::cout<<"Found actor in vertex pt1"<<std::endl;
       }
       if(m_Actor2SkeletonEdgeLs.count(pt1)){
    	   std::cout<<"Found actor in edge pt1 "<< m_Actor2SkeletonEdgeLs.count(pt1) << " " << m_Actor2SkeletonVertex.count(pt1) << std::endl;
       }
       if(pt1prev != 0){
    	   pt1prev->GetProperty()->SetColor(1,1,1);
       }
       //selected_svt.push_back(pos_svt[actor_pos[pt1]]);
       std::cout<< "chose: " << pt1 << " " << std::endl;
       pt1prev = pt1;


    } else{
       pt2 = pickedActor;

       if(m_Actor2SkeletonVertex.count(pt2)){
    	   std::cout<<"Found actor in vertex pt2"<<std::endl;
       }
       if(m_Actor2SkeletonEdgeLs.count(pt2)){
    	   std::cout<<"Found actor in edge pt2"<<std::endl;
       }
       if(pt2prev != 0){
          pt2prev->GetProperty()->SetColor(1,1,1);
        }
        //selected_svt.push_back(pos_svt[actor_pos[pt2]]);
        std::cout<< "chose: " << pt2 << " " << std::endl;
        pt2prev = pt2;


      }
    chosen = 1 - chosen;
    m_Renderer->GetRenderWindow()->Render();
#endif
  }

void PrimalGraphDrawerInteractor::OnKeyPress(){

#if 0
    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();
    std::cout << "Pressed " << key << std::endl;

     if(key == "a"){
        std::cout << "The a key was pressed." << std::endl;

        if(interpolationSwitch){

            interpolationSet->AddItem(secondInterpolationPoint);

            vtkSmartPointer<vtkActor> actorA = vtkSmartPointer<vtkActor>::New();
            vtkSmartPointer<vtkActor> actorB = vtkSmartPointer<vtkActor>::New();

            interpolationSet->InitTraversal();
            actorA = interpolationSet->GetNextItem();
            actorB = interpolationSet->GetNextItem();
            // add line from first interpolation point to first added point
            double * cord1;
            double * cord2;


            while(actorB != 0){
                cord1 = actorA->GetMapper()->GetInput()->GetCenter();
                cord2 = actorB->GetMapper()->GetInput()->GetCenter();
                vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
                //IndexType a=m_SkeletonGraph[].position;
                newLine->SetPoint1(cord1[0],cord1[1],cord1[2]);
                newLine->SetPoint2(cord2[0],cord2[1],cord2[2]);
                vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(newLine->GetOutputPort());
                vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

                boost::add_edge(m_Actor2SkeletonVertexLs[actorA],m_Actor2SkeletonVertexLs[actorB],m_Descriptor->m_SkeletonGraph_listS);
                boost::tuple<giaa::SkeletonVertexType_lS,giaa::SkeletonVertexType_lS> edgeST (m_Actor2SkeletonVertexLs[actorA], m_Actor2SkeletonVertexLs[actorB]);
                m_SkeletonEdgeLs2Actor[edgeST] = actor;
                boost::tuple<giaa::SkeletonVertexType_lS,giaa::SkeletonVertexType_lS> edgeTS (m_Actor2SkeletonVertexLs[actorB], m_Actor2SkeletonVertexLs[actorA]);
                //edge_pos_actor[edgeTS] = actor;
                interpolationSet->RemoveItem(actorA);
                actorA = actorB;
                actorB = interpolationSet->GetNextItem();

                m_Renderer->AddActor(actor);
                m_Renderer->GetRenderWindow()->Render();
            }

            interpolationSet->RemoveItem(actorA);
            interpolationSwitch = 0;
        }
        else if(m_Actor2SkeletonVertex.count(pt1) && m_Actor2SkeletonVertex.count(pt2)){
            double * cord1;
            double * cord2;
            cord1 = pt1->GetMapper()->GetInput()->GetCenter();
            cord2 = pt2->GetMapper()->GetInput()->GetCenter();
            vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
            //IndexType a=m_SkeletonGraph[].position;
            newLine->SetPoint1(cord1[0],cord1[1],cord1[2]);
            newLine->SetPoint2(cord2[0],cord2[1],cord2[2]);
            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(newLine->GetOutputPort());
            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

            actor->SetMapper(mapper);
            actor->GetProperty()->SetLineWidth(5);
            actor->GetProperty()->SetColor(0,0,0);
            m_Renderer->AddActor(actor);
            //valEdge->AddItem(actor);
            typedef itk::Point<double,3> itkpt;
            //SkeletonVertexType_lS pt1svt = vtkTogiaaVertex[pt1];
            //SkeletonVertexType_lS pt2svt = vtkTogiaaVertex[pt2];
            std::cout << "Adding edge between " << pt1 << " " << m_Actor2SkeletonVertexLs[pt1] << " " << pt2 << " " << m_Actor2SkeletonVertexLs[pt2] << std::endl;
            boost::add_edge(m_Actor2SkeletonVertexLs[pt1],m_Actor2SkeletonVertexLs[pt2], m_Descriptor->m_SkeletonGraph_listS);
            /*itkpt a = boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,pt1svt).position;
            itkpt b = boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,pt2svt).position;


            IndexType avtx;
            IndexType bvtx;

            for(int i = 0; i < 2; i++){
                avtx.push_back(a[i]);
                bvtx.push_back(b[i]);
            }*/

            std::cout << "Adding edge: " << m_Actor2SkeletonVertexLs[pt1] << " " << m_Actor2SkeletonVertexLs[pt2] << std::endl;
            boost::tuple<giaa::SkeletonVertexType_lS,giaa::SkeletonVertexType_lS> edgeST (m_Actor2SkeletonVertexLs[pt1],m_Actor2SkeletonVertexLs[pt2]);
            m_SkeletonEdgeLs2Actor[edgeST] = actor;
            boost::tuple<giaa::SkeletonVertexType_lS,giaa::SkeletonVertexType_lS> edgeTS (m_Actor2SkeletonVertexLs[pt2],m_Actor2SkeletonVertexLs[pt1]);
            m_SkeletonEdgeLs2Actor[edgeTS] = actor;

            m_Renderer->GetRenderWindow()->Render();
        }
      }else if(key == "BackSpace"){
        std::cout << "The Backspace key was pressed." << std::endl;
        std::cout<< "before " << boost::num_vertices(m_Descriptor->m_SkeletonGraph_listS)<<std::endl;

        if (chosen){
            std::cout << "In chosen" << std::endl;
            std::cout << "Removing " << pt2 << " " << m_Actor2SkeletonVertex[pt2] << " " << m_Actor2SkeletonEdgeLs.count(pt2) << " " <<  m_Actor2SkeletonVertex.count(pt2)<<  std::endl;
            //std::cout << descriptor << std::endl;


            if(m_Actor2SkeletonVertexLs[pt2] != 0){
                std::cout << " chosen vertex pt2" << std::endl;
                for(SkeletonEdgeLs2ActorType::iterator itr =m_SkeletonEdgeLs2Actor.begin(); itr != m_SkeletonEdgeLs2Actor.end(); ++itr){
                    SkeletonEdgeLsType pts = itr->first;
                    giaa::SkeletonVertexType_lS a = boost::get<0>(pts);
                    giaa::SkeletonVertexType_lS b = boost::get<1>(pts);
                   if(m_Actor2SkeletonVertexLs[pt2] == a){
                        std::cout<< m_Actor2SkeletonVertexLs[pt2] << " a  " << a << " " << itr->second <<  std::endl;
                        m_Renderer->RemoveActor(itr->second);
                   }
                   if(m_Actor2SkeletonVertexLs[pt2] == b){
                        std::cout<< m_Actor2SkeletonVertexLs[pt2] << " b  " << b << " " << itr->second <<  std::endl;
                        m_Renderer->RemoveActor(itr->second);
                   }
                }
                   std::cout<<"clearing vertex"<<std::endl;
                   boost::clear_vertex(m_Actor2SkeletonVertexLs[pt2],m_Descriptor->m_SkeletonGraph_listS);
                   boost::remove_vertex(m_Actor2SkeletonVertexLs[pt2],m_Descriptor->m_SkeletonGraph_listS);
                   std::cout<<"removed vertex"<<std::endl;

            }

            else if(m_Actor2SkeletonVertexLs.count(pt2)){
                std::cout<<"in chose edge pt2"<<std::endl;
                SkeletonEdgeLsType pts = m_Actor2SkeletonEdgeLs[pt2];
                 giaa::SkeletonVertexType_lS a = boost::get<0>(pts);
                 giaa::SkeletonVertexType_lS b = boost::get<1>(pts);
                 boost::remove_edge(a,b,m_Descriptor->m_SkeletonGraph_listS);
            }
            //boost::clear_vertex(pos_svt_edge[actor_pos[pt2]],descriptor->m_SkeletonGraph_listS);
            //boost::remove_vertex(pos_svt_edge[actor_pos[pt2]],descriptor->m_SkeletonGraph_listS);

            //valVertex->RemoveItem(pt2);
            //primalVertexFrame[globalFrame]->RemoveItem(pt2);
            m_Renderer->RemoveActor(pt2);
            m_Renderer->GetRenderWindow()->Render();

        }
        else{
            std::cout << "Removing " << pt1 << " " << m_Actor2SkeletonVertexLs[pt1] << " " << m_Actor2SkeletonEdgeLs.count(pt1) << " " <<  m_Actor2SkeletonVertexLs.count(pt1)<<  std::endl;

            if(m_Actor2SkeletonVertexLs[pt1] != 0){
                std::cout<<"in  vertex pt1: "<< pt1 <<std::endl;
                for(SkeletonEdgeLs2ActorType::iterator itr =m_SkeletonEdgeLs2Actor.begin(); itr != m_SkeletonEdgeLs2Actor.end(); ++itr){
                    SkeletonEdgeLsType pts = itr->first;
                    giaa::SkeletonVertexType_lS a = boost::get<0>(pts);
                    giaa::SkeletonVertexType_lS b = boost::get<1>(pts);
                   if(m_Actor2SkeletonVertexLs[pt1] == a){
                        std::cout<< m_Actor2SkeletonVertexLs[pt2] << " a  " << a << " " << itr->second <<  std::endl;

                        m_Renderer->RemoveActor(itr->second);

                   }
                   if(m_Actor2SkeletonVertexLs[pt1] == b){
                        std::cout<< m_Actor2SkeletonVertexLs[pt2] << " b  " << b << " " << itr->second <<  std::endl;

                        m_Renderer->RemoveActor(itr->second);

                   }
                }
                std::cout<<"clearing vertex"<<std::endl;

                boost::clear_vertex(m_Actor2SkeletonVertexLs[pt1],m_Descriptor->m_SkeletonGraph_listS);
                boost::remove_vertex(m_Actor2SkeletonVertexLs[pt1],m_Descriptor->m_SkeletonGraph_listS);
                std::cout<<"removed vertex"<<std::endl;

            }

            else if(m_Actor2SkeletonEdgeLs.count(pt1)){
                 std::cout<<"in  edge pt1"<<std::endl;
                 SkeletonEdgeLsType pts = m_Actor2SkeletonEdgeLs[pt1];
                 giaa::SkeletonVertexType_lS a = boost::get<0>(pts);
                 giaa::SkeletonVertexType_lS b = boost::get<1>(pts);
                 boost::remove_edge(a,b,m_Descriptor->m_SkeletonGraph_listS);
            }
            //boost::clear_vertex(pos_svt_edge[actor_pos[pt1]],descriptor->m_SkeletonGraph_listS);
            //boost::remove_vertex(pos_svt_edge[actor_pos[pt1]],descriptor->m_SkeletonGraph_listS);
            //valVertex->RemoveItem(pt1);
            //primalVertexFrame[globalFrame]->RemoveItem(pt1);
            m_Renderer->RemoveActor(pt1);
            m_Renderer->GetRenderWindow()->Render();
        }

        /*
        for(std::vector<SkeletonVertexType_lS>::iterator itr = selected_svt.begin(); itr != selected_svt.end(); ++itr){
            boost::clear_vertex(*itr,descriptor->m_SkeletonGraph_listS);
            boost::remove_vertex(*itr,descriptor->m_SkeletonGraph_listS);
            m_CurrentRenderer->RemoveActor(pt2);
            m_RenderWindow->Render();
            std::cout<<"Removed "<< *itr << std::endl;
        }
        selected_svt.clear();*/
        std::cout<< "after " << boost::num_vertices(m_Descriptor->m_SkeletonGraph_listS)<<std::endl;

      }else if(key == "d"){

            vtkSmartPointer<vtkActor> temp = vtkSmartPointer<vtkActor>::New();
            SkeletonEdgeLsType pt (m_Actor2SkeletonVertexLs[pt1],m_Actor2SkeletonVertexLs[pt2]);
            std::cout<< "deleting between " << pt1 << " : " << m_Actor2SkeletonVertexLs[pt1] << " and " << pt2 << " : " << m_Actor2SkeletonVertexLs[pt2] << std::endl;
            SkeletonEdgeLs2ActorType::iterator foundVal =m_SkeletonEdgeLs2Actor.find(pt);
            if(foundVal == m_SkeletonEdgeLs2Actor.end() )
            {
                std::cout << "Error " << std::endl;
            }
            else
            {
                temp = foundVal->second;
                m_Renderer->RemoveActor(temp);
                //valEdge->RemoveItem(temp);
                boost::remove_edge(m_Actor2SkeletonVertexLs[pt1],m_Actor2SkeletonVertexLs[pt2],m_Descriptor->m_SkeletonGraph_listS);
                m_Renderer->GetRenderWindow()->Render();
            }

      }else if(key == "i"){
         typedef itk::Point<double,3> itkpt;
         itkpt a = boost::get(giaa::SkeletonPointPropertyTag(),m_Descriptor->m_SkeletonGraph_listS,m_Actor2SkeletonVertexLs[pt1]).position;
         double pt1z = double(a[2]);
         itkpt b = boost::get(giaa::SkeletonPointPropertyTag(),m_Descriptor->m_SkeletonGraph_listS,m_Actor2SkeletonVertexLs[pt2]).position;
         double pt2z = double(b[2]);

         interpolationValue = (pt1z + pt2z)/2.0;
         /*std::cout<< "Interpolate: " << pt1z << " " << pt2z << " " << interpolationValue << std::endl;
         interpolationSwitch = 1;
         interpolationSet->AddItem(pt1);
         secondInterpolationPoint = pt2;*/
      }
#endif

    }

    /*SkeletonVertexType_lS getSVTfromPos(IndexType a){

        SkeletonVertexType_lS result;
        std::map<IndexType,SkeletonVertexType_lS>::iterator itr;
        for(itr = pos_svt.begin(); itr != pos_svt.end(); ++itr){
           IndexType tmp = itr->first;
           if(tmp[0] == a[0] & tmp[1] == a[1] && tmp[2] == a[2]){
                result = itr->second;
                break;
           }
        }

        return result;
    }*/
