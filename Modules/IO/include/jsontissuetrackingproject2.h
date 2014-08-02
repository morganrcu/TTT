/*
 * jsontissuetrackingproject.h
 *
 *  Created on: Jul 21, 2014
 *      Author: morgan
 */

#ifndef JSONTISSUETRACKINGPROJECT2_H_
#define JSONTISSUETRACKINGPROJECT2_H_
#include <fstream>
#include <string>
#include <json/json.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkSCIFIOImageIO.h>
#include "tissuetrackingabstractproject2.h"

namespace ttt {
class JSONTissueTrackingProject2 : public TissueTrackingAbstractProject2{

private:
	std::string m_ProjectDirectory;
public:
	inline void SetDirectory(const std::string & projectDirectory){
			m_ProjectDirectory=projectDirectory;
		}
	virtual void Open(){
		TissueTrackingAbstractProject2::Open();
	}

	virtual void Close(){
		TissueTrackingAbstractProject2::Close();
	}
	virtual void Flush(){
		TissueTrackingAbstractProject2::Flush();
	}

	virtual typename RawImageType::Pointer GetRawImage(unsigned int frame);
	virtual void SetRawImage(unsigned int frame,const typename RawImageType::Pointer & image);
	virtual bool IsRawImageAvailable(unsigned int frame);

	virtual typename DiffusedImageType::Pointer GetDiffusedImage(unsigned int frame);
	virtual void SetDiffusedImage(unsigned int frame,const typename DiffusedImageType::Pointer & image);
	virtual bool IsDiffusedImageAvailable(unsigned int frame);

	virtual typename PlatenessImageType::Pointer GetPlatenessImage(unsigned int frame);
	virtual void SetPlatenessImage(unsigned int frame,const typename PlatenessImageType::Pointer & image);
	virtual bool IsPlatenessImageAvailable(unsigned int frame);

	virtual typename VertexnessImageType::Pointer GetVertexnessImage(unsigned int frame);
	virtual void SetVertexnessImage(unsigned int frame,const typename VertexnessImageType::Pointer & image);
	virtual bool IsVertexnessImageAvailable(unsigned int frame);

	virtual typename ttt::AdherensJunctionVertices::Pointer GetAdherensJunctionVertices(unsigned int frame);
	virtual void SetAdherensJunctionVertices(unsigned int frame, const typename ttt::AdherensJunctionVertices::Pointer & vertices);
	virtual bool IsAdherensJunctionVerticesAvailable(unsigned int frame);

	virtual typename  ttt::TissueDescriptor::Pointer GetTissueDescriptor(unsigned int frame);
	virtual void SetTissueDescriptor(unsigned int frame,const typename ttt::TissueDescriptor::Pointer & descriptor);
	virtual bool IsTissueDescriptorAvailable(unsigned int frame);

	virtual typename  ttt::TrackedTissueDescriptor::Pointer GetTrackedTissueDescriptor(unsigned int frame);
	virtual void SetTrackedTissueDescriptor(unsigned int frame,const typename ttt::TrackedTissueDescriptor::Pointer & descriptor);
	virtual bool IsTrackedTissueDescriptorAvailable(unsigned int frame);

protected:
	virtual void LoadProjectInfo();
	virtual void StoreProjectInfo();
	virtual void StoreFrameInfo(unsigned int frame);
	virtual void LoadFrameInfo(unsigned int frame);

private:
	typename itk::Image<float,3>::Pointer LoadImage(const std::string & name, int frame);
	void StoreImage(const std::string & name, int frame,const typename itk::Image<float,3>::Pointer & image);
	bool IsImage(const std::string & name, int frame);
};

}


#endif /* JSONTISSUETRACKINGPROJECT_H_ */
