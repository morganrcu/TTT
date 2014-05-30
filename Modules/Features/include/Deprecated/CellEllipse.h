/*
 * CellEllipse.h
 *
 *  Created on: Apr 25, 2014
 *      Author: morgan
 */

#ifndef CELLELLIPSE_H_
#define CELLELLIPSE_H_


#include <map>
#include <itkPoint.h>
#include "tttDescriptionDataTypes.h"
#include "TissueDescriptorFeature.h"
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include "Ellipse.h"
#include "fitEllipse.h"
#include "Feature.h"

template<class T> int CalcEllipseValues(std::vector<itk::Point<T, 3> > &pts, ttt::Ellipse<T> & result) {

	std::vector<T> elval = fitEllipse(pts);

	if(elval[2]<0 || elval[3]<0) return -1;

	result.m_Xcent = elval[0];
	result.m_Ycent = elval[1];
	result.m_Xrad = sqrt(elval[2]);
	result.m_Yrad = sqrt(elval[3]);
	result.m_Theta = elval[4];

//	std::cout << "(" << result.m_Xcent << " " <<  result.m_Ycent<< " " <<  result.m_Xrad<< " "<<  result.m_Yrad<< " " << result.m_Theta << ")" << std::endl;

	if(result.m_Xcent!=result.m_Xcent || result.m_Ycent!=result.m_Ycent || result.m_Xrad!=result.m_Xrad || result.m_Yrad!=result.m_Yrad || result.m_Theta!= result.m_Theta){
		for(unsigned int i=0;i<pts.size(); i++){
	//		std::cout << "\t" << pts[i] <<std::endl;
		}
	}
}


template<class T> class CellEllipse : public TissueDescriptorFeature<T,typename ttt::TissueDescriptorTraits<T>::CellVertexType, ttt::Ellipse<double> > {

public:
	typedef CellEllipse<T> Self;
	typedef boost::shared_ptr<Self> Pointer;

	typedef itk::Point<double,3> PointType;

	typedef typename ttt::TissueDescriptorTraits<T>::CellPropertyTagType CellPropertyTagType;
	typedef typename ttt::TissueDescriptorTraits<T>::SkeletonPointPropertyTag SkeletonPointPropertyTag;
	typedef typename ttt::TissueDescriptorTraits<T>::CellType CellType;

	static Pointer New(){
		return Pointer(new Self);
	}

	ttt::Ellipse<double> GetEllipse(const CellType  & cell){


			typedef typename ttt::TissueDescriptorTraits<T>::CellPropertyTagType CellPropertyTagType;
			typedef typename ttt::TissueDescriptorTraits<T>::SkeletonPointPropertyTag SkeletonPointPropertyTag;

			//std::vector<SkeletonVertexType> svt = boost::get(CellPropertyTagType(),*this->GetTissueDescriptor()->m_CellGraph,*it).m_SkeletonNodes;

			std::vector<itk::Point<double,3> > points;

			//for(std::vector<SkeletonVertexType>::iterator itr = svt.begin(); itr != svt.end(); ++itr){
			for(typename CellType::PerimeterIterator itr = cell.PerimeterBegin(); itr != cell.PerimeterEnd(); ++itr){
				itk::Point<double,3> pos = boost::get(SkeletonPointPropertyTag(),*this->GetTissueDescriptor()->m_SkeletonGraph,*itr).position;
				points.push_back(pos);
			}

			bool done=false;
			ttt::Ellipse<double> newEllipse;
			if(points.size()>3){
				if(CalcEllipseValues(points,newEllipse)!=-1){
					done=true;
				}
			}
			if(!done){
				itk::Point<double,3> center;
				center.Fill(0);

				for(std::vector<itk::Point<double,3> >::iterator it= points.begin();it!=points.end();it++){
					for(int i=0;i<3;i++) center[i]=center[i]+(*it)[i];
				}
				for(int i=0;i<3;i++) center[i]=center[i]/points.size();

				double radius=0;

				for(std::vector<itk::Point<double,3> >::iterator it= points.begin();it!=points.end();it++){
					itk::Vector<double,3> diff = *it - center;
					radius+=sqrt(pow(diff[0],2)+ pow(diff[1],2));
				}
				radius=radius/points.size();
				newEllipse.m_Theta=0;
				newEllipse.m_Xcent=center[0];
				newEllipse.m_Ycent=center[1];
				newEllipse.m_Xrad=radius;
				newEllipse.m_Yrad=radius;
			}
			return newEllipse;
	}




	void Compute(){

		typename boost::graph_traits<typename T::DualGraphType>::vertex_iterator it,it_end;
		boost::tie(it,it_end)=boost::vertices(*(this->GetTissueDescriptor()->m_CellGraph));

		for(;it!=it_end;++it){

			CellType currentCell =boost::get(CellPropertyTagType(),*this->GetTissueDescriptor()->m_CellGraph,*it);

			this->Insert(*it,this->GetEllipse(currentCell));
		}
	}
};







#endif /* CELLELLIPSE_H_ */
