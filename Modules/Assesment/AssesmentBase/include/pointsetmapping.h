/*
 * pointsetmapping.h
 *
 *  Created on: Dec 4, 2013
 *      Author: morgan
 */

#ifndef POINTSETMAPPING_H_
#define POINTSETMAPPING_H_


template<class PointSetType,class SpacingType>void mapPointSet(const typename PointSetType::Pointer & locs,const SpacingType & spacing){

	typename PointSetType::PointsContainer::Pointer points=locs->GetPoints();
	for(typename PointSetType::PointsContainer::Iterator it=points->Begin();it!=points->End();it++){

		typename PointSetType::PointType & point = it->Value();
		point[0]*=spacing[0];
		point[1]*=spacing[1];
		point[2]*=spacing[2];
	}
	locs->SetPoints(points);
}

template<class PointSetType, class SpacingType> void unmapPointSet(const typename PointSetType::Pointer & locs,const SpacingType & spacing){

	typename PointSetType::PointsContainer::Pointer points=locs->GetPoints();
	for(typename PointSetType::PointsContainer::Iterator it=points->Begin();it!=points->End();it++){

		typename PointSetType::PointType & point = it->Value();
		point[0]/=spacing[0];
		point[1]/=spacing[1];
		point[2]/=spacing[2];
	}
	locs->SetPoints(points);
}



#endif /* POINTSETMAPPING_H_ */
