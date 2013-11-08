#include "ComputeDomainsCommand.h"
ttt::ComputeDomainsCommand::ComputeDomainsCommand() {

}

ttt::ComputeDomainsCommand::~ComputeDomainsCommand() {

}

void ttt::ComputeDomainsCommand::Do(){
	m_Domains= boost::shared_ptr<std::vector<TrackedDomain> >(new std::vector<TrackedDomain>());
	assert(m_Order>0);
	assert(m_TrackedTissueDescriptor);
	ttt::GetDomains<TrackedTissueDescriptor,TrackedDomain>(m_TrackedTissueDescriptor,*m_Domains,m_Order);

}
