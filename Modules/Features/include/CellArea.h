
#ifndef CELLAREA_H_
#define CELLAREA_H_

#include "tttDescriptionDataTypes.h"
#include <map>
#include "TissueDescriptorFeature.h"
template<class T> class CellArea : public TissueDescriptorFeature<T,typename ttt::TissueDescriptorTraits<T>::CellVertexType, double>  {

public:
	typedef CellArea<T> Self;
	typedef boost::shared_ptr<Self> Pointer;

	static Pointer New(){
		return Pointer(new Self);
	}

	void Compute(){
		typename boost::graph_traits<typename T::DualGraphType>::vertex_iterator it,it_end;
		boost::tie(it,it_end)=boost::vertices(*(this->GetTissueDescriptor()->m_CellGraph));

		for(;it!=it_end;++it){
			double area=0;
			typedef typename ttt::TissueDescriptorTraits<T>::CellPropertyTagType CellPropertyTagType;
			typedef typename ttt::TissueDescriptorTraits<T>::SkeletonPointPropertyTag SkeletonPointPropertyTag;
			typename ttt::TissueDescriptorTraits<T>::CellType cell =boost::get(CellPropertyTagType(),*(this->GetTissueDescriptor()->m_CellGraph),*it);

			itk::Point<double,3> C = cell.m_Centroid;


			typename std::vector<ttt::SkeletonVertexType>::const_iterator itSkel,itSkelNext;

			itSkel =cell.Begin();
			itSkelNext=cell.Begin();
			++itSkelNext;
			while(itSkelNext!=cell.End()){

				itk::Point<double,3> A= boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*itSkel).position;
				itk::Point<double,3> B= boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*itSkelNext).position;
				area+=abs((B[0]*A[1]-A[0]*B[1])+(C[0]*B[1]-B[0]*C[1])+(A[0]*C[1]-C[0]*A[1]))/2;
				++itSkel;
				++itSkelNext;
			}
			itSkelNext=cell.Begin();
			itk::Point<double,3> A= boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*itSkel).position;
			itk::Point<double,3> B= boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*itSkelNext).position;
			area+=abs((B[0]*A[1]-A[0]*B[1])+(C[0]*B[1]-B[0]*C[1])+(A[0]*C[1]-C[0]*A[1]))/2;

			this->Insert(*it,area);

		}


	}

};



#endif /* CELLAREA_H_ */
