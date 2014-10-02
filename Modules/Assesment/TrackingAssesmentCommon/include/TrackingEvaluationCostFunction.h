/*
 * TrackingEvaluationCostFunction.h
 *
 *  Created on: Aug 30, 2014
 *      Author: morgan
 */

#ifndef TRACKINGEVALUATIONCOSTFUNCTION_H_
#define TRACKINGEVALUATIONCOSTFUNCTION_H_
#include <vnl/vnl_cost_function.h>
#include "CellMomentCalculator.h"
void SplitCorrespondence(const ttt::TrackedTissueDescriptor::Pointer & t0,
		ttt::TrackedTissueDescriptor::Pointer & t1,
		std::map<int,int> & associations,
		std::vector<int> & creations,
		std::vector<int> & terminations,
		std::map<int,std::pair<int,int> > & mitosis){

	BGL_FORALL_VERTICES(v0,*t0->m_CellGraph,ttt::TrackedCellGraph){

		ttt::TrackedCell cell0=boost::get(ttt::TrackedCellPropertyTag(),*t0->m_CellGraph,v0);
		int trackID=cell0.GetID();

		ttt::TrackedCellVertexType v1=ttt::CellID2VertexDescriptor(trackID,t1);

		if(v1!=-1){

			ttt::TrackedCell cell1=boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,v1);
			associations[cell0.GetObservedCell()]=cell1.GetObservedCell();
		}else{
			std::pair<ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType,ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType> children= ttt::CellParentID2VertexDescriptor(trackID,t1);

			if(children.first!=-1 && children.second!=-1){
				ttt::TrackedCell cell1A= boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,children.first);
				ttt::TrackedCell cell1B= boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,children.second);

				mitosis[cell0.GetObservedCell()]=std::pair<int,int>(cell1A.GetObservedCell(),cell1B.GetObservedCell());
			}else{
				terminations.push_back(cell0.GetObservedCell());
			}
		}
	}

	BGL_FORALL_VERTICES(v1,*t1->m_CellGraph,ttt::TrackedCellGraph){
		ttt::TrackedCell cell1=boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,v1);

		int trackID=cell1.GetID();

		ttt::TrackedCellVertexType v0=ttt::CellID2VertexDescriptor(trackID,t0);

		if(v0==-1){

			int parentID=cell1.GetParentID();

			ttt::TrackedCellVertexType v0=ttt::CellID2VertexDescriptor(parentID,t0);

			if(v0==-1){
				creations.push_back(cell1.GetObservedCell());
			}


		}
	}

}


double EvaluateDataset(const std::vector<ttt::TrackedTissueDescriptor::Pointer> & reference,
		const std::vector<ttt::TissueDescriptor::Pointer> & observation,
		double distanceWeight,
		double areaWeight,
		double perimeterWeight,
		double xxWeight,
		double xyWeight,
		double yyWeight,
		double creationWeight,
		double terminationWeight,
		double associationWeight,
		double mitosisWeight,
		double apoptosisWeight){





	ttt::TrackingCommand tracker;

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

	int numFrames = observation.size();
	std::vector<ttt::TrackedTissueDescriptor::Pointer> queryTissue = tracker.GetTrackedTissue();

	int trueAssociations = 0;
	int wrongAssociations = 0;

	int associationAsTermination = 0;
	int associationAsMitosis = 0;

	int trueTermination = 0;
	int terminationAsAssociation = 0;
	int terminationAsMitosis = 0;

	int trueCreation = 0;
	int creationAsAssociation = 0;
	int creationAsMitosis = 0;

	int trueMitosis = 0;
	int wrongMitosis = 0;
	int mitosisAsAssociation = 0;
	int mitosisAsTermination = 0;

	CellMomentCalculator<ttt::TissueDescriptor> observationMomentCalculator;
	for (int t = 0; t < numFrames - 1; t++) {

		ttt::TrackedTissueDescriptor::Pointer currentReference =reference[t];
		ttt::TrackedTissueDescriptor::Pointer nextReference = reference[t + 1];

		std::map<int, int> associationsReference;
		std::vector<int> creationsReference;
		std::vector<int> terminationsReference;
		std::map<int, std::pair<int, int> > mitosisReference;

		SplitCorrespondence(currentReference, nextReference,
				associationsReference, creationsReference,
				terminationsReference, mitosisReference);
		std::cout << "True associations " << associationsReference.size() << std::endl;
		std::cout << "True creations " << creationsReference.size() << std::endl;
		std::cout << "True terminations " << terminationsReference.size() << std::endl;
		std::cout << "True mitosis " << mitosisReference.size() << std::endl;

		ttt::TrackedTissueDescriptor::Pointer currentQuery =
				queryTissue[t];
		ttt::TrackedTissueDescriptor::Pointer nextQuery =
				queryTissue[t + 1];

		std::map<int, int> associationsQuery;
		std::vector<int> creationsQuery;
		std::vector<int> terminationsQuery;
		std::map<int, std::pair<int, int> > mitosisQuery;

		SplitCorrespondence(currentQuery, nextQuery,
				associationsQuery, creationsQuery,
				terminationsQuery, mitosisQuery);

		std::cout << "Query associations " << associationsQuery.size() << std::endl;
		std::cout << "Query creations " << creationsQuery.size() << std::endl;
		std::cout << "Query terminations " << terminationsQuery.size() << std::endl;
		std::cout << "Query mitosis " << mitosisQuery.size() << std::endl;

		//std::vector<std::pair<int,int> >  currentAssignment;

		//AssignTracks(currentQuery,currentReference,currentAssignment);

		//EVALUATE ASSOCIATIONS

		for (std::map<int, int>::iterator itReferenceAssociations =
				associationsReference.begin();
				itReferenceAssociations
						!= associationsReference.end();
				itReferenceAssociations++) {
			int referenceObs =
					itReferenceAssociations->first;
			int referenceAssignment =
					itReferenceAssociations->second;
			if (associationsQuery.find(referenceObs)
					!= associationsQuery.end()) {
				if (associationsQuery[referenceObs]
						== referenceAssignment) {
					trueAssociations++;
				} else {
					wrongAssociations++;
				}
			} else {
				if (std::find(terminationsQuery.begin(),
						terminationsQuery.end(),
						referenceObs)
						!= terminationsQuery.end()) {
					associationAsTermination++;
				} else if (mitosisQuery.find(referenceObs)
						!= mitosisQuery.end()) {
					associationAsMitosis++;
				} else {
					assert(false);
				}

			}
		}

		//EVALUATE MITOSIS

		for (std::map<int, std::pair<int, int> >::iterator itReferenceMitosis =
				mitosisReference.begin();
				itReferenceMitosis != mitosisReference.end();
				itReferenceMitosis++) {

			int referenceObs = itReferenceMitosis->first;
			std::pair<int, int> referenceAssignment =
					itReferenceMitosis->second;

			if (mitosisQuery.find(referenceObs)
					!= mitosisQuery.end()) {
				if (mitosisQuery[referenceObs].first
						== referenceAssignment.first
						&& mitosisQuery[referenceObs].second
								== referenceAssignment.second) {
					trueMitosis++;
				} else {
					wrongMitosis++;
				}
			} else {
				if (std::find(terminationsQuery.begin(),
						terminationsQuery.end(),
						referenceObs)
						!= terminationsQuery.end()) {
					mitosisAsTermination++;
				} else if (associationsQuery.find(
						referenceObs)
						!= associationsQuery.end()) {
					mitosisAsAssociation++;
				} else {
					assert(false);
				}
			}
		}

		//Evaluate terminations

		for (std::vector<int>::iterator itReferenceTermination =
				terminationsReference.begin();
				itReferenceTermination
						!= terminationsReference.end();
				++itReferenceTermination) {
			int referenceObs = *itReferenceTermination;
			if (std::find(terminationsQuery.begin(),
					terminationsQuery.end(), referenceObs)
					!= terminationsQuery.end()) {
				trueTermination++;
			} else {
				if (mitosisQuery.find(referenceObs)
						!= mitosisQuery.end()) {
					terminationAsMitosis++;
				} else if (associationsQuery.find(
						referenceObs)
						!= associationsQuery.end()) {
					terminationAsAssociation++;
				} else {
					assert(false);
				}
			}
		}

		//Evaluate creations

		for (std::vector<int>::iterator itReferenceCreation =
				creationsReference.begin();
				itReferenceCreation
						!= creationsReference.end();
				++itReferenceCreation) {
			int referenceObs = *itReferenceCreation;

			if (std::find(creationsQuery.begin(),
					creationsQuery.end(), referenceObs)
					!= creationsQuery.end()) {
				trueCreation++;
			} else {
				bool isMitosis = false;
				for (std::map<int, std::pair<int, int> >::iterator itMitosis =
						mitosisQuery.begin();
						itMitosis != mitosisQuery.end();
						itMitosis++) {
					if (itMitosis->second.first
							== referenceObs
							|| itMitosis->second.second
									== referenceObs) {
						isMitosis = true;
						break;
					}
				}
				if (isMitosis) {
					creationAsMitosis++;
				} else {
					bool isAssociation = false;
					for (std::map<int, int>::iterator itAssociation =
							associationsQuery.begin();
							itAssociation
									!= associationsQuery.end();
							itAssociation++) {
						if (itAssociation->second
								== referenceObs) {
							isAssociation = true;
							break;
						}
					}
					if (isAssociation) {
						creationAsAssociation++;
					} else {
						assert(false);
					}
				}

			}
		}
	}

	double denPrecisionAssociations = (trueAssociations + wrongAssociations + associationAsTermination + associationAsMitosis);
	double precisionAssociations = denPrecisionAssociations>0 ? (double)trueAssociations/denPrecisionAssociations : 1;

	double denRecallAssociations =(trueAssociations + terminationAsAssociation + creationAsAssociation + mitosisAsAssociation);
	double recallAssociations = denRecallAssociations>0 ? (double)trueAssociations/denRecallAssociations : 1;

	double denF1Associations = precisionAssociations+recallAssociations;
	double F1Associations = denF1Associations>0 ? 2*precisionAssociations*recallAssociations/(denF1Associations) : 0;

	double denPrecisionTermination = (trueTermination + terminationAsAssociation + terminationAsMitosis);
	double precisionTermination = denPrecisionTermination>0 ? (double) trueTermination /denPrecisionTermination : 1;

	double denRecallTermination = (trueTermination + associationAsTermination + mitosisAsTermination);
	double recallTermination = denRecallTermination > 0 ? (double) trueTermination / denRecallTermination : 1;

	double denF1Termination = precisionTermination+recallTermination;
	double F1Termination = denF1Termination>0 ? 2*precisionTermination*recallTermination/(denF1Termination) : 0;


	double denPrecisionCreation =  (trueCreation + creationAsAssociation + creationAsMitosis);
	double precisionCreation = denPrecisionCreation > 0 ? (double) trueCreation / denPrecisionCreation : 1;

	double recallCreation = 1;

	double denF1Creation = precisionCreation+recallCreation;
	double F1Creation = denF1Creation>0 ? 2*precisionCreation*recallCreation/(denF1Creation) : 0;


	double denPrecisionMitosis = (trueMitosis + wrongMitosis + mitosisAsAssociation + mitosisAsTermination );
	double precisionMitosis =denPrecisionMitosis > 0 ? (double) trueMitosis / denPrecisionMitosis : 1;

	double denRecallMitosis = (trueMitosis + associationAsMitosis + terminationAsMitosis + creationAsMitosis);
	double recallMitosis = denRecallMitosis > 0 ? (double) trueMitosis / denRecallMitosis : 1;

	double denF1Mitosis = precisionMitosis+recallMitosis;
	double F1Mitosis = denF1Mitosis>0 ? 2*precisionMitosis*recallMitosis/(denF1Mitosis) : 0;


	double F1 = (F1Mitosis ==0 || F1Creation ==0 || F1Termination ==0 || F1Associations==0)?0:(4/(1/F1Mitosis + 1/F1Creation + 1/F1Termination + 1/F1Associations));

	//double F1 = (F1Mitosis + F1Creation + F1Termination + F1Associations)/4;


	//std::cout << distanceWeight << "," << areaWeight << "," << ellipseWeight  << "," << creationWeight <<"," << terminationWeight <<"," << associationWeight << "," << mitosisWeight << "----> " << F1 << std::endl;

	std::cout << distanceWeight << "," << areaWeight << "," << perimeterWeight  << ","<< xxWeight  << ","<< xyWeight  << "," << yyWeight  << "," << creationWeight <<"," << terminationWeight <<"," << associationWeight << "," << mitosisWeight << ","  << apoptosisWeight << "," << trueAssociations << "," << wrongAssociations << "," << associationAsTermination <<"," << associationAsMitosis <<"," << trueTermination <<"," << terminationAsAssociation <<"," << terminationAsMitosis <<"," << trueCreation<<"," << creationAsAssociation<<"," << creationAsMitosis<<"," << trueMitosis<<"," << wrongMitosis<<"," << mitosisAsAssociation <<"," << mitosisAsTermination << ","<<precisionAssociations  <<","<< recallAssociations <<","<< F1Associations <<","<< precisionTermination <<","<< recallTermination <<","<< F1Termination <<","<< precisionCreation <<","<< recallCreation <<","<< F1Creation <<","<< precisionMitosis <<","<< recallMitosis <<","<< F1Mitosis <<","<< F1 << std::endl;



	return F1;
}


class tracking_cost_function: public vnl_cost_function {


public:
	std::vector< std::vector<ttt::TrackedTissueDescriptor::Pointer>  > m_References;
	std::vector< std::vector< ttt::TissueDescriptor::Pointer> > m_Observations ;
	std::ofstream  m_Output;

	tracking_cost_function(char * outputName): vnl_cost_function(7){
		m_Output.open(outputName,std::ifstream::out);
		//m_Output << "distanceWeight,areaWeight,ellipseWeight,creationWeight,terminationWeight,associationWeight,mitosisWeight,trueAssociations,wrongAssociations,associationAsTermination,associationAsMitosis,trueTermination,terminationAsAssociation,terminationAsMitosis,trueCreation,creationAsAssociation,creationAsMitosis,trueMitosis,wrongMitosis,mitosisAsAssociation,mitosisAsTermination,precisionAssociations,recallAssociations,F1Associations,precisionTermination,recallTermination,F1Termination,precisionCreation,recallCreation,F1Creation,precisionMitosis,recallMitosis,F1Mitosis,F1" << std::endl;
		m_Output << "distanceWeight,areaWeight,perimeterWeight,xxWeight,xyWeight,yyWeight,creationWeight,terminationWeight,associationWeight,mitosisWeight,apoptosisWeight,F1" << std::endl;


	}
	~tracking_cost_function(){
		m_Output.close();
	}

	void AddDataset(const std::vector<ttt::TrackedTissueDescriptor::Pointer> & reference,
			const std::vector<ttt::TissueDescriptor::Pointer> & observation){
		m_References.push_back(reference);
		m_Observations.push_back(observation);
	}


public:
	double f(const vnl_vector<double>& x)  {

		double x0 =x(0);
		double x1 =x(1);
		double x2 =x(2);
		double x3 =x(3);
		double x4 =x(4);
		double x5 =x(5);
		double x6 =x(6);
		double x7 =x(7);
		double x8 =x(8);
		double x9 =x(9);
		double x10 =x(10);



		if(x0<0) x0=0;
		if(x1<0) x1=0;
		if(x2<0) x2=0;
		if(x3<0) x3=0;
		if(x4<0) x4=0;
		if(x5<0) x5=0;
		if(x6<0) x6=0;
		if(x7<0) x7=0;
		if(x8<0) x8=0;
		if(x9<0) x9=0;
		if(x10<0) x10=0;




#if 1
		//double sum1=x0+x1+x2+x3+x4+x5;
		double sum1=1;

		double distanceWeight = x0/sum1;
		double areaWeight = x1/sum1;
		double perimeterWeight=x2/sum1;
		double xxWeight=x3/sum1;
		double xyWeight=x4/sum1;
		double yyWeight=x5/sum1;


#else
		double distanceWeight = x0;
		double areaWeight = x1;
		double ellipseWeight = x2;
#endif

#if 1
		double sum2=1;
		//double sum2=x6+x7+x8+x9+x10;
		double creationWeight= x6/sum2;
		double terminationWeight=x7/sum2;
		double associationWeight=x8/sum2;
		double mitosisWeight=x9/sum2;
		double apoptosisWeight=x10/sum2;


#else
		double creationWeight= x3;
		double terminationWeight=x4;
		double associationWeight=x5;
		double mitosisWeight=x6;
#endif


		int nDatasets=m_References.size();
		std::vector<double> F1values(nDatasets);

		double F1=0;
		bool allok=true;

		for(int i=0;i<nDatasets;i++){
			F1values[i]=EvaluateDataset(m_References[i],m_Observations[i],distanceWeight,areaWeight,perimeterWeight,xxWeight,xyWeight,yyWeight,creationWeight,terminationWeight,associationWeight,mitosisWeight,apoptosisWeight);

			std::cout << "F1Values[ " << i << "] = " <<F1values[i]  << std::endl;

			if(F1values[i]==0){
				allok=false;
				break;
			}

		}

		if(allok){
			for(int i=0;i<nDatasets;i++){
				F1+=(1.0/F1values[i]);
				//F1+=F1values[i];
			}
			F1=nDatasets/F1;
		}
		m_Output << distanceWeight << "," << areaWeight << "," << perimeterWeight  << ","<< xxWeight  << ","<< xyWeight  << ","<< yyWeight  << ","  << creationWeight <<"," << terminationWeight <<"," << associationWeight << "," << mitosisWeight << ","<< apoptosisWeight << ","  << F1 << std::endl;

		double l1reg = fabs(x0) + fabs(x1) + fabs(x2);

		return -F1;

	}
};


#endif /* TRACKINGEVALUATIONCOSTFUNCTION_H_ */
