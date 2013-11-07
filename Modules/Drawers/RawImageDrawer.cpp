#include <itkImageToVTKImageFilter.h>
#include <itkImageFileWriter.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>

#include "RawImageDrawer.h"
#include <QString>
template<class TImage> void StoreImage(const typename TImage::Pointer & image,const QString & basePath, const QString & type, unsigned int numFrame){
    typedef itk::ImageFileWriter<TImage> WriterType;

    typename WriterType::Pointer writer =WriterType::New();

    QString filename(basePath+"/"+type + "-%1" + ".mha");
    filename=filename.arg(numFrame,4,10,QLatin1Char('0'));

    writer->SetFileName(filename.toStdString());
    writer->SetInput(image);
    writer->Update();
}

//template<class TOriginalImage> OriginalImageDrawer<TOriginalImage>::OriginalImageDrawer(
RawImageDrawer::RawImageDrawer(){}


//template<class TOriginalImage> void OriginalImageDrawer<TOriginalImage>::Draw() {
void RawImageDrawer::Draw() {
	std::cout << "in original" << std::endl;

	StoreImage<OriginalImageType>(m_OriginalImage,".","rawimage",0);
	std::cout << "Image to Plot " << m_OriginalImage << std::endl;
	//this->RemoveMembrane();
	typedef itk::ImageToVTKImageFilter<OriginalImageType> ImageToVTKType;
	typename ImageToVTKType::Pointer tovtk = ImageToVTKType::New();
	tovtk->SetInput(m_OriginalImage);
	tovtk->Update();

	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper= vtkSmartPointer<vtkSmartVolumeMapper>::New();

	volumeMapper->SetInputData(tovtk->GetOutput());
	volumeMapper->Update();

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(0.0, 0.0);
	compositeOpacity->AddPoint(80.0, 1.0);
	compositeOpacity->AddPoint(80.1, 0.0);
	compositeOpacity->AddPoint(255.0, 0.0);
	volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.*/

	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<
			vtkColorTransferFunction>::New();
	color->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
	color->AddRGBPoint(40.0, 1.0, 0.0, 0.0);
	color->AddRGBPoint(255.0, 1.0, 1.0, 1.0);
	volumeProperty->SetColor(color);

	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	m_Renderer->AddViewProp(volume);
}
