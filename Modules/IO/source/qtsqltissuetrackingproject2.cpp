
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "qtsqltissuetrackingproject2.h"
#include <QSqlQuery>
#include <QVariant>
namespace ttt {
typename TissueTrackingAbstractProject2::RawImageType::Pointer QTSQLTissueTrackingProject2::GetRawImage(
		unsigned int frame) {
	return this->LoadImage("RawImages",frame);
}
void QTSQLTissueTrackingProject2::SetRawImage(unsigned int frame,
		const typename RawImageType::Pointer & image) {
	this->StoreImage("RawImages",frame,image);
}
bool QTSQLTissueTrackingProject2::IsRawImageAvailable(unsigned int frame) {
	return this->IsImageAvailable("RawImages",frame);
}

typename TissueTrackingAbstractProject2::DiffusedImageType::Pointer QTSQLTissueTrackingProject2::GetDiffusedImage(
		unsigned int frame) {
	return this->LoadImage("DiffusedImages",frame);
}
void QTSQLTissueTrackingProject2::SetDiffusedImage(unsigned int frame,
		const typename DiffusedImageType::Pointer & image) {
	this->StoreImage("DiffusedImages",frame,image);
}

bool QTSQLTissueTrackingProject2::IsDiffusedImageAvailable(unsigned int frame) {
	return this->IsImageAvailable("DiffusedImages",frame);
}

typename TissueTrackingAbstractProject2::PlatenessImageType::Pointer QTSQLTissueTrackingProject2::GetPlatenessImage(
		unsigned int frame) {
	return this->LoadImage("PlatenessImages",frame);
}
void QTSQLTissueTrackingProject2::SetPlatenessImage(unsigned int frame,
		const typename PlatenessImageType::Pointer & image) {
	return this->StoreImage("PlatenessImages",frame,image);
}
bool QTSQLTissueTrackingProject2::IsPlatenessImageAvailable(
		unsigned int frame) {
	return this->IsImageAvailable("PlatenessImages",frame);
}

typename TissueTrackingAbstractProject2::VertexnessImageType::Pointer QTSQLTissueTrackingProject2::GetVertexnessImage(
		unsigned int frame) {
	return this->LoadImage("VertexnessImages",frame);
}
void QTSQLTissueTrackingProject2::SetVertexnessImage(unsigned int frame,
		const typename VertexnessImageType::Pointer & image) {
	this->StoreImage("VertexnessImages",frame,image);
}
bool QTSQLTissueTrackingProject2::IsVertexnessImageAvailable(
		unsigned int frame) {
	return this->IsImageAvailable("VertexnessImages",frame);
}

typename ttt::AdherensJunctionVertices<3>::Pointer QTSQLTissueTrackingProject2::GetAdherensJunctionVertices(
		unsigned int frame) {
	ttt::AdherensJunctionVertices<3>::Pointer vertexLocations =
			ttt::AdherensJunctionVertices<3>::New();
			std::string selectQuery("SELECT pointX,pointY,pointZ FROM VertexLocations WHERE VertexLocations.idProject=? AND VertexLocations.t=?");

			QSqlQuery prep_stmt(QString::fromStdString(selectQuery),m_DB);

			prep_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
			prep_stmt.bindValue(1, frame); //IDproject==2
			prep_stmt.exec();

		while (prep_stmt.next()) {
			ttt::AdherensJunctionVertex<3>::Pointer p =
					ttt::AdherensJunctionVertex<3>::New();
			typedef typename ttt::AdherensJunctionVertex<3>::IndexType Index;
			Index idx;

			idx[0] = prep_stmt.value("pointX").toInt();
			idx[1] = prep_stmt.value("pointY").toInt();
			idx[2] = prep_stmt.value("pointZ").toInt();
			p->SetPosition(idx);

			vertexLocations->push_back(p);
		}

	return vertexLocations;
}
void QTSQLTissueTrackingProject2::SetAdherensJunctionVertices(
		unsigned int frame,
		const typename ttt::AdherensJunctionVertices<3>::Pointer & vertices) {

		std::string deleteString("DELETE from VertexLocations WHERE idProject=? AND t=?");

		QSqlQuery delete_stmt(deleteString.c_str(),m_DB);

		delete_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		delete_stmt.bindValue(1, QVariant(frame)); //IDproject==2
		delete_stmt.exec();

		std::string insertString("INSERT INTO VertexLocations (idProject,t,pointX,pointY,pointZ) VALUES (?,?,?,?,?)");

		QSqlQuery insert_stmt(insertString.c_str(),m_DB);

		QSqlQuery transStatement(m_DB);
		transStatement.exec("START TRANSACTION;");

		for (ttt::AdherensJunctionVertices<3>::const_iterator it =vertices->begin(); it != vertices->end(); it++) {

			ttt::AdherensJunctionVertex<3>::Pointer p = *it;

			insert_stmt.bindValue(0, QVariant(m_ProjectID));
			insert_stmt.bindValue(1, QVariant(frame));
			insert_stmt.bindValue(2, QVariant((int)p->GetPosition()[0]));
			insert_stmt.bindValue(3, QVariant((int)p->GetPosition()[1]));
			insert_stmt.bindValue(4, QVariant((int)p->GetPosition()[2]));
			insert_stmt.exec();
		}
		transStatement.exec("COMMIT;");

}
bool QTSQLTissueTrackingProject2::IsAdherensJunctionVerticesAvailable(
		unsigned int frame) {
	return true;
}
typename ttt::TissueDescriptor::Pointer QTSQLTissueTrackingProject2::GetTissueDescriptor(
		unsigned int frame) {

	ttt::TissueDescriptor::Pointer tissueDescriptor =ttt::TissueDescriptor::New();

	{
		std::string queryMembranePointsString(
				"SELECT idMembranePoint,posX,posY,posZ FROM MembranePoint WHERE MembranePoint.idProject=? AND MembranePoint.t=? AND MembranePoint.idTissue=? ORDER BY idMembranePoint ASC");

		QSqlQuery queryMembranePoints(QString::fromStdString(queryMembranePointsString),m_DB);


		queryMembranePoints.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		queryMembranePoints.bindValue(1, QVariant(frame)); //IDproject==2
		queryMembranePoints.bindValue(2, QVariant(0)); //IDproject==2
		queryMembranePoints.exec();

		while (queryMembranePoints.next()) {

			ttt::SkeletonPoint newPoint;
			newPoint.position[0] = queryMembranePoints.value("posX").toDouble();
			newPoint.position[1] =  queryMembranePoints.value("posY").toDouble();
			newPoint.position[2] =  queryMembranePoints.value("posZ").toDouble();
			ttt::SkeletonVertexType vertex = boost::add_vertex(newPoint,
					*tissueDescriptor->m_SkeletonGraph);

			assert(vertex ==  queryMembranePoints.value("idMembranePoint").toUInt());

		}

	}

	{
		std::string queryPerimeterString(
				"SELECT idMembranePoint FROM TissuePerimeter WHERE idProject=? AND t=? AND idTissue=? ORDER BY vertex_order ASC");

		QSqlQuery queryPerimeter(QString::fromStdString(queryPerimeterString),m_DB);


		queryPerimeter.bindValue(0, m_ProjectID); //IDproject==2
		queryPerimeter.bindValue(1, frame); //IDproject==2
		queryPerimeter.bindValue(2, 0); //IDproject==2
		queryPerimeter.exec();

		while (queryPerimeter.next()) {

			tissueDescriptor->AddVertexToPerimeter(
					queryPerimeter.value("idMembranePoint").toUInt());

		}

	}

	{
		std::string queryMembraneEdgesString(
				"SELECT idMembranePoint1,idMembranePoint2 FROM MembranePoint_linkedTo_MembranePoint WHERE MembranePoint_linkedTo_MembranePoint.idProject=? AND MembranePoint_linkedTo_MembranePoint.t=? AND MembranePoint_linkedTo_MembranePoint.idTissue=?");

		QSqlQuery queryMembraneEdges(QString::fromStdString(queryMembraneEdgesString),m_DB);

		queryMembraneEdges.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		queryMembraneEdges.bindValue(1, QVariant(frame)); //IDproject==2
		queryMembraneEdges.bindValue(2, QVariant(0)); //IDproject==2
		queryMembraneEdges.exec();


		while (queryMembraneEdges.next()) {
			boost::add_edge(queryMembraneEdges.value("idMembranePoint1").toUInt(),
					queryMembraneEdges.value("idMembranePoint2").toUInt(),
					*tissueDescriptor->m_SkeletonGraph);
		}

	}

	{
		std::string queryCellString(
				"SELECT idCell,posX,posY,posZ FROM Cell WHERE Cell.idProject=? AND Cell.t=? AND Cell.idTissue=? ORDER BY idCell ASC");

		QSqlQuery queryCell(QString::fromStdString(queryCellString),m_DB);

		queryCell.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		queryCell.bindValue(1, QVariant(frame)); //IDproject==2
		queryCell.bindValue(2, QVariant(0)); //IDproject==2
		queryCell.exec();

		while (queryCell.next()) {
			ttt::Cell newCell;
			itk::Point<double, 3> centroid;
			centroid[0] = queryCell.value("posX").toDouble();
			centroid[1] = queryCell.value("posY").toDouble();
			centroid[2] = queryCell.value("posZ").toDouble();

			newCell.SetCentroid(centroid);

			ttt::CellVertexType vertex = boost::add_vertex(newCell,
					*tissueDescriptor->m_CellGraph);

			assert(vertex == queryCell.value("idCell").toDouble());

			{
				std::string queryCellMembraneString(
						"SELECT idCell,idMembranePoint,`order` FROM Cell_has_MembranePoint WHERE Cell_has_MembranePoint.idProject=? AND Cell_has_MembranePoint.t=? AND Cell_has_MembranePoint.idTissue=? AND Cell_has_MembranePoint.idCell=? ORDER BY `Cell_has_MembranePoint`.`order` ASC");

				QSqlQuery queryCellMembrane(QString::fromStdString(queryCellMembraneString),m_DB);


				queryCellMembrane.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
				queryCellMembrane.bindValue(1, QVariant(frame)); //IDproject==2
				queryCellMembrane.bindValue(2, QVariant(0)); //IDproject==2
				queryCellMembrane.bindValue(3, QVariant((unsigned int)vertex)); //IDproject==2
				queryCellMembrane.exec();


				while (queryCellMembrane.next()) {
					boost::get(ttt::CellPropertyTag(),
							*tissueDescriptor->m_CellGraph, vertex).AddVertexToPerimeter(
									queryCellMembrane.value("idMembranePoint").toUInt());
				}
			}

		}
	}

	{
		std::string queryCellEdgesString(
				"SELECT idCell1,idCell2 FROM Cell_neighbor_of_Cell WHERE Cell_neighbor_of_Cell.idProject=? AND Cell_neighbor_of_Cell.t=? AND Cell_neighbor_of_Cell.idTissue=?");

		QSqlQuery queryCellEdges(QString::fromStdString(queryCellEdgesString),m_DB);

		queryCellEdges.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		queryCellEdges.bindValue(1, QVariant(frame)); //IDproject==2
		queryCellEdges.bindValue(2, QVariant(0)); //IDproject==2
		queryCellEdges.exec();

		while (queryCellEdges.next()) {
			boost::add_edge(queryCellEdges.value("idCell1").toUInt(),
					queryCellEdges.value("idCell2").toUInt(),
					*tissueDescriptor->m_CellGraph);
		}
	}
	return tissueDescriptor;

}
void QTSQLTissueTrackingProject2::SetTissueDescriptor(unsigned int frame,
		const typename ttt::TissueDescriptor::Pointer & tissueDescriptor) {

	QSqlQuery transStatement(m_DB);
	transStatement.exec("START TRANSACTION;");

	std::string deleteTissueString(
					"DELETE from Tissue WHERE idProject=? AND t=? AND idTissue=?");

	QSqlQuery deleteTissue_stmt(QString::fromStdString(deleteTissueString),m_DB);

	deleteTissue_stmt.bindValue(0,QVariant(m_ProjectID));
	deleteTissue_stmt.bindValue(1,QVariant(frame));
	deleteTissue_stmt.bindValue(2,QVariant(0));
	deleteTissue_stmt.exec();
	///////////////////////////////////////////


	std::string insertTissueString(
	"INSERT INTO Tissue(idProject,t,idTissue) VALUES (?,?,?)");

	QSqlQuery insertTissue_stmt(QString::fromStdString(insertTissueString),m_DB);

	insertTissue_stmt.bindValue(0, QVariant(m_ProjectID));
	insertTissue_stmt.bindValue(1, QVariant(frame));
	insertTissue_stmt.bindValue(2, QVariant(0));
	insertTissue_stmt.exec();


		////////////////////////////////////////////////////

	std::string insertVertexString(
					"INSERT INTO MembranePoint(idProject,t,idTissue,idMembranePoint,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

	QSqlQuery insertVertex_stmt(QString::fromStdString(insertVertexString),m_DB);

	BGL_FORALL_VERTICES(v,*tissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertVertex_stmt.bindValue(0,QVariant(m_ProjectID));
			insertVertex_stmt.bindValue(1,QVariant(frame));
			insertVertex_stmt.bindValue(2,QVariant(0));
			insertVertex_stmt.bindValue(3,QVariant((unsigned int)v));
			insertVertex_stmt.bindValue(4,QVariant((double)boost::get(ttt::SkeletonPointPropertyTag(),*tissueDescriptor->m_SkeletonGraph,v).position[0]));
			insertVertex_stmt.bindValue(5,QVariant((double)boost::get(ttt::SkeletonPointPropertyTag(),*tissueDescriptor->m_SkeletonGraph,v).position[1]));
			insertVertex_stmt.bindValue(6,QVariant((double)boost::get(ttt::SkeletonPointPropertyTag(),*tissueDescriptor->m_SkeletonGraph,v).position[2]));;
			insertVertex_stmt.exec();

		}
	std::string insertEdgeString("INSERT INTO MembranePoint_linkedTo_MembranePoint(idProject,t,idTissue,idMembranePoint1,idMembranePoint2) VALUES (?,?,?,?,?)");

	QSqlQuery insertEdge_stmt(QString::fromStdString(insertVertexString),m_DB);

	BGL_FORALL_EDGES(e,*tissueDescriptor->m_SkeletonGraph,ttt::SkeletonGraph){

			insertEdge_stmt.bindValue(0,QVariant(m_ProjectID));
			insertEdge_stmt.bindValue(1, QVariant(frame));
			insertEdge_stmt.bindValue(2,QVariant(0));
			ttt::SkeletonVertexType source =std::min(boost::source(e,*tissueDescriptor->m_SkeletonGraph),boost::target(e,*tissueDescriptor->m_SkeletonGraph));
			ttt::SkeletonVertexType target =std::max(boost::source(e,*tissueDescriptor->m_SkeletonGraph),boost::target(e,*tissueDescriptor->m_SkeletonGraph));
			assert(target>source);
			insertEdge_stmt.bindValue(4,QVariant((unsigned int)source));
			insertEdge_stmt.bindValue(5,QVariant((unsigned int)target));
			insertEdge_stmt.exec();
		}

	std::string insertCellString(
					"INSERT INTO Cell(idProject,t,idTissue,idCell,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

	QSqlQuery insertCell_stmt(QString::fromStdString(insertCellString),m_DB);


	BGL_FORALL_VERTICES(v,*tissueDescriptor->m_CellGraph,ttt::CellGraph){
		insertCell_stmt.bindValue(0,QVariant(m_ProjectID));
		insertCell_stmt.bindValue(1,QVariant(frame));
		insertCell_stmt.bindValue(2,QVariant(0));
		insertCell_stmt.bindValue(3,QVariant((unsigned int)v));
		insertCell_stmt.bindValue(4,QVariant(boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).GetCentroid()[0]));
		insertCell_stmt.bindValue(5,QVariant(boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).GetCentroid()[1]));
		insertCell_stmt.bindValue(6,QVariant(boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).GetCentroid()[2]));
		insertCell_stmt.exec();
	}




	std::string insertCellEdgeString("INSERT INTO Cell_neighbor_of_Cell(idProject,t,idTissue,idCell1,idCell2) VALUES (?,?,?,?,?)");

	QSqlQuery  insertCellEdge_stmt(QString::fromStdString(insertCellEdgeString),m_DB);

	BGL_FORALL_EDGES(e,*tissueDescriptor->m_CellGraph,ttt::CellGraph){
			insertCellEdge_stmt.bindValue(0,QVariant(m_ProjectID));
			insertCellEdge_stmt.bindValue(1,QVariant(frame));
			insertCellEdge_stmt.bindValue(2,QVariant(0));
			ttt::SkeletonVertexType source =std::min(boost::source(e,*tissueDescriptor->m_CellGraph),boost::target(e,*tissueDescriptor->m_CellGraph));
			ttt::SkeletonVertexType target =std::max(boost::source(e,*tissueDescriptor->m_CellGraph),boost::target(e,*tissueDescriptor->m_CellGraph));

			insertCellEdge_stmt.bindValue(3,QVariant((unsigned int)source));
			insertCellEdge_stmt.bindValue(4,QVariant((unsigned int)target));
			insertCellEdge_stmt.exec();
		}

	std::string insertCellToMembranePointString("INSERT INTO Cell_has_MembranePoint(idProject,t,idTissue,idCell,idMembranePoint,`order`) VALUES (?,?,?,?,?,?)");

	QSqlQuery insertCellToMembranePointString_stmt(QString::fromStdString(insertCellToMembranePointString),m_DB);

	BGL_FORALL_VERTICES(v,*tissueDescriptor->m_CellGraph,ttt::CellGraph){

			int order=0;
			for(ttt::Cell::PerimeterIterator it=boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).PerimeterBegin();
					it!=boost::get(ttt::CellPropertyTag(),*tissueDescriptor->m_CellGraph,v).PerimeterEnd();
					it++) {
				insertCellToMembranePointString_stmt.bindValue(0,m_ProjectID);
				insertCellToMembranePointString_stmt.bindValue(1,frame);
				insertCellToMembranePointString_stmt.bindValue(2,0);
				insertCellToMembranePointString_stmt.bindValue(3,(unsigned int)v);
				insertCellToMembranePointString_stmt.bindValue(4,(unsigned int) *it);
				insertCellToMembranePointString_stmt.bindValue(5,order++);
				insertCellToMembranePointString_stmt.exec();
			}
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


		std::string insertPerimeterString(
				"INSERT INTO TissuePerimeter(idProject,t,idTissue,idMembranePoint,vertex_order) VALUES (?,?,?,?,?)");

		QSqlQuery insertPerimeter_stmt(QString::fromStdString(insertPerimeterString),m_DB);

		int order=0;

		for(std::vector<ttt::TissueDescriptor::PrimalGraphVertexDescriptorType>::iterator it= tissueDescriptor->BeginPerimeter();it!=tissueDescriptor->EndPerimeter();++it ){
			insertPerimeter_stmt.bindValue(0,m_ProjectID);
			insertPerimeter_stmt.bindValue(1,frame);
			insertPerimeter_stmt.bindValue(2,0);
			insertPerimeter_stmt.bindValue(3,(unsigned int)*it);
			insertPerimeter_stmt.bindValue(4,order++);
			insertPerimeter_stmt.exec();
		}


	transStatement.exec("COMMIT;");


}
bool QTSQLTissueTrackingProject2::IsTissueDescriptorAvailable(
		unsigned int frame) {
	return true;
}

typename ttt::TrackedTissueDescriptor::Pointer QTSQLTissueTrackingProject2::GetTrackedTissueDescriptor(
		unsigned int frame) {

	ttt::TrackedTissueDescriptor::Pointer trackedTissueDescriptor = ttt::TrackedTissueDescriptor::New();

	{
		std::string queryMembranePointsString(
				"SELECT idTrackedMembranePoint,posX,posY,posZ FROM TrackedMembranePoint WHERE TrackedMembranePoint.idProject=? AND TrackedMembranePoint.t=? AND TrackedMembranePoint.idTissue=? ORDER BY idTrackedMembranePoint ASC");

		QSqlQuery queryMembranePoints(QString::fromStdString(queryMembranePointsString),m_DB);


		queryMembranePoints.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		queryMembranePoints.bindValue(1, QVariant(frame)); //IDproject==2
		queryMembranePoints.bindValue(2, 0); //IDproject==2
		queryMembranePoints.exec();

		while (queryMembranePoints.next()) {

			ttt::SkeletonPoint newPoint;
			newPoint.position[0] = queryMembranePoints.value("posX").toDouble();
			newPoint.position[1] = queryMembranePoints.value("posY").toDouble();
			newPoint.position[2] = queryMembranePoints.value("posZ").toDouble();
			ttt::SkeletonVertexType vertex = boost::add_vertex(newPoint,
					*trackedTissueDescriptor->m_SkeletonGraph);

			assert(
					vertex
							== queryMembranePoints.value("idTrackedMembranePoint").toInt());

		}

	}

	{
		std::string queryMembraneEdgesString(
				"SELECT idTrackedMembranePoint1,idTrackedMembranePoint2 FROM TrackedMembranePoint_linkedTo_TrackedMembranePoint WHERE TrackedMembranePoint_linkedTo_TrackedMembranePoint.idProject=? AND TrackedMembranePoint_linkedTo_TrackedMembranePoint.t=? AND TrackedMembranePoint_linkedTo_TrackedMembranePoint.idTissue=?");

		QSqlQuery queryMembraneEdges(QString::fromStdString(queryMembraneEdgesString),m_DB);

		queryMembraneEdges.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		queryMembraneEdges.bindValue(1, QVariant(frame)); //IDproject==2
		queryMembraneEdges.bindValue(2, QVariant(0)); //IDproject==2
		queryMembraneEdges.exec();



		while (queryMembraneEdges.next()) {
			boost::add_edge(queryMembraneEdges.value("idTrackedMembranePoint1").toUInt(),
					queryMembraneEdges.value("idTrackedMembranePoint2").toUInt(),
					*trackedTissueDescriptor->m_SkeletonGraph);
		}

	}

	{

		std::string queryCellString(
				"SELECT idTrackedCell,idCell,trackID,posX,posY,posZ,velX,velY,velZ,parentID FROM TrackedCell WHERE TrackedCell.idProject=? AND TrackedCell.t=? AND TrackedCell.idTissue=? ORDER BY idTrackedCell ASC");

		QSqlQuery queryCell(QString::fromStdString(queryCellString),m_DB);


		queryCell.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		queryCell.bindValue(1, QVariant(frame)); //IDproject==2
		queryCell.bindValue(2, QVariant(0)); //IDproject==2
		queryCell.exec();

		while (queryCell.next()) {
			ttt::TrackedCell newCell;
			itk::Point<double, 3> centroid;
			centroid[0] = queryCell.value("posX").toDouble();
			centroid[1] = queryCell.value("posY").toDouble();
			centroid[2] = queryCell.value("posZ").toDouble();
			newCell.SetCentroid(centroid);

			itk::Vector<double, 3> velocity;
			velocity[0] = queryCell.value("velX").toDouble();
			velocity[1] = queryCell.value("velY").toDouble();
			velocity[2] = queryCell.value("velZ").toDouble();
			newCell.SetVelocity(velocity);

			newCell.SetID(queryCell.value("trackID").toUInt());
			newCell.SetObservedCell(queryCell.value("idCell").toUInt());
			newCell.SetParentID(queryCell.value("parentID").toInt());

			ttt::TrackedCellVertexType vertex = boost::add_vertex(newCell,
					*trackedTissueDescriptor->m_CellGraph);

			assert(vertex == queryCell.value("idTrackedCell").toUInt());

			{
				std::string queryTrackedCellMembraneString(
						"SELECT idTrackedCell,idTrackedMembranePoint,`order` FROM TrackedCell_has_TrackedMembranePoint WHERE TrackedCell_has_TrackedMembranePoint.idProject=? AND TrackedCell_has_TrackedMembranePoint.t=? AND TrackedCell_has_TrackedMembranePoint.idTissue=? AND TrackedCell_has_TrackedMembranePoint.idTrackedCell=? ORDER BY `order` ASC ");

				QSqlQuery queryTrackedCellMembrane(QString::fromStdString(queryTrackedCellMembraneString),m_DB);

				queryTrackedCellMembrane.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
				queryTrackedCellMembrane.bindValue(1, QVariant(frame)); //IDproject==2
				queryTrackedCellMembrane.bindValue(2, QVariant(0)); //IDproject==2
				queryTrackedCellMembrane.bindValue(3, QVariant((unsigned int) vertex)); //IDproject==2
				queryTrackedCellMembrane.exec();

				while (queryTrackedCellMembrane.next()) {
					boost::get(ttt::TrackedCellPropertyTag(),
							*trackedTissueDescriptor->m_CellGraph, vertex).AddVertexToPerimeter(
									queryTrackedCellMembrane.value("idTrackedMembranePoint").toUInt());
				}
			}

		}
	}

	{
		std::string queryCellEdgesString(
				"SELECT idTrackedCell1,idTrackedCell2 FROM TrackedCell_neighbor_of_TrackedCell WHERE TrackedCell_neighbor_of_TrackedCell.idProject=? AND TrackedCell_neighbor_of_TrackedCell.t=? AND TrackedCell_neighbor_of_TrackedCell.idTissue=?");

		QSqlQuery queryCellEdges(QString::fromStdString(queryCellEdgesString),m_DB);


		queryCellEdges.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		queryCellEdges.bindValue(1, QVariant(frame)); //IDproject==2
		queryCellEdges.bindValue(2, QVariant(0)); //IDproject==2
		queryCellEdges.exec();

		while (queryCellEdges.next()) {
			boost::add_edge(queryCellEdges.value("idTrackedCell1").toUInt(),
					queryCellEdges.value("idTrackedCell2").toUInt(),
					*trackedTissueDescriptor->m_CellGraph);
		}
	}
	return trackedTissueDescriptor;

}
void QTSQLTissueTrackingProject2::SetTrackedTissueDescriptor(unsigned int frame,
		const typename ttt::TrackedTissueDescriptor::Pointer & descriptor) {

	QSqlQuery transStatement(m_DB);
	transStatement.exec("START TRANSACTION;");

	std::string deleteVertexString(	"DELETE from TrackedMembranePoint WHERE idProject=? AND t=?");

	QSqlQuery deleteVertex_stmt(QString::fromStdString(deleteVertexString),m_DB);

	deleteVertex_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
	deleteVertex_stmt.bindValue(1, frame); //t==0
	deleteVertex_stmt.exec();

	std::string insertVertexString(
				"INSERT INTO TrackedMembranePoint(idProject,t,idTissue,idTrackedMembranePoint,posX,posY,posZ) VALUES (?,?,?,?,?,?,?)");

	QSqlQuery insertVertex_stmt(QString::fromStdString(insertVertexString),m_DB);

	BGL_FORALL_VERTICES(v,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){

		insertVertex_stmt.bindValue(0,QVariant(m_ProjectID));
		insertVertex_stmt.bindValue(1,QVariant(frame));
		insertVertex_stmt.bindValue(2,QVariant(0));
		insertVertex_stmt.bindValue(3,QVariant((unsigned int)v));
		insertVertex_stmt.bindValue(4,QVariant((double)boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[0]));
		insertVertex_stmt.bindValue(5,QVariant((double)boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[1]));
		insertVertex_stmt.bindValue(6,QVariant((double)boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[2]));
		insertVertex_stmt.exec();
	}

		std::string deleteEdgeString(
				"DELETE from TrackedMembranePoint_linkedTo_TrackedMembranePoint WHERE idProject=? AND t=?");

		QSqlQuery deleteEdge_stmt(QString::fromStdString(deleteEdgeString),m_DB);



		deleteEdge_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
		deleteEdge_stmt.bindValue(1, QVariant(frame)); //t==0
		deleteEdge_stmt.exec();


		std::string insertEdgeString(
				"INSERT INTO TrackedMembranePoint_linkedTo_TrackedMembranePoint(idProject,t,idTissue,idTrackedMembranePoint1,idTrackedMembranePoint2) VALUES (?,?,?,?,?)");

		QSqlQuery insertEdge_stmt(QString::fromStdString(insertEdgeString),m_DB);


		BGL_FORALL_EDGES(e,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){

		insertEdge_stmt.bindValue(0,QVariant(m_ProjectID));
		insertEdge_stmt.bindValue(1,QVariant(frame));
		insertEdge_stmt.bindValue(2,QVariant(0));

		ttt::SkeletonVertexType source =std::min(boost::source(e,*descriptor->m_SkeletonGraph),boost::target(e,*descriptor->m_SkeletonGraph));
		ttt::SkeletonVertexType target =std::max(boost::source(e,*descriptor->m_SkeletonGraph),boost::target(e,*descriptor->m_SkeletonGraph));

		insertEdge_stmt.bindValue(3,QVariant((unsigned int) source));
		insertEdge_stmt.bindValue(4,QVariant((unsigned int) target));

		insertEdge_stmt.exec();
		}

		std::string deleteCellString("DELETE from TrackedCell WHERE idProject=? AND t=?");
		QSqlQuery deleteCell_stmt(QString::fromStdString(deleteCellString),m_DB);

		deleteCell_stmt.bindValue(0, m_ProjectID); //IDproject==2
		deleteCell_stmt.bindValue(1, frame); //t==0
		deleteCell_stmt.exec();

		std::string insertCellString(
				"INSERT INTO TrackedCell(idProject,t,idTissue,idTrackedCell,idCell,trackID,posX,posY,posZ,velX,velY,velZ,parentID) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

		QSqlQuery insertCell_stmt(QString::fromStdString(insertCellString),m_DB);

		BGL_FORALL_VERTICES(v,*descriptor->m_CellGraph,ttt::TrackedCellGraph){
		insertCell_stmt.bindValue(0,m_ProjectID);
		insertCell_stmt.bindValue(1,frame);
		insertCell_stmt.bindValue(2,0);

		insertCell_stmt.bindValue(3,(unsigned int)v);

		insertCell_stmt.bindValue(4,(unsigned int)boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetObservedCell());
		insertCell_stmt.bindValue(5,(unsigned int)boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetID());

		insertCell_stmt.bindValue(6,(double) boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetCentroid()[0]);
		insertCell_stmt.bindValue(7,(double)boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetCentroid()[1]);
		insertCell_stmt.bindValue(8,(double)boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetCentroid()[2]);

		insertCell_stmt.bindValue(9,(double)boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetVelocity()[0]);
		insertCell_stmt.bindValue(10,(double)boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetVelocity()[1]);
		insertCell_stmt.bindValue(11,(double)boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetVelocity()[2]);
		insertCell_stmt.bindValue(12,(int)boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).GetParentID());

		insertCell_stmt.exec();
	}

		std::string insertCellEdgeString(
				"INSERT INTO TrackedCell_neighbor_of_TrackedCell(idProject,t,idTissue,idTrackedCell1,idTrackedCell2) VALUES (?,?,?,?,?)");

		QSqlQuery insertCellEdge_stmt(QString::fromStdString(insertCellEdgeString),m_DB);


		BGL_FORALL_EDGES(e,*descriptor->m_CellGraph,ttt::TrackedCellGraph){
			insertCellEdge_stmt.bindValue(0,m_ProjectID);
			insertCellEdge_stmt.bindValue(1,frame);
			insertCellEdge_stmt.bindValue(2,0);

			ttt::CellVertexType source =std::min(boost::source(e,*descriptor->m_CellGraph),boost::target(e,*descriptor->m_CellGraph));
			ttt::CellVertexType target =std::max(boost::source(e,*descriptor->m_CellGraph),boost::target(e,*descriptor->m_CellGraph));

			insertCellEdge_stmt.bindValue(3,(unsigned int)source);
			insertCellEdge_stmt.bindValue(4,(unsigned int)target);
			insertCellEdge_stmt.exec();
	}


		std::string insertTrackedCellToMembranePointString(
				"INSERT INTO TrackedCell_has_TrackedMembranePoint(idProject,t,idTissue,idTrackedCell,idTrackedMembranePoint,`order`) VALUES (?,?,?,?,?,?)");

		QSqlQuery  insertTrackedCellToTrackedMembranePointString_stmt(QString::fromStdString(insertTrackedCellToMembranePointString),m_DB);

		BGL_FORALL_VERTICES(v,*descriptor->m_CellGraph,ttt::TrackedCellGraph){
		int order=0;
		for(ttt::Cell::PerimeterIterator it=boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).PerimeterBegin();
				it!=boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v).PerimeterEnd();
				it++) {

			insertTrackedCellToTrackedMembranePointString_stmt.bindValue(0,m_ProjectID);
			insertTrackedCellToTrackedMembranePointString_stmt.bindValue(1,frame);
			insertTrackedCellToTrackedMembranePointString_stmt.bindValue(2,0);
			insertTrackedCellToTrackedMembranePointString_stmt.bindValue(3,(unsigned int)v);
			insertTrackedCellToTrackedMembranePointString_stmt.bindValue(4,(unsigned int)*it);
			insertTrackedCellToTrackedMembranePointString_stmt.bindValue(5,order);
			insertTrackedCellToTrackedMembranePointString_stmt.exec();
			order++;
		}
	}


	transStatement.exec("COMMIT;");


}

bool QTSQLTissueTrackingProject2::IsTrackedTissueDescriptorAvailable(
		unsigned int frame) {
	return true;
}

void QTSQLTissueTrackingProject2::LoadProjectInfo() {


	QSqlQuery prep_stmt(QString("select spacingX,spacingY,spacingZ,sizeX,sizeY,sizeZ,samplingPeriod,workingDirectory,name from Project WHERE Project.idProject=?"),m_DB);

	prep_stmt.bindValue(0, QVariant(m_ProjectID));
	prep_stmt.exec();

	prep_stmt.next();

	m_Spacing[0] = prep_stmt.value("spacingX").toDouble();
	m_Spacing[1] = prep_stmt.value("spacingY").toDouble();
	m_Spacing[2] = prep_stmt.value("spacingZ").toDouble();

	m_ProjectDirectory = prep_stmt.value("workingDirectory").toString().toStdString();
	m_ProjectName = prep_stmt.value("name").toString().toStdString();

	m_Size[0]=prep_stmt.value("sizeX").toInt();
	m_Size[1]=prep_stmt.value("sizeY").toInt();
	m_Size[2]=prep_stmt.value("sizeZ").toInt();

	m_SamplingPeriod = prep_stmt.value("samplingPeriod").toDouble();

	QSqlQuery framenum_stmt("SELECT * FROM Frame WHERE Frame.idProject=?",m_DB);

	framenum_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
	framenum_stmt.exec();
	m_NumFrames=framenum_stmt.size();

}
void QTSQLTissueTrackingProject2::StoreProjectInfo() {

}
void QTSQLTissueTrackingProject2::StoreFrameInfo(unsigned int frame) {

}
void QTSQLTissueTrackingProject2::LoadFrameInfo(unsigned int frame) {
	std::string query(
				"SELECT platenessSteps,platenessHighestScale,platenessLowestScale,vertexnessSteps,vertexnessLowestScale,vertexnessHighestScale FROM Frame WHERE Frame.idProject=? AND Frame.t=?");
	QSqlQuery prep_stmt(QString::fromStdString(query),m_DB);

	prep_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
	prep_stmt.bindValue(1, QVariant( frame)); //IDproject==2
	prep_stmt.exec();


	prep_stmt.next();
	this->m_PlatenessSteps[frame]=prep_stmt.value("platenessSteps").toInt();
	this->m_HighestPlatenessScale[frame]=prep_stmt.value("platenessHighestScale").toDouble();
	this->m_LowestPlatenessScale[frame] = prep_stmt.value("platenessLowestScale").toDouble();

	this->m_VertexnessSteps[frame]=prep_stmt.value("vertexnessSteps").toInt();
	this->m_LowestVertexnessScale[frame]=prep_stmt.value("vertexnessLowestScale").toDouble();
	this->m_HighestVertexnessScale[frame]=prep_stmt.value("vertexnessHighestScale").toDouble();
}

bool QTSQLTissueTrackingProject2::IsImageAvailable(const std::string & name,
		int frame) {

	std::stringstream ss;

	ss << "SELECT * FROM " << name << " WHERE " << name << ".idProject=? AND " << name << ".t=?";

	QSqlQuery prep_stmt(QString::fromStdString(ss.str()),m_DB);

	prep_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
	prep_stmt.bindValue(1, QVariant(frame)); //IDproject==2
	prep_stmt.exec();

	return prep_stmt.size() >= 1;

}

typename itk::Image<float, 3>::Pointer QTSQLTissueTrackingProject2::LoadImage(const std::string & table, int frame) {

	typedef itk::ImageFileReader<itk::Image<float, 3> > ReaderType;
	typename ReaderType::Pointer reader = ReaderType::New();

	std::stringstream ssQuery, ssFileName;


	ssQuery << "SELECT fileName FROM " << table << " WHERE " << table << ".idProject=? AND " << table << ".t=?";

	QSqlQuery prep_stmt(QString::fromStdString(ssQuery.str()), m_DB);

	prep_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
	prep_stmt.bindValue(1, QVariant(frame)); //IDproject==2
	prep_stmt.exec();

	prep_stmt.next();
	std::string fileName = prep_stmt.value("fileName").toString().toStdString();

	ssFileName << m_ProjectDirectory << "/" << fileName;

	reader->SetFileName(ssFileName.str());
	reader->Update();
	reader->GetOutput()->SetSpacing(m_Spacing);
	reader->GetOutput()->Print(std::cout,itk::Indent());
	return reader->GetOutput();
}

void QTSQLTissueTrackingProject2::StoreImage(const std::string & name,
		int frame, const typename itk::Image<float, 3>::Pointer & image) {

	std::stringstream ssQuery, ssFileName, ssPath;

	typedef itk::ImageFileWriter<itk::Image<float, 3> > WriterType;

	typename WriterType::Pointer writer = WriterType::New();

	ssFileName << name << "-" << frame << ".ome.tif";
	ssPath << this->m_ProjectDirectory << "/" << ssFileName.str();
	writer->SetFileName(ssPath.str());
	writer->SetInput(image);
	writer->Update();



	ssQuery << "INSERT into " << name
			<< " (idProject, t, fileName) values (?, ?, ?) on duplicate key UPDATE fileName=VALUES(fileName)";

	QSqlQuery prep_stmt(QString::fromStdString(ssQuery.str()),m_DB);

	prep_stmt.bindValue(0, QVariant(m_ProjectID)); //IDproject==2
	prep_stmt.bindValue(1, QVariant(frame)); //t==0
	prep_stmt.bindValue(2, QVariant(QString::fromStdString(ssFileName.str())));

	prep_stmt.exec();
}

FeatureMap<CellVertexType,itk::Point<double,3> > QTSQLTissueTrackingProject2::GetCentroids(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetCentroids(unsigned int frame,const FeatureMap<CellVertexType,itk::Point<double,3> >  & centroids){

}

FeatureMap<CellVertexType,double> QTSQLTissueTrackingProject2::GetXX(unsigned int frame){

}
void QTSQLTissueTrackingProject2::SetXX(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & xx){

}
FeatureMap<CellVertexType,double> QTSQLTissueTrackingProject2::GetXY(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetXY(unsigned int frame,const FeatureMap<TrackedCellVertexType,double> &xy){

}

FeatureMap<CellVertexType,double> QTSQLTissueTrackingProject2::GetYY(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetYY(unsigned int frame, const FeatureMap<CellVertexType,double> & yy){

}

FeatureMap<CellVertexType,double> QTSQLTissueTrackingProject2::GetPerimeter(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetPerimeter(unsigned int frame, const FeatureMap<CellVertexType,double> & perimeters){

}

FeatureMap<CellVertexType,double> QTSQLTissueTrackingProject2::GetAreas(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetAreas(unsigned int frame, const FeatureMap<CellVertexType,double> & areas){

}





FeatureMap<TrackedCellVertexType,itk::Point<double,3> > QTSQLTissueTrackingProject2::GetTrackedCentroids(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetTrackedCentroids(unsigned int frame,const FeatureMap<TrackedCellVertexType,itk::Point<double,3> >  & centroids){

}

FeatureMap<TrackedCellVertexType,double> QTSQLTissueTrackingProject2::GetTrackedXX(unsigned int frame){

}
void QTSQLTissueTrackingProject2::SetTrackedXX(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & xx){

}
FeatureMap<TrackedCellVertexType,double> QTSQLTissueTrackingProject2::GetTrackedXY(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetTrackedXY(unsigned int frame,const FeatureMap<TrackedCellVertexType,double> &xy){

}

FeatureMap<TrackedCellVertexType,double> QTSQLTissueTrackingProject2::GetTrackedYY(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetTrackedYY(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & yy){

}

FeatureMap<TrackedCellVertexType,double> QTSQLTissueTrackingProject2::GetTrackedPerimeter(unsigned int frame){

}


void QTSQLTissueTrackingProject2::SetTrackedPerimeter(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & perimeters){

}

FeatureMap<TrackedCellVertexType,double> QTSQLTissueTrackingProject2::GetTrackedAreas(unsigned int frame){

}

void QTSQLTissueTrackingProject2::SetTrackedAreas(unsigned int frame, const FeatureMap<TrackedCellVertexType,double> & areas){

}



}
