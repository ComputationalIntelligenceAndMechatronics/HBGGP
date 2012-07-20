/*
 *  DCDCBoostLookaheadController.h
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

#ifndef DCDCBoostLookaheadController_H
#define DCDCBoostLookaheadController_H

#include "LookaheadController.h"
#include <vector>

class DCDCBoostLookaheadController : public LookaheadController {
public:
	DCDCBoostLookaheadController(double inSimTime) : LookaheadController(inSimTime) {}
	DCDCBoostLookaheadController() : LookaheadController(0) {}
	~DCDCBoostLookaheadController() {}
	
	virtual void reset() {}
	
	void setSimulationDuration(double inDuration) { mSimTime = inDuration; }
	
	virtual void writeLog();
	
protected:
	std::vector<bool> mInitialSwitchState;
};

#endif
