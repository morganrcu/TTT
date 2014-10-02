/*
 * mysqltissuetrackingproject2.h
 *
 *  Created on: Jul 29, 2014
 *      Author: morgan
 */

#ifndef QTSQLTISSUETRACKINGPROJECT2_H_
#define QTSQLTISSUETRACKINGPROJECT2_H_

#include "tissuetrackingabstractproject2.h"
#include <QSqlDatabase>

namespace ttt {
class QTSQLTissueTrackingProject2: public TissueTrackingAbstractProject2 {
private:
	std::string m_ProjectDirectory;
	QSqlDatabase m_DB;

	std::string m_Host;
	std::string m_DBName;
	std::string m_User;
	std::string m_Password;
	unsigned int m_Port;

	int m_ProjectID;

public:
	QTSQLTissueTrackingProject2() : TissueTrackingAbstractProject2(){
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

	    m_DB = QSqlDatabase::addDatabase("QMYSQL");
	    m_DB.setHostName(QString::fromStdString(m_Host));
	    m_DB.setDatabaseName(QString::fromStdString(m_DBName));
	    m_DB.setUserName(QString::fromStdString(m_User));
	    m_DB.setPassword(QString::fromStdString(m_Password));
	    bool ok = m_DB.open();

		assert(ok);

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

	virtual void SetTrackedCentroids(unsigned int frame, const   FeatureMap<TrackedCellVertexType,itk::Point<double,3> > & centroids);
	virtual FeatureMap<TrackedCellVertexType,itk::Point<double,3> >  GetTrackedCentroids(unsigned int frame);

	virtual void SetTrackedAreas(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & areas);
	virtual FeatureMap<TrackedCellVertexType,double> GetTrackedAreas(unsigned int frame);

	virtual void SetTrackedPerimeter(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & perimeters);
	virtual FeatureMap<TrackedCellVertexType,double> GetTrackedPerimeter(unsigned int frame);

	virtual void SetTrackedXX(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & xx);
	virtual FeatureMap<TrackedCellVertexType,double> GetTrackedXX(unsigned int frame);

	virtual void SetTrackedXY(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & xy);
	virtual FeatureMap<TrackedCellVertexType,double> GetTrackedXY(unsigned int frame);

	virtual void SetTrackedYY(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & yy);
	virtual FeatureMap<TrackedCellVertexType,double> GetTrackedYY(unsigned int frame);



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
