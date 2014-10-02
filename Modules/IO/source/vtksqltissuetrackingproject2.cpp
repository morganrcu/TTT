
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "vtksqltissuetrackingproject2.h"
#include <vtkSQLQuery.h>
#include <vtkVariant.h>
namespace ttt {
typename TissueTrackingAbstractProject2::RawImageType::Pointer VTKSQLTissueTrackingProject2::GetRawImage(
		unsigned int frame) {
	return this->LoadImage("RawImages",frame);
}
void VTKSQLTissueTrackingProject2::SetRawImage(unsigned int frame,
		const typename RawImageType::Pointer & image) {
	this->StoreImage("RawImages",frame,image);
}
bool VTKSQLTissueTrackingProject2::IsRawImageAvailable(unsigned int frame) {
	return this->IsImageAvailable("RawImages",frame);
}

typename TissueTrackingAbstractProject2::DiffusedImageType::Pointer VTKSQLTissueTrackingProject2::GetDiffusedImage(
		unsigned int frame) {
	return this->LoadImage("DiffusedImages",frame);
}
void VTKSQLTissueTrackingProject2::SetDiffusedImage(unsigned int frame,
		const typename DiffusedImageType::Pointer & image) {
	this->StoreImage("DiffusedImages",frame,image);
}

bool VTKSQLTissueTrackingProject2::IsDiffusedImageAvailable(unsigned int frame) {
	return this->IsImageAvailable("DiffusedImages",frame);
}

typename TissueTrackingAbstractProject2::PlatenessImageType::Pointer VTKSQLTissueTrackingProject2::GetPlatenessImage(
		unsigned int frame) {
	return this->LoadImage("PlatenessImages",frame);
}
void VTKSQLTissueTrackingProject2::SetPlatenessImage(unsigned int frame,
		const typename PlatenessImageType::Pointer & image) {
	return this->StoreImage("PlatenessImages",frame,image);
}
bool VTKSQLTissueTrackingProject2::IsPlatenessImageAvailable(
		unsigned int frame) {
	return this->IsImageAvailable("PlatenessImages",frame);
}

typename TissueTrackingAbstractProject2::VertexnessImageType::Pointer VTKSQLTissueTrackingProject2::GetVertexnessImage(
		unsigned int frame) {
	return this->LoadImage("VertexnessImages",frame);
}
void VTKSQLTissueTrackingProject2::SetVertexnessImage(unsigned int frame,
		const typename VertexnessImageType::Pointer & image) {
	this->StoreImage("VertexnessImages",frame,image);
}
bool VTKSQLTissueTrackingProject2::IsVertexnessImageAvailable(
		unsigned int frame) {
	return this->IsImageAvailable("VertexnessImages",frame);
}

typename ttt::AdherensJunctionVertices::Pointer VTKSQLTissueTrackingProject2::GetAdherensJunctionVertices(
		unsigned int frame) {
	ttt::AdherensJunctionVertices::Pointer vertexLocations =
			ttt::AdherensJunctionVertices::New();
			std::string selectQuery("SELECT pointX,pointY,pointZ FROM VertexLocations WHERE VertexLocations.idProject=? AND VertexLocations.t=?");

		vtkSmartPointer<vtkSQLQuery> prep_stmt = m_DB->GetQueryInstance();
		prep_stmt->SetQuery(selectQuery.c_str());

		prep_stmt->BindParameter(0, m_ProjectID); //IDproject==2
		prep_stmt->BindParameter(1, frame); //IDproject==2
		prep_stmt->Execute();

		while (prep_stmt->NextRow()) {
			ttt::AdherensJunctionVertex::Pointer p =
					ttt::AdherensJunctionVertex::New();
			typedef typename ttt::AdherensJunctionVertex::IndexType Index;
			Index idx;

			idx[0] = prep_stmt->DataValue(prep_stmt->GetFieldIndex("pointX")).ToInt();
			idx[0] = prep_stmt->DataValue(prep_stmt->GetFieldIndex("pointY")).ToInt();
			idx[0] = prep_stmt->DataValue(prep_stmt->GetFieldIndex("pointZ")).ToInt();
			p->SetPosition(idx);

			vertexLocations->push_back(p);
		}

	return vertexLocations;
}
void VTKSQLTissueTrackingProject2::SetAdherensJunctionVertices(
		unsigned int frame,
		const typename ttt::AdherensJunctionVertices::Pointer & vertices) {

		std::string deleteString("DELETE from VertexLocations WHERE idProject=? AND t=?");

		vtkSmartPointer<vtkSQLQuery> delete_stmt = m_DB->GetQueryInstance();
		delete_stmt->SetQuery(deleteString.c_str());

		delete_stmt->BindParameter(0, m_ProjectID); //IDproject==2
		delete_stmt->BindParameter(1, frame); //IDproject==2
		delete_stmt->Execute();

		std::string insertString("INSERT INTO VertexLocations (idProject,t,pointX,pointY,pointZ) VALUES (?,?,?,?,?)");

		vtkSmartPointer<vtkSQLQuery> insert_stmt = m_DB->GetQueryInstance();
		insert_stmt->SetQuery(insertString.c_str());

		vtkSmartPointer<vtkSQLQuery> transStatement = m_DB->GetQueryInstance();
		transStatement->SetQuery("START TRANSACTION;");
		transStatement->Execute();

		for (ttt::AdherensJunctionVertices::const_iterator it =vertices->begin(); it != vertices->end(); it++) {

			ttt::AdherensJunctionVertex::Pointer p = *it;

			insert_stmt->BindParameter(0, m_ProjectID);
			insert_stmt->BindParameter(1, frame);
			insert_stmt->BindParameter(2, p->GetPosition()[0]);
			insert_stmt->BindParameter(3, p->GetPosition()[1]);
			insert_stmt->BindParameter(4, p->GetPosition()[2]);
			insert_stmt->Execute();
		}
		transStatement->SetQuery("COMMIT;");
		transStatement->Execute();

}
bool VTKSQLTissueTrackingProject2::IsAdherensJunctionVerticesAvailable(
		unsigned int frame) {
	return true;
}
typename ttt::TissueDescriptor::Pointer VTKSQLTissueTrackingProject2::GetTissueDescriptor(
		unsigned int frame) {
#if 0
	ttt::TissueDescriptor::Pointer tissueDescriptor =ttt::TissueDescriptor::New();

	{
		std::string queryMembranePointsString(
				"SELECT idMembranePoint,posX,posY,posZ FROM MembranePoint WHERE MembranePoint.idProject=? AND MembranePoint.t=? AND MembranePoint.idTissue=? ORDER BY idMembranePoint ASC");

		std::auto_ptr<sql::PreparedStatement> queryMembranePoints(
				m_DB->prepareStatement(queryMembranePointsString));

		queryMembranePoints->setInt(1, m_ProjectID); //IDproject==2
		queryMembranePoints->setInt(2, frame); //IDproject==2
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
					*tissueDescriptor->m_SkeletonGraph);

			assert(vertex == resMembranePoints->getInt("idMembranePoint"));

		}

	}

	{
		std::string queryPerimeterString(
				"SELECT idMembranePoint FROM TissuePerimeter WHERE idProject=? AND t=? AND idTissue=? ORDER BY vertex_order ASC");

		std::auto_ptr<sql::PreparedStatement> queryPerimeter(
				m_DB->prepareStatement(queryPerimeterString));

		queryPerimeter->setInt(1, m_ProjectID); //IDproject==2
		queryPerimeter->setInt(2, frame); //IDproject==2
		queryPerimeter->setInt(3, 0); //IDproject==2
		queryPerimeter->execute();

		std::auto_ptr<sql::ResultSet> resPerimeter(
				queryPerimeter->getResultSet());

		while (resPerimeter->next()) {

			tissueDescriptor->AddVertexToPerimeter(
					resPerimeter->getInt("idMembranePoint"));

		}

	}

	{
		std::string queryMembraneEdgesString(
				"SELECT idMembranePoint1,idMembranePoint2 FROM MembranePoint_linkedTo_MembranePoint WHERE MembranePoint_linkedTo_MembranePoint.idProject=? AND MembranePoint_linkedTo_MembranePoint.t=? AND MembranePoint_linkedTo_MembranePoint.idTissue=?");

		std::auto_ptr<sql::PreparedStatement> queryMembraneEdges(
				m_DB->prepareStatement(queryMembraneEdgesString));

		queryMembraneEdges->setInt(1, m_ProjectID); //IDproject==2
		queryMembraneEdges->setInt(2, frame); //IDproject==2
		queryMembraneEdges->setInt(3, 0); //IDproject==2
		queryMembraneEdges->execute();

		std::auto_ptr<sql::ResultSet> resMembraneEdges(
				queryMembraneEdges->getResultSet());

		while (resMembraneEdges->next()) {
			boost::add_edge(resMembraneEdges->getInt("idMembranePoint1"),
					resMembraneEdges->getInt("idMembranePoint2"),
					*tissueDescriptor->m_SkeletonGraph);
		}

	}

	{
		std::string queryCellString(
				"SELECT idCell,posX,posY,posZ FROM Cell WHERE Cell.idProject=? AND Cell.t=? AND Cell.idTissue=? ORDER BY idCell ASC");

		std::auto_ptr<sql::PreparedStatement> queryCell(
				m_DB->prepareStatement(queryCellString));

		queryCell->setInt(1, m_ProjectID); //IDproject==2
		queryCell->setInt(2, frame); //IDproject==2
		queryCell->setInt(3, 0); //IDproject==2
		queryCell->execute();

		std::auto_ptr<sql::ResultSet> resCell(queryCell->getResultSet());

		while (resCell->next()) {
			ttt::Cell newCell;
			itk::Point<double, 3> centroid;
			centroid[0] = resCell->getDouble("posX");
			centroid[1] = resCell->getDouble("posY");
			centroid[2] = resCell->getDouble("posZ");

			newCell.SetCentroid(centroid);

			ttt::CellVertexType vertex = boost::add_vertex(newCell,
					*tissueDescriptor->m_CellGraph);

			assert(vertex == resCell->getInt("idCell"));

			{
				std::string queryCellMembraneString(
						"SELECT idCell,idMembranePoint,`order` FROM Cell_has_MembranePoint WHERE Cell_has_MembranePoint.idProject=? AND Cell_has_MembranePoint.t=? AND Cell_has_MembranePoint.idTissue=? AND Cell_has_MembranePoint.idCell=? ORDER BY `Cell_has_MembranePoint`.`order` ASC");

				std::auto_ptr<sql::PreparedStatement> queryCellMembrane(
						m_DB->prepareStatement(queryCellMembraneString));

				queryCellMembrane->setInt(1, m_ProjectID); //IDproject==2
				queryCellMembrane->setInt(2, frame); //IDproject==2
				queryCellMembrane->setInt(3, 0); //IDproject==2
				queryCellMembrane->setInt(4, vertex); //IDproject==2
				queryCellMembrane->execute();

				std::auto_ptr<sql::ResultSet> resCellMembrane(
						queryCellMembrane->getResultSet());

				while (resCellMembrane->next()) {
					boost::get(ttt::CellPropertyTag(),
							*tissueDescriptor->m_CellGraph, vertex).AddVertexToPerimeter(
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
		queryCellEdges->setInt(2, frame); //IDproject==2
		queryCellEdges->setInt(3, 0); //IDproject==2
		queryCellEdges->execute();

		std::auto_ptr<sql::ResultSet> resCellEdges(
				queryCellEdges->getResultSet());

		while (resCellEdges->next()) {
			boost::add_edge(resCellEdges->getInt("idCell1"),
					resCellEdges->getInt("idCell2"),
					*tissueDescriptor->m_CellGraph);
		}
	}
	return tissueDescriptor;
#endif
}
void VTKSQLTissueTrackingProject2::SetTissueDescriptor(unsigned int frame,
		const typename ttt::TissueDescriptor::Pointer & tissueDescriptor) {
#if 0
	std::auto_ptr<sql::Statement> transStatement(m_DB->createStatement());
		transStatement->execute("START TRANSACTION;");
		try {

			std::string deleteTissueString(
					"DELETE from Tissue WHERE idProject=? AND t=? AND idTissue=?");

			std::auto_ptr<sql::PreparedStatement> deleteTissue_stmt(
					m_DB->prepareStatement(deleteTissueString));

			deleteTissue_stmt->setInt(1, m_ProjectID); //IDproject==2
			deleteTissue_stmt->setInt(2, frame); //t==0
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
			std::cout << "# ERR: SQLException in " << __FILE__;
			//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
			//cout << deleteVertexString << std::endl;
			/* Use what() (derived from std::runtime_error) to fetch the error message */
			std::cout << "# ERR: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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
			insertTissue_stmt->setInt(2, frame);
			insertTissue_stmt->setInt(3, 0);
			insertTissue_stmt->execute();

		} catch (sql::SQLException &e) {
			/*
			 The MySQL Connector/C++ throws three different exceptions:

			 - sql::MethodNotImplementedException (derived from sql::SQLException)
			 - sql::InvalidArgumentException (derived from sql::SQLException)
			 - sql::SQLException (derived from std::runtime_error)
			 */
			std::cout << "# ERR: SQLException in " << __FILE__;
			//cout << insertVertexString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
			std::cout << "# ERR: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
			transStatement->execute("ROLLBACK;");
			exit(-1);
		}

		////////////////////////////////////////////////////


		try {

			std::string insertVertexString(
					"INSERT INTO MembranePoint(idProject,t,idTissue,idMembranePoint,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertVertex_stmt(
					m_DB->prepareStatement(insertVertexString));

			BGL_FORALL_VERTICES(v,*tissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertVertex_stmt->setInt(1,m_ProjectID);
			insertVertex_stmt->setInt(2,frame);
			insertVertex_stmt->setInt(3,0);
			insertVertex_stmt->setInt(4,v);
			insertVertex_stmt->setDouble(5,boost::get(ttt::SkeletonPointPropertyTag(),*tissueDescriptor->m_SkeletonGraph,v).position[0]);
			insertVertex_stmt->setDouble(6,boost::get(ttt::SkeletonPointPropertyTag(),*tissueDescriptor->m_SkeletonGraph,v).position[1]);
			insertVertex_stmt->setDouble(7,boost::get(ttt::SkeletonPointPropertyTag(),*tissueDescriptor->m_SkeletonGraph,v).position[2]);
			insertVertex_stmt->execute();

		}

	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		std::cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertVertexString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}







	////////////////////////////////////////////////////

		try {

			std::string insertEdgeString(
					"INSERT INTO MembranePoint_linkedTo_MembranePoint(idProject,t,idTissue,idMembranePoint1,idMembranePoint2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertEdge_stmt(
					m_DB->prepareStatement(insertEdgeString));

			BGL_FORALL_EDGES(e,*tissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertEdge_stmt->setInt(1,m_ProjectID);
			insertEdge_stmt->setInt(2, frame);
			insertEdge_stmt->setInt(3,0);
			ttt::SkeletonVertexType source =std::min(boost::source(e,*tissueDescriptor->m_SkeletonGraph),boost::target(e,*tissueDescriptor->m_SkeletonGraph));
			ttt::SkeletonVertexType target =std::max(boost::source(e,*tissueDescriptor->m_SkeletonGraph),boost::target(e,*tissueDescriptor->m_SkeletonGraph));
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
		std::cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertEdgeString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}


		////////////////////////////////////////////////////


		try {

			std::string insertCellString(
					"INSERT INTO Cell(idProject,t,idTissue,idCell,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCell_stmt(
					m_DB->prepareStatement(insertCellString));

			BGL_FORALL_VERTICES(v,*tissueDescriptor->m_CellGraph,ttt::CellGraph){
			insertCell_stmt->setInt(1,m_ProjectID);
			insertCell_stmt->setInt(2,frame);
			insertCell_stmt->setInt(3,0);
			insertCell_stmt->setInt(4,v);
			insertCell_stmt->setDouble(5,boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).GetCentroid()[0]);
			insertCell_stmt->setDouble(6,boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).GetCentroid()[1]);
			insertCell_stmt->setDouble(7,boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).GetCentroid()[2]);
			insertCell_stmt->execute();
		}
	} catch(sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		std::cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}

	try {

			std::string insertCellEdgeString(
					"INSERT INTO Cell_neighbor_of_Cell(idProject,t,idTissue,idCell1,idCell2) VALUES (?,?,?,?,?)");

			std::auto_ptr<sql::PreparedStatement> insertCellEdge_stmt(
					m_DB->prepareStatement(insertCellEdgeString));

			BGL_FORALL_EDGES(e,*tissueDescriptor->m_CellGraph,ttt::CellGraph){
			insertCellEdge_stmt->setInt(1,m_ProjectID);
			insertCellEdge_stmt->setInt(2,frame);
			insertCellEdge_stmt->setInt(3,0);
			ttt::SkeletonVertexType source =std::min(boost::source(e,*tissueDescriptor->m_CellGraph),boost::target(e,*tissueDescriptor->m_CellGraph));
			ttt::SkeletonVertexType target =std::max(boost::source(e,*tissueDescriptor->m_CellGraph),boost::target(e,*tissueDescriptor->m_CellGraph));

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
		std::cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellEdgeString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();

		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		transStatement->execute("ROLLBACK;");
		exit(-1);
	}
		try {

			std::string insertCellToMembranePointString(
					"INSERT INTO Cell_has_MembranePoint(idProject,t,idTissue,idCell,idMembranePoint,`order`) VALUES (?,?,?,?,?,?)");
			std::auto_ptr<sql::PreparedStatement> insertCellToMembranePointString_stmt(
					m_DB->prepareStatement(insertCellToMembranePointString));
			BGL_FORALL_VERTICES(v,*tissueDescriptor->m_CellGraph,ttt::CellGraph){

			int order=0;
			for(ttt::Cell::PerimeterIterator it=boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).PerimeterBegin();
					it!=boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).PerimeterEnd();
					it++) {
				insertCellToMembranePointString_stmt->setInt(1,m_ProjectID);
				insertCellToMembranePointString_stmt->setInt(2,frame);
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
		std::cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertCellEdgeString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
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

		for(std::vector<ttt::TissueDescriptor::PrimalGraphVertexDescriptorType>::iterator it= tissueDescriptor->BeginPerimeter();it!=tissueDescriptor->EndPerimeter();++it ){
			insertPerimeter_stmt->setInt(1,m_ProjectID);
			insertPerimeter_stmt->setInt(2,frame);
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
		std::cout << "# ERR: SQLException in " << __FILE__;
		//cout << insertVertexString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		transStatement->execute("ROLLBACK;");

		exit(-1);
	}

	transStatement->execute("COMMIT;");

#endif
}
bool VTKSQLTissueTrackingProject2::IsTissueDescriptorAvailable(
		unsigned int frame) {
	return true;
}

typename ttt::TrackedTissueDescriptor::Pointer VTKSQLTissueTrackingProject2::GetTrackedTissueDescriptor(
		unsigned int frame) {
#if 0
	ttt::TrackedTissueDescriptor::Pointer trackedTissueDescriptor =
			ttt::TrackedTissueDescriptor::New();

	{
		std::string queryMembranePointsString(
				"SELECT idTrackedMembranePoint,posX,posY,posZ FROM TrackedMembranePoint WHERE TrackedMembranePoint.idProject=? AND TrackedMembranePoint.t=? AND TrackedMembranePoint.idTissue=? ORDER BY idTrackedMembranePoint ASC");

		std::auto_ptr<sql::PreparedStatement> queryMembranePoints(
				m_DB->prepareStatement(queryMembranePointsString));

		queryMembranePoints->setInt(1, m_ProjectID); //IDproject==2
		queryMembranePoints->setInt(2, frame); //IDproject==2
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
					*trackedTissueDescriptor->m_SkeletonGraph);

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
		queryMembraneEdges->setInt(2, frame); //IDproject==2
		queryMembraneEdges->setInt(3, 0); //IDproject==2
		queryMembraneEdges->execute();

		std::auto_ptr<sql::ResultSet> resMembraneEdges(
				queryMembraneEdges->getResultSet());

		while (resMembraneEdges->next()) {
			boost::add_edge(resMembraneEdges->getInt("idTrackedMembranePoint1"),
					resMembraneEdges->getInt("idTrackedMembranePoint2"),
					*trackedTissueDescriptor->m_SkeletonGraph);
		}

	}

	{

		std::string queryCellString(
				"SELECT idTrackedCell,idCell,trackID,posX,posY,posZ,velX,velY,velZ,parentID FROM TrackedCell WHERE TrackedCell.idProject=? AND TrackedCell.t=? AND TrackedCell.idTissue=? ORDER BY idTrackedCell ASC");

		std::auto_ptr<sql::PreparedStatement> queryCell(
				m_DB->prepareStatement(queryCellString));

		queryCell->setInt(1, m_ProjectID); //IDproject==2
		queryCell->setInt(2, frame); //IDproject==2
		queryCell->setInt(3, 0); //IDproject==2
		queryCell->execute();

		std::auto_ptr<sql::ResultSet> resCell(queryCell->getResultSet());

		while (resCell->next()) {
			ttt::TrackedCell newCell;
			itk::Point<double, 3> centroid;
			centroid[0] = resCell->getDouble("posX");
			centroid[1] = resCell->getDouble("posY");
			centroid[2] = resCell->getDouble("posZ");
			newCell.SetCentroid(centroid);

			itk::Vector<double, 3> velocity;
			velocity[0] = resCell->getDouble("velX");
			velocity[1] = resCell->getDouble("velY");
			velocity[2] = resCell->getDouble("velZ");
			newCell.SetVelocity(velocity);

			newCell.SetID(resCell->getInt("trackID"));
			newCell.SetObservedCell(resCell->getInt("idCell"));
			newCell.SetParentID(resCell->getInt("parentID"));

			ttt::TrackedCellVertexType vertex = boost::add_vertex(newCell,
					*trackedTissueDescriptor->m_CellGraph);

			assert(vertex == resCell->getInt("idTrackedCell"));

			{
				std::string queryTrackedCellMembraneString(
						"SELECT idTrackedCell,idTrackedMembranePoint,`order` FROM TrackedCell_has_TrackedMembranePoint WHERE TrackedCell_has_TrackedMembranePoint.idProject=? AND TrackedCell_has_TrackedMembranePoint.t=? AND TrackedCell_has_TrackedMembranePoint.idTissue=? AND TrackedCell_has_TrackedMembranePoint.idTrackedCell=? ORDER BY `order` ASC ");

				std::auto_ptr<sql::PreparedStatement> queryTrackedCellMembrane(
						m_DB->prepareStatement(queryTrackedCellMembraneString));

				queryTrackedCellMembrane->setInt(1, m_ProjectID); //IDproject==2
				queryTrackedCellMembrane->setInt(2, frame); //IDproject==2
				queryTrackedCellMembrane->setInt(3, 0); //IDproject==2
				queryTrackedCellMembrane->setInt(4, vertex); //IDproject==2
				queryTrackedCellMembrane->execute();

				std::auto_ptr<sql::ResultSet> resTrackedCellMembrane(
						queryTrackedCellMembrane->getResultSet());

				while (resTrackedCellMembrane->next()) {
					boost::get(ttt::TrackedCellPropertyTag(),
							*trackedTissueDescriptor->m_CellGraph, vertex).AddVertexToPerimeter(
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
		queryCellEdges->setInt(2, frame); //IDproject==2
		queryCellEdges->setInt(3, 0); //IDproject==2
		queryCellEdges->execute();

		std::auto_ptr<sql::ResultSet> resCellEdges(
				queryCellEdges->getResultSet());

		while (resCellEdges->next()) {
			boost::add_edge(resCellEdges->getInt("idTrackedCell1"),
					resCellEdges->getInt("idTrackedCell2"),
					*trackedTissueDescriptor->m_CellGraph);
		}
	}
	return trackedTissueDescriptor;
#endif
}
void VTKSQLTissueTrackingProject2::SetTrackedTissueDescriptor(unsigned int frame,
		const typename ttt::TrackedTissueDescriptor::Pointer & descriptor) {
#if 0
	std::auto_ptr<sql::Statement> transStatement(m_DB->createStatement());
	transStatement->execute("START TRANSACTION;");

	try {

		std::string deleteVertexString(
				"DELETE from TrackedMembranePoint WHERE idProject=? AND t=?");

		std::auto_ptr<sql::PreparedStatement> deleteVertex_stmt(
				m_DB->prepareStatement(deleteVertexString));

		deleteVertex_stmt->setInt(1, m_ProjectID); //IDproject==2
		deleteVertex_stmt->setInt(2, frame); //t==0
		deleteVertex_stmt->execute();
	} catch (sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		std::cout << "# ERR: SQLException in " << __FILE__;
		//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
		//cout << deleteVertexString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(-1);

	}

	try {

		std::string insertVertexString(
				"INSERT INTO TrackedMembranePoint(idProject,t,idTissue,idTrackedMembranePoint,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

		std::auto_ptr<sql::PreparedStatement> insertVertex_stmt(
				m_DB->prepareStatement(insertVertexString));

		BGL_FORALL_VERTICES(v,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){

		insertVertex_stmt->setInt(1,m_ProjectID);
		insertVertex_stmt->setInt(2,frame);
		insertVertex_stmt->setInt(3,0);
		insertVertex_stmt->setInt(4,v);
		insertVertex_stmt->setDouble(5,boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[0]);
		insertVertex_stmt->setDouble(6,boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[1]);
		insertVertex_stmt->setDouble(7,boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[2]);
		insertVertex_stmt->execute();
	}

} catch(sql::SQLException &e) {
	/*
	 The MySQL Connector/C++ throws three different exceptions:

	 - sql::MethodNotImplementedException (derived from sql::SQLException)
	 - sql::InvalidArgumentException (derived from sql::SQLException)
	 - sql::SQLException (derived from std::runtime_error)
	 */
	std::cout << "# ERR: SQLException in " << __FILE__;
	//cout << insertVertexString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
	std::cout << "# ERR: " << e.what();
	std::cout << " (MySQL error code: " << e.getErrorCode();
	std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	exit(-1);

}

	try {
		std::string deleteEdgeString(
				"DELETE from TrackedMembranePoint_linkedTo_TrackedMembranePoint WHERE idProject=? AND t=?");

		std::auto_ptr<sql::PreparedStatement> deleteEdge_stmt(
				m_DB->prepareStatement(deleteEdgeString));

		deleteEdge_stmt->setInt(1, m_ProjectID); //IDproject==2
		deleteEdge_stmt->setInt(2, frame); //t==0
		deleteEdge_stmt->execute();
	} catch (sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		std::cout << "# ERR: SQLException in " << __FILE__;
		//cout << deleteEdgeString << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(-1);

	}

	try {

		std::string insertEdgeString(
				"INSERT INTO TrackedMembranePoint_linkedTo_TrackedMembranePoint(idProject,t,idTissue,idTrackedMembranePoint1,idTrackedMembranePoint2) VALUES (?,?,?,?,?)");

		std::auto_ptr<sql::PreparedStatement> insertEdge_stmt(
				m_DB->prepareStatement(insertEdgeString));

		BGL_FORALL_EDGES(e,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){

		insertEdge_stmt->setInt(1,m_ProjectID);
		insertEdge_stmt->setInt(2,frame);
		insertEdge_stmt->setInt(3,0);

		ttt::SkeletonVertexType source =std::min(boost::source(e,*descriptor->m_SkeletonGraph),boost::target(e,*descriptor->m_SkeletonGraph));
		ttt::SkeletonVertexType target =std::max(boost::source(e,*descriptor->m_SkeletonGraph),boost::target(e,*descriptor->m_SkeletonGraph));

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
	std::cout << "# ERR: SQLException in " << __FILE__;
	//cout << insertEdgeString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
	std::cout << "# ERR: " << e.what();
	std::cout << " (MySQL error code: " << e.getErrorCode();
	std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	exit(-1);

}

	try {

		std::string deleteCellString(
				"DELETE from TrackedCell WHERE idProject=? AND t=?");
		std::auto_ptr<sql::PreparedStatement> deleteCell_stmt(
				m_DB->prepareStatement(deleteCellString));

		deleteCell_stmt->setInt(1, m_ProjectID); //IDproject==2
		deleteCell_stmt->setInt(2, frame); //t==0
		deleteCell_stmt->execute();
	} catch (sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQL	Exception)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		std::cout << "# ERR: SQLException in " << __FILE__;
		//cout << deleteCellString << std::endl;    		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(-1);

	}

	try {

		std::string insertCellString(
				"INSERT INTO TrackedCell(idProject,t,idTissue,idTrackedCell,idCell,trackID,posX,posY,posZ,velX,velY,velZ,parentID) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

		std::auto_ptr<sql::PreparedStatement> insertCell_stmt(
				m_DB->prepareStatement(insertCellString));

		BGL_FORALL_VERTICES(v,*descriptor->m_CellGraph,ttt::TrackedCellGraph){
		insertCell_stmt->setInt(1,m_ProjectID);
		insertCell_stmt->setInt(2,frame);
		insertCell_stmt->setInt(3,0);

		insertCell_stmt->setInt(4,v);

		insertCell_stmt->setInt(5,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetObservedCell()); //TODO obtain from ???
		insertCell_stmt->setInt(6,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetID());

		insertCell_stmt->setDouble(7,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetCentroid()[0]);
		insertCell_stmt->setDouble(8,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetCentroid()[1]);
		insertCell_stmt->setDouble(9,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetCentroid()[2]);

		insertCell_stmt->setDouble(10,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetVelocity()[0]);
		insertCell_stmt->setDouble(11,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetVelocity()[1]);
		insertCell_stmt->setDouble(12,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetVelocity()[2]);
		insertCell_stmt->setDouble(13,boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetParentID());

		insertCell_stmt->execute();
	}
} catch(sql::SQLException &e) {
	/*
	 The MySQL Connector/C++ throws three different exceptions:

	 - sql::MethodNotImplementedException (derived from sql::SQLException)
	 - sql::InvalidArgumentException (derived from sql::SQLException)
	 - sql::SQLException (derived from std::runtime_error)
	 */
	std::cout << "# ERR: SQLException in " << __FILE__;
	//cout << insertCellString << std::endl;
	/* Use what() (derived from std::runtime_error) to fetch the error message */
	std::cout << "# ERR: " << e.what();
	std::cout << " (MySQL error code: " << e.getErrorCode();
	std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	exit(-1);

}

	try {

		std::string insertCellEdgeString(
				"INSERT INTO TrackedCell_neighbor_of_TrackedCell(idProject,t,idTissue,idTrackedCell1,idTrackedCell2) VALUES (?,?,?,?,?)");

		std::auto_ptr<sql::PreparedStatement> insertCellEdge_stmt(
				m_DB->prepareStatement(insertCellEdgeString));

		BGL_FORALL_EDGES(e,*descriptor->m_CellGraph,ttt::TrackedCellGraph){
		insertCellEdge_stmt->setInt(1,m_ProjectID);
		insertCellEdge_stmt->setInt(2,frame);
		insertCellEdge_stmt->setInt(3,0);

		ttt::CellVertexType source =std::min(boost::source(e,*descriptor->m_CellGraph),boost::target(e,*descriptor->m_CellGraph));
		ttt::CellVertexType target =std::max(boost::source(e,*descriptor->m_CellGraph),boost::target(e,*descriptor->m_CellGraph));

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
	std::cout << "# ERR: SQLException in " << __FILE__;
	//cout << insertCellEdgeString << std::endl;
	/* Use what() (derived from std::runtime_error) to fetch the error message */
	std::cout << "# ERR: " << e.what();
	std::cout << " (MySQL error code: " << e.getErrorCode();
	std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	exit(-1);

}

	try {

		std::string insertTrackedCellToMembranePointString(
				"INSERT INTO TrackedCell_has_TrackedMembranePoint(idProject,t,idTissue,idTrackedCell,idTrackedMembranePoint,`order`) VALUES (?,?,?,?,?,?)");

		std::auto_ptr<sql::PreparedStatement> insertTrackedCellToTrackedMembranePointString_stmt(
				m_DB->prepareStatement(insertTrackedCellToMembranePointString));

		BGL_FORALL_VERTICES(v,*descriptor->m_CellGraph,ttt::TrackedCellGraph){
		int order=0;
		for(ttt::Cell::PerimeterIterator it=boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).PerimeterBegin();
				it!=boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).PerimeterEnd();
				it++) {

			insertTrackedCellToTrackedMembranePointString_stmt->setInt(1,m_ProjectID);
			insertTrackedCellToTrackedMembranePointString_stmt->setInt(2,frame);
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
	std::cout << "# ERR: SQLException in " << __FILE__;
	//cout << insertCellEdgeString << std::endl;
	/* Use what() (derived from std::runtime_error) to fetch the error message */
	std::cout << "# ERR: " << e.what();
	std::cout << " (MySQL error code: " << e.getErrorCode();
	std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	exit(-1);

}
	transStatement->execute("COMMIT;");
#endif

}

bool VTKSQLTissueTrackingProject2::IsTrackedTissueDescriptorAvailable(
		unsigned int frame) {
	return true;
}

void VTKSQLTissueTrackingProject2::LoadProjectInfo() {

	vtkSmartPointer<vtkSQLQuery> prep_stmt = m_DB->GetQueryInstance();
	assert(prep_stmt->SetQuery("select spacingX,spacingY,spacingZ,sizeX,sizeY,sizeZ,samplingPeriod,workingDirectory,name from Project WHERE Project.idProject=?"));

	prep_stmt->BindParameter(0, m_ProjectID);
	prep_stmt->Execute();

	prep_stmt->NextRow();

	m_Spacing[0] = prep_stmt->DataValue(prep_stmt->GetFieldIndex("spacingX")).ToDouble();
	m_Spacing[1] = prep_stmt->DataValue(prep_stmt->GetFieldIndex("spacingY")).ToDouble();
	m_Spacing[2] = prep_stmt->DataValue(prep_stmt->GetFieldIndex("spacingZ")).ToDouble();

	m_ProjectDirectory = prep_stmt->DataValue(prep_stmt->GetFieldIndex("workingDirectory")).ToString();
	m_ProjectName = prep_stmt->DataValue(prep_stmt->GetFieldIndex("name")).ToString();

	m_Size[0]=prep_stmt->DataValue(prep_stmt->GetFieldIndex("sizeX")).ToInt();
	m_Size[1]=prep_stmt->DataValue(prep_stmt->GetFieldIndex("sizeY")).ToInt();
	m_Size[2]=prep_stmt->DataValue(prep_stmt->GetFieldIndex("sizeZ")).ToInt();

	m_SamplingPeriod = prep_stmt->DataValue(prep_stmt->GetFieldIndex("samplingPeriod")).ToDouble();

	vtkSmartPointer<vtkSQLQuery> framenum_stmt = m_DB->GetQueryInstance();

	framenum_stmt->SetQuery("SELECT COUNT(*) FROM Frame WHERE Frame.idProject=?");

	framenum_stmt->BindParameter(0, m_ProjectID); //IDproject==2
	framenum_stmt->Execute();

	m_NumFrames=prep_stmt->DataValue(prep_stmt->GetFieldIndex("COUNT(*)")).ToInt();

}
void VTKSQLTissueTrackingProject2::StoreProjectInfo() {

}
void VTKSQLTissueTrackingProject2::StoreFrameInfo(unsigned int frame) {

}
void VTKSQLTissueTrackingProject2::LoadFrameInfo(unsigned int frame) {
#if 0
	try {
		std::string query(
				"SELECT platenessSteps,platenessHighestScale,platenessLowestScale,vertexnessSteps,vertexnessLowestScale,vertexnessHighestScale FROM Frame WHERE Frame.idProject=? AND Frame.t=?");
		std::auto_ptr<sql::PreparedStatement> prep_stmt(
				m_DB->prepareStatement(query));

		prep_stmt->setInt(1, m_ProjectID); //IDproject==2
		prep_stmt->setInt(2, frame); //IDproject==2
		prep_stmt->execute();

		std::auto_ptr<sql::ResultSet> res(prep_stmt->getResultSet());
		res->next();
		this->m_PlatenessSteps[frame]=res->getInt("platenessSteps");
		std::cout<< "Steps " << frame << "--" << this->m_PlatenessSteps[frame] <<std::endl;
		this->m_HighestPlatenessScale[frame]=res->getDouble("platenessHighestScale");
		std::cout<< "HighestPlatenesss " << frame << "--" << this->m_HighestPlatenessScale[frame] <<std::endl;
		this->m_LowestPlatenessScale[frame] = res->getDouble("platenessLowestScale");
		std::cout<< "LowestPlatenesss " << frame << "--" << this->m_LowestPlatenessScale[frame] <<std::endl;

		this->m_VertexnessSteps[frame]=res->getInt("vertexnessSteps");
		this->m_LowestVertexnessScale[frame]=res->getDouble("vertexnessLowestScale");
		this->m_HighestVertexnessScale[frame]=res->getDouble("vertexnessHighestScale");
	} catch (sql::SQLException &e) {
		/*
		 The MySQL Connector/C++ throws three different exceptions:

		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		std::cout << "# ERR: SQLException in " << __FILE__;
		//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

	}
#endif
}

bool VTKSQLTissueTrackingProject2::IsImageAvailable(const std::string & name,
		int frame) {
#if 0
	std::stringstream ss;
	try {
		ss << "SELECT COUNT(*) FROM " << name << " WHERE " << name
				<< ".idProject=? AND " << name << ".t=?";

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
		std::cout << "# ERR: SQLException in " << __FILE__;
		//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

	}
	return false;
#endif
}

typename itk::Image<float, 3>::Pointer VTKSQLTissueTrackingProject2::LoadImage(
		const std::string & table, int frame) {


	typedef itk::ImageFileReader<itk::Image<float, 3> > ReaderType;
	typename ReaderType::Pointer reader = ReaderType::New();

	std::stringstream ssQuery, ssFileName;


		ssQuery << "SELECT fileName FROM " << table << " WHERE " << table
				<< ".idProject=? AND " << table << ".t=?";

		vtkSmartPointer<vtkSQLQuery> prep_stmt= m_DB->GetQueryInstance();

		prep_stmt->BindParameter(0, m_ProjectID); //IDproject==2
		prep_stmt->BindParameter(1, frame); //IDproject==2
		prep_stmt->Execute();

		//assert(prep_stmt->NextRow());

		std::string fileName = prep_stmt->DataValue(prep_stmt->GetFieldIndex("fileName")).ToString();

		ssFileName << m_ProjectDirectory << "/" << fileName;

		reader->SetFileName(ssFileName.str());
		;
		reader->Update();
		reader->GetOutput()->SetSpacing(m_Spacing);
		reader->GetOutput()->Print(std::cout,itk::Indent());
		return reader->GetOutput();


}

void VTKSQLTissueTrackingProject2::StoreImage(const std::string & name,
		int frame, const typename itk::Image<float, 3>::Pointer & image) {
#if 0
	std::stringstream ssQuery, ssFileName, ssPath;

	typedef itk::ImageFileWriter<itk::Image<float, 3> > WriterType;

	typename WriterType::Pointer writer = WriterType::New();

	ssFileName << name << "-" << frame << ".tif";
	ssPath << this->m_ProjectDirectory << "/" << ssFileName.str();
	writer->SetFileName(ssPath.str());
	writer->SetInput(image);
	writer->Update();

	try {

		ssQuery << "INSERT into " << name
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
		std::cout << "# ERR: SQLException in " << __FILE__;
		//    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

	}
#endif
}

FeatureMap<CellVertexType,itk::Point<double,3> > VTKSQLTissueTrackingProject2::GetCentroids(unsigned int frame){

}

void VTKSQLTissueTrackingProject2::SetCentroids(unsigned int frame,const FeatureMap<CellVertexType,itk::Point<double,3> >  & centroids){

}

FeatureMap<CellVertexType,double> VTKSQLTissueTrackingProject2::GetXX(unsigned int frame){

}
void VTKSQLTissueTrackingProject2::SetXX(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & xx){

}
FeatureMap<CellVertexType,double> VTKSQLTissueTrackingProject2::GetXY(unsigned int frame){

}

void VTKSQLTissueTrackingProject2::SetXY(unsigned int frame,const FeatureMap<TrackedCellVertexType,double> &xy){

}

FeatureMap<CellVertexType,double> VTKSQLTissueTrackingProject2::GetYY(unsigned int frame){

}

void VTKSQLTissueTrackingProject2::SetYY(unsigned int frame, const FeatureMap<CellVertexType,double> & yy){

}

FeatureMap<CellVertexType,double> VTKSQLTissueTrackingProject2::GetPerimeter(unsigned int frame){

}

void VTKSQLTissueTrackingProject2::SetPerimeter(unsigned int frame, const FeatureMap<CellVertexType,double> & perimeters){

}

FeatureMap<CellVertexType,double> VTKSQLTissueTrackingProject2::GetAreas(unsigned int frame){

}

void VTKSQLTissueTrackingProject2::SetAreas(unsigned int frame, const FeatureMap<CellVertexType,double> & areas){

}


}
