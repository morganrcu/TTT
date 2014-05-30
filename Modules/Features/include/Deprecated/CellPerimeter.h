/*
 * CellPerimeter.h
 *
 *  Created on: Apr 30, 2014
 *      Author: morgan
 */

#ifndef CELLPERIMETER_H_
#define CELLPERIMETER_H_



#include <map>
#include <itkPoint.h>
#include "tttDescriptionDataTypes.h"
#include "TissueDescriptorFeature.h"
#include <vnl/algo/vnl_symmetric_eigensystem.h>


template<class T> class CellPerimeter : public TissueDescriptorFeature<T,typename ttt::TissueDescriptorTraits<T>::CellVertexType, double> {

public:
	typedef CellPerimeter<T> Self;
	typedef boost::shared_ptr<Self> Pointer;

	typedef itk::Point<double,3> PointType;

	typedef typename ttt::TissueDescriptorTraits<T>::CellPropertyTagType CellPropertyTagType;
	typedef typename ttt::TissueDescriptorTraits<T>::SkeletonPointPropertyTag SkeletonPointPropertyTag;
	typedef typename ttt::TissueDescriptorTraits<T>::CellType CellType;

	static Pointer New(){
		return Pointer(new Self);
	}

	double GetPerimeter(const CellType  & cell){


		typedef typename ttt::TissueDescriptorTraits<T>::CellPropertyTagType CellPropertyTagType;
		typedef typename ttt::TissueDescriptorTraits<T>::SkeletonPointPropertyTag SkeletonPointPropertyTag;

		double perimeter=0;

		typename ttt::TissueDescriptorTraits<T>::CellType::PerimeterIterator itSkel,itSkelNext;

		itSkel =cell.PerimeterBegin();
		itSkelNext=cell.PerimeterBegin();
		++itSkelNext;
		while(itSkelNext!=cell.PerimeterEnd()){
			itk::Point<double,3> A= boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*itSkel).position;
			itk::Point<double,3> B= boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*itSkelNext).position;

			perimeter+=(A-B).GetNorm();
			++itSkel;
			++itSkelNext;
		}
		itSkelNext=cell.PerimeterBegin();
		itk::Point<double,3> A= boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*itSkel).position;
		itk::Point<double,3> B= boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*itSkelNext).position;
		perimeter+=(A-B).GetNorm();
		return perimeter;
	}

	void Compute(){

		typename boost::graph_traits<typename T::DualGraphType>::vertex_iterator it,it_end;
		boost::tie(it,it_end)=boost::vertices(*(this->GetTissueDescriptor()->m_CellGraph));

		for(;it!=it_end;++it){

			CellType currentCell =boost::get(CellPropertyTagType(),*this->GetTissueDescriptor()->m_CellGraph,*it);

			this->Insert(*it,this->GetPerimeter(currentCell));
		}
	}
};












#endif /* CELLPERIMETER_H_ */
