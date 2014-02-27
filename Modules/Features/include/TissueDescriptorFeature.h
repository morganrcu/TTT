/*
 * TissueDescriptorFeature.h
 *
 *  Created on: Dec 12, 2013
 *      Author: morgan
 */

#ifndef TISSUEDESCRIPTORFEATURE_H_
#define TISSUEDESCRIPTORFEATURE_H_

#include "Feature.h"

template<class TissueDescriptorType,class ObjectType,class ValueType> class TissueDescriptorFeature : public Feature<ObjectType,ValueType>{

protected:
	typename TissueDescriptorType::Pointer m_TissueDescriptor;

public:
	virtual void SetTissueDescriptor(const typename TissueDescriptorType::Pointer & tissueDescriptor){
		m_TissueDescriptor=tissueDescriptor;
	}
	virtual typename TissueDescriptorType::Pointer GetTissueDescriptor(){
		return m_TissueDescriptor;
	}

};

#endif /* TISSUEDESCRIPTORFEATURE_H_ */
