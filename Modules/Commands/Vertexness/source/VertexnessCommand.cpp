#include "VertexnessCommand.h"

void ttt::VertexnessCommand::Do(){


	typedef itk::VertexnessMeasurementFunction VertexnessFunctionType;
	VertexnessFunctionType::Pointer vertexnessFunction = VertexnessFunctionType::New();

	m_Vertexness->SetObjectnessMeasurementFunction(vertexnessFunction);
	m_Vertexness->SetSigmaMin(m_SigmaMin);
	m_Vertexness->SetSigmaMax(m_SigmaMax);
	m_Vertexness->SetNumberOfSigmaSteps(m_SigmaSteps);
	m_Vertexness->SetInput(m_Input);
	m_Vertexness->Update();
	m_VertexnessRescaler->SetInput(m_Vertexness->GetOutput());
	m_VertexnessRescaler->Update();

	m_VertexnessImage=m_VertexnessRescaler->GetOutput();

}
