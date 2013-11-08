
class ValidationInteractor : public vtkInteractorStyle
{
  public:
    static ValidationInteractor* New();
    vtkTypeMacro(ValidationInteractor, vtkInteractorStyle);

    vtkSmartPointer<vtkActor> pt1 = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> pt2 = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> pt1prev = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> pt2prev = vtkSmartPointer<vtkActor>::New();
    std::vector<giaa::SkeletonVertexType_lS> selected_svt;
    int chosen = 1;


  virtual void OnLeftButtonDown(){
    vtkInteractorStyle::OnLeftButtonDown();
    double picked[3];
    typedef itk::Point<double,3> itkpt;
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.01);
    picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1],  0, m_CurrentRenderer);
    picker->GetPickPosition(picked);

    std::cout << "Picked actor: " << picker->GetActor() << std::endl;
    pt1 = picker->GetActor();
    if(pt1 != 0){

        double sel_color[3];
        pt1->GetProperty()->GetColor(sel_color);
        if(sel_color[0] == 0 && sel_color[1] == 0 && sel_color[2] == 1 ){
            pt1->GetProperty()->SetColor(1,1,1);

            itkpt color;
            color[0] = 1;
            color[1] = 1;
            color[2] = 1;

            boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,actorTogiaavertex[pt1]).color = color;
            std::cout<< "setting color " << actorTogiaavertex[pt1] << " " << color[0] << " "<< color[1] << " " << color[2] <<std::endl;
        }
        else if(sel_color[0] == 1 && sel_color[1] == 1 && sel_color[2] == 1 ){
            pt1->GetProperty()->SetColor(0,0,1);
            itkpt color;
            color[0] = 0;
            color[1] = 0;
            color[2] = 1;

            boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,actorTogiaavertex[pt1]).color = color;
            std::cout<< "setting color " << actorTogiaavertex[pt1] << " " << color[0] << " "<< color[1] << " " << color[2] <<std::endl;
        }

        m_RenderWindow->Render();
    }
  }
  private:

};

vtkStandardNewMacro(ValidationInteractor);
