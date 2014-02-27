/*
 * ScalarVolumeDrawer.h
 *
 *  Created on: Dec 9, 2013
 *      Author: morgan
 */

#ifndef SCALARVOLUMEDRAWER_H_
#define SCALARVOLUMEDRAWER_H_
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include "Drawer.h"
namespace ttt {

template<class TImage> class ScalarVolumeDrawer : public Drawer{

private:

	typename TImage::Pointer m_Image;

	vtkSmartPointer<vtkVolume> m_Volume;

	typedef itk::ImageToVTKImageFilter<TImage> ImageToVTKType;
	typename ImageToVTKType::Pointer m_ToVTK;

	vtkSmartPointer<vtkSmartVolumeMapper> m_VolumeMapper;
	bool m_Drawn;
public:
	ScalarVolumeDrawer(){
		m_Image=0;

		m_Drawn=false;
	}
	virtual ~ScalarVolumeDrawer(){

	}
	/**
	 * Sets the pointer to the diffusedImage to be drawn.
	 * You need to call this method before \link DiffusedImageDrawer::Draw() \endlink
	 * @param diffusedImage The volume to draw
	 */
	inline void SetImage(const typename TImage::Pointer & image){
		m_Image=image;
	}

	void Reset(){
		if(m_Drawn) m_Renderer->RemoveViewProp(m_Volume);
	}

	void Draw(){
		assert(m_Image);

		Reset();

		m_ToVTK = ImageToVTKType::New();
		m_Volume = vtkSmartPointer<vtkVolume>::New();
		m_VolumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

		m_ToVTK->SetInput(m_Image);
		m_ToVTK->Update();

		m_VolumeMapper->SetBlendModeToComposite();

		m_VolumeMapper->SetInputData(m_ToVTK->GetOutput());
		m_VolumeMapper->Update();


		m_Volume->SetMapper(m_VolumeMapper);
		m_Volume->SetProperty(this->GetVolumeProperty());
		m_Volume->SetPickable(false);
		m_Volume->VisibilityOff();
		m_Renderer->AddViewProp(m_Volume);
		m_Drawn=true;
	}

	void Show(){
		if(m_Drawn){
			m_Volume->VisibilityOn();
//			m_Renderer->Render();
		}
	}
	void Hide(){
		if(m_Drawn){
			m_Volume->VisibilityOff();
//			m_Renderer->Render();
		}

	}
protected:
	virtual vtkSmartPointer<vtkVolumeProperty> GetVolumeProperty()=0;
};

}

#endif /* SCALARVOLUMEDRAWER_H_ */
