/*
 * jsontissuetrackingproject.h
 *
 *  Created on: Jul 21, 2014
 *      Author: morgan
 */

#ifndef JSONTISSUETRACKINGPROJECT_H_
#define JSONTISSUETRACKINGPROJECT_H_

#include <string>
#include "tissuetrackingabstractproject.h"

namespace ttt {
class JSONTissueTrackingProject : public TissueTrackingAbstractProject{

private:

	std::string m_ProjectPath;

public:

	void Open(const std::string & projectPath){
		m_ProjectPath=projectPath;

	}

	virtual void LoadProjectInfo();
	virtual void StoreProjectInfo();
	virtual void StoreFrameInfo();
	virtual void LoadFrameInfo();

	virtual void NewFrame(){

	}

	virtual unsigned int GetNumFrames(){}

	virtual void LoadRawImage();
	virtual void LoadDiffusedImage();
	virtual void LoadPlatenessImage();
	virtual void LoadVertexnessImage();
	virtual void StoreRawImage();
	virtual void StoreDiffusedImage();
	virtual void StorePlatenessImage();
	virtual void StoreVertexnessImage();

	virtual void NewFrame(int numFrame);
	virtual bool IsRawImageReady();
	virtual bool IsDiffusedImageReady();
	virtual bool IsPlatenessImageReady();
	virtual bool IsVertexnessImageReady();


	virtual bool IsVertexLocationsReady();
	virtual void StoreVertexLocations();
	virtual void LoadVertexLocations();
	virtual void LoadTissueDescriptor();
	virtual void StoreTissueDescriptor();
	virtual bool IsTissueDescriptorReady();
	virtual void StoreTrackedTissueDescriptor();
	virtual void LoadTrackedTissueDescriptor();
	virtual bool IsTrackedTissueDescriptorReady();

	virtual void StoreTrackedCentroids();
	virtual void LoadTrackedCentroids();
	virtual bool IsTrackedCentroidsReady();

	virtual void LoadTrackedEllipses();
	virtual void StoreTrackedEllipses();
	virtual bool IsTrackedEllipsesReady();

	virtual void LoadTrackedDomains();
	virtual void StoreTrackedDomains();

	virtual bool IsTrackedDomainsReady();
	virtual void LoadDomainStrainRates();
	virtual void StoreDomainStrainRates();
	virtual bool IsDomainStrainRatesReady();

	void readImage(const std::string & table, int frame);
	void storeImage(const std::string & table, int frame);
};
}


#endif /* JSONTISSUETRACKINGPROJECT_H_ */
