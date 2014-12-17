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

/** \addtogroup TTTDrawers
 *  @{
 */

#ifndef DOMAINSTRAINRATESDRAWER_H_
#define DOMAINSTRAINRATESDRAWER_H_
#include "Drawer.h"
#include "tttDomainStrainRates.h"
#include "tttDescriptionDataTypes.h"
namespace ttt{
class DomainStrainRatesDrawer : public Drawer{

private:
	DomainStrainRatesMapTypePointer m_DomainStrainRates;
	TrackedTissueDescriptor<3>::Pointer m_TrackedTissueDescriptor;
	std::vector<vtkSmartPointer<vtkActor> > m_Actors;
	enum DrawMode{ CELL_SRT,TISSUE_SRT,INTERCALATION_SRT};
	DrawMode m_DrawMode;

public:
	DomainStrainRatesDrawer(){
		m_DrawMode=CELL_SRT;
	}

	inline void SetDomainStrainRates(const DomainStrainRatesMapTypePointer & domainStrainRates){
		m_DomainStrainRates=domainStrainRates;
	}
	inline void SetTrackedTissueDescriptor(const TrackedTissueDescriptor<3>::Pointer & trackedTissueDescriptor){
		m_TrackedTissueDescriptor=trackedTissueDescriptor;
	}
	virtual void Draw();
	virtual void Show();
	virtual void Hide();

	virtual void Reset();
	inline void SetDrawModeToCellSRT(){
		m_DrawMode=CELL_SRT;
	}
	inline void SetDrawModeToTissueSRT(){
		m_DrawMode=TISSUE_SRT;
	}
	inline void SetDrawModeToIntercalationSRT(){
		m_DrawMode=INTERCALATION_SRT;
	}
};
}

#endif /* DOMAINSTRAINRATESDRAWER_H_ */
/** @}*/
