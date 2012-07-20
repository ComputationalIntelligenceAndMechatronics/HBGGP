/*
 *  GASwitchController.h
 *  Copyright 2009 Jean-Francois Dupuis.
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
 *  This file was created by Jean-Francois Dupuis on 15/12/09.
 */

#ifndef GASwitchController_H
#define GASwitchController_H


#include "HybridBondGraph.h"

class GASwitchController : public BG::SwitchController {
public:
	GASwitchController() {}
	~GASwitchController() {}
	


	virtual void updateSwitchState(double inTime, const vector<double>& inInputs, bool inWithInitialParameters=false) = 0;
	
	virtual void createBondGraph(BG::HybridBondGraph &ioBondGraph) = 0;
	
	//virtual double computeError() = 0;
	
	void setTarget(const vector<double>& inTargets) { mTargets = inTargets;	}
	void setTransitionTable(const vector< vector<unsigned int> > &inTransitionTable) { mTransitionTable = inTransitionTable; }
	

protected:
	virtual void generateInputSymbols(vector<double> inInputs) = 0;
	
	std::vector<double> mTargets;
	vector< vector<unsigned int> > mTransitionTable;
	
	unsigned int mCurrentState;
	
	inline unsigned int convertSymbole(const vector<int>& inConditionSize, const vector<int>& inConditionStatus);
};


unsigned int GASwitchController::convertSymbole(const vector<int>& inConditionSize, const vector<int>& inConditionStatus) {
	int lMultiplier = inConditionSize[0];
	int lSymbol = inConditionStatus[0];
	for(unsigned int i = 1; i < inConditionStatus.size(); ++i) {
		lSymbol += inConditionStatus[i]*lMultiplier;
		lMultiplier *= inConditionSize[i];
	}
	return lSymbol;
}

#endif
