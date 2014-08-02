#include "jsontissuetrackingproject2.h"
#include "mysqltissuetrackingproject2.h"
int main(){

	ttt::MySQLTissueTrackingProject2 mysql;


	mysql.SetProjectID(2);
	mysql.SetUser("root");
	mysql.SetHost("localhost");
	mysql.SetDBName("TuftsTissueTracker");
	mysql.SetPassword("ttt1Tracker");

	mysql.SetProjectID(2);
	mysql.Open();

	//mysql.LoadProjectInfo();
	ttt::JSONTissueTrackingProject2 json;

	std::string jsonDir("./test/");
	json.SetDirectory(jsonDir);

	json.Open();

	json.SetProjectName(mysql.GetProjectName());
	json.SetSpacing(mysql.GetSpacing());
	json.SetSamplingPeriod(mysql.GetSamplingPeriod());
	json.SetNumFrames(mysql.GetNumFrames());


	for(unsigned int frame=0;frame<mysql.GetNumFrames();frame++){

		json.SetPlatenessSteps(frame,mysql.GetPlatenessSteps(frame));
		json.SetHighestPlatenessScale(frame,mysql.GetHighestPlatenessScale(frame));
		json.SetLowestPlatenessScale(frame,mysql.GetLowestPlatenessScale(frame));

		json.SetVertexnessSteps(frame,mysql.GetVertexnessSteps(frame));
		json.SetHighestVertexnessScale(frame,mysql.GetHighestVertexnessScale(frame));
		json.SetLowestVertexnessScale(frame,mysql.GetLowestVertexnessScale(frame));

		json.SetRawImage(frame,mysql.GetRawImage(frame));
		json.SetDiffusedImage(frame,mysql.GetDiffusedImage(frame));
		json.SetPlatenessImage(frame,mysql.GetPlatenessImage(frame));
		json.SetVertexnessImage(frame,mysql.GetVertexnessImage(frame));
		json.SetAdherensJunctionVertices(frame,mysql.GetAdherensJunctionVertices(frame));
		json.SetTissueDescriptor(frame,mysql.GetTissueDescriptor(frame));
		json.SetTrackedTissueDescriptor(frame,mysql.GetTrackedTissueDescriptor(frame));

		json.SetVertexnessImage(frame,mysql.GetVertexnessImage(frame));
		json.SetAdherensJunctionVertices(frame,mysql.GetAdherensJunctionVertices(frame));
		json.SetTissueDescriptor(frame,mysql.GetTissueDescriptor(frame));
		json.SetTrackedTissueDescriptor(frame,mysql.GetTrackedTissueDescriptor(frame));

		json.Flush();

	}

}
