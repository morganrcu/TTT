#ifndef TTTEXPLORER_H
#define TTTEXPLORER_H

#include <QDialog>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkAxesActor.h>

#include "RawImageDrawer.h"
#include "PlatenessImageDrawer.h"
#include "DiffusedImageDrawer.h"
#include "VertexnessImageDrawer.h"
#include "VertexLocationsDrawer.h"
#include "PrimalGraphDrawer.h"
#include "DualGraphDrawer.h"
#include "MotionVectorDrawer.h"
#include "DefaultColorer.h"


#include "tissuetrackingabstractproject2.h"
namespace Ui {
class TTTExplorer;
}

class TTTExplorer : public QDialog
{
    Q_OBJECT
    
public:
    explicit TTTExplorer(QWidget *parent = 0);
    ~TTTExplorer();
    void SetProject(ttt::TissueTrackingAbstractProject2 * project);
private slots:
	void SetFrame(int frame);
	void ExportMovie();

	void ShowOriginal(bool);
	void ShowEnhanced(bool);
	void ShowPlateness(bool);
	void ShowVertexness(bool);
	void ShowVertexLocations(bool);
	void ShowPrimalGraph(bool);
	void ShowDualGraph(bool);
	void ShowCellRegions(bool);
	void ShowCellMotionVectors(bool);
	void ShowTectonics(bool);
	void ShowEllipses(bool);


private:
	ttt::TissueTrackingAbstractProject2 * m_Project;

private:
    Ui::TTTExplorer *m_pUI;

    ttt::RawImageDrawer m_OriginalDrawer;
    ttt::DiffusedImageDrawer m_EnhancedDrawer;
    ttt::PlatenessImageDrawer m_PlatenessDrawer;
    ttt::VertexnessImageDrawer m_VertexnessDrawer;
    ttt::VertexLocationsDrawer m_VertexLocationsDrawer;
    ttt::PrimalGraphDrawer<ttt::TissueDescriptor> m_PrimalGraphDrawer;
    ttt::DualGraphDrawer<ttt::TissueDescriptor> m_DualGraphDrawer;

    ttt::MotionVectorDrawer m_CellMotionVectorDrawer;

    vtkSmartPointer<vtkRenderer> m_CentralRenderer;
    vtkSmartPointer<vtkRenderWindow> m_CentralRenderWindow;
    vtkSmartPointer<QVTKInteractor> m_CentralRenderWindowInteractor;
};

#endif // TTTEXPLORER_H
