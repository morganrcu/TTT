#ifndef EDGEORIENTATION_H_
#define EDGEORIENTATION_H_

#include "tttDescriptionDataTypes.h"
#include "TissueDescriptorFeature.h"
template<class TissueDescriptor> class AdherensJunctionEdgeOrientation : public TissueDescriptorFeature<TissueDescriptor,typename ttt::TissueDescriptorTraits<TissueDescriptor>::SkeletonEdgeType,double> {


public:
	typedef AdherensJunctionEdgeOrientation<TissueDescriptor> Self;
	typedef boost::shared_ptr<Self> Pointer;

	typedef typename ttt::TissueDescriptorTraits<TissueDescriptor>::SkeletonEdgeType ObjectType;
	typedef int ValueType;

	static Pointer New(){
		return Pointer(new Self);
	}

	void Compute(){
		typedef itk::Point<double, 3> itkpt;
		typename boost::graph_traits<typename TissueDescriptor::PrimalGraphType>::edge_iterator it,it_end;
		boost::tie(it,it_end)=boost::edges(*(this->GetTissueDescriptor()->m_SkeletonGraph));

		for(;it!=it_end;++it){

			typename ttt::TissueDescriptorTraits<TissueDescriptor>::SkeletonVertexType src = boost::source(*it, *(this->GetTissueDescriptor()->m_SkeletonGraph));
			typename ttt::TissueDescriptorTraits<TissueDescriptor>::SkeletonVertexType tgt = boost::target(*it, *(this->GetTissueDescriptor()->m_SkeletonGraph));
			typedef typename ttt::TissueDescriptorTraits<TissueDescriptor>::SkeletonPointPropertyTag SkeletonPointPropertyTag;
			itkpt a = boost::get(typename ttt::TissueDescriptorTraits<TissueDescriptor>::SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),src).position;
			itkpt b = boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),tgt).position;

			//double slope = (a[1] - b[1]) / (a[0] - b[0]);
			double deg = (180.0 / M_PI) * (atan2(abs(a[1]-b[1]),abs(a[0]-b[0])));


			this->Insert(*it,deg);
			std::cout << deg << std::endl;
			//if (slope < 0) {
			//	deg = 180.0 - deg;
			//}

		}

	}
};



#endif /* EDGEORIENTATION_H_ */
