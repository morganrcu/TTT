#ifndef FEATURECOLORER_H_
#define FEATURECOLORER_H_
#include "Colorer.h"
#include  "tttDescriptionDataTypes.h"
#include "Feature.h"
#include "FeatureMap.h"

namespace ttt{
template<class ObjectType, class FeatureType> class FeatureColorer : public Colorer<ObjectType > {
private:
	typedef FeatureMap<ObjectType,FeatureType> FeatureMapType;
	FeatureMapType m_FeatureMap;
public:

	void SetFeatureMap(const FeatureMapType & featureMap){
		m_FeatureMap=featureMap;
	}

	virtual void GetObjectColor(const ObjectType & object, double  color[3]) {



#if 0
		double deg = ((double)(m_Feature->at(object) -m_Feature->GetMin())) / m_Feature->GetMax();
		std::cout << deg << std::endl;
		color[0] = deg;
		color[1] = deg<0.5?deg*2:1-(deg-0.5);
		color[2] = 1 - deg;
#endif

		//FIXME
	}
	virtual ~FeatureColorer(){

	}
};
}
#endif /* EDGECOLORER_H_ */
