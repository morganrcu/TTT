/*
 * Feature.h
 *
 *  Created on: Dec 12, 2013
 *      Author: morgan
 */

#ifndef FEATURE_H_
#define FEATURE_H_

#include <map>
template<class ValueType> struct ValueTraits{
	ValueType zero=0;
};

template<class TObject,class TValue > class Feature : public std::map<TObject,TValue>{
public:
	typedef TObject ObjectType;
	typedef TValue  ValueType;

	typedef  std::map<ObjectType,ValueType> MapType;
	typedef  typename std::map<ObjectType,ValueType>::iterator Iterator;
	typedef Feature Self;
	typedef boost::shared_ptr<Self> Pointer;
private:
	//MapType m_FeatureMap;
	ValueType m_Max;
	ValueType m_Min;

public:
	Feature(){
		m_Max= std::numeric_limits<TValue>::min();
		m_Min= std::numeric_limits<TValue>::max();
	}
	void Insert(const ObjectType & obj,const ValueType & value){
		std::pair<ObjectType,ValueType> toInsert;
		toInsert.first=obj;
		toInsert.second=value;
		if(value < m_Min){
			m_Min=value;
		}
		if(value > m_Max){
			m_Max=value;
		}
		this->insert(toInsert);
	}

	virtual Iterator Begin(){
		this->begin();
	}
	virtual Iterator End(){
		this->end();
	}
	ValueType GetMax(){
		return m_Max;
	}

	ValueType GetMin(){
		return m_Min;
	}

	ValueType GetMean(){
		ValueType sum =std::accumulate(this->begin(),this->end(),ValueTraits<ValueType>::zero);
		sum=sum/this->GetNumFeatures();
		return sum;
	}

	ValueType GetStd(){
		return 0;
	}
	unsigned int GetNumFeatures(){
		return this->size();
	}
};



#endif /* FEATURE_H_ */
