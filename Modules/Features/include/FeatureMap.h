/*
 * FeatureMap.h
 *
 *  Created on: May 5, 2014
 *      Author: morgan
 */

#ifndef FEATUREMAP_H_
#define FEATUREMAP_H_
#include <map>
#include "Feature.h"
template<class TObject,class TValue >  class FeatureMap : public std::map<TObject,Feature<TValue> > {
public:
	typedef Feature<TValue> FeatureType;
};



//typedef std::map FeatureMap;




#endif /* FEATUREMAP_H_ */
