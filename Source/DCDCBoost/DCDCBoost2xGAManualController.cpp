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

#include "DCDCBoost2xGAManualController.h"
#include <cfloat>
#include <assert.h>
#include "VectorUtil.h"

void DCDCBoost2xGAManualController::writeLog() {
	assert(size() == 3);
	(*mLogger)[(*this)[0]->getName()+std::string(".state")].push_back((*this)[0]->getState());
	(*mLogger)[(*this)[1]->getName()+std::string(".state")].push_back((*this)[1]->getState());
	(*mLogger)[(*this)[2]->getName()+std::string(".state")].push_back((*this)[2]->getState());
	(*mLogger)["TargetVa"].push_back(mTargets[0]);
	(*mLogger)["TargetVb"].push_back(mTargets[1]);
	(*mLogger)["TargetI"].push_back(mTargets[2]);
	(*mLogger)["State"].push_back(mCurrentState);
}

void DCDCBoost2xGAManualController::initialize(HybridBondGraph *inBondGraph, unsigned int inInitialSwState, const vector<double> &inStateValues) {
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

void DCDCBoost2xGAManualController::reset() { 
	initialize(mBondGraph, mInitialSwState,mInitialState);
}

void DCDCBoost2xGAManualController::updateSwitchState(double inTime, const vector<double>& inInputs) {
	
//	assert(size() == 3);
//	assert(mTransitionTable.size() == NBSTATE);
//	
//	
//	generateInputSymbols(inInputs);
//	for(unsigned int i = 0; i < mInputSymbols.size(); ++i) {
//		mCurrentState = mTransitionTable[mCurrentState][mInputSymbols[i]];
//	}
	
//	setCurrentState(mCurrentState);
//	setCurrentState(0);
	
	
	//////////////////////
	
	assert(size() == 3);
	
	std::string lTargetVa = "TargetVa";
	std::string lTargetVb = "TargetVb";
	std::string lTargetI = "TargetI";
	std::string lOutputVa = mOutBonda->getName()+std::string(".e");
	std::string lOutputVb = mOutBondb->getName()+std::string(".e");
	std::string lOutputI = mIBond->getName()+std::string(".f");
	
	
	double lVa = inInputs[mSwitchTriggers[0]->getIndex()];
	double lVb = inInputs[mSwitchTriggers[1]->getIndex()];
	double lI = inInputs[mSwitchTriggers[2]->getIndex()];


	double lTVa = (*mLogger)[lTargetVa].back();
	double lTVb = (*mLogger)[lTargetVb].back();
	double lTI = 0.65;//(*mLogger)[lTargetI].back();
	

	PACC::Vector lTarget(3,0);
	lTarget[0] = lTVa - lVa;
	lTarget[1] = lTVb - lVb;
	lTarget[2] = lTI - lI;
	
	vector< PACC::Vector > lTrajetories(NBSTATE);
	
	//Get the direction of each state
	for(unsigned int i = 0; i < NBSTATE; ++i) {
		setCurrentState(i);
		vector<bool> lSwState(3,0);
		lSwState[0] = (*this)[0]->getState();
		lSwState[1] = (*this)[1]->getState();
		lSwState[2] = (*this)[2]->getState();
		
		vector<double> lResults;
		mBondGraph->simulateVirtual(lSwState,15e-6, lResults);
		
		vector<string> lNames = mBondGraph->getOutputVariableNames();
		map<std::string, vector<double> > lSimData;
		cout << "State: " << lSwState << endl;
		for(unsigned int j = 0; j < lResults.size(); ++j) {
			lSimData[lNames[j]].push_back(lResults[j]);
			cout << lNames[j] << ": " << lResults[j] << endl;
		}
		
		double lVa_ = lSimData[lOutputVa].back();
		double lVb_ = lSimData[lOutputVb].back();
		double lI_ = lSimData[lOutputI].back();
		
		lTrajetories[i].resize(3);
		lTrajetories[i][0] = lVa_ - lVa;
		lTrajetories[i][1] = lVb_ - lVb;
		lTrajetories[i][2] = lI_ - lI;
	}
	
	//Compute the angle between states trajectories and target direction
	vector<double> lTheta(lTrajetories.size(),0);
	unsigned int lMinState = 0;
	double lNormt = lTarget.computeL2Norm();
	for(unsigned int i = 0; i < lTrajetories.size(); ++i) {
		double lNorma = lTrajetories[i].computeL2Norm();
		double lDotProd = lTarget*lTrajetories[i];
		
//		lTheta[i] = (lTarget*lTrajetories[i])/lTrajetories[i].computeL2Norm();
		lTheta[i] = lDotProd/(lNormt*lNorma);
		
//		cout << lTrajetories[i] << endl;
//		cout << lTarget << endl;
		cout << "State " << i << "  norm: " << lNorma << "  dotproduct: " << lDotProd << "  theta: " << lTheta[i] << endl;
		
		if(fabs(lTheta[i]) < lTheta[lMinState])
			lMinState = i;
	}
	
	mCurrentState = lMinState;
	setCurrentState(lMinState);

}

void DCDCBoost2xGAManualController::setCurrentState(unsigned int inState) {
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
			throw runtime_error("DCDCBoost2xGAManualController::setCurrentState invalid state");
	}
	
	
}

void DCDCBoost2xGAManualController::generateInputSymbols(vector<double> inInputs) {
	
	//Input symbols are a combination of two tanks
	double lVThreshold = 0.01;
	//double lIThreshold = 0.1;
	mInputSymbols.resize(0);
	
	vector<int> lConditionSize(3,3);
	lConditionSize[2] = 2;
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

	if( fabs(inInputs[2]) >= mTargets[2] ) {
		lConditionStatus[2] = 0;
	} else {
		lConditionStatus[2] = 1;
	} 
	
//	if( fabs(inInputs[2]) < (mTargets[2] - lIThreshold) ) {
//		lConditionStatus[2] = 0;
//	} else if( fabs(inInputs[2]) > (mTargets[2] + lIThreshold) ) {
//		lConditionStatus[2] = 1;
//	} else {
//		lConditionStatus[2] = 2;
//	}
	
	//Convert to symbol number
	int lMultiplier = lConditionSize[0];
	int lSymbol = lConditionStatus[0];
	for(unsigned int i = 1; i < lConditionStatus.size(); ++i) {
		lSymbol += lConditionStatus[i]*lMultiplier;
		lMultiplier *= lConditionSize[i-1];
	}
	
	if( (find(mLastInputSymbols.begin(),mLastInputSymbols.end(), lSymbol) == mLastInputSymbols.end()) || mStart ) {
		mInputSymbols.push_back(lSymbol);
		mLastInputSymbols = mInputSymbols;
	}
	
	mStart = false;
}

double DCDCBoost2xGAManualController::computeError() {
	
	
	
	std::vector<double> lErrors(NBOUTPUTS,0);
	std::vector<bool> lZeroOutput(NBOUTPUTS,true);
	for(unsigned int k = 0; k < NBOUTPUTS; ++k) {
		const std::vector<double>& lTarget = (*mLogger)[std::string("Target_")+int2str(k)];
		std::vector<double>& lOutput = (*mLogger)[std::string("Output_")+int2str(k)];
		const std::vector<double>& lTime = (*mLogger)["time"];
		
		unsigned int lDataSize = lTime.size();
		assert(lTime.size() == lOutput.size());
		assert(lTime.size() == lTarget.size());
		assert(lDataSize != 0);
		
		std::vector<double> lError(lDataSize);
		for(unsigned int i = 0; i < lDataSize; ++i) {
			//lError[i] = fabs(lOutput[i] - lTarget[i])/lTarget[i];// * lTime[i]/lTime.back()*2;
			
			if(lOutput[i] != 0) 
				lZeroOutput[k] = false;
			
			if( k < 2 ) 
				lError[i] = fabs(lOutput[i] - lTarget[i])/lTarget[i];	
			else {
				//Compute the error on the current by making sure it doesn't exceed a limit
				double lCurrentLimit = 2;
				lError[i] = (lOutput[i] - lCurrentLimit)/lCurrentLimit;
				if( lError[i] < 0 )
					lError[i] = 0;
				else
					lError[i] = fabs(lError[i] * 10); //If over the limits, greatly penalise
			}
			
		}
		
		//Integrate the error vector
		for(unsigned int i = 0; i < lDataSize-1; ++i) {
			double dt = lTime[i+1] - lTime[i];
			lErrors[k] += (lError[i]+lError[i+1])/2*dt;
		}
	}
	
	
	//	//Take the mean errors of the two targets
	//	double lF = 0;
	//	for(unsigned int k = 0; k < lErrors.size(); ++k) {
	//		lF += lErrors[k];
	//	}
	//	lF = lF/lErrors.size();
	
	//cout << "Errors: " << lErrors << endl;
	
	//Take the worst of the two tanks
	double lF = lErrors[0];
	for(unsigned int k = 1; k < lErrors.size(); ++k) {
		lF = max(lF,lErrors[k]);
	}
	
	//	//Look at the worst of the output voltage, disregard the current
	//	double lF = max(lErrors[0],lErrors[1]);
	
	
	
	if(lF != 0)
		lF = 1/lF;
	else
		lF = DBL_MAX;
	
	//Look if the output is null
	bool lZero = true;
	for(unsigned int i = 0 ; i < lZeroOutput.size(); ++i) {
		lZero = lZero && lZeroOutput[i];
		if(!lZero)
			break;
	}
	if(lZero)
		lF = 0;
	
	return lF;		

}


//double DCDCBoost2xGAManualController::computeError() {
//	std::string lTargetVa = "TargetVa";
//	std::string lTargetVb = "TargetVb";
//	std::string lTargetI = "TargetI";
//	
//	std::string lOutputVa = mOutBonda->getName()+std::string(".e");
//	std::string lOutputVb = mOutBondb->getName()+std::string(".e");
//	std::string lOutputI = mIBond->getName()+std::string(".f");
//	unsigned int lDataSize = (*mLogger)[lTargetVa].size();
//	
//	assert((*mLogger)[lTargetVa].size() == (*mLogger)[lOutputVa].size() );
//	assert((*mLogger)[lTargetVb].size() == (*mLogger)[lOutputVb].size() );
//	assert((*mLogger)[lTargetVa].size() == (*mLogger)["time"].size() );
//	assert(lDataSize != 0);
//	
//	std::vector<double> lErrorVa(lDataSize);
//	std::vector<double> lErrorVb(lDataSize);
//	std::vector<double> lErrorI(lDataSize);
//	for(unsigned int i = 0; i < lDataSize; ++i) {
//		lErrorVa[i] = fabs((*mLogger)[lOutputVa][i] - (*mLogger)[lTargetVa][i]);
//		lErrorVa[i] *= lErrorVa[i];
//		lErrorVb[i] = fabs((*mLogger)[lOutputVb][i] - (*mLogger)[lTargetVb][i]);
//		lErrorVb[i] *= lErrorVb[i];
//		
//		lErrorI[i] = (*mLogger)[lOutputI][i] - (*mLogger)[lTargetI][i];
//		if(lErrorI[i] < 0)
//			lErrorI[i] = 0; //if under limit, fine no penalty
//		else
//			lErrorI[i] *= lErrorI[i]; //if over, greatly penalise.
//	}
//	
//	std::vector<double> lErrors(3,0);
//	for(unsigned int i = 0; i < lDataSize-1; ++i) {
//		double dt = (*mLogger)["time"][i+1] - (*mLogger)["time"][i];
//		lErrors[0] += (lErrorVa[i]+lErrorVa[i+1])/2*dt;
//		lErrors[1] += (lErrorVb[i]+lErrorVb[i+1])/2*dt;
//		lErrors[2] += (lErrorI[i]+lErrorI[i+1])/2*dt;
//	}
//	
//	//Take the mean errors of the two targets
//	//double lF = (lErrors[0]+lErrors[1])/2;
//	
//	//Take the worst of the three targets
//	double lF = max(lErrors[0],lErrors[1]); 
//	lF =  max(lF,lErrors[2]);
//
//
//	if(lF != 0)
//		lF = 1/lF;
//	else
//		lF = DBL_MAX;
//	
//	return lF;
//}

void DCDCBoost2xGAManualController::createBondGraph(HybridBondGraph &ioBondGraph) {
	
	//Create the sources
	Source *lSe = new Source(Source::eEffort);
	lSe->setValue(1.5);
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
	ioBondGraph.addComponent(lRa);
	
	Passive *lCb = new Passive(Passive::eCapacitor);
	lCb->setValue(146.6e-6);
	ioBondGraph.addComponent(lCb);
	
	Passive *lRb = new Passive(Passive::eResistor);
	lRb->setValue(34.1);
	ioBondGraph.addComponent(lRb);
	
	//Create the bonds
	ioBondGraph.connect(lSe,lJ1_1);
	mIBond = ioBondGraph.connect(lJ1_1,lI); 
	
	ioBondGraph.connect(lJ1_1,lJ0_1);
	ioBondGraph.connect(lJ0_1,lSw1);
	ioBondGraph.connect(lJ0_1,lJ1_2);
	ioBondGraph.connect(lJ1_2,lSw2);
	ioBondGraph.connect(lJ1_2,lJ0_2);
	ioBondGraph.connect(lJ0_2,lCa);
	mOutBonda = ioBondGraph.connect(lJ0_2,lRa);
	
	ioBondGraph.connect(lJ0_1,lJ1_3);
	ioBondGraph.connect(lJ1_3,lSw3);
	ioBondGraph.connect(lJ1_3,lJ0_3);
	ioBondGraph.connect(lJ0_3,lCb);
	mOutBondb = ioBondGraph.connect(lJ0_3,lRb);
	
	this->addTrigger(mOutBonda,eEffort);
	this->addTrigger(mOutBondb,eEffort);
	this->addTrigger(mIBond,eFlow);
	
	ioBondGraph.addSwitchController(this);
	
	ioBondGraph.postConnectionInitialization();
	
	
	ioBondGraph.addOutputBonds(mIBond, eFlow);
	//	ioBondGraph.addOutputBonds(lBond1, eEffort);
	//	ioBondGraph.addOutputBonds(lBond1, eFlow);
	//	ioBondGraph.addOutputBonds(lBond2, eEffort);
	//	ioBondGraph.addOutputBonds(lBond2, eFlow);
	ioBondGraph.addOutputBonds(mOutBonda, eEffort);
	ioBondGraph.addOutputBonds(mOutBondb, eEffort);
	//	ioBondGraph.addOutputBonds(lBond3, eFlow);
	
	
	//	ioBondGraph.connect(lSe,lJ1_1);
	//	Bond* lBond0 = ioBondGraph.connect(lJ1_1,lI);
	//	ioBondGraph.connect(lJ1_1,lJ0_1);
	//	ioBondGraph.connect(lJ0_1,lC);
	//	Bond* lBond3 = ioBondGraph.connect(lJ0_1,lR);
	//	ioBondGraph.addOutputBonds(lBond3, eEffort);
	//	ioBondGraph.addOutputBonds(lBond0, eFlow);

}


