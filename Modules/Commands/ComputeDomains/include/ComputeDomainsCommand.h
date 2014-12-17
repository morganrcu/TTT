//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

/** \addtogroup TTTComputeDomains
 *  @{
 */

#ifndef COMPUTEDOMAINSCOMMAND_H_
#define COMPUTEDOMAINSCOMMAND_H_
#include <boost/shared_ptr.hpp>
#include "tttDescriptionDataTypes.h"
#include "AppCommand.h"

namespace ttt{
template<int dim> class ComputeDomainsCommand : public AppCommand{

private:
	typename TrackedTissueDescriptor<dim>::Pointer m_TrackedTissueDescriptor;
	boost::shared_ptr<std::vector<TrackedDomain<dim> > > m_Domains;

	unsigned int m_Order;
public:
	ComputeDomainsCommand();
	virtual ~ComputeDomainsCommand();

	virtual void Do();

	inline void SetOrder(unsigned int order){
		m_Order=order;
	}

	inline void SetTrackedTissueDescriptor(const typename TrackedTissueDescriptor<dim>::Pointer & trackedTissueDescriptor){
		m_TrackedTissueDescriptor=trackedTissueDescriptor;
	}

	inline boost::shared_ptr<std::vector<TrackedDomain<dim> > > GetDomains(){
		return m_Domains;
	}
};
}
#endif /* COMPUTEDOMAINSCOMMAND_H_ */
/** @}*/
