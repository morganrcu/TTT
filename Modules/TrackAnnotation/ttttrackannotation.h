#ifndef TTTTRACKEXPLORER_H
#define TTTTRACKEXPLORER_H

#include <QMainWindow>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <QVTKInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>

#include <mysqltissuetrackingproject.h>
#include "PrimalGraphDrawer.h"
#include "DefaultColorer.h"


#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>


#include "MouseInteractorStyle.h"
namespace Ui {
class TrackAnnotation;
}

class TrackAnnotation : public QMainWindow
{
    Q_OBJECT


public:
    explicit TrackAnnotation(QWidget *parent = 0);
    ~TrackAnnotation();

private slots:

	void GlobalTrackChanged(int track);
	void CurrentTrackChanged(int track);
	void NextTrackChanged(int track);

	void FrameChanged(int track);


	void CurrentCellSelected(vtkSmartPointer<vtkActor>& cell);
	void NextCellSelected(vtkSmartPointer<vtkActor>& cell);

	void DoCorrection();
private:
	void SplitCorrespondence(const ttt::TrackedTissueDescriptor::Pointer & t0,
			ttt::TrackedTissueDescriptor::Pointer & t1,
			std::map<int,int> & associations,
			std::vector<int> & creations,
			std::vector<int> & terminations,
			std::map<int,std::pair<int,int> > & mitosis);
	void Draw();
	void ColorPolygons();
	void DrawPrimalNow();
	void DrawPrimalNext();
	void HighlightPolygonNow(int track);
	void DrawPolygonsNow();
	void HighlightPolygonNext(int track);

	void DrawPolygonsNext();
	typedef boost::bimap< boost::bimaps::set_of<ttt::TrackedCellVertexType>, boost::bimaps::set_of<vtkSmartPointer<vtkActor> > > CellVertexAndActorBiMap;
	void DrawPolygons(CellVertexAndActorBiMap & storage, ttt::TrackedTissueDescriptor::Pointer & tissue,vtkSmartPointer<vtkRenderer> & renderer);

	void HighlightPolygon(CellVertexAndActorBiMap & storage, ttt::TrackedTissueDescriptor::Pointer & tissue,int track,vtkSmartPointer<vtkActor> & highlightedActorA,vtkSmartPointer<vtkActor> & highlightedActorB);
private:
    Ui::TrackAnnotation *m_pUI;


    vtkSmartPointer<vtkRenderer> m_NowRenderer;
    vtkSmartPointer<vtkRenderWindow> m_NowRenderWindow;
    vtkSmartPointer<QVTKInteractor> m_NowRenderWindowInteractor;
    vtkSmartPointer<MouseInteractorStyle> m_NowInteractionStyle;

    vtkSmartPointer<vtkRenderer> m_NextRenderer;
    vtkSmartPointer<vtkRenderWindow> m_NextRenderWindow;
    vtkSmartPointer<QVTKInteractor> m_NextRenderWindowInteractor;
    vtkSmartPointer<MouseInteractorStyle> m_NextInteractionStyle;

    ttt::MySQLTissueTrackingProject m_Project;

    ttt::PrimalGraphDrawer<ttt::TrackedTissueDescriptor> m_NowPrimalDrawer;
    DefaultColorer<ttt::SkeletonVertexType> m_NowVertexColorer;
    DefaultColorer<ttt::SkeletonEdgeType> m_NowEdgeColorer;
    vtkSmartPointer<vtkActor> m_NowHighlightedActor;


    ttt::PrimalGraphDrawer<ttt::TrackedTissueDescriptor> m_NextPrimalDrawer;
    DefaultColorer<ttt::SkeletonVertexType> m_NextVertexColorer;
    DefaultColorer<ttt::SkeletonEdgeType> m_NextEdgeColorer;
    vtkSmartPointer<vtkActor> m_NextHighlightedActorA;
    vtkSmartPointer<vtkActor> m_NextHighlightedActorB;

	std::map<int,int>  m_Associations;
	std::vector<int>  m_Creations;
	std::vector<int>  m_Terminations;
	std::map<int,std::pair<int,int> >  m_Mitosis;

    CellVertexAndActorBiMap m_NowVertexAndActor;
    CellVertexAndActorBiMap m_NextVertexAndActor;

    ttt::TrackedTissueDescriptor::Pointer m_CurrentTissueDescriptor;
    ttt::TrackedTissueDescriptor::Pointer m_NextTissueDescriptor;

    int m_CurrentTrack;
    int m_NextTrack;
    int m_GlobalTrack;
    int m_CurrentFrame;

};

#endif // TTTTRACKEXPLORER_H
