/*
 * mysqltissuetrackingproject.h
 *
 *  Created on: Oct 22, 2013
 *      Author: morgan
 */

#ifndef MYSQLTISSUETRACKINGPROJECT_H_
#define MYSQLTISSUETRACKINGPROJECT_H_

#include <vector>
#include <string>

#include <iostream>
#include <sstream>

#include <vtkSmartPointer.h>

#include <itkImage.h>
#include <itkPointSet.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>

#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/graph/iteration_macros.hpp>

#include "tttDescriptionDataTypes.h"
#include "Ellipse.h"
#include "tttDomainStrainRates.h"

#if 0
#include <itkImageFileWriter.h>

#include <itkImage.h>
#include <itkPointSet.h>

#include <tectonics.h>

#endif
namespace ttt {
class TissueTrackingProject {

public:

	typedef itk::Image<unsigned char, 3> RawImageType;
	typedef itk::Image<unsigned char, 3> SurfaceSegmentedImageType;
	typedef itk::Image<float, 3> DiffusedImageType;
	typedef itk::Image<float, 3> PlatenessImageType;
	typedef itk::Image<itk::Vector<float, 3>, 3> OrientationImageType;
	typedef itk::Image<float, 3> VertexnessImageType;

	typedef ttt::TissueDescriptor TissueDescriptorType;
	typedef ttt::TrackedTissueDescriptor TrackedTissueDescriptorType;

	typedef typename RawImageType::SpacingType SpacingType;

	typedef Ellipse<double> EllipseType;

	typedef std::map<
			typename TissueDescriptorType::DualGraphVertexDescriptorType,
			EllipseType> EllipseMapType;

	typedef boost::shared_ptr<EllipseMapType> EllipseMapTypePointer;
	typedef std::vector<ttt::TrackedDomain> TrackedDomainVectorType;
	typedef boost::shared_ptr<TrackedDomainVectorType> TrackedDomainVectorTypePointer;

	typedef RawImageType::RegionType RegionType;

private:






	EllipseMapTypePointer m_TrackedEllipses;

	TrackedDomainVectorTypePointer m_TrackedDomains;

	sql::Driver * m_Driver;

	std::auto_ptr<sql::Connection> m_DB;

	RegionType m_Region;
	SpacingType m_Spacing;
	double m_TimeDelta;

	std::string m_WorkingDirectory;
	std::string m_ProjectName;

	int m_ProjectID;

	unsigned int m_Frame;

	std::string m_Host;
	std::string m_DBName;
	std::string m_User;
	std::string m_Password;
	unsigned int m_Port;
public:

	TissueTrackingProject() {
		m_ProjectID = 0;
		m_TimeDelta = -1;
		m_Frame=0;
		m_Driver = sql::mysql::get_driver_instance();
	}
	void SetHost(const std::string & host){
		m_Host=host;
	}
	void SetDBName(const std::string & dbname){
		m_DBName=dbname;
	}
	void SetUser(const std::string & user){
		m_User=user;
	}
	void SetPassword(const std::string & password){
		m_Password=password;
	}
	void SetPort(unsigned int port){
		m_Port = port;
	}

	void SetFrame(int frame){
		this->Flush();
		m_Frame=frame;
		this->Clear();
	}
	bool openDB() {

		try {
			m_DB = std::auto_ptr<sql::Connection>(
					m_Driver->connect(m_Host,m_User, m_Password));
			m_DB->setSchema(m_DBName);
			return true;
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

			return false;
		}

	}
	void OpenProject(int projectID) {
		try {
			m_ProjectID = projectID;
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(
							"SELECT spacingX,spacingY,spacingZ,sizeX,sizeY,sizeZ,timeDelta,workingDirectory,name from Project WHERE Project.idProject=?"));

			prep_stmt->setInt(1, m_ProjectID);

			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());

			assert(res->next());
			m_Spacing[0] = res->getDouble("spacingX");
			m_Spacing[1] = res->getDouble("spacingY");
			m_Spacing[2] = res->getDouble("spacingZ");
			m_WorkingDirectory = res->getString("workingDirectory");
			m_ProjectName = res->getString("name");
			m_Region.SetIndex(0, 0);
			m_Region.SetIndex(1, 0);
			m_Region.SetIndex(2, 0);

			m_Region.SetSize(0, res->getInt("sizeX"));
			m_Region.SetSize(1, res->getInt("sizeY"));
			m_Region.SetSize(2, res->getInt("sizeZ"));

			m_TimeDelta = res->getDouble("timeDelta");
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
	}
	void NewProject(const std::string & name,
			const std::string & workingDirectory, double spacingX,
			double spacingY, double spacingZ, double timeDelta,
			unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ) {

		m_Spacing[0] = spacingX;
		m_Spacing[1] = spacingY;
		m_Spacing[2] = spacingZ;

		m_Region.SetIndex(0, 0);
		m_Region.SetIndex(1, 0);
		m_Region.SetIndex(2, 0);

		m_Region.SetSize(0, sizeX);
		m_Region.SetSize(1, sizeY);
		m_Region.SetSize(2, sizeZ);

		m_WorkingDirectory = workingDirectory;
		try {
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(
							"INSERT into Project (spacingX,spacingY,spacingZ,sizeX,sizeY,sizeZ,timeDelta,name,workingDirectory) values (?, ?, ?, ?, ?, ?, ?, ?,?)"));

			prep_stmt->setDouble(1, spacingX);
			prep_stmt->setDouble(2, spacingY);
			prep_stmt->setDouble(3, spacingZ);
			prep_stmt->setInt(4, sizeX);
			prep_stmt->setInt(5, sizeY);
			prep_stmt->setInt(6, sizeZ);
			prep_stmt->setInt(7, timeDelta);
			prep_stmt->setString(8, name);
			prep_stmt->setString(9, workingDirectory);
			prep_stmt->execute();

			std::auto_ptr<sql::Statement> stmt(m_DB->createStatement());

			std::auto_ptr<sql::ResultSet> res(
					stmt->executeQuery("SELECT LAST_INSERT_ID()"));

			res->next();
			m_ProjectID = res->getInt("LAST_INSERT_ID()");

		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
	}

	void NewFrame(int numFrame) {
		//Insert new Frame to Database
		try {
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(
							"INSERT into Frame (idProject, t) values (?, ?)"));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, numFrame); //t==0

			prep_stmt->execute();
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}

	}
	unsigned int GetNumFrames() {

		try {
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(
							"SELECT COUNT(*) FROM Frame WHERE Frame.idProject=?"));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
			std::cout << res->rowsCount() << std::endl;
			res->next();
			return res->getInt("Count(*)");

		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
		return 0;
	}



	inline void SetProjectID(int ProjectID) {
		m_ProjectID = ProjectID;
	}

	void StoreConfig() {
	}

	void Load(std::string & basePath) {

	}

	void Flush(){
		if(this->m_RawImageDirty){
			this->StoreRawImage();
		}
		if(this->m_CLAHEDImageDirty){
			this->StoreCLAHEDImage();
		}
		if(this->m_LateralImageVolumeSegmentedImageDirty){
			this->StoreLateralImageVolumeSegmentedImage();
		}
		if(this->m_SurfaceSegmentedImageDirty){
			this->StoreSurfaceSegmentedImage();
		}
		if(this->m_DiffusedImageDirty){
			this->StoreDiffusedImage();
		}
		if(this->m_PlatenessImageDirty){
			this->StorePlatenessImage();
		}
		if(this->m_VertexnessImageDirty){
			this->StoreVertexnessImage();
		}
		if(this->m_VertexLocationsDirty){
			this->StoreVertexLocations();
		}
		if(this->m_TissueDescriptorDirty){
			this->StoreTissueDescriptor();
		}

		if(this->m_TrackedTissueDescriptorDirty){
			//this->StoreTrackedTissueDescriptor();
		}
	}
	void Clear(){
		this->m_RawImage=0;
		this->m_CLAHEDImage=0;
		this->m_LateralImageVolumeSegmentedImage=0;
		this->m_SurfaceSegmentedImage=0;
		this->m_DiffusedImage=0;
		this->m_PlatenessImage=0;
		this->m_VertexnessImage=0;
		this->m_VertexLocations=0;
		//this->m_TrackedTissueDescriptor=0;
		this->m_TissueDescriptor=0;
	}

#define tttDefineImageMacro(name,type) \
	type::Pointer m_##name;\
	bool m_##name##Dirty=false;

#define tttGetImageMacro(name, type) \
virtual  type::Pointer  Get##name()  \
{										\
	if(!m_##name) Load##name(); \
	return m_##name;						\
}										\

#define tttSetImageMacro(name, type) \
virtual  void Set##name(const type::Pointer & arg)  \
{										\
	m_##name##Dirty=true;\
	m_##name=arg;						\
	Store##name(); \
}										\

#define tttLoadImageMacro(name, type,tablename) \
virtual  void Load##name()  \
{										\
	m_##name=readImageFromTable< type >(tablename, m_Frame);						\
	m_##name##Dirty=false; \
} \

#define tttStoreImageMacro(name, type,tablename) \
virtual  void Store##name()  \
{										\
	storeImageInTable< type >( m_##name, tablename, m_Frame); \
	m_##name##Dirty=false; \
} \

#define tttIsImageReadyMacro(name, type,tablename) \
virtual  bool Is##name##Ready()  \
{										\
	return this->isImageInTable(tablename, m_Frame); \
} \

#define tttSetImageDirtyMacro(name) \
virtual  void Set##name##Dirty()  \
{										\
	m_##name##Dirty=false; \
} \

#define tttImageMacro(name,type,tablename) \
private: \
	tttDefineImageMacro(name,type); \
public: \
	tttGetImageMacro(name,type); \
	tttSetImageMacro(name,type); \
	tttLoadImageMacro(name,type,tablename); \
	tttStoreImageMacro(name,type,tablename); \
	tttIsImageReadyMacro(name,type,tablename); \
	tttSetImageDirtyMacro(name); \

	/////////////////////////////////// Images /////////////////////////////////////////////

	tttImageMacro(RawImage,RawImageType,"RawImages");
	tttImageMacro(LateralImageVolumeSegmentedImage,RawImageType,"LateralImageVolumeSegmentedImages");
	tttImageMacro(CLAHEDImage,RawImageType,"CLAHEDImages");
	tttImageMacro(SurfaceSegmentedImage,RawImageType,"SurfaceSegmentedImages");
	tttImageMacro(DiffusedImage,DiffusedImageType,"DiffusedImages");
	tttImageMacro(PlatenessImage,PlatenessImageType,"PlatenessImages");
	tttImageMacro(VertexnessImage,VertexnessImageType,"VertexnessImages");


/////////////////////////////////// Vertex Locations/////////////////////////////////////////////
private:
	ttt::AdherensJunctionVertices::Pointer m_VertexLocations;
	bool m_VertexLocationsDirty=false;

public:

	ttt::AdherensJunctionVertices::Pointer GetVertexLocations() {
		if(!m_VertexLocations) LoadVertexLocations();
		return m_VertexLocations;
	}

	void SetVertexLocations(const ttt::AdherensJunctionVertices::Pointer & vertexLocations) {
		m_VertexLocations=vertexLocations;
		m_VertexLocationsDirty=true;
	}


	void LoadVertexLocations() {
		m_VertexLocations = ttt::AdherensJunctionVertices::New();
		try {
			std::string selectQuery(
					"SELECT pointX,pointY,pointZ FROM VertexLocations WHERE VertexLocations.idProject=? AND VertexLocations.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(selectQuery));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());

			while (res->next()) {
				ttt::AdherensJunctionVertex::Pointer p =
						ttt::AdherensJunctionVertex::New();
				typedef typename ttt::AdherensJunctionVertex::IndexType Index;
				Index idx;

				idx[0] = res->getInt("pointX");
				idx[1] = res->getInt("pointY");
				idx[2] = res->getInt("pointZ");
				p->SetPosition(idx);

				m_VertexLocations->push_back(p);
			}

		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
		m_VertexLocationsDirty=false;
	}


	void StoreVertexLocations() {

			try {

				std::string deleteString(
						"DELETE from VertexLocations WHERE idProject=? AND t=?");

				std::auto_ptr<sql::PreparedStatement> delete_stmt(
						m_DB->prepareStatement(deleteString));

				delete_stmt->setInt(1, m_ProjectID); //IDproject==2
				delete_stmt->setInt(2, m_Frame); //t==0
				delete_stmt->execute();

				std::string insertString(
						"INSERT INTO VertexLocations (idProject,t,pointX,pointY,pointZ) VALUES (?,?,?,?,?)");

				std::auto_ptr<sql::PreparedStatement> insert_stmt(
						m_DB->prepareStatement(insertString));
				std::auto_ptr<sql::Statement> transStatement(
						m_DB->createStatement());
				transStatement->execute("START TRANSACTION;");
				for (ttt::AdherensJunctionVertices::const_iterator it =
						m_VertexLocations->begin(); it != m_VertexLocations->end();
						it++) {

					ttt::AdherensJunctionVertex::Pointer p = *it;

					insert_stmt->setInt(1, m_ProjectID);
					insert_stmt->setInt(2, m_Frame);
					insert_stmt->setInt(3, p->GetPosition()[0]);
					insert_stmt->setInt(4, p->GetPosition()[1]);
					insert_stmt->setInt(5, p->GetPosition()[2]);
					insert_stmt->execute();
				}
				transStatement->execute("COMMIT;");
			} catch (sql::SQLException &e) {
				/*
				 The MySQL Connector/C++ throws three different exceptions:

				 - sql::MethodNotImplementedException (derived from sql::SQLException)
				 - sql::InvalidArgumentException (derived from sql::SQLException)
				 - sql::SQLException (derived from std::runtime_error)
				 */
				cout << "# ERR: SQLException in " << __FILE__;
				//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
				/* Use what() (derived from std::runtime_error) to fetch the error message */
				cout << "# ERR: " << e.what();
				cout << " (MySQL error code: " << e.getErrorCode();
				cout << ", SQLState: " << e.getSQLState() << " )" << endl;

			}
			m_VertexLocationsDirty=false;
		}



	inline bool IsVertexLocationsReady() {
		//TODO return this->isImageInTable("VertexLocations", m_Frame);
		return true;
	}

	inline void SetVertexLocationsDirty(){
		m_VertexLocationsDirty=true;
		this->StoreVertexLocations();
	}
//////////////////////////////////Tissue Descriptor/////////////////////////////////////////////
private:
	TissueDescriptorType::Pointer m_TissueDescriptor;
	bool m_TissueDescriptorDirty=false;
public:

	void LoadTissueDescriptor(){
		m_TissueDescriptor = TissueDescriptorType::New();

		{
			std::string queryMembranePointsString(
					"SELECT idMembranePoint,posX,posY,posZ FROM MembranePoint WHERE MembranePoint.idProject=? AND MembranePoint.t=? AND MembranePoint.idTissue=? ORDER BY idMembranePoint ASC");

			std::auto_ptr<sql::PreparedStatement> queryMembranePoints(
					m_DB->prepareStatement(queryMembranePointsString));

			queryMembranePoints->setInt(1, m_ProjectID); //IDproject==2
			queryMembranePoints->setInt(2, m_Frame); //IDproject==2
			queryMembranePoints->setInt(3, 0); //IDproject==2
			queryMembranePoints->execute();

			std::auto_ptr<sql::ResultSet> resMembranePoints(
					queryMembranePoints->getResultSet());

			while (resMembranePoints->next()) {

				ttt::SkeletonPoint newPoint;
				newPoint.position[0] = resMembranePoints->getDouble("posX");
				newPoint.position[1] = resMembranePoints->getDouble("posY");
				newPoint.position[2] = resMembranePoints->getDouble("posZ");
				ttt::SkeletonVertexType vertex = boost::add_vertex(newPoint,
						*m_TissueDescriptor->m_SkeletonGraph);

				assert(vertex == resMembranePoints->getInt("idMembranePoint"));

			}

		}

		{
			std::string queryMembraneEdgesString(
					"SELECT idMembranePoint1,idMembranePoint2 FROM MembranePoint_linkedTo_MembranePoint WHERE MembranePoint_linkedTo_MembranePoint.idProject=? AND MembranePoint_linkedTo_MembranePoint.t=? AND MembranePoint_linkedTo_MembranePoint.idTissue=?");

			std::auto_ptr<sql::PreparedStatement> queryMembraneEdges(
					m_DB->prepareStatement(queryMembraneEdgesString));

			queryMembraneEdges->setInt(1, m_ProjectID); //IDproject==2
			queryMembraneEdges->setInt(2, m_Frame); //IDproject==2
			queryMembraneEdges->setInt(3, 0); //IDproject==2
			queryMembraneEdges->execute();

			std::auto_ptr<sql::ResultSet> resMembraneEdges(
					queryMembraneEdges->getResultSet());

			while (resMembraneEdges->next()) {
				boost::add_edge(resMembraneEdges->getInt("idMembranePoint1"),
						resMembraneEdges->getInt("idMembranePoint2"),
						*m_TissueDescriptor->m_SkeletonGraph);
			}

		}

		{
			std::string queryCellString(
					"SELECT idCell,posX,posY,posZ FROM Cell WHERE Cell.idProject=? AND Cell.t=? AND Cell.idTissue=? ORDER BY idCell ASC");

			std::auto_ptr<sql::PreparedStatement> queryCell(
					m_DB->prepareStatement(queryCellString));

			queryCell->setInt(1, m_ProjectID); //IDproject==2
			queryCell->setInt(2, m_Frame); //IDproject==2
			queryCell->setInt(3, 0); //IDproject==2
			queryCell->execute();

			std::auto_ptr<sql::ResultSet> resCell(queryCell->getResultSet());

			while (resCell->next()) {
				ttt::Cell newCell;
				newCell.m_Centroid[0] = resCell->getDouble("posX");
				newCell.m_Centroid[1] = resCell->getDouble("posY");
				newCell.m_Centroid[2] = resCell->getDouble("posZ");

				ttt::CellVertexType vertex = boost::add_vertex(newCell,
						*m_TissueDescriptor->m_CellGraph);

				assert(vertex == resCell->getInt("idCell"));

				{
					std::string queryCellMembraneString(
							"SELECT idCell,idMembranePoint FROM Cell_has_MembranePoint WHERE Cell_has_MembranePoint.idProject=? AND Cell_has_MembranePoint.t=? AND Cell_has_MembranePoint.idTissue=? AND Cell_has_MembranePoint.idCell=?");

					std::auto_ptr<sql::PreparedStatement> queryCellMembrane(
							m_DB->prepareStatement(queryCellMembraneString));

					queryCellMembrane->setInt(1, m_ProjectID); //IDproject==2
					queryCellMembrane->setInt(2, m_Frame); //IDproject==2
					queryCellMembrane->setInt(3, 0); //IDproject==2
					queryCellMembrane->setInt(4, vertex); //IDproject==2
					queryCellMembrane->execute();

					std::auto_ptr<sql::ResultSet> resCellMembrane(
							queryCellMembrane->getResultSet());

					while (resCellMembrane->next()) {
						boost::get(ttt::CellPropertyTag(),
								*m_TissueDescriptor->m_CellGraph, vertex).AddSkeletonPoint(
								resCellMembrane->getInt("idMembranePoint"));
					}
				}

			}
		}

		{
			std::string queryCellEdgesString(
					"SELECT idCell1,idCell2 FROM Cell_neighbor_of_Cell WHERE Cell_neighbor_of_Cell.idProject=? AND Cell_neighbor_of_Cell.t=? AND Cell_neighbor_of_Cell.idTissue=?");

			std::auto_ptr<sql::PreparedStatement> queryCellEdges(
					m_DB->prepareStatement(queryCellEdgesString));

			queryCellEdges->setInt(1, m_ProjectID); //IDproject==2
			queryCellEdges->setInt(2, m_Frame); //IDproject==2
			queryCellEdges->setInt(3, 0); //IDproject==2
			queryCellEdges->execute();

			std::auto_ptr<sql::ResultSet> resCellEdges(
					queryCellEdges->getResultSet());

			while (resCellEdges->next()) {
				boost::add_edge(resCellEdges->getInt("idCell1"),
						resCellEdges->getInt("idCell2"),
						*m_TissueDescriptor->m_CellGraph);
			}
		}
	}

	inline typename TissueDescriptorType::Pointer GetTissueDescriptor() {
		if(!m_TissueDescriptor) LoadTissueDescriptor();
		return m_TissueDescriptor;
	}

	void SetTissueDescriptor(const TissueDescriptor::Pointer & descriptor){
		m_TissueDescriptor=descriptor;
		m_TissueDescriptorDirty=true;
		StoreTissueDescriptor();
	}

	void StoreTissueDescriptor() {

		std::auto_ptr<sql::Statement> transStatement(m_DB->createStatement());
		transStatement->execute("START TRANSACTION;");
		try {

			std::string deleteTissueString(
					"DELETE from Tissue WHERE idProject=? AND t=? AND idTissue=?");

			std::auto_ptr<sql::PreparedStatement> deleteTissue_stmt(
					m_DB->prepareStatement(deleteTissueString));

			deleteTissue_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteTissue_stmt->setInt(2, m_Frame); //t==0
			deleteTissue_stmt->setInt(3, 0); //t==0
			deleteTissue_stmt->execute();
			//transStatement->execute("COMMIT;");
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			//cout << deleteVertexString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(-1);
		}
#if 0
		try {

			std::string deleteVertexString(
					"DELETE from MembranePoint WHERE idProject=? AND t=?");

			std::auto_ptr<sql::PreparedStatement> deleteVertex_stmt(
					m_DB->prepareStatement(deleteVertexString));

			deleteVertex_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteVertex_stmt->setInt(2, frame);//t==0
			deleteVertex_stmt->execute();
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			//cout << deleteVertexString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(-1);
		}
#if 0
		try {
			std::string deleteEdgeString(
					"DELETE from MembranePoint_linkedTo_MembranePoint WHERE idProject=? AND t=?");

			std::auto_ptr<sql::PreparedStatement> deleteEdge_stmt(
					m_DB->prepareStatement(deleteEdgeString));

			deleteEdge_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteEdge_stmt->setInt(2, frame);//t==0
			deleteEdge_stmt->execute();
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//cout << deleteEdgeString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
#endif

		try {

			std::string deleteCellString(
					"DELETE from Cell WHERE idProject=? AND t=?");
			std::auto_ptr<sql::PreparedStatement> deleteCell_stmt(
					m_DB->prepareStatement(deleteCellString));

			deleteCell_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteCell_stmt->setInt(2, frame);//t==0
			deleteCell_stmt->execute();
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//cout << deleteCellString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(-1);
		}
#endif
		transStatement->execute("COMMIT;");
////////////////////////////////////////////////////
		transStatement->execute("START TRANSACTION;");

		try {

			std::string insertTissueString(
					"INSERT INTO Tissue(idProject,t,idTissue) VALUES (?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertTissue_stmt(
					m_DB->prepareStatement(insertTissueString));

			insertTissue_stmt->setInt(1, m_ProjectID);
			insertTissue_stmt->setInt(2, m_Frame);
			insertTissue_stmt->setInt(3, 0);
			insertTissue_stmt->execute();

		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//cout << insertVertexString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(-1);
		}
		transStatement->execute("COMMIT;");
		////////////////////////////////////////////////////
		transStatement->execute("START TRANSACTION;");

		try {

			std::string insertVertexString(
					"INSERT INTO MembranePoint(idProject,t,idTissue,idMembranePoint,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertVertex_stmt(
					m_DB->prepareStatement(insertVertexString));

			BGL_FORALL_VERTICES(v,*m_TissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertVertex_stmt->setInt(1,m_ProjectID);
			insertVertex_stmt->setInt(2,m_Frame);
			insertVertex_stmt->setInt(3,0);
			insertVertex_stmt->setInt(4,v);
			insertVertex_stmt->setDouble(5,boost::get(ttt::SkeletonPointPropertyTag(),*m_TissueDescriptor->m_SkeletonGraph,v).position[0]);
			insertVertex_stmt->setDouble(6,boost::get(ttt::SkeletonPointPropertyTag(),*m_TissueDescriptor->m_SkeletonGraph,v).position[1]);
			insertVertex_stmt->setDouble(7,boost::get(ttt::SkeletonPointPropertyTag(),*m_TissueDescriptor->m_SkeletonGraph,v).position[2]);
			insertVertex_stmt->execute();

		}

	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertVertexString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);
	}
	//transStatement->execute("COMMIT;");
	////////////////////////////////////////////////////
	//transStatement->execute("START TRANSACTION;");

		try {

			std::string insertEdgeString(
					"INSERT IGNORE INTO MembranePoint_linkedTo_MembranePoint(idProject,t,idTissue,idMembranePoint1,idMembranePoint2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertEdge_stmt(
					m_DB->prepareStatement(insertEdgeString));

			BGL_FORALL_EDGES(e,*m_TissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertEdge_stmt->setInt(1,m_ProjectID);
			insertEdge_stmt->setInt(2, m_Frame);
			insertEdge_stmt->setInt(3,0);
			insertEdge_stmt->setInt(4,boost::source(e,*m_TissueDescriptor->m_SkeletonGraph));
			insertEdge_stmt->setInt(5,boost::target(e,*m_TissueDescriptor->m_SkeletonGraph));
			std::cout << m_ProjectID << "-" << m_Frame << "-" << 0 << "-"<< boost::source(e,*m_TissueDescriptor->m_SkeletonGraph) << "-" << boost::target(e,*m_TissueDescriptor->m_SkeletonGraph) << std::endl;
			insertEdge_stmt->execute();
		}
	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertEdgeString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);
	}

		transStatement->execute("COMMIT;");
		////////////////////////////////////////////////////
		transStatement->execute("START TRANSACTION;");

		try {

			std::string insertCellString(
					"INSERT INTO Cell(idProject,t,idTissue,idCell,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCell_stmt(
					m_DB->prepareStatement(insertCellString));

			BGL_FORALL_VERTICES(v,*m_TissueDescriptor->m_CellGraph,ttt::CellGraph){
			insertCell_stmt->setInt(1,m_ProjectID);
			insertCell_stmt->setInt(2,m_Frame);
			insertCell_stmt->setInt(3,0);
			insertCell_stmt->setInt(4,v);
			insertCell_stmt->setDouble(5,boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).m_Centroid[0]);
			insertCell_stmt->setDouble(6,boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).m_Centroid[1]);
			insertCell_stmt->setDouble(7,boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).m_Centroid[2]);
			insertCell_stmt->execute();
		}
	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);
	}

		try {

			std::string insertCellEdgeString(
					"INSERT IGNORE INTO Cell_neighbor_of_Cell(idProject,t,idTissue,idCell1,idCell2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCellEdge_stmt(
					m_DB->prepareStatement(insertCellEdgeString));

			BGL_FORALL_EDGES(e,*m_TissueDescriptor->m_CellGraph,ttt::CellGraph){
			insertCellEdge_stmt->setInt(1,m_ProjectID);
			insertCellEdge_stmt->setInt(2,m_Frame);
			insertCellEdge_stmt->setInt(3,0);
			insertCellEdge_stmt->setInt(4,boost::source(e,*m_TissueDescriptor->m_CellGraph));
			insertCellEdge_stmt->setInt(5,boost::target(e,*m_TissueDescriptor->m_CellGraph));
			insertCellEdge_stmt->execute();
		}

	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellEdgeString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);
	}

		try {

			std::string insertCellToMembranePointString(
					"INSERT IGNORE INTO Cell_has_MembranePoint(idProject,t,idTissue,idCell,idMembranePoint,`order`) VALUES (?,?,?,?,?,?)");
			std::auto_ptr<sql::PreparedStatement> insertCellToMembranePointString_stmt(
					m_DB->prepareStatement(insertCellToMembranePointString));
			BGL_FORALL_VERTICES(v,*m_TissueDescriptor->m_CellGraph,ttt::CellGraph){

			int order=0;
			for(std::vector<ttt::SkeletonVertexType>::iterator it=boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).Begin();
					it!=boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).End();
					it++) {
				insertCellToMembranePointString_stmt->setInt(1,m_ProjectID);
				insertCellToMembranePointString_stmt->setInt(2,m_Frame);
				insertCellToMembranePointString_stmt->setInt(3,0);
				insertCellToMembranePointString_stmt->setInt(4,v);
				insertCellToMembranePointString_stmt->setInt(5,*it);
				insertCellToMembranePointString_stmt->setInt(6,order++);
				insertCellToMembranePointString_stmt->execute();
			}
		}

	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellEdgeString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);
	}
		transStatement->execute("COMMIT;");
		m_TissueDescriptorDirty=false;
	}

	void SetTissueDescriptorDirty(){
		m_TissueDescriptorDirty=true;
	}
	inline bool IsTissueDescriptorReady() {
		return true; //TODO
	}




	//////////////////////////////??TRACKED TISSUE DESCRIPTOR/////////////////////////////////////
private:
	TrackedTissueDescriptorType::Pointer m_TrackedTissueDescriptor[10];
	bool m_TrackedTissueDescriptorDirty=false;
public:
	void LoadTrackedTissueDescriptor(){
#if 0
		m_TrackedTissueDescriptor = TrackedTissueDescriptorType::New();

				{
					std::string queryMembranePointsString(
							"SELECT idTrackedMembranePoint,posX,posY,posZ FROM TrackedMembranePoint WHERE TrackedMembranePoint.idProject=? AND TrackedMembranePoint.t=? AND TrackedMembranePoint.idTissue=? ORDER BY idTrackedMembranePoint ASC");

					std::auto_ptr<sql::PreparedStatement> queryMembranePoints(
							m_DB->prepareStatement(queryMembranePointsString));

					queryMembranePoints->setInt(1, m_ProjectID); //IDproject==2
					queryMembranePoints->setInt(2, m_Frame); //IDproject==2
					queryMembranePoints->setInt(3, 0); //IDproject==2
					queryMembranePoints->execute();

					std::auto_ptr<sql::ResultSet> resMembranePoints(
							queryMembranePoints->getResultSet());

					while (resMembranePoints->next()) {

						ttt::SkeletonPoint newPoint;
						newPoint.position[0] = resMembranePoints->getDouble("posX");
						newPoint.position[1] = resMembranePoints->getDouble("posY");
						newPoint.position[2] = resMembranePoints->getDouble("posZ");
						ttt::SkeletonVertexType vertex = boost::add_vertex(newPoint,
								*m_TrackedTissueDescriptor->m_SkeletonGraph);

						assert(
								vertex
										== resMembranePoints->getInt(
												"idTrackedMembranePoint"));

					}

				}

				{
					std::string queryMembraneEdgesString(
							"SELECT idTrackedMembranePoint1,idTrackedMembranePoint2 FROM TrackedMembranePoint_linkedTo_TrackedMembranePoint WHERE TrackedMembranePoint_linkedTo_TrackedMembranePoint.idProject=? AND TrackedMembranePoint_linkedTo_TrackedMembranePoint.t=? AND TrackedMembranePoint_linkedTo_TrackedMembranePoint.idTissue=?");

					std::auto_ptr<sql::PreparedStatement> queryMembraneEdges(
							m_DB->prepareStatement(queryMembraneEdgesString));

					queryMembraneEdges->setInt(1, m_ProjectID); //IDproject==2
					queryMembraneEdges->setInt(2, m_Frame); //IDproject==2
					queryMembraneEdges->setInt(3, 0); //IDproject==2
					queryMembraneEdges->execute();

					std::auto_ptr<sql::ResultSet> resMembraneEdges(
							queryMembraneEdges->getResultSet());

					while (resMembraneEdges->next()) {
						boost::add_edge(
								resMembraneEdges->getInt("idTrackedMembranePoint1"),
								resMembraneEdges->getInt("idTrackedMembranePoint2"),
								*m_TrackedTissueDescriptor->m_SkeletonGraph);
					}

				}

				{

					std::string queryCellString(
							"SELECT idTrackedCell,idCell,trackID,posX,posY,posZ,velX,velY,velZ FROM TrackedCell WHERE TrackedCell.idProject=? AND TrackedCell.t=? AND TrackedCell.idTissue=? ORDER BY idTrackedCell ASC");

					std::auto_ptr<sql::PreparedStatement> queryCell(
							m_DB->prepareStatement(queryCellString));

					queryCell->setInt(1, m_ProjectID); //IDproject==2
					queryCell->setInt(2, m_Frame); //IDproject==2
					queryCell->setInt(3, 0); //IDproject==2
					queryCell->execute();

					std::auto_ptr<sql::ResultSet> resCell(queryCell->getResultSet());

					while (resCell->next()) {
						ttt::TrackedCell newCell;
						newCell.m_Centroid[0] = resCell->getDouble("posX");
						newCell.m_Centroid[1] = resCell->getDouble("posY");
						newCell.m_Centroid[2] = resCell->getDouble("posZ");

						newCell.m_Velocity[0] = resCell->getDouble("velX");
						newCell.m_Velocity[1] = resCell->getDouble("velY");
						newCell.m_Velocity[2] = resCell->getDouble("velZ");

						newCell.m_ID = resCell->getInt("trackID");
						newCell.m_ObservedCell = resCell->getInt("idCell");

						ttt::TrackedCellVertexType vertex = boost::add_vertex(newCell,
								*m_TrackedTissueDescriptor->m_CellGraph);

						assert(vertex == resCell->getInt("idTrackedCell"));

						{
							std::string queryTrackedCellMembraneString(
									"SELECT idTrackedCell,idTrackedMembranePoint FROM TrackedCell_has_TrackedMembranePoint WHERE TrackedCell_has_TrackedMembranePoint.idProject=? AND TrackedCell_has_TrackedMembranePoint.t=? AND TrackedCell_has_TrackedMembranePoint.idTissue=? AND TrackedCell_has_TrackedMembranePoint.idTrackedCell=?");

							std::auto_ptr<sql::PreparedStatement> queryTrackedCellMembrane(
									m_DB->prepareStatement(
											queryTrackedCellMembraneString));

							queryTrackedCellMembrane->setInt(1, m_ProjectID); //IDproject==2
							queryTrackedCellMembrane->setInt(2, m_Frame); //IDproject==2
							queryTrackedCellMembrane->setInt(3, 0); //IDproject==2
							queryTrackedCellMembrane->setInt(4, vertex); //IDproject==2
							queryTrackedCellMembrane->execute();

							std::auto_ptr<sql::ResultSet> resTrackedCellMembrane(
									queryTrackedCellMembrane->getResultSet());

							while (resTrackedCellMembrane->next()) {
								boost::get(ttt::TrackedCellPropertyTag(),
										*m_TrackedTissueDescriptor->m_CellGraph, vertex).AddSkeletonPoint(
										resTrackedCellMembrane->getInt(
												"idTrackedMembranePoint"));
							}
						}

					}
				}

				{
					std::string queryCellEdgesString(
							"SELECT idTrackedCell1,idTrackedCell2 FROM TrackedCell_neighbor_of_TrackedCell WHERE TrackedCell_neighbor_of_TrackedCell.idProject=? AND TrackedCell_neighbor_of_TrackedCell.t=? AND TrackedCell_neighbor_of_TrackedCell.idTissue=?");

					std::auto_ptr<sql::PreparedStatement> queryCellEdges(
							m_DB->prepareStatement(queryCellEdgesString));

					queryCellEdges->setInt(1, m_ProjectID); //IDproject==2
					queryCellEdges->setInt(2, m_Frame); //IDproject==2
					queryCellEdges->setInt(3, 0); //IDproject==2
					queryCellEdges->execute();

					std::auto_ptr<sql::ResultSet> resCellEdges(
							queryCellEdges->getResultSet());

					while (resCellEdges->next()) {
						boost::add_edge(resCellEdges->getInt("idTrackedCell1"),
								resCellEdges->getInt("idTrackedCell2"),
								*m_TrackedTissueDescriptor->m_CellGraph);
					}
				}
#endif
	}
	inline typename TrackedTissueDescriptorType::Pointer GetTrackedTissueDescriptor() {
		return m_TrackedTissueDescriptor[m_Frame]; //FIXME
	}

	void SetTrackedTissueDescriptor(TrackedTissueDescriptorType::Pointer trackedTissueDescriptor){
		m_TrackedTissueDescriptor[m_Frame]=trackedTissueDescriptor;
		m_TrackedTissueDescriptorDirty=true;
		//StoreTrackedTissueDescriptor();
	}
	void StoreTrackedTissueDescriptor(){
#if 0
		std::auto_ptr<sql::Statement> transStatement(m_DB->createStatement());
		transStatement->execute("START TRANSACTION;");

		try {

			std::string deleteVertexString(
					"DELETE from TrackedMembranePoint WHERE idProject=? AND t=?");

			std::auto_ptr<sql::PreparedStatement> deleteVertex_stmt(
					m_DB->prepareStatement(deleteVertexString));

			deleteVertex_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteVertex_stmt->setInt(2, m_Frame); //t==0
			deleteVertex_stmt->execute();
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			//cout << deleteVertexString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(-1);

		}

		try {

			std::string insertVertexString(
					"INSERT INTO TrackedMembranePoint(idProject,t,idTissue,idTrackedMembranePoint,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertVertex_stmt(
					m_DB->prepareStatement(insertVertexString));

			BGL_FORALL_VERTICES(v,*m_TrackedTissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertVertex_stmt->setInt(1,m_ProjectID);
			insertVertex_stmt->setInt(2,m_Frame);
			insertVertex_stmt->setInt(3,0);
			insertVertex_stmt->setInt(4,v);
			insertVertex_stmt->setDouble(5,boost::get(ttt::SkeletonPointPropertyTag(),*m_TrackedTissueDescriptor->m_SkeletonGraph,v).position[0]);
			insertVertex_stmt->setDouble(6,boost::get(ttt::SkeletonPointPropertyTag(),*m_TrackedTissueDescriptor->m_SkeletonGraph,v).position[1]);
			insertVertex_stmt->setDouble(7,boost::get(ttt::SkeletonPointPropertyTag(),*m_TrackedTissueDescriptor->m_SkeletonGraph,v).position[2]);
			insertVertex_stmt->execute();
		}

	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertVertexString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);

	}

		try {
			std::string deleteEdgeString(
					"DELETE from TrackedMembranePoint_linkedTo_TrackedMembranePoint WHERE idProject=? AND t=?");

			std::auto_ptr<sql::PreparedStatement> deleteEdge_stmt(
					m_DB->prepareStatement(deleteEdgeString));

			deleteEdge_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteEdge_stmt->setInt(2, m_Frame); //t==0
			deleteEdge_stmt->execute();
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//cout << deleteEdgeString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(-1);

		}

		try {

			std::string insertEdgeString(
					"INSERT IGNORE INTO TrackedMembranePoint_linkedTo_TrackedMembranePoint(idProject,t,idTissue,idTrackedMembranePoint1,idTrackedMembranePoint2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertEdge_stmt(
					m_DB->prepareStatement(insertEdgeString));

			BGL_FORALL_EDGES(e,*m_TrackedTissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertEdge_stmt->setInt(1,m_ProjectID);
			insertEdge_stmt->setInt(2,m_Frame);
			insertEdge_stmt->setInt(3,0);
			insertEdge_stmt->setInt(4,boost::source(e,*m_TrackedTissueDescriptor->m_SkeletonGraph));
			insertEdge_stmt->setInt(5,boost::target(e,*m_TrackedTissueDescriptor->m_SkeletonGraph));

			insertEdge_stmt->execute();
		}
	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertEdgeString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);

	}

		try {

			std::string deleteCellString(
					"DELETE from TrackedCell WHERE idProject=? AND t=?");
			std::auto_ptr<sql::PreparedStatement> deleteCell_stmt(
					m_DB->prepareStatement(deleteCellString));

			deleteCell_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteCell_stmt->setInt(2, m_Frame); //t==0
			deleteCell_stmt->execute();
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//cout << deleteCellString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(-1);

		}

		try {

			std::string insertCellString(
					"INSERT INTO TrackedCell(idProject,t,idTissue,idTrackedCell,idCell,trackID,posX,posY,posZ,velX,velY,velZ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCell_stmt(
					m_DB->prepareStatement(insertCellString));

			BGL_FORALL_VERTICES(v,*m_TrackedTissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){
			insertCell_stmt->setInt(1,m_ProjectID);
			insertCell_stmt->setInt(2,m_Frame);
			insertCell_stmt->setInt(3,0);
			insertCell_stmt->setInt(4,v);
			insertCell_stmt->setInt(5,-1); //TODO obtain from ???
			insertCell_stmt->setInt(6,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).m_ID);
			insertCell_stmt->setDouble(7,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).m_Centroid[0]);
			insertCell_stmt->setDouble(8,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).m_Centroid[1]);
			insertCell_stmt->setDouble(9,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).m_Centroid[2]);
			insertCell_stmt->setDouble(10,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).m_Velocity[0]);
			insertCell_stmt->setDouble(11,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).m_Velocity[1]);
			insertCell_stmt->setDouble(12,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).m_Velocity[2]);
			insertCell_stmt->execute();
		}
	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);

	}

		try {

			std::string insertCellEdgeString(
					"INSERT IGNORE INTO TrackedCell_neighbor_of_TrackedCell(idProject,t,idTissue,idTrackedCell1,idTrackedCell2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCellEdge_stmt(
					m_DB->prepareStatement(insertCellEdgeString));

			BGL_FORALL_EDGES(e,*m_TrackedTissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){
			insertCellEdge_stmt->setInt(1,m_ProjectID);
			insertCellEdge_stmt->setInt(2,m_Frame);
			insertCellEdge_stmt->setInt(3,0);
			insertCellEdge_stmt->setInt(4,boost::source(e,*m_TrackedTissueDescriptor->m_CellGraph));
			insertCellEdge_stmt->setInt(5,boost::target(e,*m_TrackedTissueDescriptor->m_CellGraph));
			insertCellEdge_stmt->execute();
		}

	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellEdgeString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);

	}

		try {

			std::string insertTrackedCellToMembranePointString(
					"INSERT IGNORE INTO TrackedCell_has_TrackedMembranePoint(idProject,t,idTissue,idTrackedCell,idTrackedMembranePoint) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertTrackedCellToTrackedMembranePointString_stmt(
					m_DB->prepareStatement(
							insertTrackedCellToMembranePointString));

			BGL_FORALL_VERTICES(v,*m_TrackedTissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){

			for(std::vector<ttt::SkeletonVertexType>::iterator it=boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).Begin();
					it!=boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).End();
					it++) {

				insertTrackedCellToTrackedMembranePointString_stmt->setInt(1,m_ProjectID);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(2,m_Frame);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(3,0);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(4,v);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(5,*it);
				insertTrackedCellToTrackedMembranePointString_stmt->execute();
			}
		}

	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellEdgeString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		exit(-1);

	}
		transStatement->execute("COMMIT;");
#endif
	}


	////////////////////////////TRACKED ELLIPSES/////////////////////////////////
	void LoadTrackedEllipses(){
		m_TrackedEllipses = boost::shared_ptr<EllipseMapType>(
				new EllipseMapType);

		std::string queryTrackedEllipsesString(
				"SELECT idTrackedCell,rotR,majorR,minorR,posX,posY FROM TrackedEllipse WHERE TrackedEllipse.idProject=? AND TrackedEllipse.t=? AND TrackedEllipse.idTissue=?");

		std::auto_ptr<sql::PreparedStatement> queryTrackedEllipses(
				m_DB->prepareStatement(queryTrackedEllipsesString));

		queryTrackedEllipses->setInt(1, m_ProjectID); //IDproject==2
		queryTrackedEllipses->setInt(2, m_Frame); //IDproject==2
		queryTrackedEllipses->setInt(3, 0); //IDproject==2
		queryTrackedEllipses->execute();

		std::auto_ptr<sql::ResultSet> resTrackedEllipses(
				queryTrackedEllipses->getResultSet());
		while (resTrackedEllipses->next()) {

			Ellipse<double> ellipse;
			ellipse.m_Theta = resTrackedEllipses->getDouble("rotR");
			ellipse.m_Xrad = resTrackedEllipses->getDouble("majorR");
			ellipse.m_Yrad = resTrackedEllipses->getDouble("minorR");
			ellipse.m_Ycent = resTrackedEllipses->getDouble("posX");
			ellipse.m_Xcent = resTrackedEllipses->getDouble("posY");
			(*m_TrackedEllipses)[resTrackedEllipses->getInt("idTrackedCell")] =
					ellipse;

		}
	}

	EllipseMapTypePointer GetTrackedEllipses(unsigned int frame) {
		return m_TrackedEllipses;
	}
	////////////////////////////TRACKED DOMAINS/////////////////////////////////

	void LoadTrackedDomains(){
		std::string selectTrackedDomainsString(
				"SELECT idTrackedCell,order FROM TrackedDomain WHERE TrackedDomain.idProject=? AND TrackedDomain.t=? AND TrackedDomain.idTissue=? ORDER BY TrackedDomain.idTrackedCell ASC");
		std::auto_ptr<sql::PreparedStatement> selectTrackedDomains_stmt(
				m_DB->prepareStatement(selectTrackedDomainsString));

		m_TrackedDomains = boost::shared_ptr<TrackedDomainVectorType>(
				new TrackedDomainVectorType);

		selectTrackedDomains_stmt->setInt(1, m_ProjectID);
		selectTrackedDomains_stmt->setInt(2, m_Frame);
		selectTrackedDomains_stmt->setInt(3, 0);
		selectTrackedDomains_stmt->execute();

		std::auto_ptr<sql::ResultSet> resTrackedDomains(
				selectTrackedDomains_stmt->getResultSet());

		while (resTrackedDomains->next()) {
			ttt::TrackedDomain domain;
			domain.SetOrder(resTrackedDomains->getInt("order"));
			domain.SetNucleus(resTrackedDomains->getInt("idTrackedCell"));

			{
				std::string selectTrackedDomainsTrackedCellsString(
						"SELECT idTrackedCell FROM TrackedDomain_has_TrackedCell WHERE TrackedDomain_has_TrackedCell.idProject=? AND TrackedDomain_has_TrackedCell.t=? AND TrackedDomain_has_TrackedCell.idTissue=? AND TrackedDomain_has_TrackedCell.idTrackedNucleousCell=?");
				std::auto_ptr<sql::PreparedStatement> selectTrackedDomainsTrackedCells_stmt(
						m_DB->prepareStatement(
								selectTrackedDomainsTrackedCellsString));

				selectTrackedDomainsTrackedCells_stmt->setInt(1, m_ProjectID);
				selectTrackedDomainsTrackedCells_stmt->setInt(2, m_Frame);
				selectTrackedDomainsTrackedCells_stmt->setInt(3, 0);
				selectTrackedDomainsTrackedCells_stmt->setInt(4,
						domain.GetNucleus());
				selectTrackedDomainsTrackedCells_stmt->execute();

				std::auto_ptr<sql::ResultSet> resTrackedDomainsTrackedCells(
						selectTrackedDomainsTrackedCells_stmt->getResultSet());

				while (resTrackedDomainsTrackedCells->next()) {
					domain.AddCell(
							resTrackedDomainsTrackedCells->getInt(
									"idTrackedCell"));
				}
			}
			m_TrackedDomains->push_back(domain);
		}
	}
	TrackedDomainVectorTypePointer GetTrackedDomains() {
		return m_TrackedDomains;
	}

	void LoadDomainStrainRates(){

		boost::shared_ptr<DomainStrainRatesMapType> result = boost::shared_ptr<
						DomainStrainRatesMapType>(new DomainStrainRatesMapType);
		#if 0
				try {
					{
						//std::string selectTissueStrainRatesString("SELECT (TissueStrainRates.idTrackedCell,TissueStrainRates.data,CellStrainRates.data,IntercalationStrainRates.data) FROM TissueStrainRates INNER JOIN CellStrainRates ON TissueStrainRates.CellId= ,IntercalationStrainRates WHERE TissueStrainRates.idProject=? AND TissueStrainRates.t=? AND TissueStrainRates.idTissue=?");

						std::auto_ptr< sql::PreparedStatement > insertTissueStrainRates_stmt(m_DB->prepareStatement(insertTissueStrainRatesString));

						for(DomainStrainRatesMapType::iterator it=domainStrainRates->begin();it!=domainStrainRates->end();it++) {
							insertTissueStrainRates_stmt->setInt(1,m_ProjectID);
							insertTissueStrainRates_stmt->setInt(2,frame);
							insertTissueStrainRates_stmt->setInt(3,0);
							insertTissueStrainRates_stmt->setInt(4,it->first);

							typedef boost::iostreams::stream<boost::iostreams::array_source> array_stream;
							array_stream is((char*)it->second.Tissue_SRT.data_block(), it->second.Tissue_SRT.size()*sizeof(Tensor::element_type));
							insertTissueStrainRates_stmt->setBlob(5,&is);

							insertTissueStrainRates_stmt->execute();
						}
					}
					{
						std::string insertCellStrainRatesString("INSERT INTO CellStrainRates(idProject,t,idTissue,idTrackedCell,order,data) VALUES (?,?,?,?,?,?)");

						std::auto_ptr< sql::PreparedStatement > insertCellStrainRates_stmt(m_DB->prepareStatement(insertCellStrainRatesString));

						for(DomainStrainRatesMapType::iterator it=domainStrainRates->begin();it!=domainStrainRates->end();it++) {
							insertCellStrainRates_stmt->setInt(1,m_ProjectID);
							insertCellStrainRates_stmt->setInt(2,frame);
							insertCellStrainRates_stmt->setInt(3,0);
							insertCellStrainRates_stmt->setInt(4,it->first);

							typedef boost::iostreams::stream<boost::iostreams::array_source> array_stream;
							array_stream is((char*)it->second.CellShape_SRT.data_block(), it->second.CellShape_SRT.size()*sizeof(Tensor::element_type));
							insertCellStrainRates_stmt->setBlob(5,&is);

							insertCellStrainRates_stmt->execute();
						}
					}

					{
						std::string insertIntercalationStrainRatesString("INSERT INTO IntercalationStrainRates(idProject,t,idTissue,idTrackedCell,order,data) VALUES (?,?,?,?,?,?)");

						std::auto_ptr< sql::PreparedStatement > insertIntercalationStrainRates_stmt(m_DB->prepareStatement(insertIntercalationStrainRatesString));

						for(DomainStrainRatesMapType::iterator it=domainStrainRates->begin();it!=domainStrainRates->end();it++) {
							insertIntercalationStrainRates_stmt->setInt(1,m_ProjectID);
							insertIntercalationStrainRates_stmt->setInt(2,frame);
							insertIntercalationStrainRates_stmt->setInt(3,0);
							insertIntercalationStrainRates_stmt->setInt(4,it->first);

							typedef boost::iostreams::stream<boost::iostreams::array_source> array_stream;
							array_stream is((char*)it->second.Intercalation_SRT.data_block(), it->second.Intercalation_SRT.size()*sizeof(Tensor::element_type));
							insertIntercalationStrainRates_stmt->setBlob(5,&is);

							insertIntercalationStrainRates_stmt->execute();
						}
					}
				} catch(sql::SQLException &e) {
					/*
					 The MySQL Connector/C++ throws three different exceptions:

					 - sql::MethodNotImplementedException (derived from sql::SQLException)
					 - sql::InvalidArgumentException (derived from sql::SQLException)
					 - sql::SQLException (derived from std::runtime_error)
					 */
					cout << "# ERR: SQLException in " << __FILE__;
					//cout << insertCellEdgeString << std::endl;
					/* Use what() (derived from std::runtime_error) to fetch the error message */
					cout << "# ERR: " << e.what();
					cout << " (MySQL error code: " << e.getErrorCode();
					cout << ", SQLState: " << e.getSQLState() << " )" << endl;
				}

				return result;
#endif
	}






	void AddTrackedEllipses(unsigned int frame,
			const EllipseMapTypePointer & ellipses) {
		std::auto_ptr<sql::Statement> transStatement(m_DB->createStatement());
		transStatement->execute("START TRANSACTION;");

		try {

			std::string insertTrackedEllipsesString(
					"INSERT INTO TrackedEllipse(idProject,t,idTissue,idTrackedCell,majorR,minorR,rotR,posX,posY) VALUES (?,?,?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertTrackedEllipses_stmt(
					m_DB->prepareStatement(insertTrackedEllipsesString));

			for (EllipseMapType::iterator it = ellipses->begin();
					it != ellipses->end(); it++) {
				insertTrackedEllipses_stmt->setInt(1, m_ProjectID);
				insertTrackedEllipses_stmt->setInt(2, frame);
				insertTrackedEllipses_stmt->setInt(3, 0);
				insertTrackedEllipses_stmt->setInt(4, it->first);
				insertTrackedEllipses_stmt->setDouble(5, it->second.m_Xrad);
				insertTrackedEllipses_stmt->setDouble(6, it->second.m_Yrad);
				insertTrackedEllipses_stmt->setDouble(7, it->second.m_Theta);
				insertTrackedEllipses_stmt->setDouble(8, it->second.m_Xcent);
				insertTrackedEllipses_stmt->setDouble(9, it->second.m_Ycent);
				insertTrackedEllipses_stmt->execute();
			}

		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//cout << insertCellEdgeString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
		transStatement->execute("COMMIT;");
	}

	void AddTrackedDomains(unsigned int frame,
			const TrackedDomainVectorTypePointer & domains) {
		try {
			std::string insertTrackedDomainsString(
					"INSERT INTO TrackedDomain(idProject,t,idTissue,idTrackedCell,order) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertTrackedDomains_stmt(
					m_DB->prepareStatement(insertTrackedDomainsString));

			for (TrackedDomainVectorType::iterator it = domains->begin();
					it != domains->end(); it++) {
				insertTrackedDomains_stmt->setInt(1, m_ProjectID);
				insertTrackedDomains_stmt->setInt(2, frame);
				insertTrackedDomains_stmt->setInt(3, 0);
				insertTrackedDomains_stmt->setInt(4, it->GetNucleus());
				insertTrackedDomains_stmt->setInt(5, it->GetOrder());
				insertTrackedDomains_stmt->execute();

				for (typename std::set<ttt::TrackedCellVertexType>::iterator it2 =
						it->Begin(); it2 != it->End(); it2++) {
					std::string insertTrackedDomainsTrackedCellString(
							"INSERT INTO TrackedDomain_has_TrackedCell(idProject,t,idTissue,idTrackedCell,idNucleousCell) VALUES (?,?,?,?,?)");

					std::auto_ptr<sql::PreparedStatement> insertTrackedDomainsTrackedCell_stmt(
							m_DB->prepareStatement(
									insertTrackedDomainsTrackedCellString));

					insertTrackedDomainsTrackedCell_stmt->setInt(1,
							m_ProjectID);
					insertTrackedDomainsTrackedCell_stmt->setInt(2, frame);
					insertTrackedDomainsTrackedCell_stmt->setInt(3, 0);
					insertTrackedDomainsTrackedCell_stmt->setInt(4, *it2);
					insertTrackedDomainsTrackedCell_stmt->setInt(5,
							it->GetNucleus());
					insertTrackedDomainsTrackedCell_stmt->execute();
				}
			}

		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//cout << insertCellEdgeString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}
	void AddDomainStrainRates(unsigned int frame,
			const DomainStrainRatesMapTypePointer & domainStrainRates) {
		try {
			{
				std::string insertTissueStrainRatesString(
						"INSERT INTO TissueStrainRates(idProject,t,idTissue,idTrackedCell,order,data) VALUES (?,?,?,?,?,?)");

				std::auto_ptr<sql::PreparedStatement> insertTissueStrainRates_stmt(
						m_DB->prepareStatement(insertTissueStrainRatesString));

				for (DomainStrainRatesMapType::iterator it =
						domainStrainRates->begin();
						it != domainStrainRates->end(); it++) {
					insertTissueStrainRates_stmt->setInt(1, m_ProjectID);
					insertTissueStrainRates_stmt->setInt(2, frame);
					insertTissueStrainRates_stmt->setInt(3, 0);
					insertTissueStrainRates_stmt->setInt(4, it->first);

					typedef boost::iostreams::stream<
							boost::iostreams::array_source> array_stream;
					array_stream is((char*) it->second.Tissue_SRT.data_block(),
							it->second.Tissue_SRT.size()
									* sizeof(Tensor::element_type));
					insertTissueStrainRates_stmt->setBlob(6, &is);

					insertTissueStrainRates_stmt->execute();
				}
			}
			{
				std::string insertCellStrainRatesString(
						"INSERT INTO CellStrainRates(idProject,t,idTissue,idTrackedCell,order,data) VALUES (?,?,?,?,?,?)");

				std::auto_ptr<sql::PreparedStatement> insertCellStrainRates_stmt(
						m_DB->prepareStatement(insertCellStrainRatesString));

				for (DomainStrainRatesMapType::iterator it =
						domainStrainRates->begin();
						it != domainStrainRates->end(); it++) {
					insertCellStrainRates_stmt->setInt(1, m_ProjectID);
					insertCellStrainRates_stmt->setInt(2, frame);
					insertCellStrainRates_stmt->setInt(3, 0);
					insertCellStrainRates_stmt->setInt(4, it->first);

					typedef boost::iostreams::stream<
							boost::iostreams::array_source> array_stream;
					array_stream is(
							(char*) it->second.CellShape_SRT.data_block(),
							it->second.CellShape_SRT.size()
									* sizeof(Tensor::element_type));
					insertCellStrainRates_stmt->setBlob(6, &is);

					insertCellStrainRates_stmt->execute();
				}
			}

			{
				std::string insertIntercalationStrainRatesString(
						"INSERT INTO IntercalationStrainRates(idProject,t,idTissue,idTrackedCell,order,data) VALUES (?,?,?,?,?,?)");

				std::auto_ptr<sql::PreparedStatement> insertIntercalationStrainRates_stmt(
						m_DB->prepareStatement(
								insertIntercalationStrainRatesString));

				for (DomainStrainRatesMapType::iterator it =
						domainStrainRates->begin();
						it != domainStrainRates->end(); it++) {
					insertIntercalationStrainRates_stmt->setInt(1, m_ProjectID);
					insertIntercalationStrainRates_stmt->setInt(2, frame);
					insertIntercalationStrainRates_stmt->setInt(3, 0);
					insertIntercalationStrainRates_stmt->setInt(4, it->first);
					insertIntercalationStrainRates_stmt->setInt(5, 0); //TODO

					typedef boost::iostreams::stream<
							boost::iostreams::array_source> array_stream;
					array_stream is(
							(char*) it->second.Intercalation_SRT.data_block(),
							it->second.Intercalation_SRT.size()
									* sizeof(Tensor::element_type));
					insertIntercalationStrainRates_stmt->setBlob(6, &is);

					insertIntercalationStrainRates_stmt->execute();
				}
			}
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//cout << insertCellEdgeString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}


	DomainStrainRatesMapTypePointer GetDomainStrainRates() {
		return 	boost::shared_ptr<	DomainStrainRatesMapType>(new DomainStrainRatesMapType);;

	}



	inline bool IsEllipsesReady() {
		return true;
	}

	inline bool IsDomainStrainRatesReady() {
		return true;
	}

	inline SpacingType GetSpacing() {
		return m_Spacing;
	}
	inline double GetTemporalScale(){
		return m_TimeDelta;
	}

	inline std::string GetProjectName(){
		return m_ProjectName;
	}

	void SetProjectName(const std::string & name){
		try {

				std::string query("UPDATE Project SET Project.name=? WHERE Project.idProject=?");
				std::auto_ptr<sql::PreparedStatement> prep_stmt(m_DB->prepareStatement(query));

				prep_stmt->setString(1, name); //IDproject==2
				prep_stmt->setInt(2, m_ProjectID); //IDproject==2

				prep_stmt->execute();
			} catch (sql::SQLException &e) {
								/*
								 The MySQL Connector/C++ throws three different exceptions:

								 - sql::MethodNotImplementedException (derived from sql::SQLException)
								 - sql::InvalidArgumentException (derived from sql::SQLException)
								 - sql::SQLException (derived from std::runtime_error)
								 */
								cout << "# ERR: SQLException in " << __FILE__;
								//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
								/* Use what() (derived from std::runtime_error) to fetch the error message */
								cout << "# ERR: " << e.what();
								cout << " (MySQL error code: " << e.getErrorCode();
								cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			}
	}
	void SetSpacingX(double spacingX){
		m_Spacing[0]=spacingX;
		try {

			std::string query("UPDATE Project SET Project.spacingX=? WHERE Project.idProject=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(m_DB->prepareStatement(query));

			prep_stmt->setDouble(1, spacingX); //IDproject==2
			prep_stmt->setInt(2, m_ProjectID); //IDproject==2

			prep_stmt->execute();
		} catch (sql::SQLException &e) {
									/*
									 The MySQL Connector/C++ throws three different exceptions:

									 - sql::MethodNotImplementedException (derived from sql::SQLException)
									 - sql::InvalidArgumentException (derived from sql::SQLException)
									 - sql::SQLException (derived from std::runtime_error)
									 */
									cout << "# ERR: SQLException in " << __FILE__;
									//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
									/* Use what() (derived from std::runtime_error) to fetch the error message */
									cout << "# ERR: " << e.what();
									cout << " (MySQL error code: " << e.getErrorCode();
									cout << ", SQLState: " << e.getSQLState() << " )" << endl;
				}
	}

	void SetSpacingY(double spacingY){
		m_Spacing[1]=spacingY;
			try {

				std::string query("UPDATE Project SET Project.spacingY=? WHERE Project.idProject=?");
				std::auto_ptr<sql::PreparedStatement> prep_stmt(m_DB->prepareStatement(query));

				prep_stmt->setDouble(1, spacingY); //IDproject==2
				prep_stmt->setInt(2, m_ProjectID); //IDproject==2

				prep_stmt->execute();
			} catch (sql::SQLException &e) {
										/*
										 The MySQL Connector/C++ throws three different exceptions:

										 - sql::MethodNotImplementedException (derived from sql::SQLException)
										 - sql::InvalidArgumentException (derived from sql::SQLException)
										 - sql::SQLException (derived from std::runtime_error)
										 */
										cout << "# ERR: SQLException in " << __FILE__;
										//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
										/* Use what() (derived from std::runtime_error) to fetch the error message */
										cout << "# ERR: " << e.what();
										cout << " (MySQL error code: " << e.getErrorCode();
										cout << ", SQLState: " << e.getSQLState() << " )" << endl;
					}
		}
	void SetSpacingZ(double spacingZ){
		m_Spacing[2]=spacingZ;
			try {

				std::string query("UPDATE Project SET Project.spacingZ=? WHERE Project.idProject=?");
				std::auto_ptr<sql::PreparedStatement> prep_stmt(m_DB->prepareStatement(query));

				prep_stmt->setDouble(1, spacingZ); //IDproject==2
				prep_stmt->setInt(2, m_ProjectID); //IDproject==2

				prep_stmt->execute();
			} catch (sql::SQLException &e) {
										/*
										 The MySQL Connector/C++ throws three different exceptions:

										 - sql::MethodNotImplementedException (derived from sql::SQLException)
										 - sql::InvalidArgumentException (derived from sql::SQLException)
										 - sql::SQLException (derived from std::runtime_error)
										 */
										cout << "# ERR: SQLException in " << __FILE__;
										//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
										/* Use what() (derived from std::runtime_error) to fetch the error message */
										cout << "# ERR: " << e.what();
										cout << " (MySQL error code: " << e.getErrorCode();
										cout << ", SQLState: " << e.getSQLState() << " )" << endl;
					}
		}
	void SetSamplingRate(double rate){
		try {

				std::string query("UPDATE Project SET Project.timeDelta=? WHERE Project.idProject=?");
				std::auto_ptr<sql::PreparedStatement> prep_stmt(m_DB->prepareStatement(query));

				prep_stmt->setDouble(2, rate); //IDproject==2
				prep_stmt->setInt(2, m_ProjectID); //IDproject==2

				prep_stmt->execute();
			} catch (sql::SQLException &e) {
								/*
								 The MySQL Connector/C++ throws three different exceptions:

								 - sql::MethodNotImplementedException (derived from sql::SQLException)
								 - sql::InvalidArgumentException (derived from sql::SQLException)
								 - sql::SQLException (derived from std::runtime_error)
								 */
								cout << "# ERR: SQLException in " << __FILE__;
								//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
								/* Use what() (derived from std::runtime_error) to fetch the error message */
								cout << "# ERR: " << e.what();
								cout << " (MySQL error code: " << e.getErrorCode();
								cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			}
	}
	void SetHighestScale(double scale){
		try {

			std::string query("UPDATE Frame SET Frame.highestScale=? WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(m_DB->prepareStatement(query));

			prep_stmt->setDouble(1, scale); //IDproject==2
			prep_stmt->setInt(2, m_ProjectID); //IDproject==2
			prep_stmt->setInt(3, m_Frame); //IDproject==2
			prep_stmt->execute();
		} catch (sql::SQLException &e) {
							/*
							 The MySQL Connector/C++ throws three different exceptions:

							 - sql::MethodNotImplementedException (derived from sql::SQLException)
							 - sql::InvalidArgumentException (derived from sql::SQLException)
							 - sql::SQLException (derived from std::runtime_error)
							 */
							cout << "# ERR: SQLException in " << __FILE__;
							//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
							/* Use what() (derived from std::runtime_error) to fetch the error message */
							cout << "# ERR: " << e.what();
							cout << " (MySQL error code: " << e.getErrorCode();
							cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}
	void SetLowestScale(double scale){
			try {

				std::string query("UPDATE Frame SET Frame.lowestScale=? WHERE Frame.idProject=? AND Frame.t=?");
				std::auto_ptr<sql::PreparedStatement> prep_stmt(m_DB->prepareStatement(query));

				prep_stmt->setDouble(1, scale); //IDproject==2
				prep_stmt->setInt(2, m_ProjectID); //IDproject==2
				prep_stmt->setInt(3, m_Frame); //IDproject==2
				prep_stmt->execute();
			} catch (sql::SQLException &e) {
								/*
								 The MySQL Connector/C++ throws three different exceptions:

								 - sql::MethodNotImplementedException (derived from sql::SQLException)
								 - sql::InvalidArgumentException (derived from sql::SQLException)
								 - sql::SQLException (derived from std::runtime_error)
								 */
								cout << "# ERR: SQLException in " << __FILE__;
								//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
								/* Use what() (derived from std::runtime_error) to fetch the error message */
								cout << "# ERR: " << e.what();
								cout << " (MySQL error code: " << e.getErrorCode();
								cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			}
		}
	void SetScaleSteps(int nscales){
				try {

					std::string query("UPDATE Frame SET Frame.steps=? WHERE Frame.idProject=? AND Frame.t=?");
					std::auto_ptr<sql::PreparedStatement> prep_stmt(m_DB->prepareStatement(query));

					prep_stmt->setInt(1, nscales); //IDproject==2
					prep_stmt->setInt(2, m_ProjectID); //IDproject==2
					prep_stmt->setInt(3, m_Frame); //IDproject==2
					prep_stmt->execute();
				} catch (sql::SQLException &e) {
									/*
									 The MySQL Connector/C++ throws three different exceptions:

									 - sql::MethodNotImplementedException (derived from sql::SQLException)
									 - sql::InvalidArgumentException (derived from sql::SQLException)
									 - sql::SQLException (derived from std::runtime_error)
									 */
									cout << "# ERR: SQLException in " << __FILE__;
									//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
									/* Use what() (derived from std::runtime_error) to fetch the error message */
									cout << "# ERR: " << e.what();
									cout << " (MySQL error code: " << e.getErrorCode();
									cout << ", SQLState: " << e.getSQLState() << " )" << endl;
				}
			}

	double GetLowestScale(){
		try {
			std::string query("SELECT lowestScale FROM Frame WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
			std::cout << res->rowsCount() << std::endl;
			res->next();
			return res->getDouble("lowestScale");
		} catch (sql::SQLException &e) {
					/*
					 The MySQL Connector/C++ throws three different exceptions:

					 - sql::MethodNotImplementedException (derived from sql::SQLException)
					 - sql::InvalidArgumentException (derived from sql::SQLException)
					 - sql::SQLException (derived from std::runtime_error)
					 */
					cout << "# ERR: SQLException in " << __FILE__;
					//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
					/* Use what() (derived from std::runtime_error) to fetch the error message */
					cout << "# ERR: " << e.what();
					cout << " (MySQL error code: " << e.getErrorCode();
					cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
		return -1;
	}
	double GetHighestScale(){
		try {
			std::string query("SELECT highestScale FROM Frame WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
			std::cout << res->rowsCount() << std::endl;
			res->next();
			return res->getDouble("highestScale");
		} catch (sql::SQLException &e) {
					/*
					 The MySQL Connector/C++ throws three different exceptions:

					 - sql::MethodNotImplementedException (derived from sql::SQLException)
					 - sql::InvalidArgumentException (derived from sql::SQLException)
					 - sql::SQLException (derived from std::runtime_error)
					 */
					cout << "# ERR: SQLException in " << __FILE__;
					//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
					/* Use what() (derived from std::runtime_error) to fetch the error message */
					cout << "# ERR: " << e.what();
					cout << " (MySQL error code: " << e.getErrorCode();
					cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
		return -1;
	}

	int GetScaleSteps(){
			try {
				std::string query("SELECT steps FROM Frame WHERE Frame.idProject=? AND Frame.t=?");
				std::auto_ptr<sql::PreparedStatement> prep_stmt(
						m_DB->prepareStatement(query));

				prep_stmt->setInt(1, m_ProjectID); //IDproject==2
				prep_stmt->setInt(2, m_Frame); //IDproject==2
				prep_stmt->execute();

				std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
				std::cout << res->rowsCount() << std::endl;
				res->next();
				return res->getInt("steps");
			} catch (sql::SQLException &e) {
						/*
						 The MySQL Connector/C++ throws three different exceptions:

						 - sql::MethodNotImplementedException (derived from sql::SQLException)
						 - sql::InvalidArgumentException (derived from sql::SQLException)
						 - sql::SQLException (derived from std::runtime_error)
						 */
						cout << "# ERR: SQLException in " << __FILE__;
						//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
						/* Use what() (derived from std::runtime_error) to fetch the error message */
						cout << "# ERR: " << e.what();
						cout << " (MySQL error code: " << e.getErrorCode();
						cout << ", SQLState: " << e.getSQLState() << " )" << endl;

			}
			return -1;
		}

private:

	bool isImageInTable(const std::string & table, int frame) {

		std::stringstream ss;
		try {
			ss << "SELECT COUNT(*) FROM " << table << " WHERE " << table
					<< ".idProject=? AND " << table << ".t=?";
			std::cout << ss.str() << std::endl;
			std::cout << m_ProjectID << std::endl;
			std::cout << frame << std::endl;
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(ss.str()));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
			std::cout << res->rowsCount() << std::endl;
			res->next();
			return res->getInt("COUNT(*)") >= 1;
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
		return false;

	}

	template<class TImage> typename TImage::Pointer readImageFromTable(
			const std::string & table, int frame) {

		typedef itk::ImageFileReader<TImage> ReaderType;
		typename ReaderType::Pointer reader = ReaderType::New();

		std::stringstream ssQuery, ssFileName;

		try {
			ssQuery << "SELECT fileName FROM " << table << " WHERE " << table
					<< ".idProject=? AND " << table << ".t=?";
			std::cout << ssQuery.str() << std::endl;
			std::cout << m_ProjectID << std::endl;
			std::cout << frame << std::endl;
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(ssQuery.str()));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());

			assert(res->next());

			std::string fileName = res->getString("fileName");

			ssFileName << m_WorkingDirectory << "/" << fileName;

			reader->SetFileName(ssFileName.str());
			;
			reader->Update();
			reader->GetOutput()->SetSpacing(m_Spacing);
			return reader->GetOutput();

		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
		return NULL;

	}

	template<class TImage> void storeImageInTable(
			const typename TImage::Pointer & image, const std::string & table,
			unsigned int frame) {

		std::stringstream ssQuery, ssFileName, ssPath;

		typedef itk::ImageFileWriter<TImage> WriterType;

		typename WriterType::Pointer writer = WriterType::New();

		ssFileName << table << "-" << frame << ".tif";
		ssPath << this->m_WorkingDirectory << "/" << ssFileName.str();
		writer->SetFileName(ssPath.str());
		writer->SetInput(image);
		writer->Update();

		try {

			ssQuery << "INSERT into " << table
					<< " (idProject, t, fileName) values (?, ?, ?) on duplicate key UPDATE fileName=VALUES(fileName)";

			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(ssQuery.str()));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, frame); //t==0
			prep_stmt->setString(3, ssFileName.str());

			prep_stmt->execute();
		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}

	}

};
}

// TISSUETRACKINGPROJECT_H

#endif /* MYSQLTISSUETRACKINGPROJECT_H_ */
