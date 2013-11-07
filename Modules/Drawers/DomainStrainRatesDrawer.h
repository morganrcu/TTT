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

/** \addtogroup TTTDrawer
 *  @{
 */

#ifndef DOMAINSTRAINRATESDRAWER_H_
#define DOMAINSTRAINRATESDRAWER_H_
#include "Drawer.h"
#include "tectonics.h"
#include "giaaCellGraph2.h"

class DomainStrainRatesDrawer : public Drawer{

private:
	DomainStrainRatesMapTypePointer m_DomainStrainRates;
	giaa::TrackedTissueDescriptor::Pointer m_TrackedTissueDescriptor;

public:
	DomainStrainRatesDrawer(){

	}

	inline void SetDomainStrainRates(const DomainStrainRatesMapTypePointer & domainStrainRates){
		m_DomainStrainRates=domainStrainRates;
	}
	inline void SetTrackedTissueDescriptor(const giaa::TrackedTissueDescriptor::Pointer & trackedTissueDescriptor){
		m_TrackedTissueDescriptor=trackedTissueDescriptor;
	}
	virtual void Draw();

};


#endif /* DOMAINSTRAINRATESDRAWER_H_ */
/** @}*/
