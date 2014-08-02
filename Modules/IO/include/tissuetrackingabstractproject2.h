/*
 * tissuetrackingabstractproject.h
 *
 *  Created on: Apr 25, 2014
 *      Author: morgan
 */

#ifndef TISSUETRACKINGABSTRACTPROJECT2_H_
#define TISSUETRACKINGABSTRACTPROJECT2_H_
#include "itkImage.h"
#include "tttDescriptionDataTypes.h"
#include <vector>
namespace ttt{

class TissueTrackingAbstractProject2{
public:

	typedef itk::Image<float,3> FloatImageType;
	typedef FloatImageType::SpacingType SpacingType;
	typedef FloatImageType::SizeType SizeType;
	typedef FloatImageType RawImageType;
	typedef FloatImageType DiffusedImageType;
	typedef FloatImageType PlatenessImageType;
	typedef FloatImageType VertexnessImageType;
protected:
	std::string m_ProjectName;
	SpacingType m_Spacing;
	SizeType m_Size;
	double m_SamplingPeriod;
	std::vector<int> m_PlatenessSteps;
	std::vector<double> m_HighestPlatenessScale;
	std::vector<double> m_LowestPlatenessScale;

	std::vector<int> m_VertexnessSteps;
	std::vector<double> m_HighestVertexnessScale;
	std::vector<double> m_LowestVertexnessScale;

	int m_NumFrames;

public:
	inline void SetSize(const SizeType & size){
		m_Size=size;
	}
	inline SizeType GetSize(){
		return m_Size;
	}
	/**
	 * Sets the physical voxels size of micrometers for the projects
	 * @param spacing a itk::FixedArray<double,3>  containing voxel sizes in micrometers
	 */
	inline void SetSpacing(const SpacingType & spacing){
		m_Spacing=spacing;
		this->StoreProjectInfo();
	}
	/**
	 * Function to obtain the physical voxel size in micrometers
	 * @return an itk::FixedArray<double,3> containing voxel sizes in micrometers
	 */
	inline SpacingType GetSpacing(){
		return m_Spacing;
	}
	inline void SetSpacing(int dim,double value){
		m_Spacing[dim]=value;
		this->StoreProjectInfo();
	}
	/**
	 * Function to set up the name of the project
	 * @param name an std::string with the name of the project
	 */
	inline void SetProjectName(const std::string & projectName){
		m_ProjectName=projectName;
		this->StoreProjectInfo();
	}
	/**
	 * Function to obtain the name of the project
	 * @return an std::string containing the name of the project
	 */
	inline std::string GetProjectName(){
		return m_ProjectName;
	}
	/**
	 * Function to set the sampling period of the time lapse
	 * @param samplingPeriod a positive real value with the time passed between frames in the time-lapse
	 */
	inline void SetSamplingPeriod(double samplingPeriod){
		m_SamplingPeriod=samplingPeriod;
		this->StoreProjectInfo();
	}
	/**
	 * Function to get the sampling period of the time lapse
	 * @return a positive real value with the time passed between frames in the time-lapse
	 */
	inline double GetSamplingPeriod(){
		return m_SamplingPeriod;
	}
	/**
	 * Function to obtain the number of sampling points of the scale space employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness steps value is to be obtained
	 * @return a positive integer for the number of sampling points.
	 */
	inline int GetPlatenessSteps(int frame){
		return m_PlatenessSteps[frame];
	}
	/**
	 * Function to set the number of sampling points of the scale space employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness steps value wants to be set
	 * @param platenessSteps a positive integer for the number of sampling points.
	 */
	inline void SetPlatenessSteps(int frame,int platenessSteps){
		m_PlatenessSteps[frame]=platenessSteps;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the highest scale employed of the interval of scales employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness highest value wants to be obtained
	 * @return a double positive value with the highest scale employed
	 */
	inline double GetHighestPlatenessScale(int frame){
		return m_HighestPlatenessScale[frame];
	}
	/**
	 * Function to set the highest scale employed of the interval of scales employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness highest value wants to be set
	 * @param highestPlatenessStep a double positive value with the highest scale employed for plateness detection
	 */
	inline void SetHighestPlatenessScale(int frame,double highestPlatenessStep){
		m_HighestPlatenessScale[frame]=highestPlatenessStep;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the lowest scale employed of the interval of scales employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness lowest value wants to be set
	 * @return a double positive value with the lowest scale employed
	 */
	inline double GetLowestPlatenessScale(int frame){
		return m_LowestPlatenessScale[frame];
	}
	/**
	 * Function to set the lowest scale employed of the interval of scales employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness lowest value wants to be obtained
	 * @param lowestPlatenessStep a double positive value with the lowest scale employed for plateness detection
	 */
	inline void SetLowestPlatenessScale(int frame,double lowestPlatenessStep){
		m_LowestPlatenessScale[frame]=lowestPlatenessStep;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the number of sampling points of the scale space employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness steps value is to be obtained
	 * @return a positive integer for the number of sampling points.
	 */
	inline int GetVertexnessSteps(int frame){
		return m_VertexnessSteps[frame];
	}
	/**
	 * Function to set the number of sampling points of the scale space employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness steps value wants to be set
	 * @param vertexnessSteps a positive integer for the number of sampling points.
	 */
	inline void SetVertexnessSteps(int frame,int vertexnessSteps){
		m_VertexnessSteps[frame]=vertexnessSteps;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the highest scale employed of the interval of scales employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness highest value wants to be obtained
	 * @return a double positive value with the highest scale employed
	 */
	inline double GetHighestVertexnessScale(int frame){
		return m_HighestVertexnessScale[frame];
	}
	/**
	 * Function to set the highest scale employed of the interval of scales employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness highest value wants to be set
	 * @param highestVertenessStep a double positive value with the highest scale employed for vertexness detection
	 */
	inline void SetHighestVertexnessScale(int frame,double highestVertexnessStep){
		m_HighestVertexnessScale[frame]=highestVertexnessStep;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the lowest scale employed of the interval of scales employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness lowest value wants to be set
	 * @return a double positive value with the lowest scale employed
	 */
	inline double GetLowestVertexnessScale(int frame){
		return m_LowestVertexnessScale[frame];
	}
	/**
	 * Function to set the lowest scale employed of the interval of scales employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness lowest value wants to be obtained
	 * @param lowestVertexnessStep a double positive value with the lowest scale employed for vertexness detection
	 */
	inline void SetLowestVertexnessScale(int frame,double lowestVertexnessStep){
		m_LowestVertexnessScale[frame]=lowestVertexnessStep;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the number of frames contained in the time-lapse stored in the project
	 * @return a positive integer with the number of frames in the project
	 */
	unsigned int GetNumFrames(){
		return m_NumFrames;
	}
	/**
	 * Resizes the project to contain the specified number of frames. Use with caution
	 * @numFrames new number of frames the project must content
	 */
	void SetNumFrames(unsigned int numFrames){
		assert(numFrames>=0);
		this->m_NumFrames=numFrames;
		m_PlatenessSteps.resize(numFrames);
		m_HighestPlatenessScale.resize(numFrames);
		m_LowestPlatenessScale.resize(numFrames);

		m_VertexnessSteps.resize(numFrames);
		m_HighestVertexnessScale.resize(numFrames);
		m_LowestVertexnessScale.resize(numFrames);


		for(int i=0;i<numFrames;i++){
			this->StoreFrameInfo(i);
		}
		this->StoreProjectInfo();
	}

	virtual typename RawImageType::Pointer GetRawImage(unsigned int frame)=0;
	virtual void SetRawImage(unsigned int frame,const typename RawImageType::Pointer & image)=0;
	virtual bool IsRawImageAvailable(unsigned int frame)=0;

	virtual typename DiffusedImageType::Pointer GetDiffusedImage(unsigned int frame)=0;
	virtual void SetDiffusedImage(unsigned int frame,const typename DiffusedImageType::Pointer & image)=0;
	virtual bool IsDiffusedImageAvailable(unsigned int frame)=0;

	virtual typename PlatenessImageType::Pointer GetPlatenessImage(unsigned int frame)=0;
	virtual void SetPlatenessImage(unsigned int frame,const typename PlatenessImageType::Pointer & image)=0;
	virtual bool IsPlatenessImageAvailable(unsigned int frame)=0;

	virtual typename VertexnessImageType::Pointer GetVertexnessImage(unsigned int frame)=0;
	virtual void SetVertexnessImage(unsigned int frame,const typename VertexnessImageType::Pointer & image)=0;
	virtual bool IsVertexnessImageAvailable(unsigned int frame)=0;



	virtual typename ttt::AdherensJunctionVertices::Pointer GetAdherensJunctionVertices(unsigned int frame)=0;
	virtual void SetAdherensJunctionVertices(unsigned int frame,const typename ttt::AdherensJunctionVertices::Pointer & vertices)=0;
	virtual bool IsAdherensJunctionVerticesAvailable(unsigned int frame)=0;

	virtual typename  ttt::TissueDescriptor::Pointer GetTissueDescriptor(unsigned int frame)=0;
	virtual void SetTissueDescriptor(unsigned int frame,const typename ttt::TissueDescriptor::Pointer & descriptor)=0;
	virtual bool IsTissueDescriptorAvailable(unsigned int frame)=0;

	virtual typename  ttt::TrackedTissueDescriptor::Pointer GetTrackedTissueDescriptor(unsigned int frame)=0;
	virtual void SetTrackedTissueDescriptor(unsigned int frame,const typename ttt::TrackedTissueDescriptor::Pointer & descriptor)=0;
	virtual bool IsTrackedTissueDescriptorAvailable(unsigned int frame)=0;

	/**
	 * Opens the project, calling the backend to load metadata information
	 */
	virtual void Open(){
		LoadProjectInfo();
		this->SetNumFrames(m_NumFrames);
		for(int t=0;t<m_NumFrames;t++){
			LoadFrameInfo(t);
		}
	}

	/**
	 * Closes the project
	 */
	virtual void Close(){
		//TODO
	}
	/**
	 * Commits all the pending changes to the storage
	 */
	virtual void Flush(){
		//TODO
	}
#if 0
public:
	template<class ObjectType> void SetObject(const std::string & name,unsigned int frame,const typename ObjectType::Pointer & object);
#endif
protected:
	virtual void LoadProjectInfo()=0;
	virtual void StoreProjectInfo()=0;
	virtual void LoadFrameInfo(unsigned int frame)=0;
	virtual void StoreFrameInfo(unsigned int frame)=0;
#if 0
protected:

	virtual void StoreObject(const std::string & name, int frame, const typename itk::Image<float,3>::Pointer & object)=0;
	virtual void StoreObject(const std::string & name, int frame, const typename ttt::AdherensJunctionVertices::Pointer & object)=0;
	virtual void StoreObject(const std::string & name, int frame, const typename ttt::TissueDescriptor::Pointer & object)=0;
	virtual void StoreObject(const std::string & name, int frame, const typename ttt::TrackedTissueDescriptor::Pointer & object)=0;

	virtual void LoadObject(const std::string & name, int frame,itk::Image<float,3>::Pointer & object)=0;
	virtual void LoadObject(const std::string & name, int frame,const typename ttt::AdherensJunctionVertices::Pointer & object)=0;

	virtual bool ObjectAvailable(const std::string & name, unsigned int frame)=0;
#endif
#if 0




	template<class PropertyType> PropertyType GetProjectProperty(const std::string & property,unsigned int frame);
	template<class PropertyType> void SetFrameProperty(const std::string & property,unsigned int frame,const PropertyType & value);

	virtual int GetNumFrames(){

	}



	virtual void StoreObject(const std::string & name, int frame, const typename ttt::TissueDescriptor::Pointer & object)=0;
	virtual void StoreObject(const std::string & name, int frame, const typename ttt::TrackedTissueDescriptor::Pointer & object)=0;
	virtual void StoreObject(const std::string & name, int frame,ttt::AdherensJunctionVertices::Pointer & object)=0;


	virtual void LoadObject(const std::string & name, int frame,ttt::TissueDescriptor::Pointer & object)=0;
	virtual void LoadObject(const std::string & name, int frame,ttt::TrackedTissueDescriptor::Pointer & object)=0;
	virtual void LoadObject(const std::string & name, int frame,ttt::AdherensJunctionVertices::Pointer & object)=0;
#endif
};

#if 0
template<class ObjectType> void TissueTrackingAbstractProject2::SetObject(const std::string & property,unsigned int frame,const typename ObjectType::Pointer & value){
	this->StoreObject(property,frame,value);
}

template<class ObjectType> typename ObjectType::Pointer TissueTrackingAbstractProject2::GetObject(const std::string & name,unsigned int frame){
	typename ObjectType::Pointer object;
	this->LoadObject(name,frame,object);
	return object;
}
#endif

}


#endif /* TISSUETRACKINGABSTRACTPROJECT_H_ */
