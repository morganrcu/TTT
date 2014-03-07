#ifndef TTTDOMAINSTRAINRATES_H_
#define TTTDOMAINSTRAINRATES_H_
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include "tttDescriptionDataTypes.h"
namespace ttt{
typedef itk::Point<double, 3> Centroid ;
typedef itk::Vector<double,3> VelocityVector;
typedef vnl_matrix_fixed<double,2,2> EllipseMatrix;
typedef vnl_matrix_fixed<double,2,2> Tensor;
typedef vnl_vector_fixed<double,2> Vector;

#if 0
void domainStrainRates(const std::vector<Pt> & centroids0,
		const std::vector<Pt> & centroids1,
		const std::vector<EllipseMatrix> & ellipse0,
		const std::vector<EllipseMatrix> & ellipse1,
		double mins,
		vnl_matrix<double> & Tissue_SRT,
		vnl_matrix<double> & CellShape_SRT,
		vnl_matrix<double> & Intercalation_SRT);
#endif

typedef struct{
	Tensor Tissue_SRT;
	Tensor CellShape_SRT;
	Tensor Intercalation_SRT;
	int order;
} DomainStrainRates;

typedef std::map<typename ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType,DomainStrainRates >   DomainStrainRatesMapType;
typedef std::map<typename ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType,DomainStrainRates >::iterator   DomainStrainRatesMapIteratorType;
typedef boost::shared_ptr<DomainStrainRatesMapType > DomainStrainRatesMapTypePointer;


void tensorToPlot(const vnl_matrix<double> & tensor, vnl_vector<double> & majorDir, double * majorLength, vnl_vector<double> & minorDir, double * minorLength,double * rotation);


}




#endif
