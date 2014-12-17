#ifndef TTTMAINWINDOW_H
#define TTTMAINWINDOW_H

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <memory>
#include <QVTKInteractor.h>

#include <QMainWindow>

#include "tissuetrackingabstractproject2.h"

#include <vtkPropPicker.h>

#include "RawImageDrawer.h"
#include "PlatenessImageDrawer.h"
#include "DiffusedImageDrawer.h"
#include "VertexnessImageDrawer.h"
#include "VertexLocationsDrawer.h"
#include "PrimalGraphDrawer.h"
#include "DualGraphDrawer.h"
#include "EllipseDrawer.h"

#include "DefaultColorer.h"
#include "FeatureColorer.h"
//#include "TrackedCellId.h"
#include "CellPolygonDrawer.h"
#include "DomainStrainRatesDrawer.h"

#include "VertexAdditionInteractor.h"
#include "VertexSelectionInteractor.h"

#include "EdgeSelectionInteractor.h"
#include "VertexAdditionToPrimalInteractor.h"
#include "EdgeAdditionInteractor.h"

namespace Ui {
class TTTMainWindow;
}



class FollowVertexCallback: public vtkCommand {
public:
	typedef itk::FixedArray<double,3> SpacingType;
	static FollowVertexCallback *New() {
		return new FollowVertexCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*) {
		vtkPointWidget *pointWidget = reinterpret_cast<vtkPointWidget*>(caller);
		//pointWidget->GetPolyData(this->PolyData);

		double position[3];
		pointWidget->GetPosition(position);
		m_SphereSource->SetCenter(position);
		m_SphereSource->Update();

		itk::Index<3> index;

		index[0]=round(position[0]/m_Spacing[0]);
		index[1]=round(position[1]/m_Spacing[1]);
		index[2]=round(position[2]/m_Spacing[2]);

		m_Vertex->SetPosition(index);

	}
private:
	FollowVertexCallback() {

	}
public:
	void SetActor(const vtkSmartPointer<vtkActor> & actor){
		m_Actor=actor;
	}

	void SetSpacing(const SpacingType & spacing){
		m_Spacing=spacing;
	}

	void SetSphereSource(const vtkSmartPointer<vtkSphereSource> & sphere){
		m_SphereSource=sphere;
	}

	void SetVertex(const ttt::AdherensJunctionVertex<3>::Pointer & vertex){
		m_Vertex=vertex;
	}


private:
	vtkSmartPointer<vtkActor> m_Actor;
	vtkSmartPointer<vtkSphereSource> m_SphereSource;
	ttt::AdherensJunctionVertex<3>::Pointer m_Vertex;

	SpacingType m_Spacing;
};

class FollowVertexPrimalCallback: public vtkCommand {
public:
	typedef itk::FixedArray<double,3> SpacingType;
	static FollowVertexPrimalCallback *New() {
		return new FollowVertexPrimalCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*) {
		vtkPointWidget *pointWidget = reinterpret_cast<vtkPointWidget*>(caller);
		//pointWidget->GetPolyData(this->PolyData);

		double position[3];
		pointWidget->GetPosition(position);
		m_SphereSource->SetCenter(position);
		m_SphereSource->Update();

		boost::get(ttt::SkeletonPointPropertyTag<3>(),m_TissueDescriptor->GetAJGraph(),m_Vertex).position[0]=position[0];
		boost::get(ttt::SkeletonPointPropertyTag<3>(),m_TissueDescriptor->GetAJGraph(),m_Vertex).position[1]=position[1];
		boost::get(ttt::SkeletonPointPropertyTag<3>(),m_TissueDescriptor->GetAJGraph(),m_Vertex).position[2]=position[2];

	}
private:
	FollowVertexPrimalCallback() {

	}
public:
	void SetActor(const vtkSmartPointer<vtkActor> & actor){
		m_Actor=actor;
	}

	void SetSphereSource(const vtkSmartPointer<vtkSphereSource> & sphere){
		m_SphereSource=sphere;
	}

	void SetVertex(const ttt::TissueDescriptorTraits<ttt::TissueDescriptor<3> >::SkeletonVertexType & vertex){
		m_Vertex=vertex;
	}
	void SetTissueDescriptor(const ttt::TissueDescriptor<3>::Pointer & tissueDescriptor){
		m_TissueDescriptor=tissueDescriptor;
	}

private:
	vtkSmartPointer<vtkActor> m_Actor;
	vtkSmartPointer<vtkSphereSource> m_SphereSource;
	ttt::TissueDescriptorTraits<ttt::TissueDescriptor<3> >::SkeletonVertexType m_Vertex;

	ttt::TissueDescriptor<3>::Pointer m_TissueDescriptor;
	SpacingType m_Spacing;
};

class TTTMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TTTMainWindow(QWidget *parent = 0);
    ~TTTMainWindow();
    
private:
    void SetupProjectTab();

    void SetupSliders(unsigned int length);

    void SetupVertexStandardInteractor();
    void SetupVertexSelectionInteractor();
    void SetupVertexAdditionInteractor();
    void SetupVertexAdditionToPrimalInteractor();

    void SetupEdgeSelectionInteractor();
    void SetupEdgeAdditionInteractor();

    void SetupSliderPositions(int length);

private slots:
	void New();
	void OpenMySQL();
	void OpenJSON();
	void Close();
	void Save();
	void ShowPreferences();

	void AddFrame();
	void RemoveFrame();
	void FrameDown();
	void FrameUp();

	void HighlightProjectFrame(int frame);

	void SetupCurrentTab();

	void DoLowestScale();
	void DoHighestScale();
	void DoRangeScale();
	void DoAllScale();

	void SetupMembranessFrame(int frame);
	void SetupEnhancementFrame(int frame);
	void SetupVertexFrame(int frame);
	void SetupSegmentationFrame(int frame);
	void SetupTrackingFrame(int frame);
	void SetupTectonicsFrame(int frame);

	void EnhanceAndDraw();

	void DoEnhance();
	void DoAllEnhance();

	void VertexnessAndDraw();
	void DoVertexness();


	void VertexLocationAndDraw();
	void DoVertexLocation();
	void SelectVertex();
	void AddVertex();
	void DeleteVertex();

	void VertexSelected(vtkSmartPointer<vtkActor> & actor);
	void VertexUnselected(vtkSmartPointer<vtkActor> & actor);
	void VertexAdded(vtkSmartPointer<vtkActor> & actor);
	void VertexAdditionCancelled();

	void DoAllVertex();

	void SegmentationAndDraw();
	void DoSegmentation();
	void SelectEdge();
	void AddEdge();
	void DeleteEdge();
	void AddVertexToPrimal();

	void VertexAddedToPrimal(vtkSmartPointer<vtkActor> & actor);

	void EdgeSelected(vtkSmartPointer<vtkActor> & actor);
	void EdgeUnselected(vtkSmartPointer<vtkActor> & actor);
	void EdgeAdded(vtkSmartPointer<vtkActor> & a,vtkSmartPointer<vtkActor> & b);
	void EdgeAdditionCancelled();

	void DoDual();
	void DualAndDraw();


	void DoTracking();
	void DoInitTracking();
	void DoTrackingInteractive();
	void DoDetectTrackingOutliers();
	void DoEllipses();
	void DoTectonics();

	void PopulateFeatureTable();
	void HighlightCellInFeatureTable();
	void HighlightCellInRepresentation();


	void DrawRangePlateness();
	void DrawEnhancement();
	void DrawVertex();
	void DrawSegmentation();
	//void DrawDual();

	void DrawTracking();
	void DrawTrackingInteractive();

	void DrawInspection();

	void DrawTectonics();

	void DrawEllipses();

	void LaunchProjectExplorer();
	void LaunchPreferences();

	void PlatenessLowestScaleChanged(double scale);
	void PlatenessHighestScaleChanged(double scale);
	void PlatenessScaleStepsChanged(int steps);

	void VertexnessLowestScaleChanged(double scale);
	void VertexnessHighestScaleChanged(double scale);
	void VertexnessScaleStepsChanged(int steps);

	void ProjectNameChanged(const QString & name);
	void SamplingRateChanged(double rate);
	void SpacingXChanged(double value);
	void SpacingYChanged(double value);
	void SpacingZChanged(double value);

	void ShowPlatenessOnSegmentation(int show);
	void ShowPlatenessOnVertexLocation(int show);
	void ShowVertexLocationsOnVertexLocation(int show);
	void ShowVertexnessOnVertexLocation(int show);

	void ShowAJGraphOnCellSegmentation(int show);
	void ShowDualGraphOnCellSegmentation(int show);
	void ShowTrackedEllipsesAtTectonics(int show);

	void ShowCellStrainRates();
	void ShowTissueStrainRates();
	void ShowIntercalationStrainRates();
private:
	void SaveSettings();
	void LoadSettings();



private:

//GLOBAL MEMBERS


    Ui::TTTMainWindow *m_pUI;
    std::shared_ptr<ttt::TissueTrackingAbstractProject2<3> >  m_Project;

    QString m_SettingsFile;
	QString m_Host;
	unsigned int m_Port;
	QString m_DBName;
	QString m_User;
	QString m_Password;

	unsigned int m_CurrentFrame;

//PROJECT TAB
	vtkSmartPointer<vtkRenderer> m_ProjectRenderer;
	vtkSmartPointer<vtkRenderWindow> m_ProjectRenderWindow;
	vtkSmartPointer<QVTKInteractor> m_ProjectRenderWindowInteractor;


//SCALE SELECTION TAB
	vtkSmartPointer<vtkRenderer> m_LowestScaleSelectionRenderer;
	vtkSmartPointer<vtkRenderer> m_HighestScaleSelectionRenderer;
	vtkSmartPointer<vtkRenderer> m_RangeScaleSelectionRenderer;

	vtkSmartPointer<vtkRenderWindow> m_LowestScaleSelectionRenderWindow;
	vtkSmartPointer<vtkRenderWindow> m_HighestScaleSelectionRenderWindow;
	vtkSmartPointer<vtkRenderWindow> m_RangeScaleSelectionRenderWindow;

	vtkSmartPointer<QVTKInteractor> m_LowestScaleSelectionRenderWindowInteractor;
	vtkSmartPointer<QVTKInteractor> m_HighestScaleSelectionRenderWindowInteractor;
	vtkSmartPointer<QVTKInteractor> m_RangeScaleSelectionRenderWindowInteractor;

	ttt::PlatenessImageDrawer m_LowestPlatenessDrawer;
	ttt::PlatenessImageDrawer m_HighestPlatenessDrawer;
	ttt::PlatenessImageDrawer m_RangePlatenessDrawer;

	//ENHANCEMENT TAB
	vtkSmartPointer<vtkRenderer> m_BeforeEnhancementRenderer;
	vtkSmartPointer<vtkRenderer> m_AfterEnhancementRenderer;

	vtkSmartPointer<vtkRenderWindow> m_BeforeEnhancementRenderWindow;
	vtkSmartPointer<vtkRenderWindow> m_AfterEnhancementRenderWindow;

	vtkSmartPointer<QVTKInteractor> m_BeforeEnhancementRenderWindowInteractor;
	vtkSmartPointer<QVTKInteractor> m_AfterEnhancementRenderWindowInteractor;

    ttt::RawImageDrawer m_BeforeEnhancementDrawer;
    ttt::DiffusedImageDrawer m_AfterEnhancementDrawer;

	//VERTEX LOCATION TAB
    vtkSmartPointer<vtkRenderer> m_VertexLocationRenderer;
    vtkSmartPointer<vtkRenderWindow> m_VertexLocationRenderWindow;
    vtkSmartPointer<QVTKInteractor> m_VertexLocationRenderWindowInteractor;

    ttt::VertexnessImageDrawer m_VertexnessImageDrawer;
    ttt::VertexLocationsDrawer m_VertexLocationsDrawer;
    ttt::PlatenessImageDrawer m_PlatenessDrawerOnVertexLocation;


    vtkSmartPointer<VertexSelectionInteractor> m_VertexSelectionInteractor;
    vtkSmartPointer<VertexAdditionInteractor> m_VertexAdditionInteractor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_StandardInteractor;
    vtkSmartPointer<vtkPointWidget> m_VertexLocationPointWidget;

    ttt::AdherensJunctionVertices<3>::Pointer m_DrawnAJVertices;


    //CELL SEGMENTATION TAB

    vtkSmartPointer<vtkRenderer> m_CellSegmentationRenderer;
    vtkSmartPointer<vtkRenderWindow> m_CellSegmentationRendererWindow;
    vtkSmartPointer<QVTKInteractor> m_CellSegmentationRenderWindowInteractor;

    ttt::PrimalGraphDrawer<ttt::TissueDescriptor<3> > m_PrimalGraphDrawer;
    ttt::PlatenessImageDrawer m_PlatenessDrawerOnSegmentation;

	DefaultColorer<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<3> >::SkeletonVertexType> m_PrimalGraphVertexColorer;
	DefaultColorer<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<3>>::SkeletonEdgeType> m_PrimalGraphEdgeColorer;

    ttt::DualGraphDrawer<ttt::TissueDescriptor<3> > m_DualGraphDrawer;

    DefaultColorer<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<3> >::CellVertexType> m_DualGraphVertexColorer;
    DefaultColorer<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<3> >::CellEdgeType> m_DualGraphEdgeColorer;

    vtkSmartPointer<EdgeSelectionInteractor> m_EdgeSelectionInteractor;
    vtkSmartPointer<EdgeAdditionInteractor> m_EdgeAdditionInteractor;
    vtkSmartPointer<VertexAdditionToPrimalInteractor> m_VertexAdditionToPrimalInteractor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_EdgeStandardInteractorStyle;

    vtkSmartPointer<vtkPointWidget> m_PrimalGraphVertexPointWidget;

    ttt::TissueDescriptor<3>::Pointer m_DrawnTissueDescriptor;

    //TRACKING TAB
    vtkSmartPointer<vtkRenderer> m_TrackingRenderer;
    vtkSmartPointer<vtkRenderWindow> m_TrackingRendererWindow;
    vtkSmartPointer<QVTKInteractor> m_TrackingRenderWindowInteractor;
    ttt::PrimalGraphDrawer<ttt::TrackedTissueDescriptor<3> > m_PrimalGraphTrackingDrawer;
    ttt::CellPolygonDrawer<ttt::TrackedTissueDescriptor<3> > m_TrackingDrawer;

    ttt::FeatureColorer<ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<3> >::CellVertexType,unsigned int > m_TrackingVertexColorer;


    DefaultColorer<ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<3> >::CellEdgeType> m_TrackingEdgeColorer;

    ttt::TrackedTissueDescriptor<3>::Pointer m_DrawnTrackedTissueDescriptor;
    //INTERACTIVE TRACKING TAB
    vtkSmartPointer<vtkRenderer> m_PreviousTrackingRenderer;
    vtkSmartPointer<vtkRenderer> m_CurrentTrackingRenderer;
    vtkSmartPointer<vtkRenderWindow> m_PreviousTrackingRendererWindow;
    vtkSmartPointer<vtkRenderWindow> m_CurrentTrackingRendererWindow;

    vtkSmartPointer<QVTKInteractor> m_PreviousTrackingRendererWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_CurrentTrackingRendererWindowInteractor;


    //INSPECTION TAB
    vtkSmartPointer<vtkRenderer> m_InspectionRenderer;
    vtkSmartPointer<vtkRenderWindow> m_InspectionRenderWindow;
    vtkSmartPointer<QVTKInteractor> m_InspectionRenderWindowInteractor;

    ttt::CellPolygonDrawer<ttt::TrackedTissueDescriptor<3> > m_InspectionDrawer;

    //TECTONICS TAB
    vtkSmartPointer<vtkRenderer> m_TectonicsRenderer;
    vtkSmartPointer<vtkRenderWindow> m_TectonicsRenderWindow;
    vtkSmartPointer<QVTKInteractor> m_TectonicsRenderWindowInteractor;

    ttt::EllipseDrawer m_EllipseDrawer;

    //ttt::DomainStrainRatesDrawer m_DomainStrainRatesDrawer;








};

#endif // TTTMAINWINDOW_H
