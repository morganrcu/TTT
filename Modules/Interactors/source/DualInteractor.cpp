

/*
class DualInteractor : public vtkInteractorStyle
{
  public:
    static DualInteractor* New();
    vtkTypeMacro(DualInteractor, vtkInteractorStyle);

    vtkSmartPointer<vtkActor> pt = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> temp = vtkSmartPointer<vtkActor>::New();


  virtual void OnLeftButtonDown(){
    vtkInteractorStyle::OnLeftButtonDown();
    double picked[3];
    itk::Point<double,3> index;
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.01);
    picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_CurrentRenderer);
    picker->GetPickPosition(picked);
    std::cout << "Picked actor: " << picker->GetActor() << std::endl;
    pt = picker->GetActor();
    if(pt != 0)
    {
        std::cout << "CENTROID SELECTED " << std::endl;
        pt->GetProperty()->SetColor(0,0,1);
        Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,vtkTogiaaVertex[pt]);
        a.GetArea(descriptor->m_SkeletonGraph);
        a.GetDistanceToCentroid(descriptor->m_SkeletonGraph);
        QStandardItem * item = cellModel->item(vtkTogiaaVertex[pt],0);
        item->setCheckState(Qt::Checked);
        for(std::vector<SkeletonVertexType>::iterator i = a.Begin(); i!=a.End(); ++i){
            giaaTovtkVertexToPrimal[*i]->GetProperty()->SetColor(r_color[vtkTogiaaVertex[pt]],g_color[vtkTogiaaVertex[pt]],b_color[vtkTogiaaVertex[pt]]);
        }
        if(temp != 0){
            //temp->GetProperty()->SetColor(1,1,1);
            this->DeSelectFirstCorona(temp);
        }
        m_RenderWindow->Render();
        temp = pt;

        Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,vt);
        std::cout << "num" << " " << a.GetNumSkeletonPoints() << std::endl;
        std::cout << "SELECTED  Cell " << vt << std::endl;

        std::set<SkeletonVertexType>::iterator CellIterator;
        for(CellIterator = a.Begin(); CellIterator!= a.End(); CellIterator++){

            SkeletonVertexType bnd = *CellIterator;
            actor = vtkTogiaaVertexToPrimal[bnd];
            std::cout << "actor " << bnd << " " << actor << std::endl;

            actor->GetProperty()->SetColor(0,1,0);
        }

    }
  }

  virtual void OnKeyPress(){

    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();
    std::cout << "Pressed " << key << std::endl;

     if(key == "1")
      {
        SkeletonVertexType vt = vtkTogiaaVertex[pt];
        std::cout << "chose" << " " << vt << std::endl;
        this->SelectFirstCorona(vt);

        Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,vt);
        std::cout << "num" << " " << boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,vt).GetNumSkeletonPoints() << std::endl;
        std::cout << "SELECTED  Cell " << vt << std::endl;
      }

     if(key == "2")
     {
        SkeletonVertexType vt = vtkTogiaaVertex[pt];
        std::cout << "chose" << " " << vt << std::endl;
        for(std::vector<SkeletonVertexType>::iterator it = dualVertexToEdge[vt].begin(); it != dualVertexToEdge[vt].end(); ++it){

                SkeletonVertexType tgt = *it;
                this->SelectFirstCorona(tgt);
            }
     }

     if(key == "BackSpace")
     {
            std::cout << "Selected " << pt << " " << vtkTogiaaVertex[pt] << std::endl;
            boost::clear_vertex(vtkTogiaaVertex[pt],descriptor->m_CellGraph);
            boost::remove_vertex(vtkTogiaaVertex[pt],descriptor->m_CellGraph);
            //valVertex->RemoveItem(pt2);
            //primalVertexFrame[globalFrame]->RemoveItem(pt2);
            m_CurrentRenderer->RemoveActor(pt);
            m_RenderWindow->Render();
     }

  }

  virtual void SelectFirstCorona(SkeletonVertexType vt){
        std::cout << "finding 1st corona of" << " " << vt << std::endl;
        vtkSmartPointer<vtkActor> corona_pt = vtkSmartPointer<vtkActor>::New();
        for(std::vector<SkeletonVertexType>::iterator it = dualVertexToEdge[vt].begin(); it != dualVertexToEdge[vt].end(); ++it){

            SkeletonVertexType trgt = *it;
            tuple<SkeletonVertexType,SkeletonVertexType> srctgt (vt, trgt);
            std::map<tuple<SkeletonVertexType,SkeletonVertexType>, vtkActor*>::iterator found =giaaTovtkEdge.find(srctgt);
            if (found == giaaTovtkEdge.end())
            {
                std::cout << "Error " << std::endl;
            }
            else{
                std::cout << "corona " << " " << trgt << " " << found->second << std::endl;
                found->second->GetProperty()->SetColor(0,1,0);
                m_RenderWindow->Render();

            }
        }
        m_RenderWindow->Render();
  }

  virtual void DeSelectFirstCorona(vtkSmartPointer<vtkActor> actor){
        SkeletonVertexType vt = vtkTogiaaVertex[actor];
        vtkSmartPointer<vtkActor> corona_pt = vtkSmartPointer<vtkActor>::New();
        for(std::vector<SkeletonVertexType>::iterator it = dualVertexToEdge[vt].begin(); it != dualVertexToEdge[vt].end(); ++it){

            SkeletonVertexType trgt = *it;
            tuple<SkeletonVertexType,SkeletonVertexType> srctgt (vt, trgt);
            std::map<tuple<SkeletonVertexType,SkeletonVertexType>, vtkActor*>::iterator found =giaaTovtkEdge.find(srctgt);
            if (found == giaaTovtkEdge.end())
            {
                std::cout << "Error " << std::endl;
            }
            else{
                corona_pt = found->second;
                corona_pt->GetProperty()->SetColor(1,1,1);
                m_RenderWindow->Render();

            }
        }
  }


  private:

};
vtkStandardNewMacro(DualInteractor);
*/
