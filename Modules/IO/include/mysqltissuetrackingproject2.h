/*
 * mysqltissuetrackingproject2.h
 *
 *  Created on: Jul 29, 2014
 *      Author: morgan
 */

#ifndef MYSQLTISSUETRACKINGPROJECT2_H_
#define MYSQLTISSUETRACKINGPROJECT2_H_

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>


#include "tissuetrackingabstractproject2.h"


namespace ttt {
class MySQLTissueTrackingProject2: public TissueTrackingAbstractProject2 {
private:
	std::string m_ProjectDirectory;
	sql::Driver * m_Driver;
	std::auto_ptr<sql::Connection> m_DB;

	std::string m_Host;
	std::string m_DBName;
	std::string m_User;
	std::string m_Password;
	unsigned int m_Port;

	int m_ProjectID;

public:
	MySQLTissueTrackingProject2() : TissueTrackingAbstractProject2(){
		m_Driver = sql::mysql::get_driver_instance();
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
		try {
			m_DB = std::auto_ptr<sql::Connection>(
					m_Driver->connect(m_Host, m_User, m_Password));
			m_DB->setSchema(m_DBName);
		} catch (sql::SQLException &e) {
			std::cout << "# ERR: SQLException in " << __FILE__;
			std::cout << "# ERR: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

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
