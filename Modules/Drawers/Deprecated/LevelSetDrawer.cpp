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


typedef itk::ImageToVTKImageFilter<giaa::TissueSegmentation::LevelSetImageType> ImageToVTKType;
        std::cout << "Levelset" << std::endl;
        ImageToVTKType::Pointer tovtk = ImageToVTKType::New();
        tovtk->SetInput(m_Project.GetLevelSetImage(m_CurrentFrame));
        tovtk->Update();

        vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
        volumeMapper->SetBlendModeToComposite();

        volumeMapper->SetInput(tovtk->GetOutput());
        volumeMapper->Update();

        vtkSmartPointer<vtkVolumeProperty> volumeProperty =vtkSmartPointer<vtkVolumeProperty>::New();
        volumeProperty->ShadeOff();
        volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();

        compositeOpacity->AddPoint(0.0,0.0);
        compositeOpacity->AddPoint(9830,1.0);
        compositeOpacity->AddPoint(9831,0.0);
        compositeOpacity->AddPoint(32767,0.0);

        volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(0.0  ,0.0,0.0,1.0);
        color->AddRGBPoint(5000 ,1.0,0.0,0.0);
        color->AddRGBPoint(32767,1.0,1.0,1.0);

        volumeProperty->SetColor(color);

        vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
        volume->SetMapper(volumeMapper);
        volume->SetProperty(volumeProperty);
        m_CurrentRenderer->AddViewProp(volume);
