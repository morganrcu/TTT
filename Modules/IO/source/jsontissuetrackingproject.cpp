/*
 * jsontissuetrackingproject.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: morgan
 */




#include "jsontissuetrackingproject.h"
#include "itkImageFileReader.h"


#include <json/json.h>
#include <fstream>
void ttt::JSONTissueTrackingProject::LoadProjectInfo(){
	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectPath << "/" << "project.json";
	std::string projectConfigFile;
	fileNameStream >> projectConfigFile;


	reader.parse(projectConfigFile,root);

	m_ProjectName= root["name"].asString();
	m_Spacing[0] =root["spacing"]["x"].asDouble();
	m_Spacing[1] =root["spacing"]["y"].asDouble();
	m_Spacing[2] =root["spacing"]["z"].asDouble();
	m_SamplingPeriod = root["samplingPeriod"].asDouble();


}

void ttt::JSONTissueTrackingProject::StoreProjectInfo(){

	Json::Value root;
	Json::StyledWriter writer;

	root["name"]=m_ProjectName;
	root["spacing"]["x"]=m_Spacing[0];
	root["spacing"]["y"]=m_Spacing[1];
	root["spacing"]["z"]=m_Spacing[2];
	root["samplingPeriod"]=m_SamplingPeriod;

	std::string jsoncontent=writer.write(root);


	std::stringstream fileNameStream;
	fileNameStream << m_ProjectPath << "/" << "project.json";
	std::string projectConfigFile;
	fileNameStream >> projectConfigFile;

	std::ofstream file (projectConfigFile.c_str(), std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();

}
void ttt::JSONTissueTrackingProject::StoreFrameInfo(){


	Json::Value root;
	Json::StyledWriter writer;

	root["platenessSteps"]=this->m_PlatenessSteps;
	root["platenessHighestScale"]=this->m_HighestPlatenessScale;
	root["platenessLowestScale"]=this->m_LowestPlatenessScale;

	root["vertexnessSteps"]=this->m_VertexnessSteps;
	root["vertexnessHighestScale"]=this->m_HighestVertexnessScale;
	root["vertexnessLowestScale"]=this->m_LowestVertexnessScale;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectPath << "/" << "frame-"<< m_Frame << ".json";
	std::string projectConfigFile;
	fileNameStream >> projectConfigFile;

	std::string jsoncontent=writer.write(root);
	std::ofstream file (projectConfigFile.c_str(), std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();

}

void ttt::JSONTissueTrackingProject::LoadFrameInfo(){

}

void ttt::JSONTissueTrackingProject::LoadRawImage(){

}

void ttt::JSONTissueTrackingProject::LoadDiffusedImage(){

}

void ttt::JSONTissueTrackingProject::LoadPlatenessImage(){

}

void ttt::JSONTissueTrackingProject::LoadVertexnessImage(){

}

void ttt::JSONTissueTrackingProject::StoreRawImage(){

}

void ttt::JSONTissueTrackingProject::StoreDiffusedImage(){

}

void ttt::JSONTissueTrackingProject::StorePlatenessImage(){

}

void ttt::JSONTissueTrackingProject::StoreVertexnessImage(){

}

void ttt::JSONTissueTrackingProject::NewFrame(int numFrame){

}


bool ttt::JSONTissueTrackingProject::IsRawImageReady(){

}

bool ttt::JSONTissueTrackingProject::IsDiffusedImageReady(){

}
bool ttt::JSONTissueTrackingProject::IsPlatenessImageReady(){

}
bool ttt::JSONTissueTrackingProject::IsVertexnessImageReady(){

}
bool ttt::JSONTissueTrackingProject::IsVertexLocationsReady(){

}
void ttt::JSONTissueTrackingProject::StoreVertexLocations(){

}
void ttt::JSONTissueTrackingProject::LoadVertexLocations(){

}
void ttt::JSONTissueTrackingProject::LoadTissueDescriptor(){

}
void ttt::JSONTissueTrackingProject::StoreTissueDescriptor(){

}
bool ttt::JSONTissueTrackingProject::IsTissueDescriptorReady(){

}
void ttt::JSONTissueTrackingProject::StoreTrackedTissueDescriptor(){

}
void ttt::JSONTissueTrackingProject::LoadTrackedTissueDescriptor(){

}
bool ttt::JSONTissueTrackingProject::IsTrackedTissueDescriptorReady(){

}

void ttt::JSONTissueTrackingProject::StoreTrackedCentroids(){

}
void ttt::JSONTissueTrackingProject::LoadTrackedCentroids(){

}
bool ttt::JSONTissueTrackingProject::IsTrackedCentroidsReady(){

}

void ttt::JSONTissueTrackingProject::LoadTrackedEllipses(){

}
void ttt::JSONTissueTrackingProject::StoreTrackedEllipses(){

}
bool ttt::JSONTissueTrackingProject::IsTrackedEllipsesReady(){

}

void ttt::JSONTissueTrackingProject::LoadTrackedDomains(){

}
void ttt::JSONTissueTrackingProject::StoreTrackedDomains(){

}

bool ttt::JSONTissueTrackingProject::IsTrackedDomainsReady(){

}
void ttt::JSONTissueTrackingProject::LoadDomainStrainRates(){

}
void ttt::JSONTissueTrackingProject::StoreDomainStrainRates(){

}
bool ttt::JSONTissueTrackingProject::IsDomainStrainRatesReady(){

}
#if 0

template<class TImage> typename TImage::Pointer ttt::JSONTissueTrackingProject::readImage(const std::string & table, int frame) {
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

template<class TImage> void ttt::JSONTissueTrackingProject::storeImage(const typename TImage::Pointer & image, const std::string & table,unsigned int frame) {

		std::stringstream ssQuery, ssFileName, ssPath;

		typedef itk::ImageFileWriter<TImage> WriterType;

		typename WriterType::Pointer writer = WriterType::New();

		ssFileName << table << "-" << frame << ".tif";
		ssPath << this->m_ProjectPath << "/" << ssFileName.str();
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
#endif
