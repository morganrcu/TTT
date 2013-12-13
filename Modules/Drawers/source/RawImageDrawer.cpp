#include <itkImageToVTKImageFilter.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>

#include "RawImageDrawer.h"
#include <QString>

ttt::RawImageDrawer::RawImageDrawer(){

	m_VolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_VolumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(0.0, 0.0);
	compositeOpacity->AddPoint(0.99,0.0);
	compositeOpacity->AddPoint(1,1.0);
	compositeOpacity->AddPoint(255.0, 1.0);
	m_VolumeProperty->SetScalarOpacity(compositeOpacity); // composite first.*/

	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
	color->SetColorSpaceToDiverging();

	color->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
	color->AddRGBPoint(255.0, 1.0, 0.0, 0.0);
	m_VolumeProperty->SetColor(color);

}


vtkSmartPointer<vtkVolumeProperty> ttt::RawImageDrawer::GetVolumeProperty() {
	return m_VolumeProperty;
}




