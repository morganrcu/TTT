/*
 * TectonicsCommand.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: morgan
 */

#include <TectonicsCommand.h>
#include "giaaCellGraph2.h"
TectonicsCommand::TectonicsCommand() {
	// TODO Auto-generated constructor stub

}

TectonicsCommand::~TectonicsCommand() {
	// TODO Auto-generated destructor stub
}


void TectonicsCommand::Do(){

	assert(m_Domains);
	assert( (m_PreviousDescriptor && m_CurrentDescriptor ) || (m_PreviousDescriptor && m_NextDescriptor ) || (m_CurrentDescriptor && m_NextDescriptor));
	assert( (m_PreviousEllipses && m_CurrentEllipses ) || (m_PreviousEllipses && m_NextEllipses ) || (m_CurrentEllipses && m_NextEllipses ));

	m_DomainStrainRates=boost::shared_ptr<DomainStrainRatesMapType>(new DomainStrainRatesMapType);

	for(std::vector<giaa::TrackedDomain>::iterator domainIt=m_Domains->begin();domainIt!=m_Domains->end();domainIt++){

		std::vector<Centroid> points;
		std::vector<VelocityVector> velocities;

		std::vector<EllipseMatrix> ellipses0;
		std::vector<EllipseMatrix> ellipses1;


		int id=boost::get(giaa::TrackedCellPropertyTag(),(*m_CurrentDescriptor->m_CellGraph),domainIt->GetNucleus()).m_ID;



		bool ok=true;
		if(m_PreviousDescriptor  && giaa::CellID2VertexDescriptor(id,m_PreviousDescriptor)==-1 ){
			ok =false;
		}

		if( m_NextDescriptor && giaa::CellID2VertexDescriptor(id,m_NextDescriptor)==-1 ){
			ok=false;
		}

		if(ok){

			Centroid centroid = boost::get(giaa::TrackedCellPropertyTag(),*m_CurrentDescriptor->m_CellGraph,domainIt->GetNucleus()).m_Centroid;
			for(std::set<giaa::TrackedCellVertexType>::iterator cellIt=domainIt->Begin();
					cellIt!=domainIt->End();
					cellIt++){
				int idCell=boost::get(giaa::TrackedCellPropertyTag(),(*m_CurrentDescriptor->m_CellGraph),*cellIt).m_ID;

				if(m_PreviousDescriptor  && giaa::CellID2VertexDescriptor(idCell,m_PreviousDescriptor)==-1 ){
					ok =false;
				}

				if( m_NextDescriptor && giaa::CellID2VertexDescriptor(idCell,m_NextDescriptor)==-1 ){
					ok=false;
				}
				if(!ok) break;

				Centroid orig=boost::get(giaa::TrackedCellPropertyTag(),*m_CurrentDescriptor->m_CellGraph,*cellIt).m_Centroid;
				Centroid dst;
				for(int i=0;i<3;i++) dst[i]=orig[i] -centroid[i];

				std::cout << dst << " ";

				points.push_back(dst);
				velocities.push_back(boost::get(giaa::TrackedCellPropertyTag(),*m_CurrentDescriptor->m_CellGraph,*cellIt).m_Velocity);
				if(m_PreviousEllipses && m_NextEllipses){
	//				void parametricEllipseToMatrixEllipse(const double Rx, const double Ry, const double angle, vnl_matrix_fixed<double,2,2> & result);


					Ellipse<double> ellipse=(*m_PreviousEllipses)[giaa::CellID2VertexDescriptor(idCell,m_PreviousDescriptor)];
					EllipseMatrix ellipseMatrix;

					parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);

					std::cout << ellipseMatrix << " ";
					ellipses0.push_back(ellipseMatrix);

					ellipse=(*m_NextEllipses)[giaa::CellID2VertexDescriptor(idCell,m_NextDescriptor)];
					parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);

					std::cout << ellipseMatrix << " ";

					ellipses1.push_back(ellipseMatrix);
				}else if(m_PreviousEllipses && m_CurrentEllipses){


					Ellipse<double> ellipse=(*m_PreviousEllipses)[giaa::CellID2VertexDescriptor(idCell,m_PreviousDescriptor)];
					EllipseMatrix ellipseMatrix;
					parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
					std::cout << ellipseMatrix << " ";
					ellipses0.push_back(ellipseMatrix);

					ellipse=(*m_CurrentEllipses)[giaa::CellID2VertexDescriptor(idCell,m_CurrentDescriptor)];
					parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
					std::cout << ellipseMatrix << " ";
					ellipses1.push_back(ellipseMatrix);

				}else if(m_CurrentEllipses && m_NextEllipses){

					Ellipse<double> ellipse=(*m_CurrentEllipses)[giaa::CellID2VertexDescriptor(idCell,m_CurrentDescriptor)];
					EllipseMatrix ellipseMatrix;
					parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);

					std::cout << ellipseMatrix << " ";

					ellipses0.push_back(ellipseMatrix);

					ellipse=(*m_NextEllipses)[giaa::CellID2VertexDescriptor(idCell,m_NextDescriptor)];
					parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
					std::cout << ellipseMatrix << " ";
					ellipses1.push_back(ellipseMatrix);
				}
			}
			if(ok){
				DomainStrainRates str;
				//Tensor Tissue_SRT;
				//Tensor CellShape_SRT;
				//Tensor Intercalation_SRT;
				std::cout << "Computing tectonics for domain: " << id << std::endl;
				domainStrainRates(points,velocities,ellipses0,ellipses1,1,str.Tissue_SRT,str.CellShape_SRT,str.Intercalation_SRT);
				(*m_DomainStrainRates)[domainIt->GetNucleus()]=str;
			}


		}

	}

}
