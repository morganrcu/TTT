/*
 * jsontissuetrackingproject2.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: morgan
 */

#include "jsontissuetrackingproject2.h"
namespace ttt{

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


typename itk::Image<float,3>::Pointer ttt::JSONTissueTrackingProject2::LoadImage(const std::string & name, int frame){
	typedef itk::ImageFileReader<itk::Image<float,3> > ReaderType;
	typename ReaderType::Pointer reader = ReaderType::New();

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << name << "-T"<<frame << ".ome.tif";
	std::string frameImageFile;
	fileNameStream >> frameImageFile;
	reader->SetFileName(frameImageFile);
	reader->Update();
	reader->GetOutput()->SetSpacing(m_Spacing);

	return reader->GetOutput();

}
void ttt::JSONTissueTrackingProject2::StoreImage(const std::string & name, int frame, const typename itk::Image<float,3>::Pointer & object){
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


void ttt::JSONTissueTrackingProject2::SetRawImage(unsigned int frame,const RawImageType::Pointer & rawImage){
	this->StoreImage("RawImage",frame,rawImage);
}

typename JSONTissueTrackingProject2::RawImageType::Pointer ttt::JSONTissueTrackingProject2::GetRawImage(unsigned int frame){
	return this->LoadImage("RawImage",frame);
}

bool ttt::JSONTissueTrackingProject2::IsRawImageAvailable(unsigned int frame){
	return this->IsImage("RawImage",frame);
}

void ttt::JSONTissueTrackingProject2::SetDiffusedImage(unsigned int frame, const DiffusedImageType::Pointer & diffusedImage){
	this->StoreImage("DiffusedImage",frame,diffusedImage);
}

typename JSONTissueTrackingProject2::DiffusedImageType::Pointer ttt::JSONTissueTrackingProject2::GetDiffusedImage(unsigned int frame){
	return this->LoadImage("DiffusedImage",frame);
}
bool ttt::JSONTissueTrackingProject2::IsDiffusedImageAvailable(unsigned int frame){
	return this->IsImage("DiffusedImage",frame);
}


void ttt::JSONTissueTrackingProject2::SetPlatenessImage(unsigned int frame, const PlatenessImageType::Pointer & platenessImage){
	this->StoreImage("PlatenessImage",frame,platenessImage);
}
typename JSONTissueTrackingProject2::PlatenessImageType::Pointer ttt::JSONTissueTrackingProject2::GetPlatenessImage(unsigned int frame){
	return this->LoadImage("PlatenessImage",frame);
}
bool ttt::JSONTissueTrackingProject2::IsPlatenessImageAvailable(unsigned int frame){
	return this->IsImage("PlatenessImage",frame); //TODO
}

void ttt::JSONTissueTrackingProject2::SetVertexnessImage(unsigned int frame, const VertexnessImageType::Pointer & vertexnessImage){
	this->StoreImage("VertexnessImage",frame,vertexnessImage);
}
typename JSONTissueTrackingProject2::VertexnessImageType::Pointer ttt::JSONTissueTrackingProject2::GetVertexnessImage(unsigned int frame){
	return this->LoadImage("VertexnessImage",frame);
}
bool ttt::JSONTissueTrackingProject2::IsVertexnessImageAvailable(unsigned int frame){
	return this->IsImage("PlatenessImage",frame);
}

void ttt::JSONTissueTrackingProject2::SetAdherensJunctionVertices(unsigned int frame, const typename ttt::AdherensJunctionVertices::Pointer & vertices){
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "vertices" << "_T" << frame <<".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;

	for(unsigned int i=0;i<vertices->size();i++){
		itk::Index<3> position=(*vertices)[i]->GetPosition();
		root["Vertices"][i]["x"]=(Json::UInt64)position[0];
		root["Vertices"][i]["y"]=(Json::UInt64)position[1];
		root["Vertices"][i]["z"]=(Json::UInt64)position[2];
	}

	std::string jsoncontent=writer.write(root);
	std::ofstream file (vertexStorageFile, std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}

typename AdherensJunctionVertices::Pointer ttt::JSONTissueTrackingProject2::GetAdherensJunctionVertices(unsigned int frame){
	return 0;
}

bool ttt::JSONTissueTrackingProject2::IsAdherensJunctionVerticesAvailable(unsigned int frame){
	return false; //TODO
}

 typename  ttt::TissueDescriptor::Pointer ttt::JSONTissueTrackingProject2::GetTissueDescriptor(unsigned int frame){
	return 0; //TODO
}
 void ttt::JSONTissueTrackingProject2::SetTissueDescriptor(unsigned int frame,const typename ttt::TissueDescriptor::Pointer & descriptor){
	 Json::StyledWriter writer;
	 	Json::Value root;
	 	std::stringstream fileNameStream;
	 	fileNameStream << m_ProjectDirectory << "/" << "tissue" << "_T" << frame <<".json";

	 	std::string vertexStorageFile;
	 	fileNameStream >> vertexStorageFile;
	 	unsigned int k=0;

	 	BGL_FORALL_VERTICES_T(v,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
	 		root["primal"]["vertices"][k]["id"]=(Json::UInt64) v;
	 		root["primal"]["vertices"][k]["x"]=boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[0];
	 		root["primal"]["vertices"][k]["y"]=boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[1];
	 		root["primal"]["vertices"][k]["z"]=boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[2];
	 		k++;
	 	}
	 	k=0;
	 	BGL_FORALL_EDGES_T(e,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
	 		root["primal"]["edges"][k]["src"]=(Json::UInt64)boost::source(e,*descriptor->m_SkeletonGraph);
	 		root["primal"]["edges"][k]["tgt"]=(Json::UInt64)boost::target(e,*descriptor->m_SkeletonGraph);
	 		k++;
	 	}
	 	k=0;
	 	BGL_FORALL_VERTICES_T(v,*descriptor->m_CellGraph,ttt::CellGraph){
	 		ttt::Cell currentCell=boost::get(ttt::CellPropertyTag(),*descriptor->m_CellGraph,v);
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
	 	BGL_FORALL_EDGES_T(e,*descriptor->m_CellGraph,ttt::CellGraph){
	 		root["dual"]["edges"][k]["src"]=(Json::UInt64)boost::source(e,*descriptor->m_CellGraph);
	 		root["dual"]["edges"][k]["tgt"]=(Json::UInt64)boost::target(e,*descriptor->m_CellGraph);
	 		k++;
	 	}


	 	std::string jsoncontent=writer.write(root);
	 	std::ofstream file (vertexStorageFile, std::ofstream::out | std::ofstream::trunc);

	 	file << jsoncontent;

	 	file.close();
}
 bool ttt::JSONTissueTrackingProject2::IsTissueDescriptorAvailable(unsigned int frame){
	return false;//TODO
}

 typename  ttt::TrackedTissueDescriptor::Pointer ttt::JSONTissueTrackingProject2::GetTrackedTissueDescriptor(unsigned int frame){
	return 0;//TODO
}
 void ttt::JSONTissueTrackingProject2::SetTrackedTissueDescriptor(unsigned int frame,const typename ttt::TrackedTissueDescriptor::Pointer & descriptor){
	Json::StyledWriter writer;
		Json::Value root;
		std::stringstream fileNameStream;
		fileNameStream << m_ProjectDirectory << "/" << "trackedtissue" << "_T" << frame <<".json";

		std::string vertexStorageFile;
		fileNameStream >> vertexStorageFile;
		unsigned int k=0;

		BGL_FORALL_VERTICES_T(v,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
			root["primal"]["vertices"][k]["id"]=(Json::UInt64) v;
			root["primal"]["vertices"][k]["x"]=boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[0];
			root["primal"]["vertices"][k]["y"]=boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[1];
			root["primal"]["vertices"][k]["z"]=boost::get(ttt::SkeletonPointPropertyTag(),*descriptor->m_SkeletonGraph,v).position[2];
			k++;
		}
		k=0;
		BGL_FORALL_EDGES_T(e,*descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
			root["primal"]["edges"][k]["src"]=(Json::UInt64)boost::source(e,*descriptor->m_SkeletonGraph);
			root["primal"]["edges"][k]["tgt"]=(Json::UInt64)boost::target(e,*descriptor->m_SkeletonGraph);
			k++;
		}
		k=0;
		BGL_FORALL_VERTICES_T(v,*descriptor->m_CellGraph,ttt::TrackedCellGraph){
			ttt::TrackedCell currentCell=boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,v);
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
		BGL_FORALL_EDGES_T(e,*descriptor->m_CellGraph,ttt::TrackedCellGraph){
			root["dual"]["edges"][k]["src"]=(Json::UInt64)boost::source(e,*descriptor->m_CellGraph);
			root["dual"]["edges"][k]["tgt"]=(Json::UInt64)boost::target(e,*descriptor->m_CellGraph);
			k++;
		}


		std::string jsoncontent=writer.write(root);
		std::ofstream file (vertexStorageFile, std::ofstream::out | std::ofstream::trunc);
		file << jsoncontent;

		file.close();
}
 bool ttt::JSONTissueTrackingProject2::IsTrackedTissueDescriptorAvailable(unsigned int frame){
	return false;//TODO
}

 bool ttt::JSONTissueTrackingProject2::IsImage(const std::string & name, int frame){

	    std::stringstream fileNameStream;
	    fileNameStream << m_ProjectDirectory << "/" << name << "_T" << frame <<".ome.tif";

	    std::string fileName;
	    fileNameStream >> fileName;
	    std::ifstream infile(fileName);
	    return infile.good();
 }
}
