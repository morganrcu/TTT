
#include "mysqltissuetrackingproject2.h"
#include <boost/graph/iteration_macros.hpp>

#include "TrackVolumeDrawer.hpp"
int main(){

	ttt::MySQLTissueTrackingProject2 m_Project;


	m_Project.SetProjectID(26);
	m_Project.Open();
	//m_Project.OpenProject(2);//Notum


	int init=0;
	int end=m_Project.GetNumFrames();


	int numFrames=m_Project.GetNumFrames();

	std::vector<ttt::TrackedTissueDescriptor::Pointer> tissues(numFrames);

	for(int t=init;t<end;t++){

		tissues[t]=m_Project.GetTrackedTissueDescriptor(t);
	}

	DrawTrackVolume("notum-gt.tiff",tissues);
}
