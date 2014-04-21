/*
 * main.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: morgan
 */



#include "tttDescriptionDataTypes.h"
#include "mysqltissuetrackingproject.h"
#include "TrackingCommand.h"
#include <boost/graph/iteration_macros.hpp>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_hungarian_algorithm.h>
#include "TrackVolumeDrawer.hpp"
double TrackDistance(const std::vector<ttt::TrackedTissueDescriptor::Pointer> descA,int trackA,const std::vector<ttt::TrackedTissueDescriptor::Pointer> descB,int trackB ){
	assert(descA.size()==descB.size());
	double eps=10;
	double sum=0;
	for(unsigned int t=0;t<descA.size();t++){
		ttt::TrackedCellVertexType vertexA=ttt::CellID2VertexDescriptor(trackA,descA[t]);
		ttt::TrackedCellVertexType vertexB= ttt::CellID2VertexDescriptor(trackB,descB[t]);

		if(vertexA==-1 && vertexB==-1){
			sum+=0;
		}else if(vertexA==-1 || vertexB==-1){
			sum+=eps;
		}else{
			itk::Point<double,3> pointA=boost::get(ttt::TrackedCellPropertyTag(),*descA[t]->m_CellGraph,vertexA).m_Centroid;
			itk::Point<double,3> pointB=boost::get(ttt::TrackedCellPropertyTag(),*descB[t]->m_CellGraph,vertexB).m_Centroid;

			double dist=sqrt( pow(pointA[0]-pointB[0],2) + pow(pointA[1]-pointB[1],2) + pow(pointA[2]-pointB[2],2));
			sum+=std::min(dist,eps);
		}
	}

	return sum;
}

unsigned int NumTracks(const std::vector<ttt::TrackedTissueDescriptor::Pointer> & descriptor){

	int maxTrack=-1;
	for(unsigned int t=0;t<descriptor.size();t++){

		BGL_FORALL_VERTICES(v,*descriptor[t]->m_CellGraph,ttt::TrackedCellGraph){
			maxTrack=std::max(boost::get(ttt::TrackedCellPropertyTag(),*descriptor[t]->m_CellGraph,v).m_ID,maxTrack);
		}
	}
	return maxTrack+1;

}


void EvaluateCorrespondence(std::vector<std::pair<int,int> > & associationsQuery,
		std::vector<int> & creationsQuery,
		std::vector<int> & terminationsQuery,
		std::vector<boost::tuple<int,int,int> > & mitosisQuery,
		std::vector<std::pair<int,int> > & associationsReference,
		std::vector<int> & creationsReference,
		std::vector<int> & terminationsReference,
		std::map<int,std::pair<int,int> > & mitosis){




}

void SplitCorrespondence(const ttt::TrackedTissueDescriptor::Pointer & t0,
		ttt::TrackedTissueDescriptor::Pointer & t1,
		std::map<int,int> & associations,
		std::vector<int> & creations,
		std::vector<int> & terminations,
		std::map<int,std::pair<int,int> > & mitosis){

	BGL_FORALL_VERTICES(v0,*t0->m_CellGraph,ttt::TrackedCellGraph){

		ttt::TrackedCell cell0=boost::get(ttt::TrackedCellPropertyTag(),*t0->m_CellGraph,v0);
		int trackID=cell0.m_ID;

		ttt::TrackedCellVertexType v1=ttt::CellID2VertexDescriptor(trackID,t1);

		if(v1!=-1){

			ttt::TrackedCell cell1=boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,v1);
			associations[cell0.m_ObservedCell]=cell1.m_ObservedCell;
		}else{
			std::pair<ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType,ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType> children= ttt::CellParentID2VertexDescriptor(trackID,t1);

			if(children.first!=-1 && children.second!=-1){
				ttt::TrackedCell cell1A= boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,children.first);
				ttt::TrackedCell cell1B= boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,children.second);

				mitosis[cell0.m_ObservedCell]=std::pair<int,int>(cell1A.m_ObservedCell,cell1B.m_ObservedCell);
			}else{
				terminations.push_back(cell0.m_ObservedCell);
			}
		}
	}

	BGL_FORALL_VERTICES(v1,*t1->m_CellGraph,ttt::TrackedCellGraph){
		ttt::TrackedCell cell1=boost::get(ttt::TrackedCellPropertyTag(),*t1->m_CellGraph,v1);

		int trackID=cell1.m_ID;

		ttt::TrackedCellVertexType v0=ttt::CellID2VertexDescriptor(trackID,t0);

		if(v0==-1){

			int parentID=cell1.m_ParentID;

			ttt::TrackedCellVertexType v0=ttt::CellID2VertexDescriptor(parentID,t0);

			if(v0==-1){
				creations.push_back(cell1.m_ObservedCell);
			}


		}
	}

}

void ComputeAssignment(const std::vector<ttt::TrackedTissueDescriptor::Pointer> & query,const std::vector<ttt::TrackedTissueDescriptor::Pointer> & reference,std::vector<std::pair<int,int> > & result){
	int numTracksQuery = NumTracks(query);
	int numTracksReference = NumTracks(reference);

	vnl_matrix<double> distances(numTracksQuery,numTracksReference);

	for(int q=0;q<numTracksQuery;q++){
		for(int r=0;r<numTracksReference;r++){
			distances(q,r)=TrackDistance(query,q,reference,r);
		}
	}


	std::vector<unsigned> assignement = vnl_hungarian_algorithm(distances);

	for(unsigned i=0;i<assignement.size();i++){
		if(assignement[i]!=unsigned(-1)){
			result.push_back(std::pair<int,int>(i,assignement[i]));
		}
	}

}


void AssignTracks(const ttt::TrackedTissueDescriptor::Pointer & query,const ttt::TrackedTissueDescriptor::Pointer & reference,std::vector<std::pair<int,int> > & result){
	int numTracksQuery = boost::num_vertices(*query->m_CellGraph);
	int numTracksReference = boost::num_vertices(*query->m_CellGraph);

	vnl_matrix<double> distances(numTracksQuery,numTracksReference);

	BGL_FORALL_VERTICES(q,*query->m_CellGraph,ttt::TrackedCellGraph){
		BGL_FORALL_VERTICES(r,*reference->m_CellGraph,ttt::TrackedCellGraph){
			itk::Point<double,3> pointQ=boost::get(ttt::TrackedCellPropertyTag(),*query->m_CellGraph,q).m_Centroid;
			itk::Point<double,3> pointR=boost::get(ttt::TrackedCellPropertyTag(),*reference->m_CellGraph,r).m_Centroid;

			distances(q,r)=sqrt( pow(pointQ[0]-pointR[0],2) + pow(pointQ[1]-pointR[1],2) + pow(pointQ[2]-pointR[2],2));
		}
	}


	std::vector<unsigned> assignement = vnl_hungarian_algorithm(distances);

	for(unsigned i=0;i<assignement.size();i++){
		if(assignement[i]!=unsigned(-1)){
			result.push_back(std::pair<int,int>(i,assignement[i]));
		}
	}
}

int main(int argc,char ** argv){

	//1. Read GT from DB

	ttt::TissueTrackingProject m_Project;

	int projectNum=atoi(argv[1]);
	m_Project.openDB();
	//m_Project.OpenProject(16);
	m_Project.OpenProject(projectNum);

	int numFrames=m_Project.GetNumFrames();

	std::vector<ttt::TrackedTissueDescriptor::Pointer> referenceTissue(numFrames);
	std::vector<ttt::TissueDescriptor::Pointer> observedTissue(numFrames);




	for(int t=0;t<numFrames;t++){
		m_Project.SetFrame(t);

		observedTissue[t]=m_Project.GetTissueDescriptor();
		referenceTissue[t]=m_Project.GetTrackedTissueDescriptor();
	}

	std::ofstream output(argv[2],std::ifstream::out);
	output << "cWeight,tWeight,aWeight,mWeight,alpha,trueAssociations,wrongAssociations,associationAsTermination,associationAsMitosis,trueTermination,terminationAsAssociation,terminationAsMitosis,trueCreation,creationAsAssociation,creationAsMitosis,trueMitosis,wrongMitosis,mitosisAsAssociation,mitosisAsTermination"<< std::endl;

	double cWeight =0; //ok
	double tWeight =0.7; //ok

	double aWeight =0.45; //

	//double mWeight =0.65; //ok
	double mWeight =0.85; //ok
	double alpha=0.015625;
	//double alpha=0.0125;

	//cWeight=1;
	//alpha=1;
	//while(alpha>=pow(2,-10)){
//	while(cWeight>=0){


		//while(tWeight>=(pow(2,-7))){
		//while(tWeight>=0){

			//while(aWeight>=(pow(2,-7))){
			//while(aWeight>=0){
				//while(mWeight>=(pow(2,-7))){

				//while(mWeight>=0){


					//while(alpha>=(pow(2,-7))){
						ttt::TrackingCommand tracker;

						tracker.SetAreaWeight(alpha);
						tracker.SetDistanceWeight(1 - alpha);

						tracker.SetCreationWeight(cWeight);
						tracker.SetTerminationWeight(tWeight);
						tracker.SetAssociationWeight(aWeight);
						tracker.SetMitosisWeight(mWeight);

						tracker.SetObservedTissues(observedTissue);

						tracker.Do();
						std::vector<ttt::TrackedTissueDescriptor::Pointer> queryTissue =
								tracker.GetTrackedTissue();

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

						for (int t = 0; t < numFrames - 1; t++) {

							ttt::TrackedTissueDescriptor::Pointer currentReference =
									referenceTissue[t];
							ttt::TrackedTissueDescriptor::Pointer nextReference =
									referenceTissue[t + 1];

							std::map<int, int> associationsReference;
							std::vector<int> creationsReference;
							std::vector<int> terminationsReference;
							std::map<int, std::pair<int, int> > mitosisReference;

							SplitCorrespondence(currentReference, nextReference,
									associationsReference, creationsReference,
									terminationsReference, mitosisReference);
							//std::cout << "True associations " << associationsReference.size() << std::endl;
							//std::cout << "True creations " << creationsReference.size() << std::endl;
							//std::cout << "True terminations " << terminationsReference.size() << std::endl;
							//std::cout << "True mitosis " << mitosisReference.size() << std::endl;

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

							//std::cout << "Query associations " << associationsQuery.size() << std::endl;
							//std::cout << "Query creations " << creationsQuery.size() << std::endl;
							//std::cout << "Query terminations " << terminationsQuery.size() << std::endl;
							//std::cout << "Query mitosis " << mitosisQuery.size() << std::endl;

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

						output << cWeight << "," << tWeight << "," << aWeight  << "," << mWeight <<"," << alpha << ","  << trueAssociations << "," << wrongAssociations << "," << associationAsTermination <<"," << associationAsMitosis <<"," << trueTermination <<"," << terminationAsAssociation <<"," << terminationAsMitosis <<"," << trueCreation<<"," << creationAsAssociation<<"," << creationAsMitosis<<"," << trueMitosis<<"," << wrongMitosis<<"," << mitosisAsAssociation <<"," << mitosisAsTermination << std::endl;

#if 0
						std::cout << "Alpha " << alpha

								<< "    ==========================================="
								<< std::endl;
						std::cout << "TrueAssociations: " << trueAssociations
								<< std::endl;
						std::cout << "WrongAssociations: " << wrongAssociations
								<< std::endl;

						std::cout << "AssociationAsTermination: "
								<< associationAsTermination << std::endl;
						;
						std::cout << "associationAsMitosis: "
								<< associationAsMitosis << std::endl;
						;

						std::cout << "trueTermination: " << trueTermination
								<< std::endl;
						;
						std::cout << "terminationAsAssociation: "
								<< terminationAsAssociation << std::endl;
						;
						std::cout << "terminationAsMitosis: "
								<< terminationAsMitosis << std::endl;

						std::cout << "trueCreation: " << trueCreation
								<< std::endl;
						;
						std::cout << "creationAsAssociation: "
								<< creationAsAssociation << std::endl;
						std::cout << "creationAsMitosis: " << creationAsMitosis
								<< std::endl;

						std::cout << "trueMitosis: " << trueMitosis
								<< std::endl;
						std::cout << "wrongMitosis: " << wrongMitosis
								<< std::endl;
						std::cout << "mitosisAsAssociation: "
								<< mitosisAsAssociation << std::endl;
						std::cout << "mitosisAsTermination: "
								<< mitosisAsTermination << std::endl;
#endif
						//alpha = alpha / 4;
					//}
						//mWeight = mWeight /2;
						//}
				//aWeight = aWeight /2;
//aWeight = aWeight-.25;
						//}
			//tWeight = tWeight /2;
			//tWeight = tWeight-.25;
						//}
		cWeight = cWeight-0.05;
		//alpha =alpha/2;
		DrawTrackVolume(std::string(argv[3]),queryTissue);
		//}

	output.close();

}
