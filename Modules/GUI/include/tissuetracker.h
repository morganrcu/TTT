/** \addtogroup TTTGUI
 *  @{
 */
#ifndef TISSUETRACKER_H
#define TISSUETRACKER_H
#include <QMainWindow>
#include <QVTKWidget.h>
#include <QtGui>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkPropPicker.h>
#include "itkPoint.h"

#include "VertexLocationsDrawer.h"

#include "PrimalGraphDrawer.h"
#include "PrimalGraphStandardInteractor.h"
#include "PrimalGraphInsertionInteractor.h"
#include "PrimalGraphMoveInteractor.h"

#include "mysqltissuetrackingproject.h"

#include "CellPolygonDrawer.h"

#include "VertexSelectedCallback.h"
#include "VertexUnselectedCallback.h"
#include "VertexAddedCallback.h"
//#include "VertexAdditionCancelledCallback.h"

//#include "VertexSelectionInteractor.h"
//#include "VertexAdditionInteractor.h"

#include "GreetingsDrawer.h"
#include "RawImageDrawer.h"
#include "DiffusedImageDrawer.h"
#include "PlatenessImageDrawer.h"
#include "VertexnessImageDrawer.h"
#include "VertexLocationsDrawer.h"


#include "PrimalGraphDrawer.h"
#include "DualGraphDrawer.h"

#include "MotionVectorDrawer.h"
#include "EllipseDrawer.h"
#include "DomainStrainRatesDrawer.h"
#include "AbstractTissueTracker.h"
#include "ui_tissuetracker.h"
using namespace boost;
using namespace std;
using namespace ttt;
namespace Ui {
    class TissueTracker;
}

class VertexSelectionInteractor;
class VertexAdditionInteractor;
class VertexSelectedCallback;
class VertexUnselectedCallback;
class VertexAddedCallback;

class AbstractTissueTracker{
public:

	virtual ttt::TissueTrackingProject * GetProject()=0;
	virtual vtkSmartPointer<QVTKInteractor>  GetRenderWindowInteractor()=0;
	virtual vtkSmartPointer<VertexSelectionInteractor> GetVertexSelectionInteractor()=0;
	virtual void SetInteractionModeToVertexSelection()=0;
	AbstractTissueTracker(){

	}
	virtual ~AbstractTissueTracker(){

	}
};

/**
 * Main UI class. Instatiates a TissueTracker.ui form and controls application flow
 */
class TissueTracker : public QMainWindow, public AbstractTissueTracker
{
    Q_OBJECT

    typedef TissueTrackingProject ProjectType;
public:
    /**
     * Class constructor. Set up the components to default values and shows form
     */

    explicit TissueTracker(QWidget *parent = 0);
    ~TissueTracker();

public slots:

	/**
	 * TODO
	 */
	void SelectVertex();
	/**
	 * TODO
	 */
	void AddVertex();
	/**
	 * TODO
	 */
	void DeleteVertex();
	/**
	 * TODO
	 */
	void FinishVertex();

	/**
	 * TODO
	 */
	void ResetView();
	/**
	 * TODO
	 */
	void TakeScreenshot();

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
     * TODO
     */
    void DoLateralImageSegmentation();

    /**
     * TODO
     */
    void DoCLAHE();
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
    void UpdateVisibility();

    void DoCurrent();
    void DoAll();

public:

    virtual TissueTrackingProject * GetProject(){
    	return m_Project;
    }



private:

    Ui::TissueTracker * m_pUI;

    QStringList m_Files;

    unsigned int m_CurrentFrame;

    TissueTrackingProject * m_Project;



    vtkSmartPointer<vtkRenderer> m_CurrentRenderer;
    vtkSmartPointer<vtkRenderWindow>  m_RenderWindow;
#if 0
    vtkSmartPointer<PrimalGraphDrawer> m_PrimalGraphDrawer;
    vtkSmartPointer<PrimalGraphStandardInteractor> m_PrimalGraphStandardInteractor;
    vtkSmartPointer<PrimalGraphMoveInteractor> m_PrimalGraphMoveInteractor;
    vtkSmartPointer<PrimalGraphInsertionInteractor> m_PrimalGraphInsertionInteractor;
#endif

    vtkSmartPointer<QVTKInteractor> m_RenderWindowInteractor;// = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    //vtkSmartPointer<TrackingDrawer> m_TrackingDrawer;

private:
    GreetingsDrawer m_GreetingsDrawer;
    RawImageDrawer m_OriginalImageDrawer;
    RawImageDrawer m_LateralImageVolumeSegmentedImageDrawer;
    RawImageDrawer m_CLAHEDImageDrawer;
    RawImageDrawer m_SurfaceSegmentedImageDrawer;
    DiffusedImageDrawer m_DiffusedImageDrawer;
    PlatenessImageDrawer m_PlatenessImageDrawer;
    VertexnessImageDrawer m_VertexnessImageDrawer;
    VertexLocationsDrawer m_VertexLocationsDrawer;

    PrimalGraphDrawer m_PrimalGraphDrawer;
    DualGraphDrawer m_DualGraphDrawer;

private slots:
	void SetShowOriginalImage(bool state);
	void SetShowLateralImageVolumeSegmentedImage(bool state);
	void SetShowCLAHEDImage(bool state);
	void SetShowSurfaceSegmentedImage(bool state);
	void SetShowDiffusedImage(bool state);
	void SetShowPlatenessImage(bool state);
	void SetShowVertexnessImage(bool state);
	void SetShowVertexLocations(bool state);
	void SetShowPrimalGraph(bool state);
	void SetShowDualGraph(bool state);

private:
	void DrawOriginalImage();
	void DrawLateralImageVolumeSegmentedImage();
	void DrawCLAHEDImage();
	void DrawSurfaceSegmentedImage();
	void DrawDiffusedImage();
	void DrawPlatenessImage();
	void DrawVertexnessImage();
	void DrawVertexLocations();
	void DrawPrimalGraph();
	void DrawDualGraph();

private:
    VertexSelectedCallback * m_pVertexSelectedCallback;
    VertexUnselectedCallback *  m_pVertexUnselectedCallback;
    VertexAddedCallback  * m_pVertexAddedCallback;

    vtkSmartPointer<VertexSelectionInteractor> m_VertexSelectionInteractor;
    vtkSmartPointer<VertexAdditionInteractor> m_VertexAdditionInteractor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_StandardInteractor;


public:
    void SetInteractionModeToStandard();
    void SetInteractionModeToVertexSelection();
    void SetInteractionModeToAddVertex();

    void SetupVertexSelectionInteractor();
    void SetupVertexAdditionInteractor();
    void SetupStandardInteractor();

    inline VertexSelectedCallback * GetVertexSelectedCallback(){
    	return m_pVertexSelectedCallback;
    }

    inline VertexUnselectedCallback * GetVertexUnselectedCallback(){
    	return m_pVertexUnselectedCallback;
    }

    inline VertexAddedCallback * GetAddVertexCallback(){
    	return m_pVertexAddedCallback;
    }

    inline vtkSmartPointer<VertexSelectionInteractor> GetVertexSelectionInteractor(){
    	return m_VertexSelectionInteractor;
    }


public:
    vtkSmartPointer<QVTKInteractor> GetRenderWindowInteractor(){
    	return m_RenderWindowInteractor;
    }

};


template< class T> class GUICallback {
public:

	GUICallback(){
		m_pTissueTracker=0;
		m_pUITissueTracker=0;
		m_Caller=0;
	}
	virtual ~GUICallback (){

	}
	virtual void Notify()=0;

	inline void SetCaller(const vtkSmartPointer<T> & caller ){
		m_Caller =caller;
	}

	inline vtkSmartPointer<T> GetCaller(){
		return m_Caller;
	}

	inline void SetUI( Ui::TissueTracker * pUI){
		m_pUITissueTracker=pUI;
	}

	Ui::TissueTracker * GetUI(){
		return m_pUITissueTracker;
	}

	inline void SetTissueTracker( AbstractTissueTracker * pTissueTracker){
		m_pTissueTracker=pTissueTracker;
	}

	AbstractTissueTracker * GetTissueTracker(){
		return m_pTissueTracker;
	}

private:
	vtkSmartPointer<T>  m_Caller;
	Ui::TissueTracker * m_pUITissueTracker;
	AbstractTissueTracker * m_pTissueTracker;

};

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





class VertexSelectionInteractor : public vtkInteractorStyleTrackballCamera {
private:

    vtkSmartPointer<vtkPointWidget> m_PointWidget;
    GUICallback<VertexSelectionInteractor> * m_pVertexSelected;
    GUICallback<VertexSelectionInteractor> * m_pVertexUnselected;

protected:
	vtkSmartPointer<VertexLocationsDrawer> m_Drawer;
	vtkSmartPointer<vtkRenderer> m_Renderer;
	vtkSmartPointer<vtkActor> m_PickedVertexActor;
	vtkSmartPointer<vtkProperty> m_PickedProperty;

public:

	vtkTypeMacro(VertexSelectionInteractor, vtkInteractorStyleTrackballCamera);

	inline vtkSmartPointer<vtkActor> GetPickedVertexActor(){
		return m_PickedVertexActor;
	}

	inline void SetVertexSelectedCallback(GUICallback<VertexSelectionInteractor> * vertexSelected){
		m_pVertexSelected=vertexSelected;
	}

	inline void SetVertexUnselectedCallback(GUICallback<VertexSelectionInteractor> * vertexUnselected){
		m_pVertexUnselected=vertexUnselected;
	}

	inline void SetVertexLocationsDrawer(const vtkSmartPointer<VertexLocationsDrawer> & drawer){
		m_Drawer=drawer;
	}

	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}

	void SetSelection(const vtkSmartPointer<vtkActor> & actor){
		this->UnsetSelection();

		// Save the property of the picked actor so that we can
		// restore it next time
		this->m_PickedVertexActor = actor;

		this->m_PickedProperty->DeepCopy(this->m_PickedVertexActor->GetProperty());
		// Highlight the picked actor by changing its properties
		this->m_PickedVertexActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		this->m_PickedVertexActor->GetProperty()->SetDiffuse(1.0);
		this->m_PickedVertexActor->GetProperty()->SetSpecular(0.0);

		if (this->m_pVertexSelected) {
			this->m_pVertexSelected->SetCaller(this);
			this->m_pVertexSelected->Notify();
		}
	}

	void UnsetSelection(){
		if (this->m_PickedVertexActor){
			this->m_PickedVertexActor->GetProperty()->DeepCopy(this->m_PickedProperty);
			m_PointWidget->Off();
			if(this->m_pVertexUnselected){
				this->m_pVertexUnselected->SetCaller(this);
				this->m_pVertexUnselected->Notify();
			}
		}
		this->m_PickedVertexActor=0;
	}

	static VertexSelectionInteractor* New(){
		return new VertexSelectionInteractor;
	}
public:
	VertexSelectionInteractor(){

		m_PickedVertexActor=0;
		m_PickedProperty=vtkSmartPointer<vtkProperty>::New();

		m_PointWidget= vtkSmartPointer<vtkPointWidget>::New();

		m_pVertexSelected=0;
		m_pVertexUnselected=0;

	}
	virtual ~VertexSelectionInteractor(){

	}
	virtual void OnLeftButtonDown(){
		int* clickPos = this->GetInteractor()->GetEventPosition();

			// Pick from this location.
			vtkSmartPointer<vtkPropPicker>  picker =  	vtkSmartPointer<vtkPropPicker>::New();
			picker->Pick(clickPos[0], clickPos[1], 0, m_Renderer);

			vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();

			if(pickedActor){
				this->SetSelection(pickedActor);
			}

			vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
	virtual void OnRightButtonUp(){

		this->UnsetSelection();
		vtkInteractorStyleTrackballCamera::OnRightButtonUp();
	}
};


class VertexAdditionInteractor : public vtkInteractorStyleTrackballCamera {
private:

    GUICallback<VertexAdditionInteractor> * m_pVertexAdded;
    GUICallback<VertexAdditionInteractor> * m_pAdditionCancelled;
    itk::FixedArray<double,3> m_Spacing;
    ttt::AdherensJunctionVertices::Pointer m_AdherensJunctionsVertices;
    vtkSmartPointer<VertexLocationsDrawer> m_VertexLocationsDrawer;

    vtkSmartPointer<vtkActor> m_AddedActor;
    vtkSmartPointer<vtkSphereSource> m_AddedSphere;
    vtkSmartPointer<vtkPolyDataMapper> m_AddedMapper;
    ttt::AdherensJunctionVertex::Pointer m_AddedVertex;

protected:
	vtkSmartPointer<vtkRenderer> m_Renderer;

public:

	vtkTypeMacro(VertexAdditionInteractor, vtkInteractorStyleTrackballCamera);


	inline vtkSmartPointer<vtkActor> GetAddedActor(){
		return m_AddedActor;
	}
	inline void SetVertexAddedCallback(GUICallback<VertexAdditionInteractor> * pVertexAdded){
		m_pVertexAdded=pVertexAdded;
	}

	inline void SetRenderer(const vtkSmartPointer<vtkRenderer> & renderer){
		m_Renderer=renderer;
	}

	inline void SetAdherensJunctionVertices(const ttt::AdherensJunctionVertices::Pointer & vertices){
		m_AdherensJunctionsVertices=vertices;
	}

	inline void SetVertexLocationsDrawer(const vtkSmartPointer<VertexLocationsDrawer> & vertexLocationsDrawer){
		m_VertexLocationsDrawer=vertexLocationsDrawer;
	}

	static VertexAdditionInteractor* New(){
		return new VertexAdditionInteractor;
	}

public:
	VertexAdditionInteractor(){
		m_pVertexAdded=0;
		m_pAdditionCancelled=0;
	}
	virtual ~VertexAdditionInteractor(){

	}
	virtual void OnLeftButtonDown(){

		int* clickPos = this->GetInteractor()->GetEventPosition();

		// Pick from this location.
		vtkSmartPointer<vtkPropPicker>  picker =  	vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], 0, m_Renderer);

		double * clickPosition=picker->GetPickPosition();

		m_AddedVertex = ttt::AdherensJunctionVertex::New() ;

		itk::Index<3> index;

		index[0]=round(clickPosition[0]/m_Spacing[0]);
		index[1]=round(clickPosition[1]/m_Spacing[1]);
		index[2]=round(clickPosition[2]/m_Spacing[2]);

		m_AddedVertex->SetPosition(index);

		m_AdherensJunctionsVertices->push_back(m_AddedVertex);

		ttt::VertexLocationsDrawer::VertexSphereMapperAndActor drawnStuff= m_VertexLocationsDrawer->DrawAdherensJunctionVertex(m_AddedVertex);

		m_Renderer->Render();
		m_AddedSphere=drawnStuff.get<1>();
		m_AddedMapper=drawnStuff.get<2>();
		m_AddedActor=drawnStuff.get<3>();

		m_pVertexAdded->SetCaller(this);
		m_pVertexAdded->Notify();

	    //vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

	}
	virtual void OnRightButtonUp(){
		assert(m_pAdditionCancelled);
		m_pAdditionCancelled->Notify();
		//vtkInteractorStyleTrackballCamera::OnRightButtonUp();
	}
};


class VertexAddedCallback : public GUICallback<VertexAdditionInteractor> {
public:
	virtual void Notify(){
		this->GetTissueTracker()->GetProject()->SetVertexLocationsDirty();
		this->GetTissueTracker()->SetInteractionModeToVertexSelection();
		this->GetTissueTracker()->GetVertexSelectionInteractor()->SetSelection(this->GetCaller()->GetAddedActor());
	}

};

class VertexSelectedCallback :	public GUICallback<VertexSelectionInteractor>{

	public:
		vtkSmartPointer<vtkPointWidget> m_PointWidget;

		vtkSmartPointer<VertexLocationsDrawer>  m_VertexLocationsDrawer;
		double * bounds;
		virtual void Notify(){

			assert(this->GetCaller());
		  	vtkSmartPointer<vtkCubeSource> cube=vtkSmartPointer<vtkCubeSource>::New();
		  	//cube->SetCenter(0,0,0);
		  	//cube->SetBounds(0,bounds[0],0,bounds[1],0,bounds[2]); //GET bounds from somewhere
		  	cube->SetBounds(0,100,0,100,0,10);

		  	cube->Update();

		    vtkSmartPointer<FollowVertexCallback> myCallback = vtkSmartPointer<FollowVertexCallback>::New();

		    vtkSmartPointer<vtkActor> pickedVertexActor=this->GetCaller()->GetPickedVertexActor();

		    myCallback->SetActor(pickedVertexActor);

		    vtkSmartPointer<vtkSphereSource> sphereSource=m_VertexLocationsDrawer->GetSphereSourceFromActor(pickedVertexActor);
		    myCallback->SetSphereSource(sphereSource);

		    ttt::AdherensJunctionVertex::Pointer vertex = m_VertexLocationsDrawer->GetVertexFromActor(pickedVertexActor);


		    myCallback->SetVertex(vertex);
		    myCallback->SetSpacing(this->GetTissueTracker()->GetProject()->GetSpacing());

		  	m_PointWidget->AllOn();
		  	m_PointWidget->SetInteractor(this->GetTissueTracker()->GetRenderWindowInteractor());
		  	m_PointWidget->SetInputData(cube->GetOutput());

		  	m_PointWidget->PlaceWidget();
		  	m_PointWidget->SetPosition(sphereSource->GetCenter());
		  	m_PointWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
		  	m_PointWidget->On();

		  	this->GetTissueTracker()->GetProject()->SetVertexLocationsDirty();

		}
		inline void SetPointWidget(const vtkSmartPointer<vtkPointWidget> & pointWidget){
			m_PointWidget=pointWidget;
		}
		inline void SetVertexLocationsDrawer( VertexLocationsDrawer * vertexLocationsDrawer){
			m_VertexLocationsDrawer=vertexLocationsDrawer;
		}

	};


class VertexUnselectedCallback  : public GUICallback<VertexSelectionInteractor>{
	public:
		vtkSmartPointer<vtkPointWidget> m_PointWidget;

		virtual void Notify(){
		  	m_PointWidget->Off();
		  	m_PointWidget->RemoveAllObservers();
		}
		void SetPointWidget(const vtkSmartPointer<vtkPointWidget> & pointWidget){
			m_PointWidget=pointWidget;
		}
	};





#endif // TISSUETRACKER_H
/** @}*/
