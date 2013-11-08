#include <itkImageToVTKImageFilter.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>

#include "PlatenessImageDrawer.h"

ttt::PlatenessImageDrawer::PlatenessImageDrawer(){
}

void ttt::PlatenessImageDrawer::Draw(){

	typedef itk::ImageToVTKImageFilter<PlatenessImageType> ImageToVTKType;

        ImageToVTKType::Pointer tovtk = ImageToVTKType::New();
        //tovtk->SetInput(m_Project.GetPlatenessImage(m_CurrentFrame));
        tovtk->SetInput(m_PlatenessImage);
        tovtk->Update();

        vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
        volumeMapper->SetBlendModeToComposite();

        volumeMapper->SetInputData(tovtk->GetOutput());
        volumeMapper->Update();

        vtkSmartPointer<vtkVolumeProperty> volumeProperty =vtkSmartPointer<vtkVolumeProperty>::New();
        volumeProperty->ShadeOff();
        volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();

        compositeOpacity->AddPoint(0.0,0.0);
        compositeOpacity->AddPoint(0.3,1.0);
        compositeOpacity->AddPoint(0.31,0.0);
        compositeOpacity->AddPoint(1.0,0.0);

        volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(0.0  ,0.0,0.0,1.0);
        color->AddRGBPoint(0.15 ,1.0,0.0,0.0);
        color->AddRGBPoint(1.0,1.0,1.0,1.0);

        volumeProperty->SetColor(color);

        vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
        volume->SetMapper(volumeMapper);
        volume->SetProperty(volumeProperty);
        m_Renderer->AddViewProp(volume);

}
