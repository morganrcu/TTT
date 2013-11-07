class TrackInteractor : public vtkInteractorStyle
{
  public:
    static TrackInteractor* New();
    vtkTypeMacro(TrackInteractor, vtkInteractorStyle);

    vtkSmartPointer<vtkActor> pt = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> temp = vtkSmartPointer<vtkActor>::New();
    std::map<SkeletonVertexType, vtkActor*> SelectedCellsOneDomain;
    vtkSmartPointer<vtkRenderer> trackRenderer = vtkSmartPointer<vtkRenderer>::New();
    std::map<int, CellVertexType> tracksvtmap;
    bool choseRenderer = true;

    virtual void OnLeftButtonDown(){
    vtkInteractorStyle::OnLeftButtonDown();
    double picked[3];
    itk::Point<double,3> index;

    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.01);
    picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, trackRenderer);
    picker->GetPickPosition(picked);
    std::cout << "Picked actor: " << picker->GetActor() << std::endl;
    pt = picker->GetActor();
    if(pt != 0)
    {
        if(actorTogiaavertexDual.count(pt)){
            int trackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,actorTogiaavertexDual[pt]).trackID;
            std::cout <<"selected svt in 1 "<< actorTogiaavertexDual[pt] << " track id " << trackID << std::endl;
            QStandardItem * item = cellModel->item(actorTogiaavertexDual[pt],0);
            Qt::CheckState checkState = item->checkState();
            std::cout<<"size of selected cells "<< selectedCells.size() <<std::endl;
            if(checkState == Qt::Checked){
               item->setCheckState(Qt::Unchecked);
               pt->GetProperty()->SetColor(1,1,1);
               int i;
               for(i = 0; i < selectedCells.size(); i++){
                    int celltrackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,actorTogiaavertexDual[pt]).trackID;
                    if(selectedCells[i] == celltrackID){
                        break;
                    }
                }
                selectedCells.erase(selectedCells.begin()+i);
                std::cout<<"size of selected cells now "<< selectedCells.size() <<std::endl;
                m_RenderWindow->Render();
            }
            else{
                item->setCheckState(Qt::Checked);
                Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,actorTogiaavertexDual[pt]);
                std::vector<double> col = a.m_Color;
                pt->GetProperty()->SetColor(col[0],col[1],col[2]);
                int celltrackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,actorTogiaavertexDual[pt]).trackID;
                selectedCells.push_back(celltrackID);
                tracksvtmap[celltrackID] = actorTogiaavertexDual[pt];
                m_RenderWindow->Render();
            }
        }
        if(actorTogiaavertexDual2.count(pt)){
            int trackID = boost::get(giaa::CellPropertyTag(),descriptor2->m_CellGraph,actorTogiaavertexDual2[pt]).trackID;
            std::cout <<"selected svt in 2 "<< actorTogiaavertexDual2[pt] << " track id " << trackID << std::endl;
        }
    }
   }



  virtual void SelectCell(Cell a, vtkActor* actor, std::vector<double> color){
       std::cout<<"In Select Cell " << std::endl;
       actor->GetProperty()->SetColor(color[0],color[1],color[2]);
       for(std::vector<SkeletonVertexType>::iterator i = a.Begin(); i!=a.End(); ++i){
            giaaTovtkVertexToPrimal[*i]->GetProperty()->SetColor(color[0],color[1],color[2]);
        }
  }

  virtual void SetRenderer(vtkRenderer* renderer){
    trackRenderer = renderer;
  }

  virtual void DeSelectCell(Cell a, vtkActor* actor){
    std::cout<<"In DeSelect Cell " << std::endl;
    actor->GetProperty()->SetColor(1,1,1);
    for(std::vector<SkeletonVertexType>::iterator i = a.Begin(); i!=a.End(); ++i){
            giaaTovtkVertexToPrimal[*i]->GetProperty()->SetColor(1,1,1);
    }
  }

  virtual void ColorDomain(std::vector<int> selectedCells, int DomainID){
        for(int i = 0; i < selectedCells.size(); i++){
               giaaTovktVertexToDual[tracksvtmap[selectedCells[i]]]->GetProperty()->SetColor(r_color[DomainID],g_color[DomainID],b_color[DomainID]);
        }
  }

  virtual void OnKeyPress(){

    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();
    std::cout << "Pressed " << key << std::endl;

     if(key == "d")
      {
        Domain d;
        d.DomainID = DomainID;
        ColorDomain(selectedCells,DomainID);
        DomainID++;
        d.m_cellNodes = selectedCells;
        Domains.push_back(d);
        DomainSelected = 1;
        SelectedCellsOneDomain.clear();
        selectedCells.clear();
        tracksvtmap.clear();
        m_RenderWindow->Render();
      }


     if(key == "1"){

        CellVertexType cell = actorTogiaavertexDual[pt];
        std::cout<<"size of selected cells before 1 cor"<< selectedCells.size() <<std::endl;

        BGL_FORALL_EDGES(e,descriptor->m_CellGraph,giaa::CellGraph){
            CellVertexType src = boost::source(e,descriptor->m_CellGraph);
            CellVertexType tgt = boost::target(e,descriptor->m_CellGraph);

            if(cell == src){
                int tgttrackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,tgt).trackID;
                selectedCells.push_back(tgttrackID);
                std::cout<<"= src added " << tgttrackID << std::endl;
                QStandardItem * item = cellModel->item(tgt,0);
                item->setCheckState(Qt::Checked);
                Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,tgt);
                std::vector<double> col = a.m_Color;
                giaaTovktVertexToDual[tgt]->GetProperty()->SetColor(col[0],col[1],col[2]);
                tracksvtmap[tgttrackID] = tgt;

            }
            else if(cell == tgt){
                int srctrackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,src).trackID;
                selectedCells.push_back(srctrackID);
                std::cout<<"= tgt added " << srctrackID << std::endl;
                QStandardItem * item = cellModel->item(src,0);
                item->setCheckState(Qt::Checked);
                Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,src);
                std::vector<double> col = a.m_Color;
                giaaTovktVertexToDual[src]->GetProperty()->SetColor(col[0],col[1],col[2]);
                tracksvtmap[srctrackID] = src;
            }
        }
        std::cout<<"size of selected cells after 1 cor"<< selectedCells.size() <<std::endl;
        m_RenderWindow->Render();
     }

     if(key == "2"){

        CellVertexType cell = actorTogiaavertexDual[pt];
        std::vector<CellVertexType> firstcorona;
        std::vector<CellVertexType> allcorona;
        BGL_FORALL_EDGES(e,descriptor->m_CellGraph,giaa::CellGraph){
            CellVertexType src = boost::source(e,descriptor->m_CellGraph);
            CellVertexType tgt = boost::target(e,descriptor->m_CellGraph);

            if(cell == src){
                int tgttrackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,tgt).trackID;
                selectedCells.push_back(tgttrackID);
                Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,tgt);
                std::vector<double> col = a.m_Color;
                giaaTovktVertexToDual[tgt]->GetProperty()->SetColor(col[0],col[1],col[2]);
                tracksvtmap[tgttrackID] = tgt;
                firstcorona.push_back(tgt);
                allcorona.push_back(tgt);

            }
            else if(cell == tgt){
                int srctrackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,src).trackID;
                selectedCells.push_back(srctrackID);
                Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,src);
                std::vector<double> col = a.m_Color;
                giaaTovktVertexToDual[src]->GetProperty()->SetColor(col[0],col[1],col[2]);
                tracksvtmap[srctrackID] = src;
                firstcorona.push_back(src);
                allcorona.push_back(src);
            }
        }

        for(int i = 0; i < firstcorona.size(); i++){

            CellVertexType cell = firstcorona[i];
            BGL_FORALL_EDGES(e,descriptor->m_CellGraph,giaa::CellGraph){
                CellVertexType src = boost::source(e,descriptor->m_CellGraph);
                CellVertexType tgt = boost::target(e,descriptor->m_CellGraph);
                int tgttrackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,tgt).trackID;
                int srctrackID = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,src).trackID;

                if(cell == src){
                    bool foundtgt = false;
                    for(std::vector<CellVertexType>::iterator itr = allcorona.begin(); itr != allcorona.end(); ++itr){
                        if (*itr == tgt){
                            foundtgt = true;
                        }
                    }
                    if(!foundtgt){
                        selectedCells.push_back(tgttrackID);
                        Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,tgt);
                        std::vector<double> col = a.m_Color;
                        giaaTovktVertexToDual[tgt]->GetProperty()->SetColor(col[0],col[1],col[2]);
                        tracksvtmap[tgttrackID] = tgt;
                        allcorona.push_back(tgt);
                    }
                }

                else if(cell == tgt){
                    bool foundsrc = false;
                    for(std::vector<CellVertexType>::iterator itr = allcorona.begin(); itr != allcorona.end(); ++itr){
                        if (*itr == src){
                            foundsrc = true;
                        }
                    }
                    if(!foundsrc){
                        selectedCells.push_back(srctrackID);
                        Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,src);
                        std::vector<double> col = a.m_Color;
                        giaaTovktVertexToDual[src]->GetProperty()->SetColor(col[0],col[1],col[2]);
                        tracksvtmap[srctrackID] = src;
                        allcorona.push_back(src);
                    }
                }
            }
        }

        m_RenderWindow->Render();
     }
  }

  private:

};
vtkStandardNewMacro(TrackInteractor);
