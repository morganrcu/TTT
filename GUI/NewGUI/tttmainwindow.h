#ifndef TTTMAINWINDOW_H
#define TTTMAINWINDOW_H

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>

#include <QVTKInteractor.h>

#include <QMainWindow>

#include <mysqltissuetrackingproject.h>

#include <vtkPropPicker.h>

#include "RawImageDrawer.h"
#include "PlatenessImageDrawer.h"
#include "DiffusedImageDrawer.h"
#include "VertexnessImageDrawer.h"
#include "VertexLocationsDrawer.h"
#include "PrimalGraphDrawer.h"
#include "DualGraphDrawer.h"
#include "DefaultColorer.h"
#include "FeatureColorer.h"
#include "TrackedCellId.h"
#include "CellPolygonDrawer.h"


#include "VertexAdditionInteractor.h"
#include "VertexSelectionInteractor.h"

#include "EdgeSelectionInteractor.h"
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
		std::cout << position[0] << ", " << position[1] << ", " << position[2] << std::endl;

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

	void SetVertex(const ttt::AdherensJunctionVertex::Pointer & vertex){
		m_Vertex=vertex;
	}

private:
	vtkSmartPointer<vtkActor> m_Actor;
	vtkSmartPointer<vtkSphereSource> m_SphereSource;
	ttt::AdherensJunctionVertex::Pointer m_Vertex;

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


    void SetupSliders(int length);

    void SetupVertexSelectionInteractor();
    void SetupVertexAdditionInteractor();

    void SetupEdgeSelectionInteractor();
    void SetupEdgeAdditionInteractor();

private slots:
	void New();
	void Open();
	void Close();
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

	void SetupMembranessFrame();
	void SetupEnhancementFrame();
	void SetupVertexFrame();
	void SetupSegmentationFrame();
	void SetupTrackingFrame();

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

	void EdgeSelected(vtkSmartPointer<vtkActor> & actor);
	void EdgeUnselected(vtkSmartPointer<vtkActor> & actor);
	void EdgeAdded(vtkSmartPointer<vtkActor> & a,vtkSmartPointer<vtkActor> & b);
	void EdgeAdditionCancelled();

	void DoDual();
	void DualAndDraw();


	void DoTracking();


	void DrawRangePlateness();
	void DrawEnhancement();
	void DrawVertex();
	void DrawSegmentation();
	void DrawDual();

	void DrawTracking();


	void LaunchProjectExplorer();
	void LaunchPreferences();

	void LowestScaleChanged(double scale);
	void HighestScaleChanged(double scale);
	void ScaleStepsChanged(int steps);
	void ProjectNameChanged(const QString & name);
	void SamplingRateChanged(double rate);
	void SpacingXChanged(double value);
	void SpacingYChanged(double value);
	void SpacingZChanged(double value);

	void ShowPlatenessOnSegmentation(int show);
	void ShowPlatenessOnVertexLocation(int show);
	void ShowDualGraphOnCellSegmentation(int show);
private:
	void SaveSettings();
	void LoadSettings();


private:
    Ui::TTTMainWindow *m_pUI;
    ttt::TissueTrackingProject * m_Project;

    QString m_SettingsFile;
	QString m_Host;
	unsigned int m_Port;
	QString m_DBName;
	QString m_User;
	QString m_Password;

    //RENDERERS
    vtkSmartPointer<vtkRenderer> m_ProjectRenderer;
    vtkSmartPointer<vtkRenderer> m_LowestScaleSelectionRenderer;
    vtkSmartPointer<vtkRenderer> m_HighestScaleSelectionRenderer;
    vtkSmartPointer<vtkRenderer> m_RangeScaleSelectionRenderer;
    vtkSmartPointer<vtkRenderer> m_BeforeEnhancementRenderer;
    vtkSmartPointer<vtkRenderer> m_AfterEnhancementRenderer;
    vtkSmartPointer<vtkRenderer> m_VertexLocationRenderer;
    vtkSmartPointer<vtkRenderer> m_CellSegmentationRenderer;
    vtkSmartPointer<vtkRenderer> m_TrackingRenderer;

    //RENDER WINDOWS

    vtkSmartPointer<vtkRenderWindow> m_ProjectRenderWindow;
    vtkSmartPointer<vtkRenderWindow> m_LowestScaleSelectionRenderWindow;
    vtkSmartPointer<vtkRenderWindow> m_HighestScaleSelectionRenderWindow;
    vtkSmartPointer<vtkRenderWindow> m_RangeScaleSelectionRenderWindow;
    vtkSmartPointer<vtkRenderWindow> m_BeforeEnhancementRenderWindow;
    vtkSmartPointer<vtkRenderWindow> m_AfterEnhancementRenderWindow;
    vtkSmartPointer<vtkRenderWindow> m_VertexLocationRenderWindow;
    vtkSmartPointer<vtkRenderWindow> m_CellSegmentationRendererWindow;
    vtkSmartPointer<vtkRenderWindow> m_TrackingRendererWindow;


    //RENDER WINDOW INTERACTORS

    vtkSmartPointer<QVTKInteractor> m_ProjectRenderWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_LowestScaleSelectionRenderWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_HighestScaleSelectionRenderWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_RangeScaleSelectionRenderWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_BeforeEnhancementRenderWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_AfterEnhancementRenderWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_VertexLocationRenderWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_CellSegmentationRenderWindowInteractor;
    vtkSmartPointer<QVTKInteractor> m_TrackingRenderWindowInteractor;

    //Drawers

    ttt::PlatenessImageDrawer m_LowestPlatenessDrawer;
    ttt::PlatenessImageDrawer m_HighestPlatenessDrawer;
    ttt::PlatenessImageDrawer m_RangePlatenessDrawer;

    ttt::RawImageDrawer m_BeforeEnhancementDrawer;
    ttt::DiffusedImageDrawer m_AfterEnhancementDrawer;

    ttt::VertexnessImageDrawer m_VertexnessImageDrawer;

    ttt::VertexLocationsDrawer m_VertexLocationsDrawer;
    ttt::PlatenessImageDrawer m_PlatenessDrawerOnVertexLocation;

    ttt::PrimalGraphDrawer<ttt::TissueDescriptor> m_PrimalGraphDrawer;
    ttt::PlatenessImageDrawer m_PlatenessDrawerOnSegmentation;

    //TODO put in namespace ttt
	DefaultColorer<ttt::SkeletonVertexType> m_PrimalGraphVertexColorer;
	DefaultColorer<ttt::SkeletonEdgeType> m_PrimalGraphEdgeColorer;

    ttt::DualGraphDrawer<ttt::TissueDescriptor> m_DualGraphDrawer;

    DefaultColorer<ttt::CellVertexType> m_DualGraphVertexColorer;
    DefaultColorer<ttt::CellEdgeType> m_DualGraphEdgeColorer;

    ttt::DualGraphDrawer<ttt::TrackedTissueDescriptor> m_TrackingDrawer;

    ttt::FeatureColorer<TrackedCellId> m_TrackingVertexColorer;

    DefaultColorer<ttt::TrackedCellEdgeType> m_TrackingEdgeColorer;


    //Interactor Styles

    //VertexSelectedCallback * m_pVertexSelectedCallback;
    //VertexUnselectedCallback *  m_pVertexUnselectedCallback;
    //VertexAddedCallback  * m_pVertexAddedCallback;

    vtkSmartPointer<VertexSelectionInteractor> m_VertexSelectionInteractor;
    vtkSmartPointer<VertexAdditionInteractor> m_VertexAdditionInteractor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_StandardInteractor;

    vtkSmartPointer<EdgeSelectionInteractor> m_EdgeSelectionInteractor;
    vtkSmartPointer<EdgeAdditionInteractor> m_EdgeAdditionInteractor;

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_EdgeStandardInteractorStyle;

    vtkSmartPointer<vtkPointWidget> m_PointWidget;




};

#endif // TTTMAINWINDOW_H
