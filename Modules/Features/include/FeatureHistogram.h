/*
 * FeatureHistogram.h
 *
 *  Created on: Dec 12, 2013
 *      Author: morgan
 */

#ifndef FEATUREHISTOGRAM_H_
#define FEATUREHISTOGRAM_H_


template<class Feature> class FeatureHistogram{

private:
	Feature::Pointer m_Feature;
	int m_NumBins;

	std::vector<int> m_Count;

public:

	void SetFeature(const Feature::Pointer & feature){
		m_Feature=feature;
	}

	void SetNumBins(int numBins){
		m_NumBins=numBins;
	}

	void Compute(){
		m_Count.resize(m_NumBins);

		for(std::vector<int>::iterator it= m_Count.begin();it!=m_Count.end();it++){
			*it=0;
		}

		typename Feature::ValueType min=m_Feature->GetMin();
		typename Feature::ValueType max=m_Feature->GetMax();
		typename Feature::ValueType step = max-min/2;
		std::sort(m_Feature->begin(),m_Feature->end(),CompareSecond());
	}
};


#endif /* FEATUREHISTOGRAM_H_ */
