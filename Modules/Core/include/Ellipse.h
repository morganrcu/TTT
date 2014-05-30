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


/** \addtogroup TTTCore
 *  @{
 */

#ifndef ELLIPSE_H_
#define ELLIPSE_H_
#include <map>
namespace ttt{
/**
 * Class to represent an Ellipse
 */
template<class T=float> class Ellipse {
public:
	/**
	 * Default constructor
	 */
	Ellipse() {
		m_Xrad = -1;
		m_Yrad = -1;
		m_Xcent = 0;
		m_Ycent = 0;
		m_Theta = -1;
	}

#ifdef TTT_BOOST_SERIALIZATION
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & m_Xrad;
		ar & m_Yrad;
		ar & m_Xcent;
		ar & m_Ycent;
		ar & m_Theta;
	}
#endif
	/**
	 * x radius
	 */
	T m_Xrad;
	/**
	 * y radius
	 */
	T m_Yrad;
	/**
	 * x centroid
	 */
	T m_Xcent;
	/**
	 * y centroid
	 */
	T m_Ycent;
	/**
	 * rotation
	 */
	T m_Theta;

	T GetAspectRatio(){
		return m_Yrad/m_Xrad;
	}
};
}
#endif

/** @}*/
