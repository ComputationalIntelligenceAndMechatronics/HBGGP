/*
 *  ThreeTanksLookaheadController.h
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
 *  This file was created by Jean-Francois Dupuis on 23/02/10.
 */

#ifndef ThreeTanksLookaheadController_H
#define ThreeTanksLookaheadController_H

#include "LookaheadController.h"

class ThreeTanksLookaheadController : public LookaheadController {
public:
	ThreeTanksLookaheadController(double inSimTime) : LookaheadController(inSimTime) {}
	ThreeTanksLookaheadController() : LookaheadController(0) {}
	~ThreeTanksLookaheadController() {}
	
	//virtual void initialize(BG::HybridBondGraph *inBondGraph, unsigned int inInitialSwState, const vector<double> &inStateValues);
	
	virtual void reset() {}
	
	void setSimulationDuration(double inDuration) { mSimTime = inDuration; }
	
	virtual void writeLog();


protected:
	vector<bool> mInitialSwitchState;
	
	virtual void setCurrentState(unsigned int inState);

	virtual void map2Target(const vector<double>& inState, const vector<double>& inOutput, vector<double>& outTarget);
	virtual int getNbStates() const;
};

#endif
