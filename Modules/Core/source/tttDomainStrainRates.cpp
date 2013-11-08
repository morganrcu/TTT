/*
 * tttDomainStrainRates.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: morgan
 */

#include "tttDomainStrainRates.h"



void ttt::tensorToPlot(const vnl_matrix<double> & tensor, vnl_vector<double> & majorDir, double * majorLength, vnl_vector<double> & minorDir, double * minorLength,double * rotation){

	vnl_symmetric_eigensystem<double> eigensystem(tensor);

	majorDir= eigensystem.V.get_column(1);
	majorLength[0]=eigensystem.D(1,1);
	minorDir = eigensystem.V.get_column(0);
	minorLength[0]=eigensystem.D(0,0);
	vnl_matrix<double> Spin = (tensor - tensor.transpose()) / 2;

	assert(majorLength[0]>minorLength[0]);
	rotation[0] = tensor(1,0);
}
