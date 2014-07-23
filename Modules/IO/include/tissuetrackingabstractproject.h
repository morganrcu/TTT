/*
 * tissuetrackingabstractproject.h
 *
 *  Created on: Apr 25, 2014
 *      Author: morgan
 */

#ifndef TISSUETRACKINGABSTRACTPROJECT_H_
#define TISSUETRACKINGABSTRACTPROJECT_H_

#include "FeatureMap.h"
#include "itkImage.h"
#include "tttDescriptionDataTypes.h"
#include "Ellipse.h"
#include "tttDomainStrainRates.h"
namespace ttt{
class TissueTrackingAbstractProject{
public:

	typedef itk::Image<float, 3> RawImageType;
	//typedef itk::Image<unsigned char, 3> SurfaceSegmentedImageType;
	typedef itk::Image<float, 3> DiffusedImageType;
	typedef itk::Image<float, 3> PlatenessImageType;
	typedef itk::Image<itk::Vector<float, 3>, 3> OrientationImageType;
	typedef itk::Image<float, 3> VertexnessImageType;

	typedef ttt::TissueDescriptor TissueDescriptorType;
	typedef ttt::TrackedTissueDescriptor TrackedTissueDescriptorType;

	typedef typename RawImageType::SpacingType SpacingType;

	typedef Ellipse<double> EllipseType;

	typedef std::map<
			typename TissueDescriptorType::DualGraphVertexDescriptorType,
			EllipseType> EllipseMapType;

	typedef boost::shared_ptr<EllipseMapType> EllipseMapTypePointer;
	typedef std::vector<ttt::TrackedDomain> TrackedDomainVectorType;
	typedef boost::shared_ptr<TrackedDomainVectorType> TrackedDomainVectorTypePointer;

	typedef RawImageType::RegionType RegionType;
	typedef RawImageType::SizeType SizeType;

	RegionType m_Region;
	SpacingType m_Spacing;
	double m_TimeDelta;

	std::string m_WorkingDirectory;
	std::string m_ProjectName;


	double m_LowerPlatenessScale;
	double m_HigherPlatenessScale;
	double m_PlatenessSteps;

	double m_LowerVertexnessScale;
	double m_HigherVertexnessScale;
	double m_VertexnessSteps;


	unsigned int m_Frame;
public:
	inline SpacingType GetSpacing() {
		return m_Spacing;
	}
	inline void SetSpacingX(double spacingX){
		m_Spacing[0]=spacingX;
	}
	inline void SetSpacingY(double spacingY){
		m_Spacing[1]=spacingY;
	}
	inline void SetSpacingZ(double spacingZ){
		m_Spacing[2]=spacingZ;
	}
	inline void SetSamplingRate(double samplingRate){
		m_TimeDelta=samplingRate;
	}

	inline void SetLowerPlatenessScale(double lowerPlatenessScale){
		m_LowerPlatenessScale=lowerPlatenessScale;
	}

	inline void SetHigherPlatenessScale(double higherPlatenessScale){
		m_HigherPlatenessScale=higherPlatenessScale;
	}
	inline double GetHigherPlatenessScale(){
		return m_HigherPlatenessScale;
	}
	inline double GetLowerPlatenessScale(){
		return m_LowerPlatenessScale;
	}
	inline int GetPlatenessSteps(){
		return m_PlatenessSteps;
	}
	inline void SetPlatenessSteps(int platenessSteps){
		m_PlatenessSteps=platenessSteps;
	}

	inline double GetTemporalScale() {
		return m_TimeDelta;
	}

	inline void SetProjectName(const std::string & projectName){
		m_ProjectName=projectName;
	}

	inline std::string GetProjectName() {
		return m_ProjectName;
	}
	RegionType::SizeType GetSize(){
		return m_Region.GetSize();
	}
	int GetFrame() {
		return m_Frame;
	}
	void SetFrame(int frame) {
		this->Flush();
		m_Frame = frame;
		this->Clear();
	}

	virtual void NewFrame(int numFrame)=0;
	virtual unsigned int GetNumFrames()=0;

	void Flush() {
		if (this->m_RawImageDirty) {
			this->StoreRawImage();
		}
		if (this->m_CLAHEDImageDirty) {
			this->StoreCLAHEDImage();
		}
		if (this->m_LateralImageVolumeSegmentedImageDirty) {
			this->StoreLateralImageVolumeSegmentedImage();
		}
		if (this->m_SurfaceSegmentedImageDirty) {
			this->StoreSurfaceSegmentedImage();
		}
		if (this->m_DiffusedImageDirty) {
			this->StoreDiffusedImage();
		}
		if (this->m_PlatenessImageDirty) {
			this->StorePlatenessImage();
		}
		if (this->m_VertexnessImageDirty) {
			this->StoreVertexnessImage();
		}
		if (this->m_VertexLocationsDirty) {
			this->StoreVertexLocations();
		}
		if (this->m_TissueDescriptorDirty) {
			this->StoreTissueDescriptor();
		}
		if (this->m_TrackedTissueDescriptorDirty) {
			this->StoreTrackedTissueDescriptor();
		}
		if (this->m_TrackedEllipsesDirty){
			this->StoreTrackedEllipses();
		}
		if (this->m_DomainStrainRatesDirty){
			this->StoreDomainStrainRates();
		}
	}
	void Clear() {

		this->m_RawImage = 0;
		this->m_CLAHEDImage = 0;
		this->m_LateralImageVolumeSegmentedImage = 0;
		this->m_SurfaceSegmentedImage = 0;
		this->m_DiffusedImage = 0;
		this->m_PlatenessImage = 0;
		this->m_VertexnessImage = 0;
		this->m_VertexLocations = 0;
		this->m_TissueDescriptor = 0;
		this->m_TrackedTissueDescriptor=0;

		this->m_RawImageLoaded=false;
		this->m_CLAHEDImageLoaded=false;
		this->m_LateralImageVolumeSegmentedImageLoaded=false;
		this->m_SurfaceSegmentedImageLoaded=false;
		this->m_DiffusedImageLoaded=false;
		this->m_PlatenessImageLoaded=false;
		this->m_VertexnessImageLoaded=false;
		this->m_VertexLocationsLoaded=false;
		this->m_TissueDescriptorLoaded=false;
		this->m_TrackedTissueDescriptorLoaded=false;
	}


#define tttDefineImageMacro(name,type) \
protected: \
	type::Pointer m_##name;\
	bool m_##name##Dirty=false;\
	bool m_##name##Loaded=false;

#define tttGetImageMacro(name, type) \
virtual  type::Pointer  Get##name()  \
{										\
	if(!m_##name##Loaded) Load##name(); \
	return m_##name;						\
}										\

#define tttSetImageMacro(name, type) \
virtual  void Set##name(const type::Pointer & arg)  \
{										\
	m_##name##Dirty=true;\
	m_##name##Loaded=true;\
	m_##name=arg;						\
}										\

#define tttLoadImageMacro(name, type,tablename) \
virtual  void Load##name() =0;

#define tttStoreImageMacro(name, type,tablename) \
virtual  void Store##name() = 0;

#define tttIsImageReadyMacro(name, type,tablename) \
virtual  bool Is##name##Ready()=0;

#define tttSetImageDirtyMacro(name) \
virtual  void Set##name##Dirty()  \
{										\
	m_##name##Dirty=false; \
} \

#define tttImageMacro(name,type,tablename) \
protected: \
	tttDefineImageMacro(name,type); \
	tttLoadImageMacro(name,type,tablename); \
	tttStoreImageMacro(name,type,tablename); \
public: \
	tttGetImageMacro(name,type); \
	tttSetImageMacro(name,type); \
	tttIsImageReadyMacro(name,type,tablename); \
	tttSetImageDirtyMacro(name); \


	tttImageMacro(RawImage,RawImageType,"RawImages")
	;tttImageMacro(LateralImageVolumeSegmentedImage,RawImageType,"LateralImageVolumeSegmentedImages")
	;tttImageMacro(CLAHEDImage,RawImageType,"CLAHEDImages")
	;tttImageMacro(SurfaceSegmentedImage,RawImageType,"SurfaceSegmentedImages")
	;tttImageMacro(DiffusedImage,DiffusedImageType,"DiffusedImages")
	;tttImageMacro(PlatenessImage,PlatenessImageType,"PlatenessImages")
	;tttImageMacro(VertexnessImage,VertexnessImageType,"VertexnessImages")
	;

#undef tttGetImageMacro
#undef tttImageMacro
#undef tttDefineImageMacro
#undef tttLoadImageMacro
#undef tttStoreImageMacro
#undef tttIsImageReadyMacro
#undef tttSetImageMacro
#if 0
protected:
	RawImageType::Pointer m_RawImage;
	bool m_RawImageLoaded=false;
	bool m_RawImageDirty=false;
public:
	RawImageType::Pointer GetRawImage(){
		if(!this->m_RawImageLoaded){
			this->LoadRawImage();
		}
		return m_RawImage;
	}
	void SetRawImage(const RawImageType::Pointer & rawImage){
		m_RawImage=rawImage;
		m_RawImageDirty=true;
		m_RawImageLoaded=true;
	}
protected:
	virtual void StoreRawImage()=0;
	virtual void LoadRawImage()=0;
	virtual bool IsRawImageReady()=0;
#endif
	/////////////////////////////////// Vertex Locations/////////////////////////////////////////////
protected:
	ttt::AdherensJunctionVertices::Pointer m_VertexLocations;
	bool m_VertexLocationsDirty = false;
	bool m_VertexLocationsLoaded = false;

public:
	virtual ttt::AdherensJunctionVertices::Pointer GetVertexLocations() {
		if (!m_VertexLocationsLoaded) LoadVertexLocations();
			return m_VertexLocations;
	}

	virtual bool IsVertexLocationsReady()=0;
	void SetVertexLocations(const ttt::AdherensJunctionVertices::Pointer & vertexLocations) {
			m_VertexLocations = vertexLocations;
			m_VertexLocationsDirty = true;
			m_VertexLocationsLoaded = true;
	}
	inline void SetVertexLocationsDirty() {
		this->StoreVertexLocations();
		m_VertexLocationsDirty = true;
	}
protected:
	virtual void StoreVertexLocations()=0;
	virtual void LoadVertexLocations()=0;

	//////////////////////////////////Tissue Descriptor/////////////////////////////////////////////
protected:
	TissueDescriptorType::Pointer m_TissueDescriptor;
	bool m_TissueDescriptorDirty = false;
	bool m_TissueDescriptorLoaded=false;

protected:
	virtual void LoadTissueDescriptor()=0;
	virtual void StoreTissueDescriptor()=0;
public:
	inline typename TissueDescriptorType::Pointer GetTissueDescriptor() {
		if (!m_TissueDescriptorLoaded)
			LoadTissueDescriptor();
		return m_TissueDescriptor;
	}

	void SetTissueDescriptor(const TissueDescriptor::Pointer & descriptor) {
		m_TissueDescriptor = descriptor;
		m_TissueDescriptorDirty = true;
		StoreTissueDescriptor();
	}

	void SetTissueDescriptorDirty() {
		m_TissueDescriptorDirty = true;
	}

	virtual bool IsTissueDescriptorReady()=0;

	//////////////////////////////??TRACKED TISSUE DESCRIPTOR/////////////////////////////////////
protected:
	TrackedTissueDescriptorType::Pointer m_TrackedTissueDescriptor;

	bool m_TrackedTissueDescriptorDirty = false;
	bool m_TrackedTissueDescriptorLoaded=false;

	virtual void StoreTrackedTissueDescriptor()=0;
	virtual void LoadTrackedTissueDescriptor()=0;

public:
	virtual bool IsTrackedTissueDescriptorReady()=0;
	inline typename TrackedTissueDescriptorType::Pointer GetTrackedTissueDescriptor() {
		if(!m_TrackedTissueDescriptorLoaded) LoadTrackedTissueDescriptor();
		return m_TrackedTissueDescriptor;
	}
	virtual void SetTrackedTissueDescriptor(
				TrackedTissueDescriptorType::Pointer trackedTissueDescriptor) {
			m_TrackedTissueDescriptor = trackedTissueDescriptor;
			m_TrackedTissueDescriptorDirty = true;
			m_TrackedTissueDescriptorLoaded = true;
	}

	//////////////TRACKED CENTROIDS
protected:

	FeatureMap<ttt::TrackedCellVertexType,itk::Point<double,3> > m_TrackedCentroids;
	bool m_TrackedCentroidsDirty=false;
	bool m_TrackedCentroidsLoaded=false;

	virtual void StoreTrackedCentroids()=0;
	virtual void LoadTrackedCentroids()=0;
	virtual bool IsTrackedCentroidsReady()=0;

public:
	inline FeatureMap<ttt::TrackedCellVertexType,itk::Point<double,3> > GetTrackedCentroids(){
		if(!m_TrackedTissueDescriptorLoaded) LoadTrackedCentroids();
			return m_TrackedCentroids;
	}

	virtual void SetTrackedCentroids(
				TrackedTissueDescriptorType::Pointer trackedTissueDescriptor) {
			m_TrackedTissueDescriptor = trackedTissueDescriptor;
			m_TrackedTissueDescriptorDirty = true;
			m_TrackedTissueDescriptorLoaded = true;
	}

protected:
	FeatureMap<ttt::TrackedCellVertexType,double> m_TrackedAreas;
	bool m_TrackedAreasDirty=false;
	bool m_TrackedAreasLoaded=false;

protected:
	FeatureMap<ttt::TrackedCellVertexType,double> m_TrackedPerimeters;
	bool m_TrackedPerimetersDirty=false;
	bool m_TrackedPerimetersLoaded=false;
protected:
	FeatureMap<ttt::TrackedCellVertexType,double> m_TrackedWidths;
	bool m_TrackedWidthsDity=false;
	bool m_TrackedWidthsLoaded=false;

protected:
	FeatureMap<ttt::TrackedCellVertexType,double> m_TrackedHeights;
	bool m_TrackedHeightsDirty=false;
	bool m_TrackedHeightsLoaded=false;

protected:
	FeatureMap<ttt::TrackedCellVertexType,double> m_TrackedRotations;
	bool m_TrackedRotationsDirty=false;
	bool m_TrackedRotationsLoaded=false;

protected:
	FeatureMap<ttt::TrackedCellVertexType,itk::Vector<double,3> > m_TrackedVelocities;
	bool m_TrackedVelocitiesDirty=false;
	bool m_TrackedVelocitiesLoaded=false;
	////////////////////////////TRACKED ELLIPSES/////////////////////////////////
protected:
	EllipseMapTypePointer m_TrackedEllipses;
	bool m_TrackedEllipsesDirty=false;
	bool m_TrackedEllipsesReady=false;

	virtual void LoadTrackedEllipses()=0;
	virtual void StoreTrackedEllipses()=0;


public:
	virtual bool IsTrackedEllipsesReady()=0;
	EllipseMapTypePointer GetTrackedEllipses() {
		if(!m_TrackedEllipsesDirty)this->LoadTrackedEllipses();
		return m_TrackedEllipses;
	}

	void SetTrackedEllipses(const EllipseMapTypePointer & trackedEllipses) {
		m_TrackedEllipses = trackedEllipses;
		m_TrackedEllipsesDirty = true;
		m_TrackedEllipsesReady = true;
		//this->StoreTrackedEllipses();
	}

	////////////////////////////TRACKED DOMAINS/////////////////////////////////
protected:
	TrackedDomainVectorTypePointer m_TrackedDomains;
	bool m_TrackedDomainsDirty;
	bool m_TrackedDomainsReady;

	virtual void LoadTrackedDomains()=0;
	virtual void StoreTrackedDomains()=0;

public:
	virtual bool IsTrackedDomainsReady()=0;
	TrackedDomainVectorTypePointer GetTrackedDomains() {
		LoadTrackedDomains();
		return m_TrackedDomains;
	}
	void SetTrackedDomains(const TrackedDomainVectorTypePointer & domains) {
		m_TrackedDomains = domains;
		m_TrackedDomainsDirty = true;
		StoreTrackedDomains();
	}
protected:
	DomainStrainRatesMapTypePointer m_DomainStrainRates;
	bool m_DomainStrainRatesDirty=false;
	bool m_DomainStrainRatesReady=false;

	virtual void LoadDomainStrainRates()=0;
	virtual void StoreDomainStrainRates()=0;
public:
	virtual bool IsDomainStrainRatesReady()=0;

	DomainStrainRatesMapTypePointer GetDomainStrainRates() {
		if(!m_DomainStrainRatesReady)this->LoadDomainStrainRates();
		//return 	boost::shared_ptr<	DomainStrainRatesMapType>(new DomainStrainRatesMapType);;
		return m_DomainStrainRates;

	}
	void SetDomainStrainRates(const DomainStrainRatesMapTypePointer & domainStrainRates) {
		m_DomainStrainRates = domainStrainRates;
		this->StoreDomainStrainRates();
		m_DomainStrainRatesDirty=true;
		 m_DomainStrainRatesReady=true;
	}

};
}


#endif /* TISSUETRACKINGABSTRACTPROJECT_H_ */
