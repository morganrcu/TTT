
#ifndef CELLORDER_H_
#define CELLORDER_H_

#include "TissueDescriptorFeature.h"

template<class T> class CellOrder : public TissueDescriptorFeature<T,typename ttt::TissueDescriptorTraits<T>::CellVertexType, int> {

public:
	typedef CellOrder<T> Self;
	typedef boost::shared_ptr<Self> Pointer;

	typedef typename ttt::TissueDescriptorTraits<TissueDescriptor>::CellVertexType ObjectType;
	typedef int ValueType;

public:
	static Pointer New(){
		return Pointer(new Self);
	}

	void Compute(){
		typename boost::graph_traits<typename T::DualGraphType>::vertex_iterator it,it_end;
		boost::tie(it,it_end)=boost::vertices(*(this->GetTissueDescriptor()->m_CellGraph));

		for(;it!=it_end;++it){

			this->Insert(*it,boost::degree(*it,*(this->GetTissueDescriptor()->m_CellGraph)));
		}

	}
};


#endif /* CELLORDER_H_ */
