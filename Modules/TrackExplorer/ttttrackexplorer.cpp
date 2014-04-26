#include "ttttrackexplorer.h"
#include "ui_ttttrackexplorer.h"
#include <vtkAxesActor.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>



TrackExplorer::TrackExplorer(QWidget *parent) :
    QMainWindow(parent),
    m_pUI(new Ui::TrackExplorer)
{
	m_pUI->setupUi(this);

	m_CentralRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_CentralRenderer->SetBackground(81.0/255,87.0/255,110.0/255);

	m_CentralRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_CentralRenderWindow->AddRenderer(m_CentralRenderer);
	m_CentralRenderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

    m_InteractionStyle = vtkSmartPointer<MouseInteractorStyle>::New();

    m_InteractionStyle->SetDefaultRenderer(m_CentralRenderer);
    m_CentralRenderWindowInteractor = vtkSmartPointer<QVTKInteractor>::New();
    m_CentralRenderWindowInteractor->SetRenderWindow(m_CentralRenderWindow);
    m_CentralRenderWindowInteractor->SetInteractorStyle(m_InteractionStyle);
    m_CentralRenderWindowInteractor->Initialize();




    vtkSmartPointer<vtkAxesActor> axes =  vtkSmartPointer<vtkAxesActor>::New();

    m_CentralRenderer->AddActor(axes);
    m_PrimalDrawer.SetRenderer(m_CentralRenderer);
    m_PrimalDrawer.SetEdgeColorer(&m_EdgeColorer);
    m_PrimalDrawer.SetVertexColorer(&m_VertexColorer);

    this->m_pUI->centralQVTKWidget->SetRenderWindow(m_CentralRenderWindow);

    m_Project.openDB();
    m_Project.OpenProject(2);
    m_Project.SetFrame(0);

    this->m_pUI->frameSlider->setMinimum(0);
    this->m_pUI->frameSlider->setMaximum(this->m_Project.GetNumFrames()-1);

    this->m_pUI->trackSpinBox->setMinimum(0);
    this->m_pUI->trackSpinBox->setMaximum(this->m_Project.GetNumTracks());

    connect(this->m_pUI->frameSlider,SIGNAL(valueChanged(int)),this,SLOT(FrameChanged(int)));
    connect(this->m_pUI->trackSpinBox,SIGNAL(valueChanged(int)),this,SLOT(TrackChanged(int)));

    connect(this->m_InteractionStyle,SIGNAL(ActorSelected(vtkSmartPointer<vtkActor>&)),this,SLOT(CellSelected(vtkSmartPointer<vtkActor>&)));
    m_CurrentTrack=0;
    m_CurrentFrame=0;
    m_HighlightedActor=NULL;

    DrawPrimal();
    DrawPolygons();
}
void TrackExplorer::CellSelected(vtkSmartPointer<vtkActor>& cell){

	ttt::TrackedCellVertexType selected = m_VertexAndActor.right.at(cell);
	ttt::TrackedTissueDescriptor::Pointer tissueDescriptor = this->m_Project.GetTrackedTissueDescriptor();

	int trackID=boost::get(ttt::TrackedCellPropertyTag(),(*tissueDescriptor->m_CellGraph),selected).GetID();
	TrackChanged(trackID);
}

void TrackExplorer::TrackChanged(int track){
	m_CurrentTrack=track;
	this->m_pUI->trackSpinBox->setValue(track);
	DrawPolygon();
	m_CentralRenderWindow->Render();
}
void TrackExplorer::FrameChanged(int frame){
	m_CurrentFrame=frame;
	m_Project.SetFrame(frame);
	DrawPrimal();

	DrawPolygons();
	DrawPolygon();
	m_CentralRenderWindow->Render();
}

void TrackExplorer::DrawPrimal(){
	m_PrimalDrawer.Reset();
	ttt::TrackedTissueDescriptor::Pointer tissueDescriptor = this->m_Project.GetTrackedTissueDescriptor();
	m_PrimalDrawer.SetTissueDescriptor(tissueDescriptor);
	m_PrimalDrawer.Draw();
	m_PrimalDrawer.PickableOff();
	m_PrimalDrawer.Show();

}

void TrackExplorer::DrawPolygons(){
	for(typename CellVertexAndActorBiMap::const_iterator it = m_VertexAndActor.begin();it!=m_VertexAndActor.end();++it){
		m_CentralRenderer->RemoveActor(it->right);
	}
	m_VertexAndActor.clear();
	ttt::TrackedTissueDescriptor::Pointer tissueDescriptor = this->m_Project.GetTrackedTissueDescriptor();

	BGL_FORALL_VERTICES(v,*(tissueDescriptor->m_CellGraph),ttt::TrackedCellGraph){

		//assert(boost::get(ttt::TrackedCellPropertyTag(),*(tissueDescriptor->m_CellGraph),v).GetNumSkeletonPoints()>0);

		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		int npoints = 0;

		for (std::vector<ttt::SkeletonVertexType>::iterator it = boost::get(ttt::TrackedCellPropertyTag(), *(tissueDescriptor->m_CellGraph), v).PerimeterBegin();
				it!= boost::get(ttt::TrackedCellPropertyTag(),*(tissueDescriptor->m_CellGraph), v).PerimeterEnd();
				++it) {

						points->InsertNextPoint(boost::get(ttt::SkeletonPointPropertyTag(),
										*(tissueDescriptor->m_SkeletonGraph), *it).position[0],
								boost::get(ttt::SkeletonPointPropertyTag(),
										*(tissueDescriptor->m_SkeletonGraph), *it).position[1],
								boost::get(ttt::SkeletonPointPropertyTag(),
										*(tissueDescriptor->m_SkeletonGraph), *it).position[2]);

						npoints++;
					}
					vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
					polygon->GetPointIds()->SetNumberOfIds(npoints); //make a quad
					for (int i = 0; i < npoints; i++) {
						polygon->GetPointIds()->SetId(i, i);
					}
					// Add the polygon to a list of polygons
					vtkSmartPointer<vtkCellArray> polygons =
							vtkSmartPointer<vtkCellArray>::New();
					polygons->InsertNextCell(polygon);

					// Create a PolyData
					vtkSmartPointer<vtkPolyData> polygonPolyData =
							vtkSmartPointer<vtkPolyData>::New();
					polygonPolyData->SetPoints(points);
					polygonPolyData->SetPolys(polygons);

					// Create a mapper and actor
					vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<
							vtkPolyDataMapper>::New();

					mapper->SetInputData(polygonPolyData);

					vtkSmartPointer<vtkActor> polygonActor = vtkSmartPointer<vtkActor>::New();

					polygonActor->SetMapper(mapper);
					polygonActor->GetProperty()->SetColor(1,1,1);
					polygonActor->GetProperty()->SetOpacity(0.1);
					polygonActor->SetPickable(1);

					m_CentralRenderer->AddActor(polygonActor);

				    typedef typename CellVertexAndActorBiMap::value_type value_type;

				    m_VertexAndActor.insert(value_type(v,polygonActor));
	}

}
void TrackExplorer::DrawPolygon(){
	ttt::TrackedTissueDescriptor::Pointer tissueDescriptor = this->m_Project.GetTrackedTissueDescriptor();
	ttt::TrackedCellVertexType v = ttt::CellID2VertexDescriptor(m_CurrentTrack,tissueDescriptor);
	if(v!=-1){
		if(m_HighlightedActor){
			m_HighlightedActor->GetProperty()->SetColor(1,1,1);
			m_HighlightedActor->GetProperty()->SetOpacity(0.1);

		}
		m_HighlightedActor=m_VertexAndActor.left.at(v);

		m_HighlightedActor->GetProperty()->SetColor(1.0,1.0,1.0);
		m_HighlightedActor->GetProperty()->SetOpacity(1.0);
	}
#if 0
	ttt::TrackedTissueDescriptor::Pointer tissueDescriptor = this->m_Project.GetTrackedTissueDescriptor();
	if(m_PolygonActor) m_CentralRenderer->RemoveActor(m_PolygonActor);

		ttt::TrackedCellVertexType v = ttt::CellID2VertexDescriptor(m_CurrentTrack,tissueDescriptor);
		if(v!=-1){
			vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
			int npoints = 0;



			for (std::vector<ttt::SkeletonVertexType>::iterator it = boost::get(
					ttt::TrackedCellPropertyTag(), *(tissueDescriptor->m_CellGraph), v).Begin();
					it!= boost::get(ttt::TrackedCellPropertyTag(),
									*(tissueDescriptor->m_CellGraph), v).End(); ++it) {

				points->InsertNextPoint(boost::get(ttt::SkeletonPointPropertyTag(),
								*(tissueDescriptor->m_SkeletonGraph), *it).position[0],
						boost::get(ttt::SkeletonPointPropertyTag(),
								*(tissueDescriptor->m_SkeletonGraph), *it).position[1],
						boost::get(ttt::SkeletonPointPropertyTag(),
								*(tissueDescriptor->m_SkeletonGraph), *it).position[2]);

				npoints++;
			}
			vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
			polygon->GetPointIds()->SetNumberOfIds(npoints); //make a quad
			for (int i = 0; i < npoints; i++) {
				polygon->GetPointIds()->SetId(i, i);
			}
			// Add the polygon to a list of polygons
			vtkSmartPointer<vtkCellArray> polygons =
					vtkSmartPointer<vtkCellArray>::New();
			polygons->InsertNextCell(polygon);

			// Create a PolyData
			vtkSmartPointer<vtkPolyData> polygonPolyData =
					vtkSmartPointer<vtkPolyData>::New();
			polygonPolyData->SetPoints(points);
			polygonPolyData->SetPolys(polygons);

			// Create a mapper and actor
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<
					vtkPolyDataMapper>::New();

			mapper->SetInputData(polygonPolyData);

			m_PolygonActor = vtkSmartPointer<vtkActor>::New();

			m_PolygonActor->SetMapper(mapper);


			std::vector<double> color(3);
			color[0]=1;
			color[1]=0;
			color[2]=0;

			m_CentralRenderer->AddActor(m_PolygonActor);

		}else{
			m_PolygonActor=NULL;
		}
#endif
}
void TrackExplorer::Draw(){




}
TrackExplorer::~TrackExplorer()
{
    delete m_pUI;
}
