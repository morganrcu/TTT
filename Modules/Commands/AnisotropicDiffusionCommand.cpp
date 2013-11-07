#include "AnisotropicDiffusionCommand.h"


void AnisotropicDiffusionCommand::Do(){

	m_Diffuser->SetSigmaMin(m_DiffusionSigmaMin);
	m_Diffuser->SetSigmaMax(m_DiffusionSigmaMax);
	m_Diffuser->SetNumberOfSigmaSteps(m_DiffusionSigmaSteps);
	m_Diffuser->SetNumberOfIterations(m_DiffusionIterations);

	m_Diffuser->SetInput(m_Input);
	m_Diffuser->Update();
	m_Output=m_Diffuser->GetOutput();

}
