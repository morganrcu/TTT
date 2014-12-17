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
template<int dim> class JSONTissueTrackingProject2 : public TissueTrackingAbstractProject2<dim>{
public:
	typedef typename TissueTrackingAbstractProject2<dim>::RawImageType RawImageType;
	typedef typename TissueTrackingAbstractProject2<dim>::PlatenessImageType PlatenessImageType;
	typedef typename TissueTrackingAbstractProject2<dim>::DiffusedImageType DiffusedImageType;
	typedef typename TissueTrackingAbstractProject2<dim>::VertexnessImageType VertexnessImageType;

private:
	std::string m_ProjectDirectory;
public:
	inline void SetDirectory(const std::string & projectDirectory){
			m_ProjectDirectory=projectDirectory;
		}
	virtual void Open(){
		TissueTrackingAbstractProject2<dim>::Open();
	}

	virtual void Close(){
		TissueTrackingAbstractProject2<dim>::Close();
	}
	virtual void Flush(){
		TissueTrackingAbstractProject2<dim>::Flush();
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

	virtual typename ttt::AdherensJunctionVertices<dim>::Pointer GetAdherensJunctionVertices(unsigned int frame);
	virtual void SetAdherensJunctionVertices(unsigned int frame, const typename ttt::AdherensJunctionVertices<dim>::Pointer & vertices);
	virtual bool IsAdherensJunctionVerticesAvailable(unsigned int frame);

	virtual typename  ttt::TissueDescriptor<dim>::Pointer GetTissueDescriptor(unsigned int frame);
	virtual void SetTissueDescriptor(unsigned int frame,const typename ttt::TissueDescriptor<dim>::Pointer & descriptor);
	virtual bool IsTissueDescriptorAvailable(unsigned int frame);

	virtual typename  ttt::TrackedTissueDescriptor<dim>::Pointer GetTrackedTissueDescriptor(unsigned int frame);
	virtual void SetTrackedTissueDescriptor(unsigned int frame,const typename ttt::TrackedTissueDescriptor<dim>::Pointer & descriptor);
	virtual bool IsTrackedTissueDescriptorAvailable(unsigned int frame);

	virtual void SetCentroids(unsigned int frame, const   FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,itk::Point<double,dim> > & centroids);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,itk::Point<double,dim> >  GetCentroids(unsigned int frame);

	virtual void SetAreas(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> & areas);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> GetAreas(unsigned int frame);

	virtual void SetPerimeter(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> & perimeters);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> GetPerimeter(unsigned int frame);

	virtual void SetXX(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> & xx);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> GetXX(unsigned int frame);

	virtual void SetXY(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> & xy);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> GetXY(unsigned int frame);

	virtual void SetYY(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> & yy);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType,double> GetYY(unsigned int frame);

	virtual void SetTrackedCentroids(unsigned int frame, const   FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,itk::Point<double,3> > & centroids);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,itk::Point<double,dim> >  GetTrackedCentroids(unsigned int frame);

	virtual void SetTrackedAreas(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> & areas);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> GetTrackedAreas(unsigned int frame);

	virtual void SetTrackedPerimeter(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> & perimeters);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> GetTrackedPerimeter(unsigned int frame);

	virtual void SetTrackedXX(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> & xx);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> GetTrackedXX(unsigned int frame);

	virtual void SetTrackedXY(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> & xy);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> GetTrackedXY(unsigned int frame);

	virtual void SetTrackedYY(unsigned int frame, const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> & yy);
	virtual FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType,double> GetTrackedYY(unsigned int frame);


protected:
	virtual void LoadProjectInfo();
	virtual void StoreProjectInfo();
	virtual void StoreFrameInfo(unsigned int frame);
	virtual void LoadFrameInfo(unsigned int frame);

private:
	typename itk::Image<float,dim>::Pointer LoadImage(const std::string & name, int frame);
	void StoreImage(const std::string & name, int frame,const typename itk::Image<float,dim>::Pointer & image);
	bool IsImage(const std::string & name, int frame);
};

}

#include "jsontissuetrackingproject2.hxx"

#endif /* JSONTISSUETRACKINGPROJECT_H_ */
