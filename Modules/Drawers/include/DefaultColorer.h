/*
 * EdgeColorer.h
 *
 *  Created on: Dec 11, 2013
 *      Author: morgan
 */

#ifndef DEFAULTCOLORER_H_
#define DEFAULTCOLORER_H_
#include "Colorer.h"

template<class T> class DefaultColorer : public Colorer<T>{
public:
	void GetObjectColor(const T & object, double * result){

		result[0]=1.0;
		result[1]=1.0;
		result[2]=1.0;

	}

};

#endif /* EDGECOLORER_H_ */
