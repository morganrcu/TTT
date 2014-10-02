/*
 * jsontissuetrackingproject2.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: morgan
 */

#include "jsontissuetrackingproject2.h"
namespace ttt {

void ttt::JSONTissueTrackingProject2::LoadProjectInfo() {
	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "project.json";
	std::string projectConfigFile;
	fileNameStream >> projectConfigFile;

	std::cout << projectConfigFile << std::endl;

	std::ifstream jsonProjectConfigFile(projectConfigFile);
	reader.parse(jsonProjectConfigFile, root);

	m_ProjectName = root["name"].asString();
	std::cout << "ProjectName: " << m_ProjectName << std::endl;
	m_Spacing[0] = root["spacing"]["x"].asDouble();
	m_Spacing[1] = root["spacing"]["y"].asDouble();
	m_Spacing[2] = root["spacing"]["z"].asDouble();
	std::cout << "Spacing: " << m_Spacing << std::endl;
	m_SamplingPeriod = root["samplingPeriod"].asDouble();
	std::cout << "Sampling: " << m_SamplingPeriod << std::endl;

	m_NumFrames = root["numFrames"].asInt();
}

void ttt::JSONTissueTrackingProject2::StoreProjectInfo() {

	Json::Value root;
	Json::StyledWriter writer;

	root["name"] = m_ProjectName;
	root["spacing"]["x"] = m_Spacing[0];
	root["spacing"]["y"] = m_Spacing[1];
	root["spacing"]["z"] = m_Spacing[2];
	root["samplingPeriod"] = m_SamplingPeriod;
	root["numFrames"] = m_NumFrames;

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

void ttt::JSONTissueTrackingProject2::StoreFrameInfo(unsigned int frame) {

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
void ttt::JSONTissueTrackingProject2::LoadFrameInfo(unsigned int frame) {

	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "frame_T" << frame
			<< ".json";

	std::string frameMetadataFile;
	fileNameStream >> frameMetadataFile;
	std::cout << "File: " << frameMetadataFile << std::endl;

	std::ifstream jsonFrameFile(frameMetadataFile);

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

typename itk::Image<float, 3>::Pointer ttt::JSONTissueTrackingProject2::LoadImage(
		const std::string & name, int frame) {
	typedef itk::ImageFileReader<itk::Image<float, 3> > ReaderType;
	typename ReaderType::Pointer reader = ReaderType::New();

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << name << "_T" << frame
			<< ".ome.tif";
	std::string frameImageFile;
	fileNameStream >> frameImageFile;
	reader->SetFileName(frameImageFile);
	reader->Update();
	reader->GetOutput()->SetSpacing(m_Spacing);

	return reader->GetOutput();

}
void ttt::JSONTissueTrackingProject2::StoreImage(const std::string & name,
		int frame, const typename itk::Image<float, 3>::Pointer & object) {
	std::cout << "Store-> " << name << std::endl;

	typedef itk::ImageFileWriter<itk::Image<float, 3> > WriterType;

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

void ttt::JSONTissueTrackingProject2::SetRawImage(unsigned int frame,
		const RawImageType::Pointer & rawImage) {
	this->StoreImage("RawImage", frame, rawImage);
}

typename JSONTissueTrackingProject2::RawImageType::Pointer ttt::JSONTissueTrackingProject2::GetRawImage(
		unsigned int frame) {
	return this->LoadImage("RawImage", frame);
}

bool ttt::JSONTissueTrackingProject2::IsRawImageAvailable(unsigned int frame) {
	return this->IsImage("RawImage", frame);
}

void ttt::JSONTissueTrackingProject2::SetDiffusedImage(unsigned int frame,
		const DiffusedImageType::Pointer & diffusedImage) {
	this->StoreImage("DiffusedImage", frame, diffusedImage);
}

typename JSONTissueTrackingProject2::DiffusedImageType::Pointer ttt::JSONTissueTrackingProject2::GetDiffusedImage(
		unsigned int frame) {
	return this->LoadImage("DiffusedImage", frame);
}
bool ttt::JSONTissueTrackingProject2::IsDiffusedImageAvailable(
		unsigned int frame) {
	return this->IsImage("DiffusedImage", frame);
}

void ttt::JSONTissueTrackingProject2::SetPlatenessImage(unsigned int frame,
		const PlatenessImageType::Pointer & platenessImage) {
	this->StoreImage("PlatenessImage", frame, platenessImage);
}
typename JSONTissueTrackingProject2::PlatenessImageType::Pointer ttt::JSONTissueTrackingProject2::GetPlatenessImage(
		unsigned int frame) {
	return this->LoadImage("PlatenessImage", frame);
}
bool ttt::JSONTissueTrackingProject2::IsPlatenessImageAvailable(
		unsigned int frame) {
	return this->IsImage("PlatenessImage", frame); //TODO
}

void ttt::JSONTissueTrackingProject2::SetVertexnessImage(unsigned int frame,
		const VertexnessImageType::Pointer & vertexnessImage) {
	this->StoreImage("VertexnessImage", frame, vertexnessImage);
}
typename JSONTissueTrackingProject2::VertexnessImageType::Pointer ttt::JSONTissueTrackingProject2::GetVertexnessImage(
		unsigned int frame) {
	return this->LoadImage("VertexnessImage", frame);
}
bool ttt::JSONTissueTrackingProject2::IsVertexnessImageAvailable(
		unsigned int frame) {
	return this->IsImage("VertexnessImage", frame);
}

void ttt::JSONTissueTrackingProject2::SetAdherensJunctionVertices(
		unsigned int frame,
		const typename ttt::AdherensJunctionVertices::Pointer & vertices) {
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
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}

typename AdherensJunctionVertices::Pointer ttt::JSONTissueTrackingProject2::GetAdherensJunctionVertices(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "vertices_T" << frame
			<< ".json";

	std::string ajVerticesFile;
	fileNameStream >> ajVerticesFile;

	std::cout << ajVerticesFile << std::endl;

	std::ifstream jsonAJVerticesFile(ajVerticesFile);
	reader.parse(jsonAJVerticesFile, root);

	typename AdherensJunctionVertices::Pointer vertices =
			AdherensJunctionVertices::New();

	int numvertices = root["Vertices"].size();

	for (int i = 0; i < numvertices; i++) {
		ttt::AdherensJunctionVertex::Pointer newVertex =
				ttt::AdherensJunctionVertex::New();
		itk::Index<3> position;
		position[0] = root["Vertices"][i]["x"].asInt();
		position[1] = root["Vertices"][i]["y"].asInt();
		position[2] = root["Vertices"][i]["z"].asInt();
		newVertex->SetPosition(position);

		vertices->push_back(newVertex);
	}

	return vertices;
}

bool ttt::JSONTissueTrackingProject2::IsAdherensJunctionVerticesAvailable(
		unsigned int frame) {

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "vertices_T" << frame
			<< ".json";

	std::string fileName;
	fileNameStream >> fileName;

	std::ifstream infile(fileName);

	return infile.good();
}

typename ttt::TissueDescriptor::Pointer ttt::JSONTissueTrackingProject2::GetTissueDescriptor(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "tissue" << "_T" << frame
			<< ".json";

	std::string ajTissueFile;
	fileNameStream >> ajTissueFile;

	std::ifstream jsonAJTissueFile(ajTissueFile);
	reader.parse(jsonAJTissueFile, root);

	ttt::TissueDescriptor::Pointer descriptor = ttt::TissueDescriptor::New();

	int numverticesprimal = root["primal"]["vertices"].size();
	int numedgesprimal = root["primal"]["edges"].size();
	int numverticesdual = root["dual"]["vertices"].size();
	int numedgesdual = root["dual"]["edges"].size();

	for (int i = 0; i < numverticesprimal; i++) {
		ttt::SkeletonPoint newPoint;
		newPoint.position[0] = root["primal"]["vertices"][i]["x"].asDouble();
		newPoint.position[1] = root["primal"]["vertices"][i]["y"].asDouble();
		newPoint.position[2] = root["primal"]["vertices"][i]["z"].asDouble();

		ttt::SkeletonVertexType vertex = boost::add_vertex(newPoint,
				*descriptor->m_SkeletonGraph);

	}

	for (int i = 0; i < numedgesprimal; i++) {
		ttt::SkeletonVertexType src =
				root["primal"]["edges"][i]["src"].asUInt64();
		ttt::SkeletonVertexType tgt =
				root["primal"]["edges"][i]["tgt"].asUInt64();
		boost::add_edge(src, tgt, *descriptor->m_SkeletonGraph);
	}

	for (int i = 0; i < numverticesdual; i++) {
		ttt::Cell newCell;
		itk::Point<float, 3> centroid;
		centroid[0] = root["dual"]["vertices"][i]["x"].asDouble();
		centroid[1] = root["dual"]["vertices"][i]["y"].asDouble();
		centroid[2] = root["dual"]["vertices"][i]["z"].asDouble();

		newCell.SetCentroid(centroid);
		int perimeterLength = root["dual"]["vertices"][i]["perimeter"].size();
		for (int j = 0; j < perimeterLength; j++) {
			newCell.AddVertexToPerimeter(
					root["dual"]["vertices"][i]["perimeter"][j].asUInt64());
		}

		ttt::CellVertexType vertex = boost::add_vertex(newCell,
				*descriptor->m_CellGraph);

	}

	for (int i = 0; i < numedgesdual; i++) {
		ttt::SkeletonVertexType src =
				root["dual"]["edges"][i]["src"].asUInt64();
		ttt::SkeletonVertexType tgt =
				root["dual"]["edges"][i]["tgt"].asUInt64();
		boost::add_edge(src, tgt, *descriptor->m_CellGraph);
	}

	int perimeterLength = root["perimeter"].size();

	for (int i = 0; i < perimeterLength; i++) {
		descriptor->AddVertexToPerimeter(root["perimeter"][i].asUInt64());
	}

	return descriptor;
}
void ttt::JSONTissueTrackingProject2::SetTissueDescriptor(unsigned int frame,
		const typename ttt::TissueDescriptor::Pointer & descriptor) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "tissue" << "_T" << frame
			<< ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	unsigned int k = 0;

	BGL_FORALL_VERTICES_T(v, *descriptor->m_SkeletonGraph, ttt::SkeletonGraph)
	{
		root["primal"]["vertices"][k]["id"] = (Json::UInt64) v;
		root["primal"]["vertices"][k]["x"] = boost::get(
				ttt::SkeletonPointPropertyTag(), *descriptor->m_SkeletonGraph,
				v).position[0];
		root["primal"]["vertices"][k]["y"] = boost::get(
				ttt::SkeletonPointPropertyTag(), *descriptor->m_SkeletonGraph,
				v).position[1];
		root["primal"]["vertices"][k]["z"] = boost::get(
				ttt::SkeletonPointPropertyTag(), *descriptor->m_SkeletonGraph,
				v).position[2];
		k++;
	}
	k = 0;
	BGL_FORALL_EDGES_T(e, *descriptor->m_SkeletonGraph, ttt::SkeletonGraph)
	{
		root["primal"]["edges"][k]["src"] = (Json::UInt64) boost::source(e,
				*descriptor->m_SkeletonGraph);
		root["primal"]["edges"][k]["tgt"] = (Json::UInt64) boost::target(e,
				*descriptor->m_SkeletonGraph);
		k++;
	}
	k = 0;
	BGL_FORALL_VERTICES_T(v, *descriptor->m_CellGraph, ttt::CellGraph)
	{
		ttt::Cell currentCell = boost::get(ttt::CellPropertyTag(),
				*descriptor->m_CellGraph, v);
		root["dual"]["vertices"][k]["id"] = (Json::UInt64) v;
		root["dual"]["vertices"][k]["x"] = currentCell.GetCentroid()[0];
		root["dual"]["vertices"][k]["y"] = currentCell.GetCentroid()[1];
		root["dual"]["vertices"][k]["z"] = currentCell.GetCentroid()[2];
		unsigned int l = 0;
		for (ttt::Cell::PerimeterIterator perimeterIt =
				currentCell.PerimeterBegin();
				perimeterIt != currentCell.PerimeterEnd(); ++perimeterIt) {
			root["dual"]["vertices"][k]["perimeter"][l] =
					(Json::UInt64) *perimeterIt;
			l++;
		}
		k++;
	}
	k = 0;
	BGL_FORALL_EDGES_T(e, *descriptor->m_CellGraph, ttt::CellGraph)
	{
		root["dual"]["edges"][k]["src"] = (Json::UInt64) boost::source(e,
				*descriptor->m_CellGraph);
		root["dual"]["edges"][k]["tgt"] = (Json::UInt64) boost::target(e,
				*descriptor->m_CellGraph);
		k++;
	}
	k = 0;
	for (auto it = descriptor->BeginPerimeter();
			it != descriptor->EndPerimeter(); it++) {
		root["perimeter"][k++] = (Json::UInt64) *it;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
}
bool ttt::JSONTissueTrackingProject2::IsTissueDescriptorAvailable(
		unsigned int frame) {
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "tissue_T" << frame
			<< ".json";

	std::string fileName;
	fileNameStream >> fileName;
	std::ifstream infile(fileName);
	return infile.good();
}

typename ttt::TrackedTissueDescriptor::Pointer ttt::JSONTissueTrackingProject2::GetTrackedTissueDescriptor(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedtissue" << "_T"
			<< frame << ".json";

	std::string ajTissueFile;
	fileNameStream >> ajTissueFile;

	std::ifstream jsonAJTissueFile(ajTissueFile);
	reader.parse(jsonAJTissueFile, root);

	ttt::TrackedTissueDescriptor::Pointer descriptor =
			ttt::TrackedTissueDescriptor::New();

	int numverticesprimal = root["primal"]["vertices"].size();
	int numedgesprimal = root["primal"]["edges"].size();
	int numverticesdual = root["dual"]["vertices"].size();
	int numedgesdual = root["dual"]["edges"].size();

	for (int i = 0; i < numverticesprimal; i++) {
		ttt::SkeletonPoint newPoint;
		newPoint.position[0] = root["primal"]["vertices"][i]["x"].asDouble();
		newPoint.position[1] = root["primal"]["vertices"][i]["y"].asDouble();
		newPoint.position[2] = root["primal"]["vertices"][i]["z"].asDouble();

		ttt::SkeletonVertexType vertex = boost::add_vertex(newPoint,
				*descriptor->m_SkeletonGraph);

	}

	for (int i = 0; i < numedgesprimal; i++) {
		ttt::SkeletonVertexType src =
				root["primal"]["edges"][i]["src"].asUInt64();
		ttt::SkeletonVertexType tgt =
				root["primal"]["edges"][i]["tgt"].asUInt64();
		boost::add_edge(src, tgt, *descriptor->m_SkeletonGraph);
	}

	for (int i = 0; i < numverticesdual; i++) {
		ttt::TrackedCell newCell;
		itk::Point<float, 3> centroid;
		centroid[0] = root["dual"]["vertices"][i]["x"].asDouble();
		centroid[1] = root["dual"]["vertices"][i]["y"].asDouble();
		centroid[2] = root["dual"]["vertices"][i]["z"].asDouble();
		newCell.SetCentroid(centroid);

		int id = root["dual"]["vertices"][i]["trackID"].asInt();
		int parentID = root["dual"]["vertices"][i]["parentID"].asInt();

		newCell.SetID(id);
		newCell.SetParentID(parentID);

		itk::Vector<double, 3> velocity;

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

		ttt::TrackedCellVertexType vertex = boost::add_vertex(newCell,
				*descriptor->m_CellGraph);

	}

	for (int i = 0; i < numedgesdual; i++) {
		ttt::SkeletonVertexType src =
				root["dual"]["edges"][i]["src"].asUInt64();
		ttt::SkeletonVertexType tgt =
				root["dual"]["edges"][i]["tgt"].asUInt64();
		boost::add_edge(src, tgt, *descriptor->m_CellGraph);
	}
	int perimeterLength = root["perimeter"].size();

	for (int i = 0; i < perimeterLength; i++) {
		descriptor->AddVertexToPerimeter(root["perimeter"][i].asUInt64());
	}

	return descriptor;
}
void ttt::JSONTissueTrackingProject2::SetTrackedTissueDescriptor(
		unsigned int frame,
		const typename ttt::TrackedTissueDescriptor::Pointer & descriptor) {
	Json::StyledWriter writer;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedtissue" << "_T"
			<< frame << ".json";

	std::string vertexStorageFile;
	fileNameStream >> vertexStorageFile;
	unsigned int k = 0;

	BGL_FORALL_VERTICES_T(v, *descriptor->m_SkeletonGraph, ttt::SkeletonGraph)
	{
		root["primal"]["vertices"][k]["id"] = (Json::UInt64) v;
		root["primal"]["vertices"][k]["x"] = boost::get(
				ttt::SkeletonPointPropertyTag(), *descriptor->m_SkeletonGraph,
				v).position[0];
		root["primal"]["vertices"][k]["y"] = boost::get(
				ttt::SkeletonPointPropertyTag(), *descriptor->m_SkeletonGraph,
				v).position[1];
		root["primal"]["vertices"][k]["z"] = boost::get(
				ttt::SkeletonPointPropertyTag(), *descriptor->m_SkeletonGraph,
				v).position[2];
		k++;
	}
	k = 0;
	BGL_FORALL_EDGES_T(e, *descriptor->m_SkeletonGraph, ttt::SkeletonGraph)
	{
		root["primal"]["edges"][k]["src"] = (Json::UInt64) boost::source(e,
				*descriptor->m_SkeletonGraph);
		root["primal"]["edges"][k]["tgt"] = (Json::UInt64) boost::target(e,
				*descriptor->m_SkeletonGraph);
		k++;
	}
	k = 0;
	BGL_FORALL_VERTICES_T(v, *descriptor->m_CellGraph, ttt::TrackedCellGraph)
	{
		ttt::TrackedCell currentCell = boost::get(ttt::TrackedCellPropertyTag(),
				*descriptor->m_CellGraph, v);
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
		for (ttt::Cell::PerimeterIterator perimeterIt =
				currentCell.PerimeterBegin();
				perimeterIt != currentCell.PerimeterEnd(); ++perimeterIt) {
			root["dual"]["vertices"][k]["perimeter"][l] =
					(Json::UInt64) *perimeterIt;
			l++;
		}
		k++;
	}
	k = 0;
	BGL_FORALL_EDGES_T(e, *descriptor->m_CellGraph, ttt::TrackedCellGraph)
	{
		root["dual"]["edges"][k]["src"] = (Json::UInt64) boost::source(e,
				*descriptor->m_CellGraph);
		root["dual"]["edges"][k]["tgt"] = (Json::UInt64) boost::target(e,
				*descriptor->m_CellGraph);
		k++;
	}
	k = 0;
	for (auto it = descriptor->BeginPerimeter();
			it != descriptor->EndPerimeter(); it++) {
		root["perimeter"][k++] = (Json::UInt64) *it;
	}

	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
bool ttt::JSONTissueTrackingProject2::IsTrackedTissueDescriptorAvailable(
		unsigned int frame) {
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedtissue_T" << frame
			<< ".json";

	std::string fileName;
	fileNameStream >> fileName;
	std::ifstream infile(fileName);
	return infile.good();
}

bool ttt::JSONTissueTrackingProject2::IsImage(const std::string & name,
		int frame) {

	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << name << "_T" << frame
			<< ".ome.tif";

	std::string fileName;
	fileNameStream >> fileName;
	std::ifstream infile(fileName);
	return infile.good();
}

//CENTROIDS
FeatureMap<CellVertexType, itk::Point<double, 3> > ttt::JSONTissueTrackingProject2::GetCentroids(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "centroids" << "_T" << frame
			<< ".json";

	std::string centroidsStorageFile;
	fileNameStream >> centroidsStorageFile;

	std::ifstream jsonCentroidsFile(centroidsStorageFile);
	reader.parse(jsonCentroidsFile, root);

	int numcells = root["centroid"].size();

	FeatureMap<CellVertexType, itk::Point<double, 3> > centroids;

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

void ttt::JSONTissueTrackingProject2::SetCentroids(unsigned int frame,
		const FeatureMap<CellVertexType, itk::Point<double, 3> > & centroids) {
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
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//XX
FeatureMap<CellVertexType, double> ttt::JSONTissueTrackingProject2::GetXX(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "xx" << "_T" << frame
			<< ".json";

	std::string xxStorageFile;
	fileNameStream >> xxStorageFile;

	std::ifstream jsonXXFile(xxStorageFile);
	reader.parse(jsonXXFile, root);

	int numcells = root["xx"].size();

	FeatureMap<CellVertexType, double> xxs;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["xx"][k]["id"].asUInt64();
		double value = root["xx"][k]["value"].asDouble();

		xxs[cell].SetValue(value);

		k++;
	}
	return xxs;
}
void ttt::JSONTissueTrackingProject2::SetXX(unsigned int frame,
		const FeatureMap<TrackedCellVertexType, double> & xx) {

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
		double value = (Json::UInt64) it->second.GetValue();
		root["xx"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
//XY
FeatureMap<CellVertexType, double> ttt::JSONTissueTrackingProject2::GetXY(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "xy" << "_T" << frame
			<< ".json";

	std::string xyStorageFile;
	fileNameStream >> xyStorageFile;

	std::ifstream jsonXYFile(xyStorageFile);
	reader.parse(jsonXYFile, root);

	int numcells = root["xy"].size();

	FeatureMap<CellVertexType, double> xys;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["xy"][k]["id"].asUInt64();
		double value = root["xy"][k]["value"].asDouble();

		xys[cell].SetValue(value);

		k++;
	}
	return xys;
}

void ttt::JSONTissueTrackingProject2::SetXY(unsigned int frame,
		const FeatureMap<TrackedCellVertexType, double> & xy) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["xy"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//YY
FeatureMap<CellVertexType, double> ttt::JSONTissueTrackingProject2::GetYY(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "yy" << "_T" << frame
			<< ".json";

	std::string yyStorageFile;
	fileNameStream >> yyStorageFile;

	std::ifstream jsonYYFile(yyStorageFile);
	reader.parse(jsonYYFile, root);

	int numcells = root["yy"].size();

	FeatureMap<CellVertexType, double> yys;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["yy"][k]["id"].asUInt64();
		double value = root["yy"][k]["value"].asDouble();

		yys[cell].SetValue(value);

		k++;
	}
	return yys;
}

void ttt::JSONTissueTrackingProject2::SetYY(unsigned int frame,
		const FeatureMap<CellVertexType, double> & yy) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["yy"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//PERIMETER
FeatureMap<CellVertexType, double> ttt::JSONTissueTrackingProject2::GetPerimeter(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "perimeter" << "_T" << frame
			<< ".json";

	std::string perimeterStorageFile;
	fileNameStream >> perimeterStorageFile;

	std::ifstream jsonPerimeterFile(perimeterStorageFile);
	reader.parse(jsonPerimeterFile, root);

	int numcells = root["perimeter"].size();

	FeatureMap<CellVertexType, double> perimeters;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["perimeter"][k]["id"].asUInt64();
		double value = root["perimeter"][k]["value"].asDouble();

		perimeters[cell].SetValue(value);

		k++;
	}
	return perimeters;
}

void ttt::JSONTissueTrackingProject2::SetPerimeter(unsigned int frame,
		const FeatureMap<CellVertexType, double> & perimeter) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["perimeter"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//AREAS
FeatureMap<CellVertexType, double> ttt::JSONTissueTrackingProject2::GetAreas(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "areas" << "_T" << frame
			<< ".json";

	std::string areasStorageFile;
	fileNameStream >> areasStorageFile;

	std::ifstream jsonAreasFile(areasStorageFile);
	reader.parse(jsonAreasFile, root);

	int numcells = root["areas"].size();

	FeatureMap<CellVertexType, double> perimeters;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["areas"][k]["id"].asUInt64();
		double value = root["areas"][k]["value"].asDouble();

		perimeters[cell].SetValue(value);
		k++;
	}
	return perimeters;
}

void ttt::JSONTissueTrackingProject2::SetAreas(unsigned int frame,
		const FeatureMap<CellVertexType, double> & areas) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["areas"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

//TrackedCentroids
FeatureMap<TrackedCellVertexType, itk::Point<double, 3> > ttt::JSONTissueTrackingProject2::GetTrackedCentroids(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedcentroids" << "_T"
			<< frame << ".json";

	std::string centroidsStorageFile;
	fileNameStream >> centroidsStorageFile;

	std::ifstream jsonCentroidsFile(centroidsStorageFile);
	reader.parse(jsonCentroidsFile, root);

	int numcells = root["centroid"].size();

	FeatureMap<TrackedCellVertexType, itk::Point<double, 3> > centroids;

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
void ttt::JSONTissueTrackingProject2::SetTrackedCentroids(unsigned int frame,
		const FeatureMap<TrackedCellVertexType, itk::Point<double, 3> > & centroids) {
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
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}


//TrackedAreas
void ttt::JSONTissueTrackingProject2::SetTrackedAreas(unsigned int frame,
		const FeatureMap<TrackedCellVertexType, double> & areas) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["areas"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}

FeatureMap<TrackedCellVertexType, double> ttt::JSONTissueTrackingProject2::GetTrackedAreas(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedareas" << "_T"
			<< frame << ".json";

	std::string areasStorageFile;
	fileNameStream >> areasStorageFile;

	std::ifstream jsonAreasFile(areasStorageFile);
	reader.parse(jsonAreasFile, root);

	int numcells = root["areas"].size();

	FeatureMap<TrackedCellVertexType, double> areas;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["areas"][k]["id"].asUInt64();
		double value = root["areas"][k]["value"].asDouble();

		areas[cell].SetValue(value);
		k++;
	}
	return areas;
}

//TrackedPerimeter
void ttt::JSONTissueTrackingProject2::SetTrackedPerimeter(unsigned int frame,
		const FeatureMap<TrackedCellVertexType, double> & perimeter) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["perimeter"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
FeatureMap<TrackedCellVertexType, double> ttt::JSONTissueTrackingProject2::GetTrackedPerimeter(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedperimeter" << "_T"
			<< frame << ".json";

	std::string perimeterStorageFile;
	fileNameStream >> perimeterStorageFile;

	std::ifstream jsonPerimeterFile(perimeterStorageFile);
	reader.parse(jsonPerimeterFile, root);

	int numcells = root["perimeter"].size();

	FeatureMap<TrackedCellVertexType, double> perimeters;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["perimeter"][k]["id"].asUInt64();
		double value = root["perimeter"][k]["value"].asDouble();

		perimeters[cell].SetValue(value);

		k++;
	}
	return perimeters;
}

//TrackedXX
void ttt::JSONTissueTrackingProject2::SetTrackedXX(unsigned int frame,
		const FeatureMap<TrackedCellVertexType, double> & xx) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["xx"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
FeatureMap<TrackedCellVertexType, double> ttt::JSONTissueTrackingProject2::GetTrackedXX(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedxx" << "_T" << frame
			<< ".json";

	std::string xxStorageFile;
	fileNameStream >> xxStorageFile;

	std::ifstream jsonXXFile(xxStorageFile);
	reader.parse(jsonXXFile, root);

	int numcells = root["xx"].size();

	FeatureMap<TrackedCellVertexType, double> xxs;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["xx"][k]["id"].asUInt64();
		double value = root["xx"][k]["value"].asDouble();

		xxs[cell].SetValue(value);

		k++;
	}
	return xxs;
}

//TrackedXY
void ttt::JSONTissueTrackingProject2::SetTrackedXY(unsigned int frame,
		const FeatureMap<TrackedCellVertexType, double> & xy) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["xy"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
FeatureMap<TrackedCellVertexType, double> ttt::JSONTissueTrackingProject2::GetTrackedXY(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedxy" << "_T" << frame
			<< ".json";

	std::string xyStorageFile;
	fileNameStream >> xyStorageFile;

	std::ifstream jsonXYFile(xyStorageFile);
	reader.parse(jsonXYFile, root);

	int numcells = root["xy"].size();

	FeatureMap<TrackedCellVertexType, double> xys;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["xy"][k]["id"].asUInt64();
		double value = root["xy"][k]["value"].asDouble();

		xys[cell].SetValue(value);

		k++;
	}
	return xys;
}

//TrackedYY
void ttt::JSONTissueTrackingProject2::SetTrackedYY(unsigned int frame,
		const FeatureMap<TrackedCellVertexType, double> & yy) {
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
		double value = (Json::UInt64) it->second.GetValue();
		root["yy"][k]["value"] = value;
		k++;
	}
	std::string jsoncontent = writer.write(root);
	std::ofstream file(vertexStorageFile,
			std::ofstream::out | std::ofstream::trunc);
	file << jsoncontent;
	file.close();
}
FeatureMap<TrackedCellVertexType, double> ttt::JSONTissueTrackingProject2::GetTrackedYY(
		unsigned int frame) {
	Json::Reader reader;
	Json::Value root;
	std::stringstream fileNameStream;
	fileNameStream << m_ProjectDirectory << "/" << "trackedyy" << "_T" << frame
			<< ".json";

	std::string yyStorageFile;
	fileNameStream >> yyStorageFile;

	std::ifstream jsonYYFile(yyStorageFile);
	reader.parse(jsonYYFile, root);

	int numcells = root["yy"].size();

	FeatureMap<TrackedCellVertexType, double> yys;

	for (int k = 0; k < numcells; k++) {
		unsigned long cell = root["yy"][k]["id"].asUInt64();
		double value = root["yy"][k]["value"].asDouble();

		yys[cell].SetValue(value);

		k++;
	}
	return yys;
}
}
