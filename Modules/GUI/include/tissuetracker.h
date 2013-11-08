#ifndef TISSUETRACKER_H
#define TISSUETRACKER_H
#include <QMainWindow>
#include <QVTKWidget.h>
#include <QtGui>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include "itkPoint.h"
#ifndef Q_MOC_RUN
//#include "giaaTissueSegmentation.h"
#include "giaaCellTracker.h"
#endif

#include "PrimalGraphDrawer.h"
#include "PrimalGraphStandardInteractor.h"
#include "PrimalGraphInsertionInteractor.h"
#include "PrimalGraphMoveInteractor.h"

#include "mysqltissuetrackingproject.h"

#include "TrackingDrawer.h"

using namespace boost;
using namespace std;
using namespace ttt;
namespace Ui {
    class TissueTracker;
}
/**
 * Main UI class. Instatiates a TissueTracker.ui form and controls application flow
 */
class TissueTracker : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Data storage type specification for the different objects in the system
     */
#if 0
    typedef
    TissueTrackingProject<
    giaa::TissueSegmentation::RawImageType,
    itk::Image<unsigned char,3>,
    itk::Image<float,3>,
    itk::Image<float,3>,
    itk::Image<itk::Vector<float,3>,3>,
    itk::Image<float,3>,
    itk::PointSet<float, 3> ,
#ifndef VINODTH_STYLE
    giaa::TissueSegmentation::LevelSetImageType,
    giaa::TissueSegmentation::BinaryImageType,
    giaa::TissueSegmentation::SkeletonImageType,
#endif
    giaa::TissueDescriptor,
    giaa::TrackedTissueDescriptor>
    ProjectType;
#endif
    typedef TissueTrackingProject ProjectType;
    /**
     * Class constructor. Set up the components to default values and shows form
     */

    explicit TissueTracker(QWidget *parent = 0);
    ~TissueTracker();

private slots:
	/**
	 * Opens an already existing project
	 */
    void OpenFile();
    /**
     * Rises a new project dialog to create a new project
     */
    void NewProject();
    /**
     * Redraws the 3D view according to the current display setup
     */
    void UpdateDisplay();

    /**
     * Captures the click event from Surface Segmentation button, causing the system to perform the surface segmentation of current frame
     */
    void DoSurfaceSegmentation();
    /**
     * Captures the click event from Anisotropic Diffusion button, causing the system to perform the anisotropic diffusion of current frame. The input to
     * Anisotropic Diffusion is set according to the UI configuration
     */
    void DoAnisotropicDiffusion();
    /**
     * Captures the click event from Plateness button, causing the system to compute the plateness for the current frame. The input to
     * Plateness filter is set according to the UI configuration
     */
    void DoPlateness();
    /**
     * Captures the click event from Vertexness button, causing the system to compute the vertexness for the current frame. The input to
     * Vertexness filter is set according to the UI configuration
     */
    void DoVertexness();
    /**
     * Captures the click event from Vertex location button, causing the system to compute the vertex locations for the current frame.
     */
    void DoVertexLocation();
    /**
     * Captures the click event from Primal Graph button, causing the system to compute tissue segmentation
     */
    void DoPrimalCalculation();
    /**
     * Captures the click event from Validation button, allowing selection of actors in 3D window
     */
    void DoValidation();
    /**
     * Captures the click event from Insert button, inserting new vertices and edges in the primal graph
     */
    void DoInsert();
    /**
     * Captures the click event from Delete button, removing the selected element from primal graph
     */

    void DoMove();


    void DoDeletion();
    /**
     * Captures the click event from Dual button, causing the system to compute dual graph from primal
     */


    void DoDualCalculation();
    void DoTracking();
    /**
     * Captures the click event from Ellipses button, causing the system to compute the corresponding ellipses for the cells
     */
    void DoEllipses();

    /**
    * Captures the click event from Domain button, causing the system to compute the different domains in the tissue with the specified order
    */
    void DoDomains();




    void DoTectonics();



    void SetupCurrentFrame();
    void SetFrame(int frame);

    void SetupNavigation();
    void UpdateControls();

    void DoCurrent();
    void DoAll();







private:

    Ui::TissueTracker * m_pUI;

    QStringList m_Files;

    unsigned int m_CurrentFrame;
    
    bool m_DrawOriginal;
    bool m_DrawDiffused;
    bool m_DrawVesselness;
    bool m_DrawVertexness;
    bool m_DrawLevelset;
    bool m_DrawSegmentation;
    bool m_DrawSkeleton;
    TissueTrackingProject * m_Project;


    vtkSmartPointer<vtkRenderer> m_CurrentRenderer;
    vtkSmartPointer<vtkRenderWindow>  m_RenderWindow;

    vtkSmartPointer<PrimalGraphDrawer> m_PrimalGraphDrawer;
    vtkSmartPointer<PrimalGraphStandardInteractor> m_PrimalGraphStandardInteractor;
    vtkSmartPointer<PrimalGraphMoveInteractor> m_PrimalGraphMoveInteractor;
    vtkSmartPointer<PrimalGraphInsertionInteractor> m_PrimalGraphInsertionInteractor;


    vtkSmartPointer<QVTKInteractor> m_RenderWindowInteractor;// = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    vtkSmartPointer<TrackingDrawer> m_TrackingDrawer;


};

#endif // TISSUETRACKER_H
