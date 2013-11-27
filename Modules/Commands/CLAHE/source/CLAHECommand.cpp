#include "CLAHECommand.h"

namespace ttt {

CLAHECommand::CLAHECommand() {

	m_AdaptiveHistogramEqualizationImageFilter = CLAHEImageFilterType::New();
	m_MaxSlope=-1;
	m_Radius=-1;
}

CLAHECommand::~CLAHECommand() {

}

void CLAHECommand::Do() {
	assert(m_MaxSlope>0);
	assert(m_Radius>0);
	assert(m_InputImage);
	m_AdaptiveHistogramEqualizationImageFilter->SetInput(m_InputImage);
	m_AdaptiveHistogramEqualizationImageFilter->SetRadius(m_Radius);
	m_AdaptiveHistogramEqualizationImageFilter->SetMaxSlope(m_MaxSlope);
	m_AdaptiveHistogramEqualizationImageFilter->Update();

	m_OutputImage=m_AdaptiveHistogramEqualizationImageFilter->GetOutput();
}


} /* namespace ttt */
