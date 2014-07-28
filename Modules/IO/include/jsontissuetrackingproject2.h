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
class JSONTissueTrackingProject2 : public TissueTrackingAbstractProject2{

private:
	std::string m_ProjectDirectory;
public:
	inline void SetDirectory(std::string & projectDirectory){
			m_ProjectDirectory=projectDirectory;
		}
	virtual void Open(){
		TissueTrackingAbstractProject2::Open();
	}

	virtual void Close(){
		TissueTrackingAbstractProject2::Close();
	}
	virtual void Flush(){
		TissueTrackingAbstractProject2::Flush();
	}
protected:
	virtual void LoadProjectInfo();
	virtual void StoreProjectInfo();
	virtual void StoreFrameInfo(unsigned int frame);
	virtual void LoadFrameInfo(unsigned int frame);
	virtual void StoreObject(const std::string & name, int frame, const typename itk::Image<float,3>::Pointer & object);
	virtual void StoreObject(const std::string & name, int frame, const typename ttt::AdherensJunctionVertices::Pointer & object);
	virtual void StoreObject(const std::string & name, int frame, const typename ttt::TissueDescriptor::Pointer & object);
	virtual void StoreObject(const std::string & name, int frame, const typename ttt::TrackedTissueDescriptor::Pointer & object);

	virtual void LoadObject(const std::string & name, int frame, typename itk::Image<float,3>::Pointer & object);
	virtual void LoadObject(const std::string & name, int frame,const typename ttt::AdherensJunctionVertices::Pointer & object);
#if 0
	virtual bool ObjectAvailable(const std::string & name, unsigned int frame){

	}




protected:
	virtual void StoreObject(const std::string & name, int frame, const typename itk::Image<float,3>::Pointer & object);
virtual void StoreObject(const std::string & name, int frame, const typename ttt::TissueDescriptor::Pointer & object);
		virtual void StoreObject(const std::string & name, int frame, const typename ttt::TrackedTissueDescriptor::Pointer & object);
		virtual void StoreObject(const std::string & name, int frame,ttt::AdherensJunctionVertices::Pointer & object);

		virtual void LoadObject(const std::string & name, int frame,itk::Image<float,3>::Pointer & object);
		virtual void LoadObject(const std::string & name, int frame,ttt::TissueDescriptor::Pointer & object);
		virtual void LoadObject(const std::string & name, int frame,ttt::TrackedTissueDescriptor::Pointer & object);
		virtual void LoadObject(const std::string & name, int frame,ttt::AdherensJunctionVertices::Pointer & object);
#endif


};
void ttt::JSONTissueTrackingProject2::StoreObject(const std::string & name, int frame, const typename ttt::AdherensJunctionVertices::Pointer & object){
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "vertices" << "_T" << frame <<".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;

	for(unsigned int i=0;i<object->size();i++){
		itk::Index<3> position=(*object)[i]->GetPosition();
		root["Vertices"][i]["x"]=(Json::UInt64)position[0];
		root["Vertices"][i]["y"]=(Json::UInt64)position[1];
		root["Vertices"][i]["z"]=(Json::UInt64)position[2];
	}

	std::string jsoncontent=writer.write(root);
	std::ofstream file (vertexStorageFile, std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}
void ttt::JSONTissueTrackingProject2::LoadObject(const std::string & name, int frame,const typename ttt::AdherensJunctionVertices::Pointer & object){

}

void ttt::JSONTissueTrackingProject2::StoreObject(const std::string & name, int frame, const typename ttt::TissueDescriptor::Pointer & object){
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "tissue" << "_T" << frame <<".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	unsigned int k=0;

	BGL_FORALL_VERTICES_T(v,*object->m_SkeletonGraph,ttt::SkeletonGraph){
		root["primal"]["vertices"][k]["id"]=(Json::UInt64) v;
		root["primal"]["vertices"][k]["x"]=boost::get(ttt::SkeletonPointPropertyTag(),*object->m_SkeletonGraph,v).position[0];
		root["primal"]["vertices"][k]["y"]=boost::get(ttt::SkeletonPointPropertyTag(),*object->m_SkeletonGraph,v).position[1];
		root["primal"]["vertices"][k]["z"]=boost::get(ttt::SkeletonPointPropertyTag(),*object->m_SkeletonGraph,v).position[2];
		k++;
	}
	k=0;
	BGL_FORALL_EDGES_T(e,*object->m_SkeletonGraph,ttt::SkeletonGraph){
		root["primal"]["edges"][k]["src"]=(Json::UInt64)boost::source(e,*object->m_SkeletonGraph);
		root["primal"]["edges"][k]["tgt"]=(Json::UInt64)boost::target(e,*object->m_SkeletonGraph);
		k++;
	}
	k=0;
	BGL_FORALL_VERTICES_T(v,*object->m_CellGraph,ttt::CellGraph){
		ttt::Cell currentCell=boost::get(ttt::CellPropertyTag(),*object->m_CellGraph,v);
		root["dual"]["vertices"][k]["id"]=(Json::UInt64) v;
		root["dual"]["vertices"][k]["x"]=currentCell.GetCentroid()[0];
		root["dual"]["vertices"][k]["y"]=currentCell.GetCentroid()[1];
		root["dual"]["vertices"][k]["z"]=currentCell.GetCentroid()[2];
		unsigned int l=0;
		for(ttt::Cell::PerimeterIterator perimeterIt=currentCell.PerimeterBegin();perimeterIt!=currentCell.PerimeterEnd();++perimeterIt){
			root["dual"]["vertices"][k]["perimeter"][l]=(Json::UInt64)*perimeterIt;
			l++;
		}
		k++;
	}
	k=0;
	BGL_FORALL_EDGES_T(e,*object->m_CellGraph,ttt::CellGraph){
		root["dual"]["edges"][k]["src"]=(Json::UInt64)boost::source(e,*object->m_CellGraph);
		root["dual"]["edges"][k]["tgt"]=(Json::UInt64)boost::target(e,*object->m_CellGraph);
		k++;
	}


	std::string jsoncontent=writer.write(root);
	std::ofstream file (vertexStorageFile, std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}
void ttt::JSONTissueTrackingProject2::StoreObject(const std::string & name, int frame, const typename ttt::TrackedTissueDescriptor::Pointer & object){
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedtissue" << "_T" << frame <<".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	unsigned int k=0;

	BGL_FORALL_VERTICES_T(v,*object->m_SkeletonGraph,ttt::SkeletonGraph){
		root["primal"]["vertices"][k]["id"]=(Json::UInt64) v;
		root["primal"]["vertices"][k]["x"]=boost::get(ttt::SkeletonPointPropertyTag(),*object->m_SkeletonGraph,v).position[0];
		root["primal"]["vertices"][k]["y"]=boost::get(ttt::SkeletonPointPropertyTag(),*object->m_SkeletonGraph,v).position[1];
		root["primal"]["vertices"][k]["z"]=boost::get(ttt::SkeletonPointPropertyTag(),*object->m_SkeletonGraph,v).position[2];
		k++;
	}
	k=0;
	BGL_FORALL_EDGES_T(e,*object->m_SkeletonGraph,ttt::SkeletonGraph){
		root["primal"]["edges"][k]["src"]=(Json::UInt64)boost::source(e,*object->m_SkeletonGraph);
		root["primal"]["edges"][k]["tgt"]=(Json::UInt64)boost::target(e,*object->m_SkeletonGraph);
		k++;
	}
	k=0;
	BGL_FORALL_VERTICES_T(v,*object->m_CellGraph,ttt::TrackedCellGraph){
		ttt::TrackedCell currentCell=boost::get(ttt::TrackedCellPropertyTag(),*object->m_CellGraph,v);
		root["dual"]["vertices"][k]["id"]=(Json::UInt64) v;
		root["dual"]["vertices"][k]["x"]=currentCell.GetCentroid()[0];
		root["dual"]["vertices"][k]["y"]=currentCell.GetCentroid()[1];
		root["dual"]["vertices"][k]["z"]=currentCell.GetCentroid()[2];
		root["dual"]["vertices"][k]["trackID"]=currentCell.GetID();
		root["dual"]["vertices"][k]["observed"]=currentCell.GetObservedCell();
		root["dual"]["vertices"][k]["dx"]=currentCell.GetVelocity()[0];
		root["dual"]["vertices"][k]["dy"]=currentCell.GetVelocity()[1];
		root["dual"]["vertices"][k]["dz"]=currentCell.GetVelocity()[2];

		unsigned int l=0;
		for(ttt::Cell::PerimeterIterator perimeterIt=currentCell.PerimeterBegin();perimeterIt!=currentCell.PerimeterEnd();++perimeterIt){
			root["dual"]["vertices"][k]["perimeter"][l]=(Json::UInt64)*perimeterIt;
			l++;
		}
		k++;
	}
	k=0;
	BGL_FORALL_EDGES_T(e,*object->m_CellGraph,ttt::TrackedCellGraph){
		root["dual"]["edges"][k]["src"]=(Json::UInt64)boost::source(e,*object->m_CellGraph);
		root["dual"]["edges"][k]["tgt"]=(Json::UInt64)boost::target(e,*object->m_CellGraph);
		k++;
	}


	std::string jsoncontent=writer.write(root);
	std::ofstream file (vertexStorageFile, std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;

	file.close();

}

void ttt::JSONTissueTrackingProject2::LoadProjectInfo(){
	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "project.json";
	std::string projectConfigFile;
	fileNameStream >> projectConfigFile;

	reader.parse(projectConfigFile,root);

	m_ProjectName= root["name"].asString();
	m_Spacing[0] =root["spacing"]["x"].asDouble();
	m_Spacing[1] =root["spacing"]["y"].asDouble();
	m_Spacing[2] =root["spacing"]["z"].asDouble();
	m_SamplingPeriod = root["samplingPeriod"].asDouble();

	m_NumFrames=root["numFrames"].asInt();
}

void ttt::JSONTissueTrackingProject2::StoreProjectInfo(){

	Json::Value root;
	Json::StyledWriter writer;

	root["name"]=m_ProjectName;
	root["spacing"]["x"]=m_Spacing[0];
	root["spacing"]["y"]=m_Spacing[1];
	root["spacing"]["z"]=m_Spacing[2];
	root["samplingPeriod"]=m_SamplingPeriod;
	root["numFrames"]=m_NumFrames;


	std::string jsoncontent=writer.write(root);


	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "project.json";
	std::string projectConfigFile;
	fileNameStream >> projectConfigFile;

	std::ofstream file (projectConfigFile.c_str(), std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}

void ttt::JSONTissueTrackingProject2::StoreFrameInfo(unsigned int frame){


	Json::Value root;
	Json::StyledWriter writer;

	root["platenessSteps"]=this->m_PlatenessSteps[frame];
	root["platenessHighestScale"]=this->m_HighestPlatenessScale[frame];
	root["platenessLowestScale"]=this->m_LowestPlatenessScale[frame];

	root["vertexnessSteps"]=this->m_VertexnessSteps[frame];
	root["vertexnessHighestScale"]=this->m_HighestVertexnessScale[frame];
	root["vertexnessLowestScale"]=this->m_LowestVertexnessScale[frame];

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "frame-"<< frame << ".json";
	std::string frameMetadataFile;
	fileNameStream >> frameMetadataFile;

	std::string jsoncontent=writer.write(root);
	std::ofstream file (frameMetadataFile.c_str(), std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();

}

void ttt::JSONTissueTrackingProject2::LoadFrameInfo(unsigned int frame){

	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "frame-"<< frame << ".json";
	std::string frameMetadataFile;
	fileNameStream >> frameMetadataFile;


	reader.parse(frameMetadataFile,root);


	this->m_PlatenessSteps[frame]=root["platenessSteps"].asInt();
	this->m_HighestPlatenessScale[frame]=root["platenessHighestScale"].asDouble();
	this->m_LowestPlatenessScale[frame]=root["platenessLowestScale"].asDouble();

	this->m_VertexnessSteps[frame]=root["vertexnessSteps"].asInt();
	this->m_HighestVertexnessScale[frame]=root["vertexnessHighestScale"].asDouble();
	this->m_LowestVertexnessScale[frame]=root["vertexnessLowestScale"].asDouble();

}

void ttt::JSONTissueTrackingProject2::LoadObject(const std::string & name, int frame, typename itk::Image<float,3>::Pointer & object){
	typedef itk::ImageFileReader<itk::Image<float,3> > ReaderType;
	typename ReaderType::Pointer reader = ReaderType::New();

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << name << "-T"<<frame << ".ome.tif";
	std::string frameImageFile;
	fileNameStream >> frameImageFile;
	reader->SetFileName(frameImageFile);
	reader->Update();
	reader->GetOutput()->SetSpacing(m_Spacing);

	//object->CopyInformation(reader->GetOutput());
	object=reader->GetOutput();


}
void ttt::JSONTissueTrackingProject2::StoreObject(const std::string & name, int frame, const typename itk::Image<float,3>::Pointer & object){
	std::cout << "Store-> " << name <<std::endl;

	typedef itk::ImageFileWriter<itk::Image<float,3> > WriterType;

	typename WriterType::Pointer writer = WriterType::New();

	typedef itk::SCIFIOImageIO ImageIO;
	ImageIO::Pointer io= ImageIO::New();
	writer->SetImageIO(io);
	io->SetSeries(true);
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << name << "-T"<<frame << ".ome.tif";
	std::string frameImageFile;
	fileNameStream >> frameImageFile;

	writer->SetFileName(frameImageFile);
	writer->SetInput(object);
	writer->Update();
}
}




#endif /* JSONTISSUETRACKINGPROJECT_H_ */
