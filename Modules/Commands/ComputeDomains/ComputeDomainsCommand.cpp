/*
 * ComputeDomainsCommand.cpp
 *
 *  Created on: Oct 12, 2013
 *      Author: morgan
 */

#include <ComputeDomainsCommand.h>
#include "giaaCellGraph2.h"
ComputeDomainsCommand::ComputeDomainsCommand() {

}

ComputeDomainsCommand::~ComputeDomainsCommand() {

}

void ComputeDomainsCommand::Do(){
	m_Domains= boost::shared_ptr<std::vector<giaa::TrackedDomain> >(new std::vector<giaa::TrackedDomain>());
	assert(m_Order>0);
	assert(m_TrackedTissueDescriptor);
	giaa::GetDomains<giaa::TrackedTissueDescriptor,giaa::TrackedDomain>(m_TrackedTissueDescriptor,*m_Domains,m_Order);

}
