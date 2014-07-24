/*
 * mysqltissuetrackingproject.h
 *
 *  Created on: Oct 22, 2013
 *      Author: morgan
 */

#ifndef MYSQLTISSUETRACKINGPROJECT_H_
#define MYSQLTISSUETRACKINGPROJECT_H_

#include <vector>
#include <string>

#include <iostream>
#include <sstream>

#include <vtkSmartPointer.h>

#include <itkImage.h>
#include <itkPointSet.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>

#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/graph/iteration_macros.hpp>

#include "tttDescriptionDataTypes.h"
#include "Ellipse.h"
#include "tttDomainStrainRates.h"

#if 0
#include <itkImageFileWriter.h>

#include <itkImage.h>
#include <itkPointSet.h>

#include <tectonics.h>

#endif
#include "tissuetrackingabstractproject.h"
namespace ttt {
class MySQLTissueTrackingProject : public TissueTrackingAbstractProject{

private:

	sql::Driver * m_Driver;
	std::auto_ptr<sql::Connection> m_DB;



	std::string m_Host;
	std::string m_DBName;
	std::string m_User;
	std::string m_Password;
	unsigned int m_Port;


	std::string m_WorkingDirectory;
	int m_ProjectID;

	/////////////////////////////////// Images /////////////////////////////////////////////
#define tttIsImageReadyMacro(name, type,tablename) \
virtual  bool Is##name##Ready()  \
{										\
	return this->isImageInTable(tablename, m_Frame); \
}

#define tttLoadImageMacro(name, type,tablename) \
virtual  void Load##name()  \
{										\
	m_##name=readImageFromTable< type >(tablename, m_Frame);						\
	m_##name##Dirty=false; \
	m_##name##Loaded=true; \
}
#define tttStoreImageMacro(name, type,tablename) \
virtual  void Store##name()  \
{										\
	storeImageInTable< type >( m_##name, tablename, m_Frame); \
	m_##name##Dirty=false; \
} \

#define tttImageMacro(name,type,tablename) \
protected: \
	tttLoadImageMacro(name,type,tablename); \
	tttStoreImageMacro(name,type,tablename); \
public: \
	tttIsImageReadyMacro(name,type,tablename); \


	tttImageMacro(RawImage,RawImageType,"RawImages")
	//;tttImageMacro(LateralImageVolumeSegmentedImage,RawImageType,"LateralImageVolumeSegmentedImages")
	//	;tttImageMacro(CLAHEDImage,RawImageType,"CLAHEDImages")
	//;tttImageMacro(SurfaceSegmentedImage,RawImageType,"SurfaceSegmentedImages")
	;tttImageMacro(DiffusedImage,DiffusedImageType,"DiffusedImages")
	;tttImageMacro(PlatenessImage,PlatenessImageType,"PlatenessImages")
	;tttImageMacro(VertexnessImage,VertexnessImageType,"VertexnessImages")
	;


public:
	MySQLTissueTrackingProject();
	void SetHost(const std::string & host);
	void SetDBName(const std::string & dbname);
	void SetUser(const std::string & user);
	void SetPassword(const std::string & password);
	void SetPort(unsigned int port);
	bool openDB();
	void OpenProject(int projectID);
	void NewProject(const std::string & name,
				const std::string & workingDirectory, double spacingX,
				double spacingY, double spacingZ, double timeDelta,
				unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ);

	virtual void NewFrame(int numFrame);

	virtual unsigned  int GetNumFrames();

	void SetProjectID(int ProjectID);
	void StoreConfig();
	void Load(std::string & basePath);

	virtual void LoadProjectInfo();
	virtual void StoreProjectInfo();

	virtual void LoadFrameInfo();
	virtual void StoreFrameInfo();


	virtual void LoadVertexLocations();
	virtual void StoreVertexLocations();
	virtual bool IsVertexLocationsReady();

	virtual void LoadTissueDescriptor();
	virtual void StoreTissueDescriptor();
	virtual void SetTissueDescriptorDirty();
	virtual bool IsTissueDescriptorReady();

	virtual void LoadTrackedTissueDescriptor();
	virtual void StoreTrackedTissueDescriptor();
	virtual bool IsTrackedTissueDescriptorReady();

	virtual void LoadTrackedCentroids();
	virtual void StoreTrackedCentroids();
	virtual bool IsTrackedCentroidsReady();

	virtual void LoadTrackedAreas();
	virtual void StoreTrackedAreas();
	virtual bool IsTrackedAreasReady();

	virtual void LoadTrackedPerimeter();
	virtual void StoreTrackedPerimeter();
	virtual bool IsTrackedPerimeterReady();

	virtual void LoadTrackedWidth();
	virtual void StoreTrackedWidth();
	virtual bool IsTrackedWidthReady();

	virtual void LoadTrackedHeight();
	virtual void StoreTrackedHeight();
	virtual bool IsTrackedHeightReady();

	virtual void LoadTrackedRotation();
	virtual void StoreTrackedRotation();
	virtual bool IsTrackedRotationReady();

	virtual void LoadTrackedEllipses();
	virtual void StoreTrackedEllipses();
	virtual bool IsTrackedEllipsesReady();

	virtual void LoadTrackedDomains();
	virtual void StoreTrackedDomains();
	virtual bool IsTrackedDomainsReady();

	virtual void LoadDomainStrainRates();
	virtual void StoreDomainStrainRates();
	virtual bool IsDomainStrainRatesReady();

	virtual int	GetNumTracks();

private:
	bool isImageInTable(const std::string & table, int frame) ;
	template<class TImage> typename TImage::Pointer readImageFromTable(const std::string & table, int frame);

	template<class TImage> void storeImageInTable(const typename TImage::Pointer & image, const std::string & table,unsigned int frame);

};
}

// TISSUETRACKINGPROJECT_H

#endif /* MYSQLTISSUETRACKINGPROJECT_H_ */
