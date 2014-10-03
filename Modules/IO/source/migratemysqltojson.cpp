#include "jsontissuetrackingproject2.h"
#include "qtsqltissuetrackingproject2.h"
#include "CellMomentCalculator.h"
int main(int argc,char ** argv){

	if(argc!=3){
		std::cerr << "Usage: " << argv[0] << " mumProject targetDirectory" << std::endl;
	}
	ttt::QTSQLTissueTrackingProject2 mysql;


	mysql.SetProjectID(2);
	mysql.SetUser("root");
	mysql.SetHost("localhost");
	mysql.SetDBName("TuftsTissueTracker");
	mysql.SetPassword("ttt1Tracker");

	mysql.SetProjectID(atoi(argv[1]));
	mysql.Open();

	//mysql.LoadProjectInfo();
	ttt::JSONTissueTrackingProject2 json;

	std::string jsonDir(argv[2]);

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

		CellMomentCalculator<ttt::TissueDescriptor> momentCalculator;
		momentCalculator.SetTissueDescriptor(mysql.GetTissueDescriptor(frame));
		momentCalculator.Compute();

		json.SetAreas(frame,momentCalculator.GetAreas());
		json.SetPerimeter(frame,momentCalculator.GetPerimeter());
		json.SetCentroids(frame,momentCalculator.GetCentroid());
		json.SetXX(frame,momentCalculator.GetXX());
		json.SetXY(frame,momentCalculator.GetXY());
		json.SetYY(frame,momentCalculator.GetYY());


		CellMomentCalculator<ttt::TrackedTissueDescriptor> trackedMomentCalculator;
		trackedMomentCalculator.SetTissueDescriptor(mysql.GetTrackedTissueDescriptor(frame));
		trackedMomentCalculator.Compute();

		json.SetTrackedAreas(frame,trackedMomentCalculator.GetAreas());
		json.SetTrackedPerimeter(frame,trackedMomentCalculator.GetPerimeter());
		json.SetTrackedCentroids(frame,trackedMomentCalculator.GetCentroid());
		json.SetTrackedXX(frame,trackedMomentCalculator.GetXX());
		json.SetTrackedXY(frame,trackedMomentCalculator.GetXY());
		json.SetTrackedYY(frame,trackedMomentCalculator.GetYY());


		json.Flush();

	}

}
