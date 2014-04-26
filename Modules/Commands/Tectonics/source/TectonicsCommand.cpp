/*
 * TectonicsCommand.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: morgan
 */

#include "TectonicsCommand.h"
ttt::TectonicsCommand::TectonicsCommand() {
	// TODO Auto-generated constructor stub

}

ttt::TectonicsCommand::~TectonicsCommand() {
	// TODO Auto-generated destructor stub
}


void ttt::TectonicsCommand::Do(){

	assert(m_Domains);
	assert( (m_PreviousDescriptor && m_CurrentDescriptor ) || (m_PreviousDescriptor && m_NextDescriptor ) || (m_CurrentDescriptor && m_NextDescriptor));
	assert( (m_PreviousEllipses && m_CurrentEllipses ) || (m_PreviousEllipses && m_NextEllipses ) || (m_CurrentEllipses && m_NextEllipses ));
	double rate=0;

	m_DomainStrainRates=boost::shared_ptr<DomainStrainRatesMapType>(new DomainStrainRatesMapType);

	for(std::vector<ttt::TrackedDomain>::iterator domainIt=m_Domains->begin();domainIt!=m_Domains->end();domainIt++){

		std::vector<Centroid> points;
		std::vector<VelocityVector> velocities;

		std::vector<EllipseMatrix> ellipses0;
		std::vector<EllipseMatrix> ellipses1;


		int id=boost::get(ttt::TrackedCellPropertyTag(),(*m_CurrentDescriptor->m_CellGraph),domainIt->GetNucleus()).GetID();



		bool ok=true;
		if(m_PreviousDescriptor  && ttt::CellID2VertexDescriptor(id,m_PreviousDescriptor)==-1 ){
			ok =false;
		}

		if( m_NextDescriptor && ttt::CellID2VertexDescriptor(id,m_NextDescriptor)==-1 ){
			ok=false;
		}

		if(ok){

			Centroid centroid = boost::get(ttt::TrackedCellPropertyTag(),*m_CurrentDescriptor->m_CellGraph,domainIt->GetNucleus()).GetCentroid();
			for(std::set<ttt::TrackedCellVertexType>::iterator cellIt=domainIt->Begin();
					cellIt!=domainIt->End();
					cellIt++){
				int idCell=boost::get(ttt::TrackedCellPropertyTag(),(*m_CurrentDescriptor->m_CellGraph),*cellIt).GetID();

				bool valid=true;

				if(m_PreviousDescriptor  && ttt::CellID2VertexDescriptor(idCell,m_PreviousDescriptor)==-1 ){
					valid =false;
				}

				if( m_NextDescriptor && ttt::CellID2VertexDescriptor(idCell,m_NextDescriptor)==-1 ){
					valid=false;
				}

				if(valid){

					Centroid orig=boost::get(ttt::TrackedCellPropertyTag(),*m_CurrentDescriptor->m_CellGraph,*cellIt).GetCentroid();
					Centroid dst;

					for(int i=0;i<3;i++) dst[i]=orig[i] -centroid[i];
					points.push_back(dst);

					velocities.push_back(boost::get(ttt::TrackedCellPropertyTag(),*m_CurrentDescriptor->m_CellGraph,*cellIt).GetVelocity());

					if(m_PreviousEllipses && m_NextEllipses){
						rate=2;

						Ellipse<double> ellipse=(*m_PreviousEllipses)[ttt::CellID2VertexDescriptor(idCell,m_PreviousDescriptor)];
						EllipseMatrix ellipseMatrix;

						parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
						ellipses0.push_back(ellipseMatrix);

						ellipse=(*m_NextEllipses)[ttt::CellID2VertexDescriptor(idCell,m_NextDescriptor)];

						parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
						ellipses1.push_back(ellipseMatrix);


					}else if(m_PreviousEllipses && m_CurrentEllipses){
						rate=1;

						Ellipse<double> ellipse=(*m_PreviousEllipses)[ttt::CellID2VertexDescriptor(idCell,m_PreviousDescriptor)];
						EllipseMatrix ellipseMatrix;

						parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
						ellipses0.push_back(ellipseMatrix);

						ellipse=(*m_CurrentEllipses)[ttt::CellID2VertexDescriptor(idCell,m_CurrentDescriptor)];

						parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
						ellipses1.push_back(ellipseMatrix);

					}else if(m_CurrentEllipses && m_NextEllipses){
						rate=1;
						Ellipse<double> ellipse=(*m_CurrentEllipses)[ttt::CellID2VertexDescriptor(idCell,m_CurrentDescriptor)];
						EllipseMatrix ellipseMatrix;
						parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
						ellipses0.push_back(ellipseMatrix);

						ellipse=(*m_NextEllipses)[ttt::CellID2VertexDescriptor(idCell,m_NextDescriptor)];

						parametricEllipseToMatrixEllipse(ellipse.m_Xrad,ellipse.m_Yrad,ellipse.m_Theta,ellipseMatrix);
						ellipses1.push_back(ellipseMatrix);
					}
				}
			}
			if(ok){
				DomainStrainRates str;

				std::cout << "Computing tectonics for domain: " << id << std::endl;
				domainStrainRates(points,velocities,ellipses0,ellipses1,rate,str.Tissue_SRT,str.CellShape_SRT,str.Intercalation_SRT);
				str.order=domainIt->GetOrder();
				(*m_DomainStrainRates)[domainIt->GetNucleus()]=str;
			}


		}

	}

}
