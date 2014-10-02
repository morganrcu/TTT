/*
 * mysqltissuetrackingproject2.h
 *
 *  Created on: Jul 29, 2014
 *      Author: morgan
 */

#ifndef VTKSQLTISSUETRACKINGPROJECT2_H_
#define VTKSQLTISSUETRACKINGPROJECT2_H_

#include "tissuetrackingabstractproject2.h"

#include <vtkSmartPointer.h>
#include <vtkSQLDatabase.h>

#include <vtkMySQLDatabase.h>


namespace ttt {
class VTKSQLTissueTrackingProject2: public TissueTrackingAbstractProject2 {
private:
	std::string m_ProjectDirectory;
	vtkSmartPointer<vtkSQLDatabase> m_DB;

	std::string m_Host;
	std::string m_DBName;
	std::string m_User;
	std::string m_Password;
	unsigned int m_Port;

	int m_ProjectID;

public:
	VTKSQLTissueTrackingProject2() : TissueTrackingAbstractProject2(){
		m_Host="localhost";
	}
	inline void SetHost(const std::string & host) {
		m_Host = host;
	}
	inline void SetDBName(const std::string & dbname) {
		m_DBName = dbname;
	}
	inline void SetUser(const std::string & user) {
		m_User = user;
	}
	inline void SetPassword(const std::string & password) {
		m_Password = password;
	}
	inline void SetPort(unsigned int port) {
		m_Port = port;
	}

	inline void SetProjectID(int projectID) {
		m_ProjectID = projectID;
	}

	virtual void Open() {
		std::stringstream url;
		url << "mysql://" << m_User  << "@" << m_Host << ":" << m_Port << "/" << m_DBName;

		m_DB=vtkSQLDatabase::CreateFromURL(url.str().c_str() );
		std::cout << url.str() << std::endl;
		bool status =m_DB->Open(m_Password.c_str());

		assert(status);

		TissueTrackingAbstractProject2::Open();
	}
	virtual void Close() {
		TissueTrackingAbstractProject2::Close();
	}
	virtual void Flush() {
		TissueTrackingAbstractProject2::Flush();
	}
	virtual typename RawImageType::Pointer GetRawImage(unsigned int frame);
	virtual void SetRawImage(unsigned int frame,
			const typename RawImageType::Pointer & image);
	virtual bool IsRawImageAvailable(unsigned int frame);

	virtual typename DiffusedImageType::Pointer GetDiffusedImage(
			unsigned int frame);
	virtual void SetDiffusedImage(unsigned int frame,
			const typename DiffusedImageType::Pointer & image);
	virtual bool IsDiffusedImageAvailable(unsigned int frame);

	virtual typename PlatenessImageType::Pointer GetPlatenessImage(
			unsigned int frame);
	virtual void SetPlatenessImage(unsigned int frame,
			const typename PlatenessImageType::Pointer & image);
	virtual bool IsPlatenessImageAvailable(unsigned int frame);

	virtual typename VertexnessImageType::Pointer GetVertexnessImage(
			unsigned int frame);
	virtual void SetVertexnessImage(unsigned int frame,
			const typename VertexnessImageType::Pointer & image);
	virtual bool IsVertexnessImageAvailable(unsigned int frame);

	virtual typename ttt::AdherensJunctionVertices::Pointer GetAdherensJunctionVertices(
			unsigned int frame);
	virtual void SetAdherensJunctionVertices(unsigned int frame,
			const typename ttt::AdherensJunctionVertices::Pointer & vertices);
	virtual bool IsAdherensJunctionVerticesAvailable(unsigned int frame);
	virtual typename ttt::TissueDescriptor::Pointer GetTissueDescriptor(
			unsigned int frame);
	virtual void SetTissueDescriptor(unsigned int frame,
			const typename ttt::TissueDescriptor::Pointer & descriptor);
	virtual bool IsTissueDescriptorAvailable(unsigned int frame);

	virtual typename ttt::TrackedTissueDescriptor::Pointer GetTrackedTissueDescriptor(
			unsigned int frame);
	virtual void SetTrackedTissueDescriptor(unsigned int frame,
			const typename ttt::TrackedTissueDescriptor::Pointer & descriptor);
	virtual bool IsTrackedTissueDescriptorAvailable(unsigned int frame);

	virtual void SetCentroids(unsigned int frame, const   FeatureMap<CellVertexType,itk::Point<double,3> > & centroids);
	virtual FeatureMap<CellVertexType,itk::Point<double,3> >  GetCentroids(unsigned int frame);

	virtual void SetAreas(unsigned int frame, const FeatureMap<CellVertexType,double> & areas);
	virtual FeatureMap<CellVertexType,double> GetAreas(unsigned int frame);

	virtual void SetPerimeter(unsigned int frame, const FeatureMap<CellVertexType,double> & perimeters);
	virtual FeatureMap<CellVertexType,double> GetPerimeter(unsigned int frame);

	virtual void SetXX(unsigned int frame, const FeatureMap<CellVertexType,double> & xx);
	virtual FeatureMap<CellVertexType,double> GetXX(unsigned int frame);

	virtual void SetXY(unsigned int frame, const FeatureMap<CellVertexType,double> & xy);
	virtual FeatureMap<CellVertexType,double> GetXY(unsigned int frame);

	virtual void SetYY(unsigned int frame, const FeatureMap<CellVertexType,double> & yy);
	virtual FeatureMap<CellVertexType,double> GetYY(unsigned int frame);

protected:
	virtual void LoadProjectInfo();
	virtual void StoreProjectInfo();
	virtual void StoreFrameInfo(unsigned int frame);
	virtual void LoadFrameInfo(unsigned int frame);
private:
	typename itk::Image<float, 3>::Pointer LoadImage(const std::string & name,
			int frame);
	void StoreImage(const std::string & name, int frame,
			const typename itk::Image<float, 3>::Pointer & image);
	bool IsImageAvailable(const std::string & name,int frame);

};

}

#endif /* MYSQLTISSUETRACKINGPROJECT2_H_ */
