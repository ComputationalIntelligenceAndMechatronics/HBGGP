/*
 *  DCDCBoostBG.cpp
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
 *  This file was created by Jean-Francois Dupuis on 14/12/09.
 */

#include "DCDCBoostGAController.h"
#include <cfloat>
#include <assert.h>

void DCDCBoostGAController::writeLog() {
	assert(size() == 2);
	(*mLogger)[(*this)[0]->getName()+std::string(".state")].push_back((*this)[0]->getState());
	(*mLogger)[(*this)[1]->getName()+std::string(".state")].push_back((*this)[1]->getState());
	(*mLogger)["TargetV"].push_back(mTargets[0]);
	(*mLogger)["TargetI"].push_back(mTargets[1]);
}

void DCDCBoostGAController::initialize(HybridBondGraph *inBondGraph, unsigned int inInitialSwState, const vector<double> &inStateValues) {
	mBondGraph = inBondGraph;
	mInitialSwState = inInitialSwState;
	vector<bool> lSwState(2,0);
	if( inInitialSwState == 1 ) {
		lSwState[0] = true;
		lSwState[1] = false;
	}
	else {
		lSwState[0]  = false;
		lSwState[1] = true;	
	}
//	lSwState[0] = true;
//	lSwState[1] = false;
//	mInitialSwState = 1;
	
	mInitialState = inStateValues;
	mCurrentState = inInitialSwState;
	
	inBondGraph->setInitialState(lSwState,inStateValues);
	setCurrentState(mCurrentState);
	
}

void DCDCBoostGAController::reset() { 
	initialize(mBondGraph, mInitialSwState,mInitialState);
}

void DCDCBoostGAController::updateSwitchState(double inTime, const vector<double>& inInputs) {
	
//	assert(mSwitchTriggers.size() == 2);
//	
//	double lV = inInputs[mSwitchTriggers[0]->getIndex()];
//	double lI = inInputs[mSwitchTriggers[1]->getIndex()];
//	
//	mP = sqrt( (lV-3.33)*(lV-3.33) + (lI-1.2)*(lI-1.2) );
//	
//	if( (mP > 0.33) && (mPrevP < mP) ) {
//		if(mCurrentState == 0)
//			mCurrentState = 1;
//		else
//			mCurrentState = 0;
//	}
//	
//	mPrevP = mP;
//	
//	if( mCurrentState == 1 ) {
//		(*this)[0]->setState(true);
//		(*this)[1]->setState(false);
//	}
//	else {
//		(*this)[0]->setState(false);
//		(*this)[1]->setState(true);	
//	}
//	
//	return;
//	
//	/////////////////////////////////////////////////////////
	
	assert(size() == 2);
	assert(mTransitionTable.size() == NBSTATE);

	
	generateInputSymbols(inInputs);
	for(unsigned int i = 0; i < mInputSymbols.size(); ++i) {
		mCurrentState = mTransitionTable[mCurrentState][mInputSymbols[i]];
	}
	
	setCurrentState(mCurrentState);

}

void DCDCBoostGAController::setCurrentState(unsigned int inState) {
	if( inState == 1 ) {
		(*this)[0]->setState(true);
		(*this)[1]->setState(false);
	}
	else {
		(*this)[0]->setState(false);
		(*this)[1]->setState(true);	
	}
}

void DCDCBoostGAController::generateInputSymbols(vector<double> inInputs) {
	
	//Input symbols are a combination of two tanks
	double lVThreshold = 0.1;
	double lIThreshold = 0.1;
	mInputSymbols.resize(0);
	
//	if( inInputs[0] < (mTargets[0] - lVThreshold) ) { // V<
//		if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 0) == mLastInputSymbols.end()) || mStart ) {
//			mInputSymbols.push_back(0);
//			mLastInputSymbols = mInputSymbols;
//		}
//	} else if( inInputs[0] > (mTargets[0] + lVThreshold) ) { //V>
//		if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 1) == mLastInputSymbols.end()) || mStart ) {
//			mInputSymbols.push_back(1);
//			mLastInputSymbols = mInputSymbols;
//		}
//	}
//	else { // V=
//		if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 2) == mLastInputSymbols.end()) || mStart ) {
//			mInputSymbols.push_back(2);
//			mLastInputSymbols = mInputSymbols;
//		}			
//	}
	
	
	if( inInputs[0] < (mTargets[0] - lVThreshold) ) { // V<
		if( inInputs[1] < (mTargets[1] - lIThreshold) ) { // I<
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 0) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(0);
				mLastInputSymbols = mInputSymbols;
			}
		} else if( inInputs[1] > (mTargets[1] + lIThreshold) ) { //I>
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 1) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(1);
				mLastInputSymbols = mInputSymbols;
			}
		}
		else { // I=
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 2) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(2);
				mLastInputSymbols = mInputSymbols;
			}			
		}
	} else if( inInputs[0] > (mTargets[0] + lVThreshold) ) { //V>
		if( inInputs[1] < (mTargets[1] - lIThreshold) ) { // I<
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 0) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(3);
				mLastInputSymbols = mInputSymbols;
			}
		} else if( inInputs[1] > (mTargets[1] + lIThreshold) ) { //I>
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 1) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(4);
				mLastInputSymbols = mInputSymbols;
			}
		}
		else { // I=
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 2) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(5);
				mLastInputSymbols = mInputSymbols;
			}			
		}
	}
	else { // V=
		if( inInputs[1] < (mTargets[1] - lIThreshold) ) { // I<
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 0) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(6);
				mLastInputSymbols = mInputSymbols;
			}
		} else if( inInputs[1] > (mTargets[1] + lIThreshold) ) { //I>
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 1) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(7);
				mLastInputSymbols = mInputSymbols;
			}
		}
		else { // I=
			if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), 2) == mLastInputSymbols.end()) || mStart ) {
				mInputSymbols.push_back(8);
				mLastInputSymbols = mInputSymbols;
			}			
		}		
	}
	
	mStart = false;
}

double DCDCBoostGAController::computeError() {
	std::string lTargetV = "TargetV";
	std::string lTargetI = "TargetI";
	
	std::string lOutputV = mOutBond->getName()+std::string(".e");
	std::string lOutputI = mIBond->getName()+std::string(".f");
	unsigned int lDataSize = (*mLogger)[lTargetV].size();
	
	assert((*mLogger)[lTargetV].size() == (*mLogger)[lOutputV].size() );
	assert((*mLogger)[lTargetV].size() == (*mLogger)["time"].size() );
	assert(lDataSize != 0);
	
	std::vector<double> lErrorV(lDataSize);
	std::vector<double> lErrorI(lDataSize);
	for(unsigned int i = 0; i < lDataSize; ++i) {
		lErrorV[i] = fabs((*mLogger)[lOutputV][i] - (*mLogger)[lTargetV][i]);

		lErrorI[i] = (*mLogger)[lOutputI][i] - (*mLogger)[lTargetI][i];
		if(lErrorI[i] < 0)
			lErrorI[i] = 0; //if under limit, fine no penalty
		else
			lErrorI[i] *= lErrorI[i]; //if over, greatly penalise.
	}
	
	std::vector<double> lErrors(2,0);
	for(unsigned int i = 0; i < lDataSize-1; ++i) {
		double dt = (*mLogger)["time"][i+1] - (*mLogger)["time"][i];
		lErrors[0] += (lErrorV[i]+lErrorV[i+1])/2*dt;
		lErrors[1] += (lErrorI[i]+lErrorI[i+1])/2*dt;
	}
	
	//Take the mean errors of the two targets
	//double lF = (lErrors[0]+lErrors[1])/2;
	
	//Take the worst of the two tanks
	double lF = max(lErrors[0],lErrors[1]);

	if(lF != 0)
		lF = 1/lF;
	else
		lF = DBL_MAX;
	
	return lF;
}

void DCDCBoostGAController::createBondGraph(HybridBondGraph &ioBondGraph) {
	
	//Create the sources
	Source *lSe = new Source(Source::eEffort);
	lSe->setValue(1.5);
	ioBondGraph.addComponent(lSe);
	
	//Create a switch
	Switch *lSw1 = new Switch;
	push_back(lSw1);
	Switch *lSw2 = new Switch;
	push_back(lSw2);
	
	
	//Create the jonctions
	Junction *lJ1_1 = new Junction(Junction::eOne);
	ioBondGraph.addComponent(lJ1_1);
	
	Junction *lJ0_1 = new Junction(Junction::eZero);
	ioBondGraph.addComponent(lJ0_1);
	
	Junction *lJ1_2 = new Junction(Junction::eOne);
	ioBondGraph.addComponent(lJ1_2);
	
	Junction *lJ0_2 = new Junction(Junction::eZero);
	ioBondGraph.addComponent(lJ0_2);
	
	//Create component
	Passive *lI = new Passive(Passive::eInductor);
	lI->setValue(150e-6);
	ioBondGraph.addComponent(lI);
	
	Passive *lC = new Passive(Passive::eCapacitor);
	lC->setValue(110e-6);
	ioBondGraph.addComponent(lC);
	
	Passive *lR = new Passive(Passive::eResistor);
	lR->setValue(6);
	ioBondGraph.addComponent(lR);
	
	
	//Create the bonds
	ioBondGraph.connect(lSe,lJ1_1);
	mIBond = ioBondGraph.connect(lJ1_1,lI); 
	ioBondGraph.connect(lJ1_1,lJ0_1);
	ioBondGraph.connect(lJ0_1,lSw1);
	ioBondGraph.connect(lJ0_1,lJ1_2);
	ioBondGraph.connect(lJ1_2,lSw2);
	ioBondGraph.connect(lJ1_2,lJ0_2);
	ioBondGraph.connect(lJ0_2,lC);
	mOutBond = ioBondGraph.connect(lJ0_2,lR);
	
//	this->addTrigger(mOutBond,eEffort);
//	this->addTrigger(mIBond,eFlow);
	
	ioBondGraph.addSwitchController(this);
	
	ioBondGraph.postConnectionInitialization();
	
	
	ioBondGraph.addOutputBonds(mIBond, eFlow);
	//	ioBondGraph.addOutputBonds(lBond1, eEffort);
	//	ioBondGraph.addOutputBonds(lBond1, eFlow);
	//	ioBondGraph.addOutputBonds(lBond2, eEffort);
	//	ioBondGraph.addOutputBonds(lBond2, eFlow);
	ioBondGraph.addOutputBonds(mOutBond, eEffort);
	//	ioBondGraph.addOutputBonds(lBond3, eFlow);
	
	
	//	ioBondGraph.connect(lSe,lJ1_1);
	//	Bond* lBond0 = ioBondGraph.connect(lJ1_1,lI);
	//	ioBondGraph.connect(lJ1_1,lJ0_1);
	//	ioBondGraph.connect(lJ0_1,lC);
	//	Bond* lBond3 = ioBondGraph.connect(lJ0_1,lR);
	//	ioBondGraph.addOutputBonds(lBond3, eEffort);
	//	ioBondGraph.addOutputBonds(lBond0, eFlow);
	
}


