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

#include "DCDCBoost2xGAController.h"
#include <cfloat>
#include <assert.h>
#include "stringutil.h"
#include "VectorUtil.h"

void DCDCBoost2xGAController::writeLog() {
	assert(size() == 3);
	
	const std::vector<double>& lOutputsVariables = mBondGraph->getOutputVariables();
	
	(*mLogger)[(*this)[0]->getName()+std::string(".state")].push_back((*this)[0]->getState());
	(*mLogger)[(*this)[1]->getName()+std::string(".state")].push_back((*this)[1]->getState());
	(*mLogger)[(*this)[2]->getName()+std::string(".state")].push_back((*this)[2]->getState());

	(*mLogger)["ControlSymbols"].push_back(mLastInputSymbols[0]);
	(*mLogger)["State"].push_back(mCurrentState);
	
	for(unsigned int i = 0; i < mTargets.size(); ++i) {
		(*mLogger)[std::string("Target_")+int2str(i)].push_back(mTargets[i]);
		(*mLogger)[std::string("Output_")+int2str(i)].push_back(lOutputsVariables[i]);
	}
}

void DCDCBoost2xGAController::initialize(HybridBondGraph *inBondGraph, unsigned int inInitialSwState, const vector<double> &inStateValues) {
	mBondGraph = inBondGraph;
	mInitialSwState = inInitialSwState;
	
	mInitialState = inStateValues;
	mCurrentState = inInitialSwState;

	setCurrentState(mCurrentState);
	vector<bool> lSwState(3,0);
	lSwState[0] = (*this)[0]->getState();
	lSwState[1] = (*this)[1]->getState();
	lSwState[2] = (*this)[2]->getState();
	
	inBondGraph->setInitialState(lSwState,inStateValues);

	
}

void DCDCBoost2xGAController::reset() { 
	initialize(mBondGraph, mInitialSwState,mInitialState);
}

void DCDCBoost2xGAController::updateSwitchState(double inTime, const vector<double>& inInputs, bool inWithInitialParameters) {
	
	assert(size() == 3);
	assert(mTransitionTable.size() == NBSTATE);

	
	generateInputSymbols(inInputs);
	for(unsigned int i = 0; i < mInputSymbols.size(); ++i) {
		mCurrentState = mTransitionTable[mCurrentState][mInputSymbols[i]];
	}
	
	setCurrentState(mCurrentState);

}

void DCDCBoost2xGAController::setCurrentState(unsigned int inState) {
	switch(inState){
		case 0:
			(*this)[0]->setState(false);
			(*this)[1]->setState(true);
			(*this)[2]->setState(true);
			break;
		case 1:
			(*this)[0]->setState(true);
			(*this)[1]->setState(false);
			(*this)[2]->setState(true);
			break;
		case 2:
			(*this)[0]->setState(true);
			(*this)[1]->setState(true);
			(*this)[2]->setState(false);
			break;
//		case 3:
//			(*this)[0]->setState(true);
//			(*this)[1]->setState(false);
//			(*this)[2]->setState(false);
//			break;
		default:
			throw runtime_error(string("DCDCBoost2xGAController::setCurrentState invalid state ")+uint2str(inState));
	}
	
	
}

void DCDCBoost2xGAController::generateInputSymbols(vector<double> inInputs) {
	
	//Input symbols are a combination of two tanks
	double lVThreshold = 0.01;
	double lIThreshold = 0.01;
	double lCurrentLimit = 2.5;
	mInputSymbols.resize(0);
	
#ifdef NOCURRENT_LIMIT
	vector<int> lConditionSize(3,3);
#else
	vector<int> lConditionSize(5,3);
#endif
	vector<int> lConditionStatus(lConditionSize.size(),0);
	
	if( inInputs[0] < (mTargets[0] - lVThreshold) ) {
		lConditionStatus[0] = 0;
	} else if( inInputs[0] > (mTargets[0] + lVThreshold) ) {
		lConditionStatus[0] = 1;
	} else {
		lConditionStatus[0] = 2;
	}
	
	if( inInputs[1] < (mTargets[1] - lVThreshold) ) {
		lConditionStatus[1] = 0;
	} else if( inInputs[1] > (mTargets[1] + lVThreshold) ) {
		lConditionStatus[1] = 1;
	} else {
		lConditionStatus[1] = 2;
	}

#ifdef NOCURRENT_LIMIT
	if( fabs(inInputs[2]) >= mTargets[2] ) {
		lConditionStatus[2] = 0;
	} else if (inInputs[2] < 0) {
		lConditionStatus[2] = 1;
	} else {
		lConditionStatus[2] = 2;
	}
#else
	if( inInputs[2] < (mTargets[2] - lIThreshold) ) {
		lConditionStatus[2] = 0;
	} else if( inInputs[2] > (mTargets[2] + lIThreshold) ) {
		lConditionStatus[2] = 1;
	} else {
		lConditionStatus[2] = 2;
	}
	
	
	lConditionSize[3] = 2;
	if( fabs(inInputs[2]) >= lCurrentLimit ) {
		lConditionStatus[3] = 0;
	} else {
		lConditionStatus[3] = 1;
	}
	
	lConditionSize[4] = 2;
	if( fabs(inInputs[2]) < 0 ) {
		lConditionStatus[4] = 0;
	} else {
		lConditionStatus[4] = 1;
	}
	
#endif
	
//	if( fabs(inInputs[2]) < (mTargets[2] - lIThreshold) ) {
//		lConditionStatus[2] = 0;
//	} else if( fabs(inInputs[2]) > (mTargets[2] + lIThreshold) ) {
//		lConditionStatus[2] = 1;
//	} else {
//		lConditionStatus[2] = 2;
//	}
	


	//Convert to symbol number
//	int lMultiplier = lConditionSize[0];
//	int lSymbol = lConditionStatus[0];
//	for(unsigned int i = 1; i < lConditionStatus.size(); ++i) {
//		lSymbol += lConditionStatus[i]*lMultiplier;
//		lMultiplier *= lConditionSize[i];
//	}
	
	int lSymbol = convertSymbole(lConditionSize,lConditionStatus);
	
	if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), lSymbol) == mLastInputSymbols.end()) || mStart ) {
		mInputSymbols.push_back(lSymbol);
		mLastInputSymbols = mInputSymbols;
	}
	
	mStart = false;

}

void DCDCBoost2xGAController::createBondGraph(HybridBondGraph &ioBondGraph) {
	
	mParametricComponents.clear();
	
	//Create the sources
	Source *lSe = new Source(Source::eEffort);
	lSe->setValue(1.5);
	mParametricComponents.push_back(lSe);
	ioBondGraph.addComponent(lSe);
	
	//Create a switch
	Switch *lSw1 = new Switch;
	push_back(lSw1);
	Switch *lSw2 = new Switch;
	push_back(lSw2);
	Switch *lSw3 = new Switch;
	push_back(lSw3);
	
	
	//Create the jonctions
	Junction *lJ1_1 = new Junction(Junction::eOne);
	ioBondGraph.addComponent(lJ1_1);
	
	Junction *lJ0_1 = new Junction(Junction::eZero);
	ioBondGraph.addComponent(lJ0_1);
	
	Junction *lJ1_2 = new Junction(Junction::eOne);
	ioBondGraph.addComponent(lJ1_2);
	
	Junction *lJ0_2 = new Junction(Junction::eZero);
	ioBondGraph.addComponent(lJ0_2);
	
	Junction *lJ0_3 = new Junction(Junction::eZero);
	ioBondGraph.addComponent(lJ0_3);
	
	Junction *lJ1_3 = new Junction(Junction::eOne);
	ioBondGraph.addComponent(lJ1_3);
	
	//Create component
	Passive *lI = new Passive(Passive::eInductor);
	lI->setValue(75e-6);
	ioBondGraph.addComponent(lI);
	
	Passive *lCa = new Passive(Passive::eCapacitor);
	lCa->setValue(800e-6);
	ioBondGraph.addComponent(lCa);
	
	Passive *lRa = new Passive(Passive::eResistor);
	lRa->setValue(6.25);
	mParametricComponents.push_back(lRa);
	ioBondGraph.addComponent(lRa);
	
	Passive *lCb = new Passive(Passive::eCapacitor);
	lCb->setValue(146.6e-6);
	ioBondGraph.addComponent(lCb);
	
	Passive *lRb = new Passive(Passive::eResistor);
	lRb->setValue(34.1);
	mParametricComponents.push_back(lRb);
	ioBondGraph.addComponent(lRb);
	
	//Create the bonds
	std::vector<Bond*> lEffortOutputBonds(2);
	std::vector<Bond*> lFlowtOutputBonds(1);
	
	ioBondGraph.connect(lSe,lJ1_1);
	lFlowtOutputBonds[0] = ioBondGraph.connect(lJ1_1,lI); 
	
	ioBondGraph.connect(lJ1_1,lJ0_1);
	ioBondGraph.connect(lJ0_1,lSw1);
	ioBondGraph.connect(lJ0_1,lJ1_2);
	ioBondGraph.connect(lJ1_2,lSw2);
	ioBondGraph.connect(lJ1_2,lJ0_2);
	ioBondGraph.connect(lJ0_2,lCa);
	lEffortOutputBonds[0] = ioBondGraph.connect(lJ0_2,lRa);
	
	ioBondGraph.connect(lJ0_1,lJ1_3);
	ioBondGraph.connect(lJ1_3,lSw3);
	ioBondGraph.connect(lJ1_3,lJ0_3);
	ioBondGraph.connect(lJ0_3,lCb);
	lEffortOutputBonds[1] = ioBondGraph.connect(lJ0_3,lRb);
	
//	this->addTrigger(mOutBonda,eEffort);
//	this->addTrigger(mOutBondb,eEffort);
//	this->addTrigger(mIBond,eFlow);
	
	ioBondGraph.setOutputBonds(lEffortOutputBonds,lFlowtOutputBonds);
	
	ioBondGraph.addSwitchController(this);
	ioBondGraph.postConnectionInitialization();


}


