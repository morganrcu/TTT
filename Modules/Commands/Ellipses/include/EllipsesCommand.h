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

/** \addtogroup TTTEllipses
 *  @{
 */

#ifndef ELLIPSESCOMMAND_H_
#define ELLIPSESCOMMAND_H_
#include <vector>
#include <boost/shared_ptr.hpp>


#include "tttDomainStrainRates.h"
#include  "Ellipse.h"
#include "fitEllipse.h"
#include "AppCommand.h"

template<class TissueDescriptor> class EllipsesCommand : public AppCommand{
private:
	typename TissueDescriptor::Pointer m_Descriptor;
	typedef Ellipse<double> EllipseType;
	typedef std::map<typename TissueDescriptor::DualGraphVertexDescriptorType,EllipseType >   EllipseMapType;
	typedef boost::shared_ptr<EllipseMapType > EllipseMapTypePointer;

	EllipseMapTypePointer m_Ellipses;
public:
	EllipsesCommand();
	virtual ~EllipsesCommand();
	virtual void Do();
	inline void SetTissueDescriptor(const typename TissueDescriptor::Pointer & descriptor){
		m_Descriptor=descriptor;
	}
	inline EllipseMapTypePointer GetEllipses(){
		assert(m_Ellipses);
		return m_Ellipses;
	}
};




template<class T> int CalcEllipseValues(std::vector<itk::Point<T, 3> > &pts, Ellipse<T> & result) {

	std::vector<T> elval = fitEllipse(pts);

	if(elval[2]<0 || elval[3]<0) return -1;

	result.m_Xcent = elval[0];
	result.m_Ycent = elval[1];
	result.m_Xrad = sqrt(elval[2]);
	result.m_Yrad = sqrt(elval[3]);
	result.m_Theta = elval[4];

	std::cout << "(" << result.m_Xcent << " " <<  result.m_Ycent<< " " <<  result.m_Xrad<< " "<<  result.m_Yrad<< " " << result.m_Theta << ")" << std::endl;

	if(result.m_Xcent!=result.m_Xcent || result.m_Ycent!=result.m_Ycent || result.m_Xrad!=result.m_Xrad || result.m_Yrad!=result.m_Yrad || result.m_Theta!= result.m_Theta){
		for(unsigned int i=0;i<pts.size(); i++){
			std::cout << "\t" << pts[i] <<std::endl;
		}
	}
	return 0;
}

template<class TissueDescriptor> EllipsesCommand<TissueDescriptor>::EllipsesCommand() {
}

template<class TissueDescriptor> EllipsesCommand<TissueDescriptor>::~EllipsesCommand() {
}

template<class TissueDescriptor> void EllipsesCommand<TissueDescriptor>::Do(){
	m_Ellipses= boost::shared_ptr<EllipseMapType>(new EllipseMapType);
	assert(m_Descriptor);
	assert(m_Descriptor->m_CellGraph);

	BGL_FORALL_VERTICES(v,*m_Descriptor->m_CellGraph,CellGraph) {


		//std::vector<SkeletonVertexType> svt = boost::get(TrackedCellPropertyTag(),*m_Descriptor->m_CellGraph,v).m_SkeletonNodes;
		typedef typename ttt::TissueDescriptorTraits<TissueDescriptor>::CellType CellType;
		CellType currentCell =boost::get(TrackedCellPropertyTag(),*m_Descriptor->m_CellGraph,v);
		std::vector<itk::Point<double,3> > points;

		for(typename CellType::PerimeterIterator itr = currentCell.PerimeterBegin(); itr != currentCell.PerimeterEnd(); ++itr){
			itk::Point<double,3> pos = boost::get(SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph,*itr).position;
			points.push_back(pos);
		}
		bool done=false;
		EllipseType newEllipse;
		if(points.size()>3){
			if(CalcEllipseValues(points,newEllipse)!=-1){
				done=true;
			}
		}
		if(!done){
			itk::Point<double,3> center;
			center.Fill(0);

			for(std::vector<itk::Point<double,3> >::iterator it= points.begin();it!=points.end();it++){
				for(int i=0;i<3;i++) center[i]=center[i]+(*it)[i];
			}
			for(int i=0;i<3;i++) center[i]=center[i]/points.size();

			double radius=0;

			for(std::vector<itk::Point<double,3> >::iterator it= points.begin();it!=points.end();it++){
				itk::Vector<double,3> diff = *it - center;
				radius+=sqrt(pow(diff[0],2)+ pow(diff[1],2));
			}
			radius=radius/points.size();
			newEllipse.m_Theta=0;
			newEllipse.m_Xcent=center[0];
			newEllipse.m_Ycent=center[1];
			newEllipse.m_Xrad=radius;
			newEllipse.m_Yrad=radius;
		}
		(*m_Ellipses)[v]=newEllipse;
	}
}
#endif /* ELLIPSESCOMMAND_H_ */
/** @}*/
