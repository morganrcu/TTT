#include "AnisotropicDiffusionCommand.h"
#include "tttPlatenessMeasurementFunction.h"

void ttt::AnisotropicDiffusionCommand::Do(){

	typedef PlatenessMeasurementFunction PlatenessMeasurementFunctionType;

	PlatenessMeasurementFunctionType::Pointer platenessMeasurementFunction=PlatenessMeasurementFunctionType::New();
	m_Diffuser->SetPlatenessMeasurementFunction(platenessMeasurementFunction);
	m_Diffuser->SetSigmaMin(m_DiffusionSigmaMin);
	m_Diffuser->SetSigmaMax(m_DiffusionSigmaMax);
	m_Diffuser->SetNumberOfSigmaSteps(m_DiffusionSigmaSteps);
	m_Diffuser->SetNumberOfIterations(m_DiffusionIterations);

	m_Diffuser->SetInput(m_Input);
	m_Diffuser->Update();
	m_Output=m_Diffuser->GetOutput();

}
