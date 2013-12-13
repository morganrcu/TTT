#ifndef FEATURECOLORER_H_
#define FEATURECOLORER_H_
#include "Colorer.h"
#include  "tttDescriptionDataTypes.h"
#include "Feature.h"
namespace ttt{
template<class Feature> class FeatureColorer: public Colorer<typename Feature::ObjectType> {
private:
	typename Feature::Pointer m_Feature;
public:

	void SetFeature(const typename Feature::Pointer & feature){
		m_Feature=feature;
	}
	virtual void GetObjectColor(const typename Feature::ObjectType & object, double  color[3]) {



		double deg = ((double)(m_Feature->at(object) -m_Feature->GetMin())) / m_Feature->GetMax();
		std::cout << deg << std::endl;
		color[0] = deg;
		color[1] = deg<0.5?deg*2:1-(deg-0.5);
		color[2] = 1 - deg;

	}
	virtual ~FeatureColorer(){

	}
};
}
#endif /* EDGECOLORER_H_ */
