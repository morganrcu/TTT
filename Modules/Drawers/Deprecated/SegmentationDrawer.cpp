   typedef itk::ImageToVTKImageFilter<giaa::TissueSegmentation::BinaryImageType> ImageToVTKType;
        std::cout << "Thresholded seg box checked " << this->m_pUI->showSegmentationCBox  << std::endl;
        ImageToVTKType::Pointer tovtk = ImageToVTKType::New();
        tovtk->SetInput(m_Project.GetThresholdedImage(m_CurrentFrame));
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
        compositeOpacity->AddPoint(80.0,1.0);
        compositeOpacity->AddPoint(80.1,0.0);
        compositeOpacity->AddPoint(255.0,0.0);

        volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(0.0  ,0.0,0.0,1.0);
        color->AddRGBPoint(40.0  ,1.0,0.0,0.0);
        color->AddRGBPoint(255.0,1.0,1.0,1.0);
        volumeProperty->SetColor(color);

        vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
        volume->SetMapper(volumeMapper);
        volume->SetProperty(volumeProperty);
        m_CurrentRenderer->AddViewProp(volume);
