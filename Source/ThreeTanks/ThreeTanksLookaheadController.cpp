/*
 *  ThreeTanksLookaheadController.cpp
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

#include "ThreeTanksLookaheadController.h"
#include "stringutil.h"
#include "BGException.h"

void ThreeTanksLookaheadController::writeLog() {
	
	vector<double> lLevels;
	const vector<double>& lStateVariables = mBondGraph->getStateVariables();
	const vector<double>& lOutputVarialbes = mBondGraph->getOutputVariables();
	map2Target(lStateVariables, lOutputVarialbes, lLevels);
	
	(*mLogger)["State"].push_back(mCurrentState);
	for(unsigned int i = 0; i < mTargets.size(); ++i) {
		(*mLogger)[string("Target_")+int2str(i)].push_back(mTargets[i]);
		(*mLogger)[string("Output_")+int2str(i)].push_back(lLevels[i]);
	}
}

void ThreeTanksLookaheadController::map2Target(const vector<double>& inState, const vector<double>& inOutputs, vector<double>& outTarget) {
	//double lC = 1.570e-6;
	double g = 9.81;
	double lFluidDensity = 998.2;
	
	outTarget.resize(inOutputs.size());
	for(unsigned int i = 0; i < inOutputs.size(); ++i) {
		outTarget[i] = inOutputs[i]/(lFluidDensity*g);
	}
}

void ThreeTanksLookaheadController::setCurrentState(unsigned int inState) {
	double lF =  1.833e-3*4;
	
	vector<BG::Source*> lSources = mBondGraph->getSources();
	
	vector<bool> lSwState(this->size()+lSources.size(),false);
	for(unsigned int i = 0; i < lSwState.size(); ++i) {
		lSwState[i] = (inState >> i) & 1;
	}
	
	for(unsigned int i = 0; i < this->size(); ++i) {
		(*this)[i]->setState( lSwState[i] );
	}
	
	//Set source value
	for(unsigned int i = 0; i < lSources.size(); ++i) {
		if(lSwState[this->size()+i])
			lSources[i]->setValue(0);
		else
			lSources[i]->setValue(lF);
	}
}

int ThreeTanksLookaheadController::getNbStates() const {
	return int(pow(2.0,double(mBondGraph->getSources().size() + this->size())));
}
