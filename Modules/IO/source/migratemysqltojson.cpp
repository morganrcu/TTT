#include "jsontissuetrackingproject2.h"
#include "mysqltissuetrackingproject.h"
int main(){

	ttt::MySQLTissueTrackingProject mysql;

	mysql.openDB();
	mysql.OpenProject(2);

	mysql.LoadProjectInfo();
	ttt::JSONTissueTrackingProject2 json;

	std::string jsonDir("./test/");
	json.SetDirectory(jsonDir);

	json.Open();

	json.SetProjectName(mysql.GetProjectName());
	json.SetSpacing(mysql.GetSpacing());
	json.SetSamplingPeriod(mysql.GetSamplingPeriod());
	json.SetNumFrames(mysql.GetNumFrames());


	for(unsigned int frame=0;frame<mysql.GetNumFrames();frame++){
		mysql.SetFrame(frame);
		mysql.LoadFrameInfo();

		json.SetPlatenessSteps(frame,mysql.GetPlatenessSteps());
		json.SetHighestPlatenessScale(frame,mysql.GetHighestPlatenessScale());
		json.SetLowestPlatenessScale(frame,mysql.GetLowestPlatenessScale());

		json.SetVertexnessSteps(frame,mysql.GetVertexnessSteps());
		json.SetHighestVertexnessScale(frame,mysql.GetHighestVertexnessScale());
		json.SetLowestVertexnessScale(frame,mysql.GetLowestVertexnessScale());


		json.SetObject<typename itk::Image<float,3> >(std::string("RawImage"),frame,mysql.GetRawImage());
		itk::Image<float,3>::Pointer rawImage=json.GetObject<typename itk::Image<float,3> >(std::string("RawImage"),frame);

		json.SetObject<typename itk::Image<float,3> >(std::string("DiffusedImage"),frame,mysql.GetDiffusedImage());
		json.SetObject<typename itk::Image<float,3> >(std::string("PlatenessImage"),frame,mysql.GetPlatenessImage());
		json.SetObject<typename itk::Image<float,3> >(std::string("VertexnessImage"),frame,mysql.GetVertexnessImage());

		json.SetObject<typename ttt::AdherensJunctionVertices>(std::string("VertexLocations"),frame,mysql.GetVertexLocations());
		json.SetObject<typename ttt::TissueDescriptor>(std::string("TissueDescriptor"),frame,mysql.GetTissueDescriptor());
		json.SetObject<typename ttt::TrackedTissueDescriptor>(std::string("TrackedTissueDescriptor"),frame,mysql.GetTrackedTissueDescriptor());




#if 0


		json.SetVertexnessImage(mysql.GetVertexnessImage());
		json.SetVertexLocations(mysql.GetVertexLocations());
		json.SetTissueDescriptor(mysql.GetTissueDescriptor());
		json.SetTrackedTissueDescriptor(mysql.GetTrackedTissueDescriptor());

		json.Flush();
#endif
	}

}
