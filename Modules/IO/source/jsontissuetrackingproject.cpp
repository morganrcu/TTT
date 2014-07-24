/*
 * jsontissuetrackingproject.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: morgan
 */




#include "jsontissuetrackingproject.h"



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
	m_Spacing[0] =root["spacingX"].asDouble();
	m_Spacing[1] =root["spacingY"].asDouble();
	m_Spacing[2] =root["spacingZ"].asDouble();
	m_SamplingPeriod = root["samplingPeriod"].asDouble();


}
void ttt::JSONTissueTrackingProject::StoreProjectInfo(){

	Json::Value root;
	Json::StyledWriter writer;

	root["name"]=m_ProjectName;
	root["spacingX"]=m_Spacing[0];
	root["spacingY"]=m_Spacing[1];
	root["spacingZ"]=m_Spacing[2];
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
