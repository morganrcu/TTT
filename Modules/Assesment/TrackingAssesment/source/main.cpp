/*
 * main.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: morgan
 */



#include "tttDescriptionDataTypes.h"
#include "qtsqltissuetrackingproject2.h"
#include "jsontissuetrackingproject2.h"
#include "TrackingCommand.h"
#include <boost/graph/iteration_macros.hpp>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_hungarian_algorithm.h>
#include "TrackVolumeDrawer.hpp"
#include "TrackingEvaluationCostFunction.h"

int main(int argc,char ** argv){
	vnl_vector<double> x0(11),x1(11);

	//std::ofstream output(argv[1],std::ifstream::out);
	//output << "w0,w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,F1"<< std::endl;

	//1. Read GT from DB

	std::vector<ttt::TrackedTissueDescriptor::Pointer>  referenceNotum,referenceLeg,referenceMitosis,referenceApoptosis;
	std::vector<ttt::TissueDescriptor::Pointer> observationNotum,observationLeg,observationMitosis,observationApoptosis;

	tracking_cost_function trackingFunction(argv[1]);
		//1. Read datasets

	{
	ttt::JSONTissueTrackingProject2 * projectJSONApoptosis = new ttt::JSONTissueTrackingProject2;
	projectJSONApoptosis->SetDirectory("/home/morgan/TTTProjects/miniroi/");
	ttt::TissueTrackingAbstractProject2 * projectApoptosis=projectJSONApoptosis;

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
		ttt::QTSQLTissueTrackingProject2 * projectqtNotum = new ttt::QTSQLTissueTrackingProject2;
		projectqtNotum->SetDBName("TuftsTissueTracker");
		projectqtNotum->SetHost("localhost");
		projectqtNotum->SetUser("root");
		projectqtNotum->SetPassword("ttt1Tracker");

		projectqtNotum->SetProjectID(2);
		ttt::TissueTrackingAbstractProject2 * projectNotum=projectqtNotum;
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
		ttt::QTSQLTissueTrackingProject2 * projectqtLeg = new ttt::QTSQLTissueTrackingProject2;
		projectqtLeg->SetDBName("TuftsTissueTracker");
		projectqtLeg->SetHost("localhost");
		projectqtLeg->SetUser("root");
		projectqtLeg->SetPassword("ttt1Tracker");

		projectqtLeg->SetProjectID(16);
		ttt::TissueTrackingAbstractProject2 * projectLeg=projectqtLeg;
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
		ttt::QTSQLTissueTrackingProject2 * projectMitosis = new ttt::QTSQLTissueTrackingProject2;
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




		//trackingFunction.AddDataset(referenceMitosis,observationMitosis);
		//trackingFunction.AddDataset(referenceLeg,observationLeg);
		trackingFunction.AddDataset(referenceNotum,observationNotum);
		//trackingFunction.AddDataset(referenceApoptosis,observationApoptosis);


#if 1
	x0(0)=0.808531; //distance
	x0(1)=0.00592618; //area
	x0(2)=0.0111654; //perimeter
	x0(3)=0.0572238; //xx
	x0(4)=0.0442711; //xy
	x0(5)=0.0728824; //yy
#else
	x0(0)=1; //distance
	x0(1)=1; //area
	x0(2)=1; //perimeter
	x0(3)=1; //xx
	x0(4)=1; //xy
	x0(5)=1; //yy
#endif
#if 1
	x0(6)=0.189574; //creation
	x0(7)=0.0190789; //termination
	x0(8)=0.0930222; //association
	x0(9)=0.698077;//0.15015; //mitosis
	x0(10)=0.000247825; //apoptosis
#endif





	int nvalues=11;

	x0(0)=0.808531; //distance
	x0(1)=0.00592618; //area
	x0(2)=0.0111654; //perimeter
	x0(3)=0.0572238; //xx
	x0(4)=0.0442711; //xy
	x0(5)=0.0728824; //yy
	x0(6)=0.189574; //creation
	x0(7)=0.0190789; //termination
	x0(8)=0.0930222; //association
	x0(9)=0.698077;//0.15015; //mitosis
	x0(10)=0.000247825; //apoptosis


	double step=0.9;

	trackingFunction.f(x0);
	for(int i=0;i<nvalues;i++){

		x1=x0;

		double F1=1;
		bool end=false;
		while(F1>0 && !end){
			x1(i)=x1(i)/step;
			if(x1(i)>2){
				end=true;
				break;
			}
#if 0
			double sum1=x1(0)+x1(1)+x1(2)+x1(3)+x1(4)+x1(5);
			x1(0)=x1(0)/sum1;
			x1(1)=x1(1)/sum1;
			x1(2)=x1(2)/sum1;
			x1(3)=x1(3)/sum1;
			x1(4)=x1(4)/sum1;
			x1(5)=x1(5)/sum1;

			double sum2=x1(6)+x1(7)+x1(8)+x1(9)+x1(10);
			x1(6)=x1(6)/sum2;
			x1(7)=x1(7)/sum2;
			x1(8)=x1(8)/sum2;
			x1(9)=x1(9)/sum2;
			x1(10)=x1(10)/sum2;
#endif
			std::cout << x1 << std::endl;
			std::cout <<"=============================================="  << std::endl;
			F1=trackingFunction.f(x1);
			F1=-F1;
			std::cout <<"=============================================="  << std::endl;
			std::cout << F1 << std::endl;

		}
		x1=x0;
		F1=1;
		end=false;
		while(F1>0 && !end ){
			x1(i)=x1(i)*step;
			if(x1(i)<1e-3){
				break;
				end=true;
			}
#if 0
			//
			double sum1=x1(0)+x1(1)+x1(2)+x1(3)+x1(4)+x1(5);
			x1(0)=x1(0)/sum1;
			x1(1)=x1(1)/sum1;
			x1(2)=x1(2)/sum1;
			x1(3)=x1(3)/sum1;
			x1(4)=x1(4)/sum1;
			x1(5)=x1(5)/sum1;

			double sum2=x1(6)+x1(7)+x1(8)+x1(9)+x1(10);
			x1(6)=x1(6)/sum2;
			x1(7)=x1(7)/sum2;
			x1(8)=x1(8)/sum2;
			x1(9)=x1(9)/sum2;
			x1(10)=x1(10)/sum2;
#endif
			std::cout << x1 << std::endl;
			std::cout <<"=============================================="  << std::endl;
			F1=trackingFunction.f(x1);
			F1=-F1;
			std::cout <<"=============================================="  << std::endl;
			std::cout << F1 << std::endl;
		}

	}
	//output.close();
}
