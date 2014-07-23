/*
 * jsontissuetrackingproject.h
 *
 *  Created on: Jul 21, 2014
 *      Author: morgan
 */

#ifndef JSONTISSUETRACKINGPROJECT_H_
#define JSONTISSUETRACKINGPROJECT_H_

#include <string>
#include "tissuetrackingabstractproject.h"

namespace ttt {
class JSONTissueTrackingProject : public TissueTrackingAbstractProject{

private:

	std::string m_ProjectPath;



};
}


#endif /* JSONTISSUETRACKINGPROJECT_H_ */
