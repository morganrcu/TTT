#ifndef LATERALIMAGEVOLUMESEGMENTATIONCOMMANDH_H_
#define LATERALIMAGEVOLUMESEGMENTATIONCOMMANDH_H_
#include "AppCommand.h"
#include "itkImage.h"
#include "tttLateralHistogramVolumeSegmentation.h"
#include "itkAndImageFilter.h"
namespace ttt {

class LateralImageVolumeSegmentationCommand : public AppCommand{
public:
	typedef itk::Image<unsigned char,3> InputImageType;
	typedef itk::Image<unsigned char,3> OutputImageType;
	LateralImageVolumeSegmentationCommand();
	virtual ~LateralImageVolumeSegmentationCommand();
	inline void SetRelativeThreshold(float thresh){
		m_RelativeThreshold=thresh;
	}

	void SetInputImage(const InputImageType::Pointer & input){
		m_InputImage=input;
	}
	typename OutputImageType::Pointer GetOutput(){
		return m_OutputImage;
	}
	virtual void Do();
private:
	typedef ttt::LateralHistogramVolumeSegmentationFilter<InputImageType> SegmentationFilterType;

	SegmentationFilterType::Pointer m_SegmentationFilter;

	typedef itk::AndImageFilter<OutputImageType> AndFilterType;
	AndFilterType::Pointer m_AndFilter;

	float m_RelativeThreshold;
	InputImageType::Pointer m_InputImage;

	OutputImageType::Pointer m_OutputImage;

};

} /* namespace ttt */

#endif /* LATERALIMAGEVOLUMESEGMENTATIONCOMMANDH_H_ */
