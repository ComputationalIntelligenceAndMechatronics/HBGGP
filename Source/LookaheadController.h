/*
 *  LookaheadController.h
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
 *  This file was created by Jean-Francois Dupuis on 22/02/10.
 */

#ifndef LookaheadController_H
#define LookaheadController_H

#include <assert.h>
#include "HybridBondGraph.h"
#include "SwitchController.h"

class LookaheadController : public BG::SwitchController {
protected:
	double mSimTime;
	
	virtual void setCurrentState(unsigned int inState);
	
	unsigned int mCurrentState;

	BG::HybridBondGraph *mBondGraph;
	
	std::vector<double> mTargets;
	std::vector<PACC::Vector> mTargetTrajectories;
	
	virtual void mapTarget2State(const std::vector<double>& inTargets, std::vector<double>& outStates);
	virtual void map2Target(const std::vector<double>& inState, const std::vector<double>& inOutputs, std::vector<double>& outTarget);
	virtual int getNbStates() const;
	
	std::vector<int> mExcludedStates;
	
public:	
	LookaheadController(double inSimTime) : mSimTime(inSimTime) {}
	
	virtual void initialize() {}
	
	virtual void initialize(BG::HybridBondGraph *inBondGraph, unsigned int inInitialSwState);
	virtual void initialize(BG::HybridBondGraph *inBondGraph, unsigned int inInitialSwState, const std::vector<double> &inOutputValues);
	virtual void writeLog();
	virtual void updateSwitchState(double inTime, const std::vector<double>& inInputs, bool inWithInitialParameters=false);
	
	virtual void setTarget(const std::vector<double>& inTargets);
	
	void createBondGraph(BG::HybridBondGraph &ioBondGraph) {}
};

#endif
