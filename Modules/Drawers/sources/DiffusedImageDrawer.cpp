//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

#include "DiffusedImageDrawer.h"
#include <itkImageToVTKImageFilter.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>

//template<class TDiffusedImage>
//DiffusedImageDrawer<TDiffusedImage>::DiffusedImageDrawer(vtkSmartPointer<vtkRenderer> & renderer, const typename TDiffusedImage::Pointer & diffusedImage)
DiffusedImageDrawer::DiffusedImageDrawer(){
}
//template<class TDiffusedImage>
//void DiffusedImageDrawer<TDiffusedImage>::Draw(){
void DiffusedImageDrawer::Draw() {
	typedef itk::ImageToVTKImageFilter<DiffusedImageType> ImageToVTKType;
	typename ImageToVTKType::Pointer tovtk = ImageToVTKType::New();

	tovtk->SetInput(m_DiffusedImage);
	tovtk->Update();

	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper->SetBlendModeToComposite();

	volumeMapper->SetInputData(tovtk->GetOutput());
	//volumeMapper->SetInput(tovtk->GetOutput());
	volumeMapper->Update();

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<
			vtkVolumeProperty>::New();
	volumeProperty->ShadeOff();
	volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<
			vtkPiecewiseFunction>::New();
#if 0
	zcompositeOpacity->AddPoint(0.0,0.0);
	compositeOpacity->AddPoint(80.0,1.0);
	compositeOpacity->AddPoint(80.1,0.0);
	compositeOpacity->AddPoint(255.0,0.0);
#endif
	compositeOpacity->AddPoint(0.0, 0.0);
	compositeOpacity->AddPoint(0.3, 1.0);
	compositeOpacity->AddPoint(0.31, 0.0);
	compositeOpacity->AddPoint(1.0, 0.0);

	volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<
			vtkColorTransferFunction>::New();
#if 0
	color->AddRGBPoint(0.0 ,0.0,0.0,1.0);
	color->AddRGBPoint(40.0 ,1.0,0.0,0.0);
	color->AddRGBPoint(255.0,1.0,1.0,1.0);
#endif
	color->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
	color->AddRGBPoint(0.15, 1.0, 0.0, 0.0);
	color->AddRGBPoint(1.0, 1.0, 1.0, 1.0);

	volumeProperty->SetColor(color);

	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	m_Renderer->AddViewProp(volume);

}
