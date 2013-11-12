/** \addtogroup TTTTectonics
 *  @{
 */
#ifndef TECTONICS_H
#define TECTONICS_H
#include <vnl/algo/vnl_symmetric_eigensystem.h>


#include <itkPoint.h>
#include <vnl/vnl_matrix_fixed.h>
#include "tttDescriptionDataTypes.h"

#include "tttDomainStrainRates.h"

namespace ttt{
void domainStrainRates(const std::vector<Centroid> & centroids0,
		const std::vector<VelocityVector> & speeds,
		const std::vector<EllipseMatrix> & ellipse0,
		const std::vector<EllipseMatrix> & ellipse1,
		double mins,
		Tensor & Tissue_SRT,
		Tensor & CellShape_SRT,
		Tensor & Intercalation_SRT);

void parametricEllipseToMatrixEllipse(const double Rx, const double Ry, const double angle, vnl_matrix_fixed<double,2,2> & result);

}
#endif
/** @}*/
