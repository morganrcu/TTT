/*
 * jsontissuetrackingproject2.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: morgan
 */

#include "jsontissuetrackingproject2.h"
#include <iostream>
#include <fstream>
#include <sstream>
namespace ttt {

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::LoadProjectInfo() {
	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "project.json";
	std::string projectConfigFile;
	fileNameStream >> projectConfigFile;

	std::cout << projectConfigFile << std::endl;

	std::ifstream jsonProjectConfigFile(projectConfigFile.c_str(),std::ifstream::in);
	reader.parse(jsonProjectConfigFile, root);

	this->m_ProjectName = root["name"].asString();
	std::cout << "ProjectName: " << this->m_ProjectName << std::endl;
	this->m_Spacing[0] = root["spacing"]["x"].asDouble();
	this->m_Spacing[1] = root["spacing"]["y"].asDouble();
	this->m_Spacing[2] = root["spacing"]["z"].asDouble();
	std::cout << "Spacing: " << this->m_Spacing << std::endl;
	this->m_SamplingPeriod = root["samplingPeriod"].asDouble();
	std::cout << "Sampling: " << this->m_SamplingPeriod << std::endl;

	this->m_NumFrames = root["numFrames"].asInt();
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::StoreProjectInfo() {

	Json::Value root;
	Json::StyledWriter writer;

	root["name"] = this->m_ProjectName;
	root["spacing"]["x"] = this->m_Spacing[0];
	root["spacing"]["y"] = this->m_Spacing[1];
	root["spacing"]["z"] = this->m_Spacing[2];
	root["samplingPeriod"] = this->m_SamplingPeriod;
	root["numFrames"] = this->m_NumFrames;

	std::string jsoncontent = writer.write(root);

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "project.json";
	std::string projectConfigFile;
	fileNameStream >> projectConfigFile;

	std::ofstream file(projectConfigFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::StoreFrameInfo(unsigned int frame) {

	Json::Value root;
	Json::StyledWriter writer;
	std::cout << "JSON StepsPlateness" << this->m_PlatenessSteps[frame]
			<< std::endl;
	root["platenessSteps"] = this->m_PlatenessSteps[frame];
	root["platenessHighestScale"] = this->m_HighestPlatenessScale[frame];
	root["platenessLowestScale"] = this->m_LowestPlatenessScale[frame];

	root["vertexnessSteps"] = this->m_VertexnessSteps[frame];
	root["vertexnessHighestScale"] = this->m_HighestVertexnessScale[frame];
	root["vertexnessLowestScale"] = this->m_LowestVertexnessScale[frame];

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "frame_T" << frame
			<< ".json";
	std::string frameMetadataFile;
	fileNameStream >> frameMetadataFile;

	std::string jsoncontent = writer.write(root);
	std::ofstream file(frameMetadataFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();

}
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::LoadFrameInfo(unsigned int frame) {

	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "frame_T" << frame
			<< ".json";

	std::string frameMetadataFile;
	fileNameStream >> frameMetadataFile;
	std::cout << "File: " << frameMetadataFile << std::endl;

	std::ifstream jsonFrameFile(frameMetadataFile.c_str());

	reader.parse(jsonFrameFile, root);

	this->m_PlatenessSteps[frame] = root["platenessSteps"].asInt();
	std::cout << "PlatenessSteps: " << this->m_PlatenessSteps[frame]
			<< std::endl;
	this->m_HighestPlatenessScale[frame] =
			root["platenessHighestScale"].asDouble();
	this->m_LowestPlatenessScale[frame] =
			root["platenessLowestScale"].asDouble();

	this->m_VertexnessSteps[frame] = root["vertexnessSteps"].asInt();
	this->m_HighestVertexnessScale[frame] =
			root["vertexnessHighestScale"].asDouble();
	this->m_LowestVertexnessScale[frame] =
			root["vertexnessLowestScale"].asDouble();

}

template<int dim> typename itk::Image<float, dim>::Pointer ttt::JSONTissueTrackingProject2<dim>::LoadImage(
		const std::string & name, int frame) {
	typedef itk::ImageFileReader<itk::Image<float, dim> > ReaderType;
	typename ReaderType::Pointer reader = ReaderType::New();

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << name << "_T" << frame
			<< ".ome.tif";
	std::string frameImageFile;
	fileNameStream >> frameImageFile;
	reader->SetFileName(frameImageFile);
	reader->Update();
	reader->GetOutput()->SetSpacing(this->m_Spacing);

	return reader->GetOutput();

}
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::StoreImage(const std::string & name,
		int frame, const typename itk::Image<float, dim>::Pointer & object) {
	std::cout << "Store-> " << name << std::endl;

	typedef itk::ImageFileWriter<itk::Image<float, dim> > WriterType;

	typename WriterType::Pointer writer = WriterType::New();

	typedef itk::SCIFIOImageIO ImageIO;
	ImageIO::Pointer io = ImageIO::New();
	writer->SetImageIO(io);
	io->SetSeries(true);
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << name << "_T" << frame
			<< ".ome.tif";
	std::string frameImageFile;
	fileNameStream >> frameImageFile;

	writer->SetFileName(frameImageFile);
	writer->SetInput(object);
	writer->Update();
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetRawImage(unsigned int frame,
		const typename RawImageType::Pointer & rawImage) {
	this->StoreImage("RawImage", frame, rawImage);
}

template<int dim> typename JSONTissueTrackingProject2<dim>::RawImageType::Pointer ttt::JSONTissueTrackingProject2<dim>::GetRawImage(
		unsigned int frame) {
	return this->LoadImage("RawImage", frame);
}

template<int dim> bool ttt::JSONTissueTrackingProject2<dim>::IsRawImageAvailable(unsigned int frame) {
	return this->IsImage("RawImage", frame);
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetDiffusedImage(unsigned int frame,
		const typename DiffusedImageType::Pointer & diffusedImage) {
	this->StoreImage("DiffusedImage", frame, diffusedImage);
}

template<int dim> typename JSONTissueTrackingProject2<dim>::DiffusedImageType::Pointer ttt::JSONTissueTrackingProject2<dim>::GetDiffusedImage(
		unsigned int frame) {
	return this->LoadImage("DiffusedImage", frame);
}
template<int dim> bool ttt::JSONTissueTrackingProject2<dim>::IsDiffusedImageAvailable(
		unsigned int frame) {
	return this->IsImage("DiffusedImage", frame);
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetPlatenessImage(unsigned int frame,
		const typename PlatenessImageType::Pointer & platenessImage) {
	this->StoreImage("PlatenessImage", frame, platenessImage);
}
template<int dim> typename JSONTissueTrackingProject2<dim>::PlatenessImageType::Pointer ttt::JSONTissueTrackingProject2<dim>::GetPlatenessImage(
		unsigned int frame) {
	return this->LoadImage("PlatenessImage", frame);
}
template<int dim> bool ttt::JSONTissueTrackingProject2<dim>::IsPlatenessImageAvailable(
		unsigned int frame) {
	return this->IsImage("PlatenessImage", frame); //TODO
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetVertexnessImage(unsigned int frame,
		const typename VertexnessImageType::Pointer & vertexnessImage) {
	this->StoreImage("VertexnessImage", frame, vertexnessImage);
}
template<int dim> typename JSONTissueTrackingProject2<dim>::VertexnessImageType::Pointer ttt::JSONTissueTrackingProject2<dim>::GetVertexnessImage(
		unsigned int frame) {
	return this->LoadImage("VertexnessImage", frame);
}
template<int dim> bool ttt::JSONTissueTrackingProject2<dim>::IsVertexnessImageAvailable(
		unsigned int frame) {
	return this->IsImage("VertexnessImage", frame);
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetAdherensJunctionVertices(
		unsigned int frame,
		const typename ttt::AdherensJunctionVertices<dim>::Pointer & vertices) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "vertices" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;

	for (unsigned int i = 0; i < vertices->size(); i++) {
		itk::Index<3> position = (*vertices)[i]->GetPosition();
		root["Vertices"][i]["x"] = (Json::UInt64) position[0];
		root["Vertices"][i]["y"] = (Json::UInt64) position[1];
		root["Vertices"][i]["z"] = (Json::UInt64) position[2];
	}

	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}

template<int dim> typename AdherensJunctionVertices<dim>::Pointer ttt::JSONTissueTrackingProject2<dim>::GetAdherensJunctionVertices(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "vertices_T" << frame
			<< ".json";

	std::string ajVerticesFile;
	fileNameStream >> ajVerticesFile;

	std::cout << ajVerticesFile << std::endl;

	std::ifstream jsonAJVerticesFile(ajVerticesFile.c_str());
	reader.parse(jsonAJVerticesFile, root);

	typename AdherensJunctionVertices<3>::Pointer vertices =
			AdherensJunctionVertices<3>::New();

	int numvertices = root["Vertices"].size();

	for (int i = 0; i < numvertices; i++) {
		ttt::AdherensJunctionVertex<3>::Pointer newVertex =
				ttt::AdherensJunctionVertex<3>::New();
		itk::Index<3> position;
		position[0] = root["Vertices"][i]["x"].asInt();
		position[1] = root["Vertices"][i]["y"].asInt();
		position[2] = root["Vertices"][i]["z"].asInt();
		newVertex->SetPosition(position);

		vertices->push_back(newVertex);
	}

	return vertices;
}

template<int dim> bool ttt::JSONTissueTrackingProject2<dim>::IsAdherensJunctionVerticesAvailable(
		unsigned int frame) {

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "vertices_T" << frame
			<< ".json";

	std::string fileName;
	fileNameStream >> fileName;

	std::ifstream infile(fileName.c_str());

	return infile.good();
}

template<int dim> typename ttt::TissueDescriptor<dim>::Pointer ttt::JSONTissueTrackingProject2<dim>::GetTissueDescriptor(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	typedef unsigned long int SkeletonVertexType;
	typedef unsigned long int CellVertexType;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "tissue" << "_T" << frame
			<< ".json";

	std::string ajTissueFile;
	fileNameStream >> ajTissueFile;

	std::ifstream jsonAJTissueFile(ajTissueFile.c_str());
	reader.parse(jsonAJTissueFile, root);

	typename ttt::TissueDescriptor<dim>::Pointer descriptor = ttt::TissueDescriptor<dim>::New();

	int numverticesprimal = root["primal"]["vertices"].size();
	int numedgesprimal = root["primal"]["edges"].size();
	int numverticesdual = root["dual"]["vertices"].size();
	int numedgesdual = root["dual"]["edges"].size();

	for (int i = 0; i < numverticesprimal; i++) {
		ttt::SkeletonPoint<dim> newPoint;
		newPoint.position[0] = root["primal"]["vertices"][i]["x"].asDouble();
		newPoint.position[1] = root["primal"]["vertices"][i]["y"].asDouble();
		newPoint.position[2] = root["primal"]["vertices"][i]["z"].asDouble();

		SkeletonVertexType vertex = boost::add_vertex(newPoint,
				descriptor->GetAJGraph());

	}

	for (int i = 0; i < numedgesprimal; i++) {
		SkeletonVertexType src =
				root["primal"]["edges"][i]["src"].asUInt64();
		SkeletonVertexType tgt =
				root["primal"]["edges"][i]["tgt"].asUInt64();
		boost::add_edge(src, tgt, descriptor->GetAJGraph());
	}

	for (int i = 0; i < numverticesdual; i++) {
		ttt::Cell<dim> newCell;
		itk::Point<float, dim> centroid;
		centroid[0] = root["dual"]["vertices"][i]["x"].asDouble();
		centroid[1] = root["dual"]["vertices"][i]["y"].asDouble();
		centroid[2] = root["dual"]["vertices"][i]["z"].asDouble();

		newCell.SetCentroid(centroid);
		int perimeterLength = root["dual"]["vertices"][i]["perimeter"].size();
		for (int j = 0; j < perimeterLength; j++) {
			newCell.AddVertexToPerimeter(
					root["dual"]["vertices"][i]["perimeter"][j].asUInt64());
		}

		CellVertexType vertex = boost::add_vertex(newCell,
				descriptor->GetCellGraph());

	}

	for (int i = 0; i < numedgesdual; i++) {
		SkeletonVertexType src =
			root["dual"]["edges"][i]["src"].asUInt64();
		SkeletonVertexType tgt =
				root["dual"]["edges"][i]["tgt"].asUInt64();
		boost::add_edge(src, tgt, descriptor->GetCellGraph());
	}

	int perimeterLength = root["perimeter"].size();

	for (int i = 0; i < perimeterLength; i++) {
		descriptor->AddVertexToPerimeter(root["perimeter"][i].asUInt64());
	}

	return descriptor;
}
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetTissueDescriptor(unsigned int frame,
		const typename ttt::TissueDescriptor<dim>::Pointer & descriptor) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "tissue" << "_T" << frame
			<< ".json";
	typedef unsigned long int SkeletonVertexType;
	typedef unsigned long int CellVertexType;

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	unsigned int k = 0;

	BGL_FORALL_VERTICES_T(v, descriptor->GetAJGraph(), ttt::SkeletonGraph<dim>)
	{
		root["primal"]["vertices"][k]["id"] = (Json::UInt64) v;
		root["primal"]["vertices"][k]["x"] = boost::get(
				ttt::SkeletonPointPropertyTag<dim>(), descriptor->GetAJGraph(),
				v).position[0];
		root["primal"]["vertices"][k]["y"] = boost::get(
				ttt::SkeletonPointPropertyTag<dim>(), descriptor->GetAJGraph(),
				v).position[1];
		root["primal"]["vertices"][k]["z"] = boost::get(
				ttt::SkeletonPointPropertyTag<dim>(), descriptor->GetAJGraph(),
				v).position[2];
		k++;
	}
	k = 0;
	BGL_FORALL_EDGES_T(e, descriptor->GetAJGraph(), ttt::SkeletonGraph<dim>)
	{
		root["primal"]["edges"][k]["src"] = (Json::UInt64) boost::source(e,
				descriptor->GetAJGraph());
		root["primal"]["edges"][k]["tgt"] = (Json::UInt64) boost::target(e,
				descriptor->GetAJGraph());
		k++;
	}
	k = 0;
	BGL_FORALL_VERTICES_T(v, descriptor->GetCellGraph(), ttt::CellGraph<dim>)
	{
		ttt::Cell<dim> currentCell = boost::get(ttt::CellPropertyTag<dim>(),
				descriptor->GetCellGraph(), v);
		root["dual"]["vertices"][k]["id"] = (Json::UInt64) v;
		root["dual"]["vertices"][k]["x"] = currentCell.GetCentroid()[0];
		root["dual"]["vertices"][k]["y"] = currentCell.GetCentroid()[1];
		root["dual"]["vertices"][k]["z"] = currentCell.GetCentroid()[2];
		unsigned int l = 0;
		for (typename ttt::Cell<dim>::PerimeterIterator perimeterIt =
				currentCell.PerimeterBegin();
				perimeterIt != currentCell.PerimeterEnd(); ++perimeterIt) {
			root["dual"]["vertices"][k]["perimeter"][l] =
					(Json::UInt64) *perimeterIt;
			l++;
		}
		k++;
	}
	k = 0;
	BGL_FORALL_EDGES_T(e, descriptor->GetCellGraph(), ttt::CellGraph<dim>)
	{
		root["dual"]["edges"][k]["src"] = (Json::UInt64) boost::source(e,
				descriptor->GetCellGraph());
		root["dual"]["edges"][k]["tgt"] = (Json::UInt64) boost::target(e,
				descriptor->GetCellGraph());
		k++;
	}
	k = 0;
	for (auto it = descriptor->BeginPerimeter();
			it != descriptor->EndPerimeter(); it++) {
		root["perimeter"][k++] = (Json::UInt64) *it;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}
template<int dim> bool ttt::JSONTissueTrackingProject2<dim>::IsTissueDescriptorAvailable(
		unsigned int frame) {
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "tissue_T" << frame
			<< ".json";

	std::string fileName;
	fileNameStream >> fileName;
	std::ifstream infile(fileName.c_str());
	return infile.good();
}

template<int dim> typename ttt::TrackedTissueDescriptor<dim>::Pointer ttt::JSONTissueTrackingProject2<dim>::GetTrackedTissueDescriptor(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;

	typedef unsigned long int SkeletonVertexType;
	typedef unsigned long int CellVertexType;
	typedef unsigned long int TrackedCellVertexType;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedtissue" << "_T"
			<< frame << ".json";

	std::string ajTissueFile;
	fileNameStream >> ajTissueFile;

	std::ifstream jsonAJTissueFile(ajTissueFile.c_str());
	reader.parse(jsonAJTissueFile, root);

	typename ttt::TrackedTissueDescriptor<dim>::Pointer descriptor =
			ttt::TrackedTissueDescriptor<dim>::New();

	int numverticesprimal = root["primal"]["vertices"].size();
	int numedgesprimal = root["primal"]["edges"].size();
	int numverticesdual = root["dual"]["vertices"].size();
	int numedgesdual = root["dual"]["edges"].size();

	for (int i = 0; i < numverticesprimal; i++) {
		ttt::SkeletonPoint<dim> newPoint;
		newPoint.position[0] = root["primal"]["vertices"][i]["x"].asDouble();
		newPoint.position[1] = root["primal"]["vertices"][i]["y"].asDouble();
		newPoint.position[2] = root["primal"]["vertices"][i]["z"].asDouble();

		SkeletonVertexType vertex = boost::add_vertex(newPoint,
				descriptor->GetAJGraph());

	}

	for (int i = 0; i < numedgesprimal; i++) {
		SkeletonVertexType src =
				root["primal"]["edges"][i]["src"].asUInt64();
		SkeletonVertexType tgt =
				root["primal"]["edges"][i]["tgt"].asUInt64();
		boost::add_edge(src, tgt, descriptor->GetAJGraph());
	}

	for (int i = 0; i < numverticesdual; i++) {
		ttt::TrackedCell<dim> newCell;
		itk::Point<float, dim> centroid;
		centroid[0] = root["dual"]["vertices"][i]["x"].asDouble();
		centroid[1] = root["dual"]["vertices"][i]["y"].asDouble();
		centroid[2] = root["dual"]["vertices"][i]["z"].asDouble();
		newCell.SetCentroid(centroid);

		int id = root["dual"]["vertices"][i]["trackID"].asInt();
		int parentID = root["dual"]["vertices"][i]["parentID"].asInt();

		newCell.SetID(id);
		newCell.SetParentID(parentID);

		itk::Vector<double, dim> velocity;

		velocity[0] = root["dual"]["vertices"][i]["dx"].asDouble();
		velocity[1] = root["dual"]["vertices"][i]["dy"].asDouble();
		velocity[2] = root["dual"]["vertices"][i]["dz"].asDouble();
		newCell.SetVelocity(velocity);

		int obs = root["dual"]["vertices"][i]["observed"].asInt();
		newCell.SetObservedCell(obs);

		int perimeterLength = root["dual"]["vertices"][i]["perimeter"].size();
		for (int j = 0; j < perimeterLength; j++) {
			newCell.AddVertexToPerimeter(
					root["dual"]["vertices"][i]["perimeter"][j].asUInt64());
		}

		TrackedCellVertexType vertex = boost::add_vertex(newCell,
				descriptor->GetCellGraph());

	}

	for (int i = 0; i < numedgesdual; i++) {
		SkeletonVertexType src =
				root["dual"]["edges"][i]["src"].asUInt64();
		SkeletonVertexType tgt =
				root["dual"]["edges"][i]["tgt"].asUInt64();
		boost::add_edge(src, tgt, descriptor->GetCellGraph());
	}
	int perimeterLength = root["perimeter"].size();

	for (int i = 0; i < perimeterLength; i++) {
		descriptor->AddVertexToPerimeter(root["perimeter"][i].asUInt64());
	}

	return descriptor;
}
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetTrackedTissueDescriptor(
		unsigned int frame,
		const typename ttt::TrackedTissueDescriptor<dim>::Pointer & descriptor) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedtissue" << "_T"
			<< frame << ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	unsigned int k = 0;

	BGL_FORALL_VERTICES_T(v, descriptor->GetAJGraph(), ttt::SkeletonGraph<dim>)
	{
		root["primal"]["vertices"][k]["id"] = (Json::UInt64) v;
		root["primal"]["vertices"][k]["x"] = boost::get(
				ttt::SkeletonPointPropertyTag<dim>(), descriptor->GetAJGraph(),
				v).position[0];
		root["primal"]["vertices"][k]["y"] = boost::get(
				ttt::SkeletonPointPropertyTag<dim>(), descriptor->GetAJGraph(),
				v).position[1];
		root["primal"]["vertices"][k]["z"] = boost::get(
				ttt::SkeletonPointPropertyTag<dim>(), descriptor->GetAJGraph(),
				v).position[2];
		k++;
	}
	k = 0;
	BGL_FORALL_EDGES_T(e, descriptor->GetAJGraph(), ttt::SkeletonGraph<dim>)
	{
		root["primal"]["edges"][k]["src"] = (Json::UInt64) boost::source(e,
				descriptor->GetAJGraph());
		root["primal"]["edges"][k]["tgt"] = (Json::UInt64) boost::target(e,
				descriptor->GetAJGraph());
		k++;
	}
	k = 0;
	BGL_FORALL_VERTICES_T(v, descriptor->GetCellGraph(), ttt::TrackedCellGraph<dim>)
	{
		ttt::TrackedCell<dim> currentCell = boost::get(ttt::TrackedCellPropertyTag<dim>(),
				descriptor->GetCellGraph(), v);
		root["dual"]["vertices"][k]["id"] = (Json::UInt64) v;
		root["dual"]["vertices"][k]["x"] = currentCell.GetCentroid()[0];
		root["dual"]["vertices"][k]["y"] = currentCell.GetCentroid()[1];
		root["dual"]["vertices"][k]["z"] = currentCell.GetCentroid()[2];
		root["dual"]["vertices"][k]["trackID"] = currentCell.GetID();
		root["dual"]["vertices"][k]["parentID"] = currentCell.GetParentID();
		root["dual"]["vertices"][k]["observed"] = currentCell.GetObservedCell();
		root["dual"]["vertices"][k]["dx"] = currentCell.GetVelocity()[0];
		root["dual"]["vertices"][k]["dy"] = currentCell.GetVelocity()[1];
		root["dual"]["vertices"][k]["dz"] = currentCell.GetVelocity()[2];
		unsigned int l = 0;
		for (typename ttt::Cell<dim>::PerimeterIterator perimeterIt =
				currentCell.PerimeterBegin();
				perimeterIt != currentCell.PerimeterEnd(); ++perimeterIt) {
			root["dual"]["vertices"][k]["perimeter"][l] =
					(Json::UInt64) *perimeterIt;
			l++;
		}
		k++;
	}
	k = 0;
	BGL_FORALL_EDGES_T(e, descriptor->GetCellGraph(), ttt::TrackedCellGraph<dim>)
	{
		root["dual"]["edges"][k]["src"] = (Json::UInt64) boost::source(e,
				descriptor->GetCellGraph());
		root["dual"]["edges"][k]["tgt"] = (Json::UInt64) boost::target(e,
				descriptor->GetCellGraph());
		k++;
	}
	k = 0;
	for (auto it = descriptor->BeginPerimeter();
			it != descriptor->EndPerimeter(); it++) {
		root["perimeter"][k++] = (Json::UInt64) *it;
	}

	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
template<int dim> bool ttt::JSONTissueTrackingProject2<dim>::IsTrackedTissueDescriptorAvailable(
		unsigned int frame) {
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedtissue_T" << frame
			<< ".json";

	std::string fileName;
	fileNameStream >> fileName;
	std::ifstream infile(fileName.c_str());
	return infile.good();
}

template<int dim> bool ttt::JSONTissueTrackingProject2<dim>::IsImage(const std::string & name,
		int frame) {

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << name << "_T" << frame
			<< ".ome.tif";

	std::string fileName;
	fileNameStream >> fileName;
	std::ifstream infile(fileName.c_str());
	return infile.good();
}

//CENTROIDS
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, itk::Point<double, dim> > ttt::JSONTissueTrackingProject2<dim>::GetCentroids(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "centroids" << "_T" << frame
			<< ".json";

	std::string centroidsStorageFile;
	fileNameStream >> centroidsStorageFile;

	std::ifstream jsonCentroidsFile(centroidsStorageFile.c_str());
	reader.parse(jsonCentroidsFile, root);

	int numcells = root["centroid"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, itk::Point<double, dim> > centroids;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["centroid"][k]["id"].asUInt64();
		itk::Point<double, 3> centroid;
		centroid[0] = root["centroid"][k]["value"]["x"].asDouble();
		centroid[1] = root["centroid"][k]["value"]["y"].asDouble();
		centroid[2] = root["centroid"][k]["value"]["z"].asDouble();

		centroids[cell].SetValue(centroid);
		k++;
	}
	return centroids;
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetCentroids(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, itk::Point<double, dim> > & centroids) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "centroids" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;

	int k = 0;
	for (auto it = centroids.begin(); it != centroids.end(); it++) {
		root["centroid"][k]["id"] = (Json::UInt64) it->first;
		itk::Point<double, 3> centroid = it->second.GetValue();
		root["centroid"][k]["value"]["x"] = centroid[0];
		root["centroid"][k]["value"]["y"] = centroid[1];
		root["centroid"][k]["value"]["z"] = centroid[2];
		k++;
	}

	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//XX
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetXX(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "xx" << "_T" << frame
			<< ".json";

	std::string xxStorageFile;
	fileNameStream >> xxStorageFile;

	std::ifstream jsonXXFile(xxStorageFile.c_str());
	reader.parse(jsonXXFile, root);

	int numcells = root["xx"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> xxs;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["xx"][k]["id"].asUInt64();
		double value = root["xx"][k]["value"].asDouble();

		xxs[cell].SetValue(value);

		k++;
	}
	return xxs;
}
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetXX(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> & xx) {

	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "xx" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	int k = 0;
	for (auto it = xx.begin(); it != xx.end(); it++) {
		root["xx"][k]["id"] = (Json::UInt64) it->first;
		double value =it->second.GetValue();
		root["xx"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
//XY
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetXY(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "xy" << "_T" << frame
			<< ".json";

	std::string xyStorageFile;
	fileNameStream >> xyStorageFile;

	std::ifstream jsonXYFile(xyStorageFile.c_str());
	reader.parse(jsonXYFile, root);

	int numcells = root["xy"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> xys;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["xy"][k]["id"].asUInt64();
		double value = root["xy"][k]["value"].asDouble();

		xys[cell].SetValue(value);

		k++;
	}
	return xys;
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetXY(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> & xy) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "xy" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	int k = 0;
	for (auto it = xy.begin(); it != xy.end(); it++) {
		root["xy"][k]["id"] = (Json::UInt64) it->first;
		double value = it->second.GetValue();
		root["xy"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//YY
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetYY(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "yy" << "_T" << frame
			<< ".json";

	std::string yyStorageFile;
	fileNameStream >> yyStorageFile;

	std::ifstream jsonYYFile(yyStorageFile.c_str());
	reader.parse(jsonYYFile, root);

	int numcells = root["yy"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> yys;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["yy"][k]["id"].asUInt64();
		double value = root["yy"][k]["value"].asDouble();

		yys[cell].SetValue(value);

		k++;
	}
	return yys;
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetYY(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> & yy) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "yy" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;

	int k = 0;
	for (auto it = yy.begin(); it != yy.end(); it++) {
		root["yy"][k]["id"] = (Json::UInt64) it->first;
		double value = it->second.GetValue();
		root["yy"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//PERIMETER
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetPerimeter(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "perimeter" << "_T" << frame
			<< ".json";

	std::string perimeterStorageFile;
	fileNameStream >> perimeterStorageFile;

	std::ifstream jsonPerimeterFile(perimeterStorageFile.c_str());
	reader.parse(jsonPerimeterFile, root);

	int numcells = root["perimeter"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> perimeters;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["perimeter"][k]["id"].asUInt64();
		double value = root["perimeter"][k]["value"].asDouble();

		perimeters[cell].SetValue(value);

		k++;
	}
	return perimeters;
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetPerimeter(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> & perimeter) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "perimeter" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	int k = 0;
	for (auto it = perimeter.begin(); it != perimeter.end(); it++) {
		root["perimeter"][k]["id"] = (Json::UInt64) it->first;
		double value =  it->second.GetValue();
		root["perimeter"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//AREAS
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetAreas(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "areas" << "_T" << frame
			<< ".json";

	std::string areasStorageFile;
	fileNameStream >> areasStorageFile;

	std::ifstream jsonAreasFile(areasStorageFile.c_str());
	reader.parse(jsonAreasFile, root);

	int numcells = root["areas"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> areas;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["areas"][k]["id"].asUInt64();
		double value = root["areas"][k]["value"].asDouble();

		areas[cell].SetValue(value);
		k++;
	}
	return areas;
}

template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetAreas(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TissueDescriptor<dim> >::CellVertexType, double> & areas) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "areas" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	int k = 0;
	for (auto it = areas.begin(); it != areas.end(); it++) {
		root["areas"][k]["id"] = (Json::UInt64) it->first;
		double value =  it->second.GetValue();
		root["areas"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//TrackedCentroids
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, itk::Point<double, dim> > ttt::JSONTissueTrackingProject2<dim>::GetTrackedCentroids(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedcentroids" << "_T"
			<< frame << ".json";

	std::string centroidsStorageFile;
	fileNameStream >> centroidsStorageFile;

	std::ifstream jsonCentroidsFile(centroidsStorageFile.c_str());
	reader.parse(jsonCentroidsFile, root);

	int numcells = root["centroid"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, itk::Point<double, 3> > centroids;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["centroid"][k]["id"].asUInt64();
		itk::Point<double, 3> centroid;
		centroid[0] = root["centroid"][k]["value"]["x"].asDouble();
		centroid[1] = root["centroid"][k]["value"]["y"].asDouble();
		centroid[2] = root["centroid"][k]["value"]["z"].asDouble();

		centroids[cell].SetValue(centroid);
		k++;
	}
	return centroids;
}
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetTrackedCentroids(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, itk::Point<double, 3> > & centroids) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedcentroids" << "_T"
			<< frame << ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;

	int k = 0;
	for (auto it = centroids.begin(); it != centroids.end(); it++) {
		root["centroid"][k]["id"] = (Json::UInt64) it->first;
		itk::Point<double, 3> centroid = it->second.GetValue();
		root["centroid"][k]["value"]["x"] = centroid[0];
		root["centroid"][k]["value"]["y"] = centroid[1];
		root["centroid"][k]["value"]["z"] = centroid[2];
		k++;
	}

	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}


//TrackedAreas
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetTrackedAreas(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> & areas) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedareas" << "_T"
			<< frame << ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	int k = 0;
	for (auto it = areas.begin(); it != areas.end(); it++) {
		root["areas"][k]["id"] = (Json::UInt64) it->first;
		double value =  it->second.GetValue();
		root["areas"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetTrackedAreas(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedareas" << "_T"
			<< frame << ".json";

	std::string areasStorageFile;
	fileNameStream >> areasStorageFile;

	std::ifstream jsonAreasFile(areasStorageFile.c_str());
	reader.parse(jsonAreasFile, root);

	int numcells = root["areas"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> areas;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["areas"][k]["id"].asUInt64();
		double value = root["areas"][k]["value"].asDouble();

		areas[cell].SetValue(value);
		k++;
	}
	return areas;
}

//TrackedPerimeter
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetTrackedPerimeter(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> & perimeter) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedperimeter" << "_T"
			<< frame << ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	int k = 0;
	for (auto it = perimeter.begin(); it != perimeter.end(); it++) {
		root["perimeter"][k]["id"] = (Json::UInt64) it->first;
		double value =  it->second.GetValue();
		root["perimeter"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetTrackedPerimeter(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedperimeter" << "_T"
			<< frame << ".json";

	std::string perimeterStorageFile;
	fileNameStream >> perimeterStorageFile;

	std::ifstream jsonPerimeterFile(perimeterStorageFile.c_str());
	reader.parse(jsonPerimeterFile, root);

	int numcells = root["perimeter"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> perimeters;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["perimeter"][k]["id"].asUInt64();
		double value = root["perimeter"][k]["value"].asDouble();

		perimeters[cell].SetValue(value);

		k++;
	}
	return perimeters;
}

//TrackedXX
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetTrackedXX(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> & xx) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedxx" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	int k = 0;
	for (auto it = xx.begin(); it != xx.end(); it++) {
		root["xx"][k]["id"] = (Json::UInt64) it->first;
		double value =  it->second.GetValue();
		root["xx"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetTrackedXX(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedxx" << "_T" << frame
			<< ".json";

	std::string xxStorageFile;
	fileNameStream >> xxStorageFile;

	std::ifstream jsonXXFile(xxStorageFile.c_str());
	reader.parse(jsonXXFile, root);

	int numcells = root["xx"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> xxs;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["xx"][k]["id"].asUInt64();
		double value = root["xx"][k]["value"].asDouble();

		xxs[cell].SetValue(value);

		k++;
	}
	return xxs;
}

//TrackedXY
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetTrackedXY(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> & xy) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedxy" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	int k = 0;
	for (auto it = xy.begin(); it != xy.end(); it++) {
		root["xy"][k]["id"] = (Json::UInt64) it->first;
		double value =  it->second.GetValue();
		root["xy"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetTrackedXY(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedxy" << "_T" << frame
			<< ".json";

	std::string xyStorageFile;
	fileNameStream >> xyStorageFile;

	std::ifstream jsonXYFile(xyStorageFile.c_str());
	reader.parse(jsonXYFile, root);

	int numcells = root["xy"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> xys;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["xy"][k]["id"].asUInt64();
		double value = root["xy"][k]["value"].asDouble();

		xys[cell].SetValue(value);

		k++;
	}
	return xys;
}

//TrackedYY
template<int dim> void ttt::JSONTissueTrackingProject2<dim>::SetTrackedYY(unsigned int frame,
		const FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> & yy) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedyy" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;

	int k = 0;
	for (auto it = yy.begin(); it != yy.end(); it++) {
		root["yy"][k]["id"] = (Json::UInt64) it->first;
		double value =  it->second.GetValue();
		root["yy"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile.c_str(),
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
template<int dim> FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> ttt::JSONTissueTrackingProject2<dim>::GetTrackedYY(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedyy" << "_T" << frame
			<< ".json";

	std::string yyStorageFile;
	fileNameStream >> yyStorageFile;

	std::ifstream jsonYYFile(yyStorageFile.c_str());
	reader.parse(jsonYYFile, root);

	int numcells = root["yy"].size();

	FeatureMap<typename ttt::TissueDescriptorTraits<ttt::TrackedTissueDescriptor<dim> >::CellVertexType, double> yys;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["yy"][k]["id"].asUInt64();
		double value = root["yy"][k]["value"].asDouble();

		yys[cell].SetValue(value);

		k++;
	}
	return yys;
}
}
