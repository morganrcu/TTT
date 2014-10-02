/*
 * giaaCellTracker.h
 *
 *  Created on: 17/11/2011
 *      Author: rod
 */
#include <boost/numeric/ublas/matrix.hpp>
#include "itkImage.h"
#include "itkMacro.h"
#include "tttDescriptionDataTypes.h"
#pragma once
using namespace itk;

namespace ttt {
/**
 * TODO Comment
 */
class CellTracker: public itk::ProcessObject {

public:
	typedef CellTracker Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer< const Self> ConstPointer;

	itkNewMacro(Self);
    virtual void SetObservations(const std::vector<TissueDescriptor::Pointer> & observation) {
        m_Observations = observation;
    }

    std::vector<TrackedTissueDescriptor::Pointer> GetTracks() {
        return m_Tracks;
    }

    virtual void Track();

    CellTracker() : m_NextID(0) {

    }
protected:

    std::vector<TrackedTissueDescriptor::Pointer> m_Tracks;
    int m_NextID;
    std::vector<TissueDescriptor::Pointer> m_Observations;

};

}
