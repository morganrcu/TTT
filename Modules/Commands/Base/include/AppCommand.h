#ifndef APPCOMMAND_H_
#define APPCOMMAND_H_
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

/** \addtogroup TTTBase
 *  @{
 */

namespace ttt{
/**
 * \class AppCommand
 * Abstract Class defining the interface for data modification objects
 * TODO
 */
class AppCommand{
public:
	/**
	 * Virtual method where the logic of the command should be implemented
	 */
	virtual void Do()=0;
	/**
	 * Does nothing
	 */
	virtual ~AppCommand(){

	}

};
}
#endif /* APPCOMMAND_H_ */
/** @}*/
