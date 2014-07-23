
#include "mysqltissuetrackingproject.h"
#include <boost/graph/iteration_macros.hpp>

#include "TrackVolumeDrawer.hpp"
int main(){

	ttt::MySQLTissueTrackingProject m_Project;

	m_Project.openDB();
	m_Project.OpenProject(26);//LEG
	//m_Project.OpenProject(2);//Notum


	int init=0;
	int end=m_Project.GetNumFrames();


	int numFrames=m_Project.GetNumFrames();

	std::vector<ttt::TrackedTissueDescriptor::Pointer> tissues(numFrames);

	for(int t=init;t<end;t++){
		m_Project.SetFrame(t);
		tissues[t]=m_Project.GetTrackedTissueDescriptor();
	}

	DrawTrackVolume("notum-gt.tiff",tissues);
}
