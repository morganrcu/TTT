#include "LateralImageVolumeSegmentationCommand.h"

namespace ttt {

LateralImageVolumeSegmentationCommand::LateralImageVolumeSegmentationCommand() {
	m_AndFilter =AndFilterType::New();

	m_SegmentationFilter= SegmentationFilterType::New();
	m_RelativeThreshold=-1;
}

LateralImageVolumeSegmentationCommand::~LateralImageVolumeSegmentationCommand() {

}

void LateralImageVolumeSegmentationCommand::Do() {
	assert(m_RelativeThreshold!=-1);
	assert(m_InputImage);

	m_SegmentationFilter->SetInput(m_InputImage);
	m_SegmentationFilter->SetLowerThresholdPercentage(m_RelativeThreshold);
	m_SegmentationFilter->Update();

	m_AndFilter->SetInput1(m_SegmentationFilter->GetOutput());
	m_AndFilter->SetInput2(m_InputImage);
	m_AndFilter->Update();

	m_OutputImage=m_AndFilter->GetOutput();
}


} /* namespace ttt */
