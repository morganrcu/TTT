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
class TrackExplorer;
}

class TrackExplorer : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrackExplorer(QWidget *parent = 0);
    ~TrackExplorer();

private slots:

	void TrackChanged(int track);
	void FrameChanged(int track);
	void CellSelected(vtkSmartPointer<vtkActor>& cell);
private:
	void Draw();
	void DrawPrimal();
	void DrawPolygon();
	void DrawPolygons();
private:
    Ui::TrackExplorer *m_pUI;


    vtkSmartPointer<vtkRenderer> m_CentralRenderer;
    vtkSmartPointer<vtkRenderWindow> m_CentralRenderWindow;
    vtkSmartPointer<QVTKInteractor> m_CentralRenderWindowInteractor;

    vtkSmartPointer<MouseInteractorStyle> m_InteractionStyle;

    ttt::MySQLTissueTrackingProject m_Project;

    ttt::PrimalGraphDrawer<ttt::TrackedTissueDescriptor> m_PrimalDrawer;
    DefaultColorer<ttt::SkeletonVertexType> m_VertexColorer;
    DefaultColorer<ttt::SkeletonEdgeType> m_EdgeColorer;

    vtkSmartPointer<vtkActor> m_HighlightedActor;

    typedef boost::bimap< boost::bimaps::set_of<ttt::TrackedCellVertexType>, boost::bimaps::set_of<vtkSmartPointer<vtkActor> > > CellVertexAndActorBiMap;
    CellVertexAndActorBiMap m_VertexAndActor;

    int m_CurrentTrack;
    int m_CurrentFrame;

};

#endif // TTTTRACKEXPLORER_H
