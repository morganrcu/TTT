
#include "qtsqltissuetrackingproject2.h"
#include "jsontissuetrackingproject2.h"
#include <boost/graph/iteration_macros.hpp>

#include "TrackVolumeDrawer.hpp"
#include "TrackingCommand.h"

int main(int argc, char ** argv){
	std::vector<ttt::TissueDescriptor::Pointer> observation;


	{
		ttt::JSONTissueTrackingProject2 * projectJSONApoptosis = new ttt::JSONTissueTrackingProject2;
		projectJSONApoptosis->SetDirectory("/home/morgan/TTTProjects/miniroi/");
		ttt::TissueTrackingAbstractProject2 * projectApoptosis=projectJSONApoptosis;

		projectApoptosis->Open();

		int numFrames=projectApoptosis->GetNumFrames();

		observation.resize(numFrames);

		for(int t=0;t<numFrames;t++){

			observation[t]=projectApoptosis->GetTissueDescriptor(t);

		}

	}

#if 0
	{
			ttt::QTSQLTissueTrackingProject2 * projectqtNotum = new ttt::QTSQLTissueTrackingProject2;
			projectqtNotum->SetDBName("TuftsTissueTracker");
			projectqtNotum->SetHost("localhost");
			projectqtNotum->SetUser("root");
			projectqtNotum->SetPassword("ttt1Tracker");

			projectqtNotum->SetProjectID(26);
			ttt::TissueTrackingAbstractProject2 * projectNotum=projectqtNotum;
			projectNotum->Open();

			int numFrames=projectNotum->GetNumFrames();
			numFrames=20;


			observation.resize(numFrames);

			for(int t=0;t<numFrames;t++){
				observation[t]=projectNotum->GetTissueDescriptor(t);
			}
			}
#endif
	ttt::TrackingCommand tracker;


	double distanceWeight=0.808531;
	double areaWeight=0.00592618;
	double perimeterWeight=0.0111654;
	double xxWeight=0.0572238;
	double xyWeight=0.0442711;
	double yyWeight=0.0728824;
	double creationWeight=0.189574;
	double terminationWeight=0.0190789;
	double associationWeight=0.0930222;
	double mitosisWeight=0.698077;
	double apoptosisWeight=0.000247825;


	tracker.SetDistanceWeight(distanceWeight);
	tracker.SetAreaWeight(areaWeight);
	tracker.SetPerimeterWeight(perimeterWeight);
	tracker.SetXXWeight(xxWeight);
	tracker.SetXYWeight(xyWeight);
	tracker.SetYYWeight(yyWeight);


	tracker.SetCreationWeight(creationWeight);
	tracker.SetTerminationWeight(terminationWeight);
	tracker.SetAssociationWeight(associationWeight);
	tracker.SetMitosisWeight(mitosisWeight);
	tracker.SetApoptosisWeight(apoptosisWeight);



	tracker.SetObservedTissues(observation);
	tracker.Do();

	std::vector<ttt::TrackedTissueDescriptor::Pointer>  trackedApoptosis=tracker.GetTrackedTissue();

	DrawTrackVolume("apoptosis-lateral.tiff",trackedApoptosis);
}
