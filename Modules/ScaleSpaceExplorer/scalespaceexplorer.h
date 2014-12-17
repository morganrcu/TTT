#ifndef SCALESPACEEXPLORER_H
#define SCALESPACEEXPLORER_H

#include <QMainWindow>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <QVTKInteractor.h>
#include <vtkSmartPointer.h>
#include <itkImage.h>

#include "PlatenessImageDrawer.h"
#include "VertexnessImageDrawer.h"
#include "VertexLocationsDrawer.h"

#include "tttPyramidLocalMaximumImageFilter.h"

namespace Ui {
class ScaleSpaceExplorer;
}

class ScaleSpaceExplorer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScaleSpaceExplorer(QWidget *parent = 0);
    ~ScaleSpaceExplorer();

private slots:

	void ComputePlateness();
	void ComputeVertexness();
	void Open();

	void ShowPlateness(int);
	void ShowVertexness(int);
	void ShowVertexLocations(int);

	void DrawPlateness(int);
	void DrawVertexness(int);

	void SaveScreenshot();

	void SavePlateness();
	void SaveVertexness();

private:
    Ui::ScaleSpaceExplorer *m_pUI;

    vtkSmartPointer<vtkRenderer> m_CentralRenderer;
    vtkSmartPointer<vtkRenderWindow> m_CentralRenderWindow;
    vtkSmartPointer<QVTKInteractor> m_CentralRenderWindowInteractor;

    typedef itk::Image<float,3> ImageType;
    typedef itk::Image<float,3> VertexnessImageType;
    typedef itk::Image<float,3> PlatenessImageType;

    ImageType::Pointer m_InputImage;
    ImageType::Pointer m_ReescaledImage;

    std::vector<VertexnessImageType::Pointer> m_VertexnessImages;
    std::vector<typename ttt::AdherensJunctionVertices<3>::Pointer > m_LocalMaxima;

    std::vector<VertexnessImageType::Pointer> m_PlatenessImages;

    ttt::VertexnessImageDrawer m_VertexnessDrawer;
    ttt::VertexLocationsDrawer m_VertexLocationsDrawer;

    ttt::PlatenessImageDrawer m_PlatenessDrawer;

    double m_LowerScaleVertexness;
    double m_UpperScaleVertexness;
    double m_ScaleStepVertexness;


    double m_LowerScalePlateness;
    double m_UpperScalePlateness;
    double m_ScaleStepPlateness;

    int m_CurrentPlateness;
    int m_CurrentVertexness;
	ImageType::SpacingType m_Spacing;

};

#endif // SCALESPACEEXPLORER_H
