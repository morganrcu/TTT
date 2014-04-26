#include "mysqltissuetrackingproject.h"

namespace ttt {
/*
 * mysqltissuetrackingproject.h
 *
 *  Created on: Oct 22, 2013
 *      Author: morgan
 */

TissueTrackingProject::TissueTrackingProject() : TissueTrackingAbstractProject(),m_Host("localhost"), m_DBName("TuftsTissueTracker"),m_User("root"),m_Password("ttt1Tracker") {
	m_ProjectID = 0;
	m_TimeDelta = -1;
	m_Frame = 0;
	m_Driver = sql::mysql::get_driver_instance();
}

void TissueTrackingProject::SetHost(const std::string & host) {
	m_Host = host;
}
void TissueTrackingProject::SetDBName(const std::string & dbname) {
	m_DBName = dbname;
}
void TissueTrackingProject::SetUser(const std::string & user) {
	m_User = user;
}
void TissueTrackingProject::SetPassword(const std::string & password) {
	m_Password = password;
}
void TissueTrackingProject::SetPort(unsigned int port) {
	m_Port = port;
}


bool TissueTrackingProject::openDB() {

	try {
		m_DB = std::auto_ptr<sql::Connection>(
					m_Driver->connect(m_Host, m_User, m_Password));
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

void TissueTrackingProject::OpenProject(int projectID) {

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

			m_Spacing[1] = res->getDouble("spacingX");

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
			cout << "# ERR: SQLException in " << __FILE__ << ":" << __LINE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(-1);

		}
}

void TissueTrackingProject::NewProject(const std::string & name,
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

void TissueTrackingProject::NewFrame(int numFrame) {
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
unsigned int TissueTrackingProject::GetNumFrames() {

		try {
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(
							"SELECT COUNT(*) FROM Frame WHERE Frame.idProject=?"));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
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

void TissueTrackingProject::SetProjectID(int ProjectID) {
		m_ProjectID = ProjectID;
}

void TissueTrackingProject::StoreConfig() {

}

void TissueTrackingProject::Load(std::string & basePath) {

}
void TissueTrackingProject::LoadVertexLocations() {
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
		m_VertexLocationsDirty = false;
		m_VertexLocationsLoaded = true;
	}

void TissueTrackingProject::StoreVertexLocations() {

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
		m_VertexLocationsDirty = false;
	}

inline bool TissueTrackingProject::IsVertexLocationsReady() {
	//TODO
	return true;
}

 void TissueTrackingProject::LoadTissueDescriptor() {
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
			std::string queryPerimeterString(
					"SELECT idMembranePoint FROM TissuePerimeter WHERE idProject=? AND t=? AND idTissue=? ORDER BY vertex_order ASC");

			std::auto_ptr<sql::PreparedStatement> queryPerimeter(
					m_DB->prepareStatement(queryPerimeterString));

			queryPerimeter->setInt(1, m_ProjectID); //IDproject==2
			queryPerimeter->setInt(2, m_Frame); //IDproject==2
			queryPerimeter->setInt(3, 0); //IDproject==2
			queryPerimeter->execute();

			std::auto_ptr<sql::ResultSet> resPerimeter(
					queryPerimeter->getResultSet());

			while (resPerimeter->next()) {

				m_TissueDescriptor->AddVertexToPerimeter(resPerimeter->getInt("idMembranePoint"));

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
				itk::Point<double,3> centroid;
				centroid[0] = resCell->getDouble("posX");
				centroid[1] = resCell->getDouble("posY");
				centroid[2] = resCell->getDouble("posZ");

				newCell.SetCentroid(centroid);

				ttt::CellVertexType vertex = boost::add_vertex(newCell,
						*m_TissueDescriptor->m_CellGraph);

				assert(vertex == resCell->getInt("idCell"));

				{
					std::string queryCellMembraneString(
							"SELECT idCell,idMembranePoint,`order` FROM Cell_has_MembranePoint WHERE Cell_has_MembranePoint.idProject=? AND Cell_has_MembranePoint.t=? AND Cell_has_MembranePoint.idTissue=? AND Cell_has_MembranePoint.idCell=? ORDER BY `Cell_has_MembranePoint`.`order` ASC");

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
		m_TissueDescriptorDirty = false;
		m_TissueDescriptorLoaded=true;
	}



 void TissueTrackingProject::StoreTissueDescriptor() {

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
			transStatement->execute("ROLLBACK;");
			exit(-1);
		}
///////////////////////////////////////////

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
			transStatement->execute("ROLLBACK;");
			exit(-1);
		}

		////////////////////////////////////////////////////


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
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}







	////////////////////////////////////////////////////

		try {

			std::string insertEdgeString(
					"INSERT INTO MembranePoint_linkedTo_MembranePoint(idProject,t,idTissue,idMembranePoint1,idMembranePoint2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertEdge_stmt(
					m_DB->prepareStatement(insertEdgeString));

			BGL_FORALL_EDGES(e,*m_TissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertEdge_stmt->setInt(1,m_ProjectID);
			insertEdge_stmt->setInt(2, m_Frame);
			insertEdge_stmt->setInt(3,0);
			ttt::SkeletonVertexType source =std::min(boost::source(e,*m_TissueDescriptor->m_SkeletonGraph),boost::target(e,*m_TissueDescriptor->m_SkeletonGraph));
			ttt::SkeletonVertexType target =std::max(boost::source(e,*m_TissueDescriptor->m_SkeletonGraph),boost::target(e,*m_TissueDescriptor->m_SkeletonGraph));
			assert(target>source);
			insertEdge_stmt->setInt(4,source);
			insertEdge_stmt->setInt(5,target);
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
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}


		////////////////////////////////////////////////////


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
			insertCell_stmt->setDouble(5,boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).GetCentroid()[0]);
			insertCell_stmt->setDouble(6,boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).GetCentroid()[1]);
			insertCell_stmt->setDouble(7,boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).GetCentroid()[2]);
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
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}

	try {

			std::string insertCellEdgeString(
					"INSERT INTO Cell_neighbor_of_Cell(idProject,t,idTissue,idCell1,idCell2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCellEdge_stmt(
					m_DB->prepareStatement(insertCellEdgeString));

			BGL_FORALL_EDGES(e,*m_TissueDescriptor->m_CellGraph,ttt::CellGraph){
			insertCellEdge_stmt->setInt(1,m_ProjectID);
			insertCellEdge_stmt->setInt(2,m_Frame);
			insertCellEdge_stmt->setInt(3,0);
			ttt::SkeletonVertexType source =std::min(boost::source(e,*m_TissueDescriptor->m_CellGraph),boost::target(e,*m_TissueDescriptor->m_CellGraph));
			ttt::SkeletonVertexType target =std::max(boost::source(e,*m_TissueDescriptor->m_CellGraph),boost::target(e,*m_TissueDescriptor->m_CellGraph));

			insertCellEdge_stmt->setInt(4,source);
			insertCellEdge_stmt->setInt(5,target);
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
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}
		try {

			std::string insertCellToMembranePointString(
					"INSERT INTO Cell_has_MembranePoint(idProject,t,idTissue,idCell,idMembranePoint,`order`) VALUES (?,?,?,?,?,?)");
			std::auto_ptr<sql::PreparedStatement> insertCellToMembranePointString_stmt(
					m_DB->prepareStatement(insertCellToMembranePointString));
			BGL_FORALL_VERTICES(v,*m_TissueDescriptor->m_CellGraph,ttt::CellGraph){

			int order=0;
			for(std::vector<ttt::SkeletonVertexType>::iterator it=boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).PerimeterBegin();
					it!=boost::get(ttt::CellPropertyTag(),*m_TissueDescriptor->m_CellGraph,v).PerimeterEnd();
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
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}

#if 0
	///////////////////////////////////////////////////////
	transStatement->execute("COMMIT;");

	transStatement->execute("START TRANSACTION;");
		try {

			std::string deleteTissueString(
					"DELETE from TissuePerimeter WHERE idProject=? AND t=? AND idTissue=?");

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
			transStatement->execute("ROLLBACK;");
			exit(-1);
		}
#endif
	try {

		std::string insertPerimeterString(
				"INSERT INTO TissuePerimeter(idProject,t,idTissue,idMembranePoint,vertex_order) VALUES (?,?,?,?,?)");

		std::auto_ptr<sql::PreparedStatement> insertPerimeter_stmt(
				m_DB->prepareStatement(insertPerimeterString));

		int order=0;

		for(std::vector<ttt::TissueDescriptor::PrimalGraphVertexDescriptorType>::iterator it= m_TissueDescriptor->BeginPerimeter();it!=m_TissueDescriptor->EndPerimeter();++it ){
			insertPerimeter_stmt->setInt(1,m_ProjectID);
			insertPerimeter_stmt->setInt(2,m_Frame);
			insertPerimeter_stmt->setInt(3,0);
			insertPerimeter_stmt->setInt(4,*it);
			insertPerimeter_stmt->setInt(5,order++);
			insertPerimeter_stmt->execute();
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

		transStatement->execute("ROLLBACK;");

		exit(-1);
	}

	transStatement->execute("COMMIT;");


		m_TissueDescriptorDirty = false;
	}

void TissueTrackingProject::SetTissueDescriptorDirty() {
		m_TissueDescriptorDirty = true;
}
bool TissueTrackingProject::IsTissueDescriptorReady() {
		return true;
}


//////////////////////////////??TRACKED TISSUE DESCRIPTOR/////////////////////////////////////


bool TissueTrackingProject::IsTrackedTissueDescriptorReady(){
	return true;
}
void TissueTrackingProject::LoadTrackedTissueDescriptor() {
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
					"SELECT idTrackedCell,idCell,trackID,posX,posY,posZ,velX,velY,velZ,parentID FROM TrackedCell WHERE TrackedCell.idProject=? AND TrackedCell.t=? AND TrackedCell.idTissue=? ORDER BY idTrackedCell ASC");

			std::auto_ptr<sql::PreparedStatement> queryCell(
					m_DB->prepareStatement(queryCellString));

			queryCell->setInt(1, m_ProjectID); //IDproject==2
			queryCell->setInt(2, m_Frame); //IDproject==2
			queryCell->setInt(3, 0); //IDproject==2
			queryCell->execute();

			std::auto_ptr<sql::ResultSet> resCell(queryCell->getResultSet());

			while (resCell->next()) {
				ttt::TrackedCell newCell;
				itk::Point<double,3> centroid;
				centroid[0] = resCell->getDouble("posX");
				centroid[1] = resCell->getDouble("posY");
				centroid[2] = resCell->getDouble("posZ");
				newCell.SetCentroid(centroid);

				itk::Vector<double,3> velocity;
				velocity[0] = resCell->getDouble("velX");
				velocity[1] = resCell->getDouble("velY");
				velocity[2] = resCell->getDouble("velZ");
				newCell.SetVelocity(velocity);

				newCell.SetID(resCell->getInt("trackID"));
				newCell.SetObservedCell(resCell->getInt("idCell"));
				newCell.SetParentID(resCell->getInt("parentID"));

				ttt::TrackedCellVertexType vertex = boost::add_vertex(newCell,
						*m_TrackedTissueDescriptor->m_CellGraph);

				assert(vertex == resCell->getInt("idTrackedCell"));

				{
					std::string queryTrackedCellMembraneString(
							"SELECT idTrackedCell,idTrackedMembranePoint,`order` FROM TrackedCell_has_TrackedMembranePoint WHERE TrackedCell_has_TrackedMembranePoint.idProject=? AND TrackedCell_has_TrackedMembranePoint.t=? AND TrackedCell_has_TrackedMembranePoint.idTissue=? AND TrackedCell_has_TrackedMembranePoint.idTrackedCell=? ORDER BY `order` ASC ");

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
		m_TrackedTissueDescriptorLoaded=true;
	}


void TissueTrackingProject::StoreTrackedTissueDescriptor() {

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
					"INSERT INTO TrackedMembranePoint_linkedTo_TrackedMembranePoint(idProject,t,idTissue,idTrackedMembranePoint1,idTrackedMembranePoint2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertEdge_stmt(
					m_DB->prepareStatement(insertEdgeString));

			BGL_FORALL_EDGES(e,*m_TrackedTissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertEdge_stmt->setInt(1,m_ProjectID);
			insertEdge_stmt->setInt(2,m_Frame);
			insertEdge_stmt->setInt(3,0);

			ttt::SkeletonVertexType source =std::min(boost::source(e,*m_TrackedTissueDescriptor->m_SkeletonGraph),boost::target(e,*m_TrackedTissueDescriptor->m_SkeletonGraph));
			ttt::SkeletonVertexType target =std::max(boost::source(e,*m_TrackedTissueDescriptor->m_SkeletonGraph),boost::target(e,*m_TrackedTissueDescriptor->m_SkeletonGraph));

			insertEdge_stmt->setInt(4,source);
			insertEdge_stmt->setInt(5,target);

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

			 - sql::MethodNotImplementedException (derived from sql::SQL	Exception)
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
					"INSERT INTO TrackedCell(idProject,t,idTissue,idTrackedCell,idCell,trackID,posX,posY,posZ,velX,velY,velZ,parentID) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCell_stmt(
					m_DB->prepareStatement(insertCellString));

			BGL_FORALL_VERTICES(v,*m_TrackedTissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){
			insertCell_stmt->setInt(1,m_ProjectID);
			insertCell_stmt->setInt(2,m_Frame);
			insertCell_stmt->setInt(3,0);

			insertCell_stmt->setInt(4,v);

			insertCell_stmt->setInt(5,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetObservedCell()); //TODO obtain from ???
			insertCell_stmt->setInt(6,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetID());

			insertCell_stmt->setDouble(7,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetCentroid()[0]);
			insertCell_stmt->setDouble(8,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetCentroid()[1]);
			insertCell_stmt->setDouble(9,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetCentroid()[2]);

			insertCell_stmt->setDouble(10,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetVelocity()[0]);
			insertCell_stmt->setDouble(11,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetVelocity()[1]);
			insertCell_stmt->setDouble(12,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetVelocity()[2]);
			insertCell_stmt->setDouble(13,boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).GetParentID());

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
					"INSERT INTO TrackedCell_neighbor_of_TrackedCell(idProject,t,idTissue,idTrackedCell1,idTrackedCell2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCellEdge_stmt(
					m_DB->prepareStatement(insertCellEdgeString));

			BGL_FORALL_EDGES(e,*m_TrackedTissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){
			insertCellEdge_stmt->setInt(1,m_ProjectID);
			insertCellEdge_stmt->setInt(2,m_Frame);
			insertCellEdge_stmt->setInt(3,0);

			ttt::CellVertexType source =std::min(boost::source(e,*m_TrackedTissueDescriptor->m_CellGraph),boost::target(e,*m_TrackedTissueDescriptor->m_CellGraph));
			ttt::CellVertexType target =std::max(boost::source(e,*m_TrackedTissueDescriptor->m_CellGraph),boost::target(e,*m_TrackedTissueDescriptor->m_CellGraph));

			insertCellEdge_stmt->setInt(4,source);
			insertCellEdge_stmt->setInt(5,target);
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
					"INSERT INTO TrackedCell_has_TrackedMembranePoint(idProject,t,idTissue,idTrackedCell,idTrackedMembranePoint,`order`) VALUES (?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertTrackedCellToTrackedMembranePointString_stmt(
					m_DB->prepareStatement(
							insertTrackedCellToMembranePointString));

			BGL_FORALL_VERTICES(v,*m_TrackedTissueDescriptor->m_CellGraph,ttt::TrackedCellGraph){
			int order=0;
			for(std::vector<ttt::SkeletonVertexType>::iterator it=boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).PerimeterBegin();
					it!=boost::get(ttt::TrackedCellPropertyTag(),*m_TrackedTissueDescriptor->m_CellGraph,v).PerimeterEnd();
					it++) {

				insertTrackedCellToTrackedMembranePointString_stmt->setInt(1,m_ProjectID);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(2,m_Frame);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(3,0);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(4,v);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(5,*it);
				insertTrackedCellToTrackedMembranePointString_stmt->setInt(6,order);
				insertTrackedCellToTrackedMembranePointString_stmt->execute();
				order++;
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
		m_TrackedTissueDescriptorDirty=false;

	}


void TissueTrackingProject::LoadTrackedEllipses() {
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
		m_TrackedEllipsesReady=true;
		m_TrackedEllipsesDirty=true;
	}

void TissueTrackingProject::StoreTrackedEllipses() {
		std::auto_ptr<sql::Statement> transStatement(m_DB->createStatement());
		transStatement->execute("START TRANSACTION;");

		try {

			std::string insertTrackedEllipsesString(
					"INSERT INTO TrackedEllipse(idProject,t,idTissue,idTrackedCell,majorR,minorR,rotR,posX,posY) VALUES (?,?,?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertTrackedEllipses_stmt(
					m_DB->prepareStatement(insertTrackedEllipsesString));

			for (EllipseMapType::iterator it = m_TrackedEllipses->begin();
					it != m_TrackedEllipses->end(); it++) {
				insertTrackedEllipses_stmt->setInt(1, m_ProjectID);
				insertTrackedEllipses_stmt->setInt(2, m_Frame);
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
		m_TrackedEllipsesDirty = false;
	}

bool TissueTrackingProject::IsTrackedEllipsesReady() {
		try {
			std::string trackedEllipsesReadyQuery(
					"SELECT COUNT(*) FROM  TrackedEllipse WHERE TrackedEllipse.idProject=? AND TrackedEllipse.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(trackedEllipsesReadyQuery));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
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

void TissueTrackingProject::LoadTrackedDomains() {
		std::string selectTrackedDomainsString(
				"SELECT idTrackedCell,`order` FROM TrackedDomain WHERE TrackedDomain.idProject=? AND TrackedDomain.t=? AND TrackedDomain.idTissue=? ORDER BY TrackedDomain.idTrackedCell ASC");
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

void TissueTrackingProject::StoreTrackedDomains() {
		std::auto_ptr<sql::Statement> transStatement(m_DB->createStatement());
		transStatement->execute("START TRANSACTION;");
		try {

			std::string deleteTrackedDomainString(
					"DELETE from TrackedDomain WHERE idProject=? AND t=? AND idTissue=?");

			std::auto_ptr<sql::PreparedStatement> deleteTrackedDomain_stmt(
					m_DB->prepareStatement(deleteTrackedDomainString));

			deleteTrackedDomain_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteTrackedDomain_stmt->setInt(2, m_Frame); //t==0
			deleteTrackedDomain_stmt->setInt(3, 0); //t==0
			deleteTrackedDomain_stmt->execute();

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
			std::string insertTrackedDomainsString(
					"INSERT INTO TrackedDomain(idProject,t,idTissue,idTrackedCell,`order`) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertTrackedDomains_stmt(
					m_DB->prepareStatement(insertTrackedDomainsString));

			for (TrackedDomainVectorType::iterator it =
					m_TrackedDomains->begin(); it != m_TrackedDomains->end();
					it++) {
				insertTrackedDomains_stmt->setInt(1, m_ProjectID);
				insertTrackedDomains_stmt->setInt(2, m_Frame);
				insertTrackedDomains_stmt->setInt(3, 0);
				insertTrackedDomains_stmt->setInt(4, it->GetNucleus());
				insertTrackedDomains_stmt->setInt(5, it->GetOrder());
				insertTrackedDomains_stmt->execute();

				for (typename std::set<ttt::TrackedCellVertexType>::iterator it2 =
						it->Begin(); it2 != it->End(); it2++) {
					std::string insertTrackedDomainsTrackedCellString(
							"INSERT INTO TrackedDomain_has_TrackedCell(idProject,t,idTissue,idTrackedCell,idTrackedNucleousCell) VALUES (?,?,?,?,?)");

					std::auto_ptr<sql::PreparedStatement> insertTrackedDomainsTrackedCell_stmt(
							m_DB->prepareStatement(
									insertTrackedDomainsTrackedCellString));

					insertTrackedDomainsTrackedCell_stmt->setInt(1,
							m_ProjectID);
					insertTrackedDomainsTrackedCell_stmt->setInt(2, m_Frame);
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
		transStatement->execute("COMMIT;");
	}

bool TissueTrackingProject::IsTrackedDomainsReady() {
		try {
			std::string trackedDomainsReadyQuery(
					"SELECT COUNT(*) FROM  TrackedDomain WHERE TrackedDomain.idProject=? AND TrackedDomain.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(trackedDomainsReadyQuery));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
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


void TissueTrackingProject::LoadDomainStrainRates() {
		 m_DomainStrainRates =boost::shared_ptr<DomainStrainRatesMapType>(new DomainStrainRatesMapType);

		try {
			{

				std::string selectTissueStrainRatesString("SELECT idTrackedCell,`order`,data From TissueStrainRates WHERE idProject=? AND t=? AND idTissue=?");

				std::auto_ptr<sql::PreparedStatement> insertTissueStrainRates_stmt(m_DB->prepareStatement(selectTissueStrainRatesString));


				insertTissueStrainRates_stmt->setInt(1, m_ProjectID);
				insertTissueStrainRates_stmt->setInt(2, m_Frame);
				insertTissueStrainRates_stmt->setInt(3, 0);

				insertTissueStrainRates_stmt->execute();

				std::auto_ptr<sql::ResultSet> res(insertTissueStrainRates_stmt->getResultSet());


				while(res->next()){
					(*m_DomainStrainRates)[res->getInt("idTrackedCell")].order=res->getInt("order");
					double data[4];
					res->getBlob("data")->read((char*)data,sizeof(double)*4) ;
					(*m_DomainStrainRates)[res->getInt("idTrackedCell")].Tissue_SRT.set(data);

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

		try{
			{

			std::string selectCellStrainRatesString("SELECT idTrackedCell,`order`,data From CellStrainRates WHERE idProject=? AND t=? AND idTissue=?");

			std::auto_ptr<sql::PreparedStatement> selectCellStrainRates_stmt(m_DB->prepareStatement(selectCellStrainRatesString));


			selectCellStrainRates_stmt->setInt(1, m_ProjectID);
			selectCellStrainRates_stmt->setInt(2, m_Frame);
			selectCellStrainRates_stmt->setInt(3, 0);

			selectCellStrainRates_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(selectCellStrainRates_stmt->getResultSet());


			while(res->next()){
				(*m_DomainStrainRates)[res->getInt("idTrackedCell")].order=res->getInt("order");
				double data[4];
				res->getBlob("data")->read((char*)data,sizeof(double)*4) ;
				(*m_DomainStrainRates)[res->getInt("idTrackedCell")].CellShape_SRT.set(data);

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

	try{
		{

		std::string selectIntercalationCellStrainRatesString("SELECT idTrackedCell,`order`,data From IntercalationStrainRates WHERE idProject=? AND t=? AND idTissue=?");

		std::auto_ptr<sql::PreparedStatement> selectIntercalationStrainRates_stmt(m_DB->prepareStatement(selectIntercalationCellStrainRatesString));


		selectIntercalationStrainRates_stmt->setInt(1, m_ProjectID);
		selectIntercalationStrainRates_stmt->setInt(2, m_Frame);
		selectIntercalationStrainRates_stmt->setInt(3, 0);

		selectIntercalationStrainRates_stmt->execute();

		std::auto_ptr<sql::ResultSet> res(selectIntercalationStrainRates_stmt->getResultSet());


		while(res->next()){
			(*m_DomainStrainRates)[res->getInt("idTrackedCell")].order=res->getInt("order");
			double data[4];
			res->getBlob("data")->read((char*)data,sizeof(double)*4) ;
			(*m_DomainStrainRates)[res->getInt("idTrackedCell")].Intercalation_SRT.set(data);

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
	m_DomainStrainRatesDirty=true;
	m_DomainStrainRatesReady=true;
	}

void TissueTrackingProject::StoreDomainStrainRates() {
		try {
			{
				std::string insertTissueStrainRatesString(
						"INSERT INTO TissueStrainRates(idProject,t,idTissue,idTrackedCell,`order`,data) VALUES (?,?,?,?,?,?)");

				std::auto_ptr<sql::PreparedStatement> insertTissueStrainRates_stmt(
						m_DB->prepareStatement(insertTissueStrainRatesString));

				for (DomainStrainRatesMapType::iterator it =
						m_DomainStrainRates->begin();
						it != m_DomainStrainRates->end(); it++) {
					insertTissueStrainRates_stmt->setInt(1, m_ProjectID);
					insertTissueStrainRates_stmt->setInt(2, m_Frame);
					insertTissueStrainRates_stmt->setInt(3, 0);
					insertTissueStrainRates_stmt->setInt(4, it->first);
					insertTissueStrainRates_stmt->setInt(5, it->second.order);
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
						"INSERT INTO CellStrainRates(idProject,t,idTissue,idTrackedCell,`order`,data) VALUES (?,?,?,?,?,?)");

				std::auto_ptr<sql::PreparedStatement> insertCellStrainRates_stmt(
						m_DB->prepareStatement(insertCellStrainRatesString));

				for (DomainStrainRatesMapType::iterator it =
						m_DomainStrainRates->begin();
						it != m_DomainStrainRates->end(); it++) {
					insertCellStrainRates_stmt->setInt(1, m_ProjectID);
					insertCellStrainRates_stmt->setInt(2, m_Frame);
					insertCellStrainRates_stmt->setInt(3, 0);
					insertCellStrainRates_stmt->setInt(4, it->first);
					insertCellStrainRates_stmt->setInt(5, it->second.order); //TODO
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
						"INSERT INTO IntercalationStrainRates(idProject,t,idTissue,idTrackedCell,`order`,data) VALUES (?,?,?,?,?,?)");

				std::auto_ptr<sql::PreparedStatement> insertIntercalationStrainRates_stmt(
						m_DB->prepareStatement(
								insertIntercalationStrainRatesString));

				for (DomainStrainRatesMapType::iterator it =
						m_DomainStrainRates->begin();
						it != m_DomainStrainRates->end(); it++) {
					insertIntercalationStrainRates_stmt->setInt(1, m_ProjectID);
					insertIntercalationStrainRates_stmt->setInt(2, m_Frame);
					insertIntercalationStrainRates_stmt->setInt(3, 0);
					insertIntercalationStrainRates_stmt->setInt(4, it->first);
					insertIntercalationStrainRates_stmt->setInt(5, it->second.order); //TODO

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
		bool m_DomainStrainRatesDirty=false;
	}

bool TissueTrackingProject::IsDomainStrainRatesReady() {
		try {
			std::string strainRatesReadyQuery(
					"SELECT COUNT(*) FROM  TissueStrainRates WHERE TissueStrainRates.idProject=? AND TissueStrainRates.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(strainRatesReadyQuery));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
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

void TissueTrackingProject::SetProjectName(const std::string & name) {
		try {

			std::string query(
					"UPDATE Project SET Project.name=? WHERE Project.idProject=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

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

void TissueTrackingProject::SetSpacingX(double spacingX) {
		m_Spacing[0] = spacingX;
		try {

			std::string query(
					"UPDATE Project SET Project.spacingX=? WHERE Project.idProject=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

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

void TissueTrackingProject::SetSpacingY(double spacingY) {
		m_Spacing[1] = spacingY;
		try {

			std::string query(
					"UPDATE Project SET Project.spacingY=? WHERE Project.idProject=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

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

void TissueTrackingProject::SetSpacingZ(double spacingZ) {
		m_Spacing[2] = spacingZ;
		try {

			std::string query(
					"UPDATE Project SET Project.spacingZ=? WHERE Project.idProject=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

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

void TissueTrackingProject::SetSamplingRate(double rate) {
		try {

			std::string query(
					"UPDATE Project SET Project.timeDelta=? WHERE Project.idProject=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

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

void TissueTrackingProject::SetHighestScale(double scale) {
		try {

			std::string query(
					"UPDATE Frame SET Frame.highestScale=? WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

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

void TissueTrackingProject::SetLowestScale(double scale) {
		try {

			std::string query(
					"UPDATE Frame SET Frame.lowestScale=? WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

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
void TissueTrackingProject::SetScaleSteps(int nscales) {
		try {

			std::string query(
					"UPDATE Frame SET Frame.steps=? WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

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

double TissueTrackingProject::GetLowestScale() {
		try {
			std::string query(
					"SELECT lowestScale FROM Frame WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
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
double TissueTrackingProject::GetHighestScale() {
		try {
			std::string query(
					"SELECT highestScale FROM Frame WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
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

int TissueTrackingProject::GetScaleSteps() {
		try {
			std::string query(
					"SELECT steps FROM Frame WHERE Frame.idProject=? AND Frame.t=?");
			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(query));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, m_Frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
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
int TissueTrackingProject::GetNumTracks() {
			try {
				std::string query(
						"SELECT Count(Distinct(trackId)) from TrackedCell WHERE TrackedCell.idProject=?");
				std::auto_ptr<sql::PreparedStatement> prep_stmt(
						m_DB->prepareStatement(query));

				prep_stmt->setInt(1, m_ProjectID); //IDproject==2
				prep_stmt->execute();

				std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
				res->next();
				return res->getInt("Count(Distinct(trackId))");
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

bool TissueTrackingProject::isImageInTable(const std::string & table, int frame) {
		std::stringstream ss;
		try {
			ss << "SELECT COUNT(*) FROM " << table << " WHERE " << table
					<< ".idProject=? AND " << table << ".t=?";

			std::auto_ptr<sql::PreparedStatement> prep_stmt(
					m_DB->prepareStatement(ss.str()));

			prep_stmt->setInt(1, m_ProjectID); //IDproject==2
			prep_stmt->setInt(2, frame); //IDproject==2
			prep_stmt->execute();

			std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
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

template<class TImage> typename TImage::Pointer TissueTrackingProject::readImageFromTable(
			const std::string & table, int frame) {
		typedef itk::ImageFileReader<TImage> ReaderType;
		typename ReaderType::Pointer reader = ReaderType::New();

		std::stringstream ssQuery, ssFileName;

		try {
			ssQuery << "SELECT fileName FROM " << table << " WHERE " << table
					<< ".idProject=? AND " << table << ".t=?";

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

template<class TImage> void TissueTrackingProject::storeImageInTable(
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
}
