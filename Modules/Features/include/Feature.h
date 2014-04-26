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
	static const ValueType zero;
};

template<> struct ValueTraits< double >{
	static const double zero=0;
};


static bool operator<(const itk::Point<double,3> & a,const itk::Point<double,3> & b){
	if(a[0] < b[0] ){
		return true;
	}else if(a[0]==b[0]){
		if(a[1] < b[1] ){
			return true;
		}else if(a[1]==b[1]){
			if(a[2] < b[2] ){
				return true;
			}
		}
	}
	return false;
}

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
		if( m_Max < value){
			m_Max=value;
		}
		this->insert(toInsert);
	}

	virtual Iterator Begin(){
		return this->begin();
	}
	virtual Iterator End(){
		return this->end();
	}
	ValueType GetMax(){
		return m_Max;
	}

	ValueType GetMin(){
		return m_Min;
	}

	ValueType GetMean(){;
		ValueType sum = ValueTraits<ValueType>::zero;

		for(Iterator it= this->begin();it!=this->end();++it){
			sum= sum + it->second;
		}
		sum=sum/this->GetNumFeatures();
		return sum;
	}

	ValueType GetStd(){
		ValueType sum = ValueTraits<ValueType>::zero;
		ValueType sum2 = ValueTraits<ValueType>::zero;

		for(Iterator it= this->begin();it!=this->end();++it){
			sum= sum + it->second;
			sum2=sum2 + it->second*it->second;
		}
		sum=sum/this->GetNumFeatures();
		sum2=sum2/this->GetNumFeatures();

		return sqrt(sum2 - sum*sum);
	}
	unsigned int GetNumFeatures(){
		return this->size();
	}
};



#endif /* FEATURE_H_ */
