/*
 * main.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: morgan
 */



#include "tttDescriptionDataTypes.h"

#include "TrackingCommand.h"
#include <boost/graph/iteration_macros.hpp>
#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_amoeba.h>

#include "TrackVolumeDrawer.hpp"

#include "qtsqltissuetrackingproject2.h"
#include "jsontissuetrackingproject2.h"

#include "TrackingEvaluationCostFunction.h"

int main(int argc,char ** argv){

	//1. Read GT from DB
	static const int NumDimensions=3;
	std::vector<typename ttt::TrackedTissueDescriptor<NumDimensions>::Pointer>  referenceNotum,referenceLeg,referenceMitosis,referenceApoptosis,decimatedReferenceNotum,decimatedReferenceLeg,decimatedReferenceMitosis,decimatedReferenceApoptosis;
	std::vector<typename ttt::TissueDescriptor<NumDimensions>::Pointer> observationNotum,observationLeg,observationMitosis,observationApoptosis,decimatedObservationNotum,decimatedObservationLeg,decimatedObservationMitosis,decimatedObservationApoptosis;

	tracking_cost_function<NumDimensions> trackingFunction(argv[1]);
	//1. Read datasets

	{
	ttt::JSONTissueTrackingProject2<NumDimensions> * projectJSONApoptosis = new ttt::JSONTissueTrackingProject2<NumDimensions>;
	projectJSONApoptosis->SetDirectory("/home/morgan/TTTProjects/miniroi/");
	ttt::TissueTrackingAbstractProject2<NumDimensions> * projectApoptosis=projectJSONApoptosis;

	projectApoptosis->Open();

	int numFrames=projectApoptosis->GetNumFrames();

	referenceApoptosis.resize(numFrames);
	observationApoptosis.resize(numFrames);

	for(int t=0;t<numFrames;t++){

		observationApoptosis[t]=projectApoptosis->GetTissueDescriptor(t);
		referenceApoptosis[t]=projectApoptosis->GetTrackedTissueDescriptor(t);
	}



	}

	{
	ttt::QTSQLTissueTrackingProject2<NumDimensions> * projectqtNotum = new ttt::QTSQLTissueTrackingProject2<NumDimensions>;
	projectqtNotum->SetDBName("TuftsTissueTracker");
	projectqtNotum->SetHost("localhost");
	projectqtNotum->SetUser("root");
	projectqtNotum->SetPassword("ttt1Tracker");

	projectqtNotum->SetProjectID(2);
	ttt::TissueTrackingAbstractProject2<NumDimensions> * projectNotum=projectqtNotum;
	projectNotum->Open();

	int numFrames=projectNotum->GetNumFrames();
	numFrames=20;

	referenceNotum.resize(numFrames);
	observationNotum.resize(numFrames);

	for(int t=0;t<numFrames;t++){
		observationNotum[t]=projectNotum->GetTissueDescriptor(t);
		referenceNotum[t]=projectNotum->GetTrackedTissueDescriptor(t);
	}
	}
	{
	ttt::QTSQLTissueTrackingProject2<NumDimensions> * projectqtLeg = new ttt::QTSQLTissueTrackingProject2<NumDimensions>;
	projectqtLeg->SetDBName("TuftsTissueTracker");
	projectqtLeg->SetHost("localhost");
	projectqtLeg->SetUser("root");
	projectqtLeg->SetPassword("ttt1Tracker");

	projectqtLeg->SetProjectID(16);
	ttt::TissueTrackingAbstractProject2<NumDimensions> * projectLeg=projectqtLeg;
	projectLeg->Open();

	int numFrames=projectLeg->GetNumFrames();

	referenceLeg.resize(numFrames);
	observationLeg.resize(numFrames);

	for(int t=0;t<numFrames;t++){
		observationLeg[t]=projectLeg->GetTissueDescriptor(t);
		referenceLeg[t]=projectLeg->GetTrackedTissueDescriptor(t);
	}



	}

	{
	ttt::QTSQLTissueTrackingProject2<NumDimensions> * projectMitosis = new ttt::QTSQLTissueTrackingProject2<NumDimensions>;
	projectMitosis->SetDBName("TuftsTissueTracker");
	projectMitosis->SetHost("localhost");
	projectMitosis->SetUser("root");
	projectMitosis->SetPassword("ttt1Tracker");
	projectMitosis->SetProjectID(26);
	projectMitosis->Open();

	int numFrames=projectMitosis->GetNumFrames();

	referenceMitosis.resize(numFrames);
	observationMitosis.resize(numFrames);

	for(int t=0;t<numFrames;t++){
		observationMitosis[t]=projectMitosis->GetTissueDescriptor(t);
		referenceMitosis[t]=projectMitosis->GetTrackedTissueDescriptor(t);
	}

	}


	int DROPFRAMES=2;
	decimatedReferenceMitosis=Decimate(referenceMitosis,DROPFRAMES);
	decimatedObservationMitosis=Decimate(observationMitosis,DROPFRAMES);

	decimatedReferenceLeg=Decimate(referenceLeg,DROPFRAMES);
	decimatedObservationLeg=Decimate(observationLeg,DROPFRAMES);

	decimatedReferenceNotum=Decimate(referenceNotum,DROPFRAMES);
	decimatedObservationNotum=Decimate(observationNotum,DROPFRAMES);

	decimatedReferenceApoptosis=Decimate(referenceApoptosis,DROPFRAMES);
	decimatedObservationApoptosis=Decimate(observationApoptosis,DROPFRAMES);

	trackingFunction.AddDataset(decimatedReferenceMitosis,decimatedObservationMitosis);
	//trackingFunction.AddDataset(referenceMitosis,observationMitosis);

	trackingFunction.AddDataset(decimatedReferenceLeg,decimatedObservationLeg);
	//trackingFunction.AddDataset(referenceLeg,observationLeg);
	trackingFunction.AddDataset(decimatedReferenceNotum,decimatedObservationNotum);
	//trackingFunction.AddDataset(referenceNotum,observationNotum);
	trackingFunction.AddDataset(decimatedReferenceApoptosis,decimatedObservationApoptosis);
	//	trackingFunction.AddDataset(referenceApoptosis,observationApoptosis);







	vnl_amoeba optim(trackingFunction);

	vnl_vector<double> x0(13);
	//10.6129,0,1.04587,1.131,1.00051,1.15699,0.211418,0.0231487,0.107582,0.835998,0.000247864,0.971626
	//0.122819,0.0572238,0.0442711,0.0728824,0.189574,0.0190789,0.0930222,0.698077,0.000247825,2,0.670564
	//0.594869,0.0194649,0.119349,0.0616608,0.379737,0.0785335,0.204273,0.0205582,0.100235,0.666043,0.000247842,2,0.822569
#if 1
	x0(0)=0.594869; //distance
	x0(1)=0.0194649; //area
	x0(2)=0.119349; //perimeter
	x0(3)=0.0616608; //xx
	x0(4)=0.379737; //xy
	x0(5)=0.0785335; //yy
#else
	x0(0)=1; //distance
	x0(1)=1; //area
	x0(2)=1; //perimeter
	x0(3)=1; //xx
	x0(4)=1; //xy
	x0(5)=1; //yy
#endif
#if 1
	x0(6)=0.204273; //creation
	x0(7)=0.0205582; //termination
	x0(8)=0.100235; //association
	x0(9)=0.666043;//0.15015; //mitosis
	x0(10)=0.000247842; //apoptosis
#endif
	x0(11)=1; //apoptosis
	x0(12)=1; //apoptosis
#if 0
	for(int i=0;i<50;i++){
		x0(12)=i;
		trackingFunction.f(x0);
	}
#endif

	optim.set_relative_diameter(10);
	optim.set_f_tolerance(1);
	optim.set_max_iterations(10000);
	optim.minimize(x0);

}
