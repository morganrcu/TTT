
#ifndef CELLREGULARITY_H_
#define CELLREGULARITY_H_
#include "tttDescriptionDataTypes.h"
#include <map>

template<class T> class CellRegularity : public TissueDescriptorFeature<T,ttt::TissueDescriptorTraits<T>::CellVertex, double>  {

private:
	typename T::Pointer m_TissueDescriptor;
public:
	void SetTissueDescriptor(const typename T::Pointer & tissueDescriptor){
		m_TissueDescriptor=tissueDescriptor;
	}

	void Compute(){
		//Compute regularity of each feature
	}
};



#endif /* CELLREGULARITY_H_ */
