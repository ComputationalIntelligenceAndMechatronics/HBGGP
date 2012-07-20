/*
 *  DCDCBoostLookaheadController.cpp
 *  Copyright 2010 Jean-Francois Dupuis.
 *
 *  This file is part of HBGGP.
 *  
 *  HBGGP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  HBGGP is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with HBGGP.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  This file was created by Jean-Francois Dupuis on 25/03/10.
 */

#include "DCDCBoostLookaheadController.h"
#include "stringutil.h"
#include "BGException.h"


void DCDCBoostLookaheadController::writeLog() {
	
	const std::vector<double>& lOutputsVariables = mBondGraph->getOutputVariables();
	
	(*mLogger)["State"].push_back(mCurrentState);
	for(unsigned int i = 0; i < mTargets.size(); ++i) {
		(*mLogger)[std::string("Target_")+int2str(i)].push_back(mTargets[i]);
		(*mLogger)[std::string("Output_")+int2str(i)].push_back(lOutputsVariables[i]);
	}
}



