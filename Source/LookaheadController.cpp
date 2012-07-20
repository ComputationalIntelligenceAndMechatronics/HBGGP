/*
 *  LookaheadController.cpp
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

#include "LookaheadController.h"

#include "VectorUtil.h"
#include "stringutil.h"
#include "BGException.h"
//#include "SVD.h"

using namespace BG;
using namespace std;

void LookaheadController::writeLog() {
	(*mLogger)["State"].push_back(mCurrentState);
	for(unsigned int i = 0; i < mTargets.size(); ++i) {
		(*mLogger)[string("Target_")+int2str(i)].push_back(mTargets[i]);
	}
}

void LookaheadController::initialize(HybridBondGraph *inBondGraph, unsigned int inInitialSwState) {
	
	mExcludedStates.resize(0);
	
	mBondGraph = inBondGraph;
	
	setCurrentState(inInitialSwState);
	
	vector<bool> lSwState(this->size(),0);
	for(unsigned int j = 0; j < this->size(); ++j) {
		lSwState[j] = (*this)[j]->getState();
	}
	
	inBondGraph->setInitialSwitchState(lSwState);
	
	inBondGraph->setInitialStateVariable(std::vector<double>(inBondGraph->getStateVariables().size(),0));
}

void LookaheadController::initialize(HybridBondGraph *inBondGraph, unsigned int inInitialSwState, const vector<double> &inOutputValues) {
	mExcludedStates.resize(0);
	
	vector<double> lStateValue(inOutputValues.size());
	
	mBondGraph = inBondGraph;
	
	setCurrentState(inInitialSwState);

	vector<bool> lSwState(this->size(),0);
	for(unsigned int j = 0; j < this->size(); ++j) {
		lSwState[j] = (*this)[j]->getState();
	}
	
	inBondGraph->setInitialSwitchState(lSwState);
	
	mapTarget2State(inOutputValues,lStateValue);
	
	inBondGraph->setInitialStateVariable(lStateValue);
}

/*! \brief Set the control target
 *  The methodes assign the target for the controller. The target will be matched against
 *	the output of the BondGraph::getOutputVariables(). Therefore, the effort output bonds
 *	target should be defined first in the target vector. Then, the flow output bonds
 *	corresponding target should follow.
 */
void LookaheadController::setTarget(const vector<double>& inTargets) {
	mTargets = inTargets;
}


/*! \brief Compute the state variable value from the bond graph output values.
 *  This methods actually inverte the output matrix of the bond graph to find
 *	state variable value according to the desired outputs.
 *  \param  inOutputs Bond graph output value.
 *  \param  outStates Bond graph state variable.
 */
void LookaheadController::mapTarget2State(const vector<double>& inOutputs, vector<double>& outStates) {
	PACC::Matrix lC,lD,lD2;
	mBondGraph->getOutputMatrix(lC,lD,lD2);
	
	if(lC.getRows() > 0) {
		PACC::Vector lu = PACC::Vector(mBondGraph->getInputs());
		PACC::Vector ldudt = PACC::Vector(mBondGraph->getInputsDt());
		if(ldudt.size() == 0)
			ldudt.resize(lu.size());
		
		PACC::Vector lOutputs(inOutputs);
		
		PACC::Vector lb = lOutputs - lD*lu;
		if(mBondGraph->hasDeferentialCausality()) {
			lb -= lD2*ldudt;
		}

		PACC::Vector lx = lC.solve(lb);
		outStates = lx;
	} else {
		outStates.resize(0);
	}
}

/*! \brief Compute the target value from the state variable and output values.
 *  This method should be over defined if the user want to make some computation to calculate
 *	the output value of the bond graph so that they are compatible with the controller
 *	target values.
 *  \param  inState Bond graph state variables
 *  \param  inOutputs Bond grpah output variables 
 *  \param  outTarget Output of the bond graph formulated in the target space.
 */
void LookaheadController::map2Target(const vector<double>& inState, const vector<double>& inOutputs, vector<double>& outTarget) {
	outTarget = inOutputs;
}

int LookaheadController::getNbStates() const {
	return (int( pow(2.0,int(this->size()) ) ));
}

//Version working the output variables
void LookaheadController::updateSwitchState(double inTime, const vector<double>& inInputs, bool inWithInitialParameters) {
	const vector<double>& lStateVariables = mBondGraph->getStateVariables();
	const vector<double>& lOutputsVariables = mBondGraph->getOutputVariables();;
	
	std::vector<double> lCurrentOutput;
	map2Target(lStateVariables,lOutputsVariables,lCurrentOutput); 
	
#ifdef DEBUG_CONTROLLER
	cout << "Time: " << mBondGraph->getSimulationTime() << endl;
	cout << "Current states: " << lStateVariables << endl;
	cout << "Current outputs: " << lCurrentOutput << endl;
	cout << "Current targets:" << mTargets << endl;
#endif
	
	if(mTargets.size() != lCurrentOutput.size()) {
		throw runtime_error("Target and output don't have the same size.");
	}
	
	vector<double> lTargetVector( mTargets.size() );
	for(unsigned int i = 0; i < mTargets.size(); ++i) {
		lTargetVector[i] = mTargets[i] - lCurrentOutput[i];
	}
	
	int lNbState = getNbStates();
	
	//Get the direction of each state
	vector<double> lDistance2Target(lNbState);
	vector< vector<double> > lTrajetories(lNbState);
	for(unsigned int i = 0; i < lNbState; ++i) {
		if(find(mExcludedStates.begin(),mExcludedStates.end(),i) != mExcludedStates.end())
			continue;
		
		try{
			vector<bool> lSwState(this->size());
			unsigned int k = i;
			for(unsigned int j = 0; j < lSwState.size(); ++j) {
				lSwState[j] = (k >> j) & 1;
			}
	
			//Simulation forward at current state
			vector<double> lResults;
			vector<double> lStateResults;
			
			
			if(inWithInitialParameters) {
				mBondGraph->simulateVirtualFixParameters(lSwState,mSimTime, lResults, lStateResults);
			}
			else {
				mBondGraph->simulateVirtual(lSwState,mSimTime, lResults, lStateResults);
			}
			
			lTrajetories[i].resize(mTargets.size());

			std::vector<double> lExpectedOutput;
			map2Target(lStateResults,lResults,lExpectedOutput);
		
			for(unsigned int j = 0; j < mTargets.size(); ++j) {
				lTrajetories[i][j] = lExpectedOutput[j] - lCurrentOutput[j];
			}
			
			lDistance2Target[i] = norm(mTargets - lExpectedOutput);
			
		} catch(BG::CausalityException inError) {
			mExcludedStates.push_back(i);
			continue;
		} 
	}
	
	if(mExcludedStates.size() == lNbState) //All state has differential causality
		throw runtime_error("All switches state have conflicting causality");
	
	//Establish the stating best state.
	int lBestState = 0;
	for(unsigned int i = 0; i < lTrajetories.size(); ++i) {
		if(find(mExcludedStates.begin(),mExcludedStates.end(),i) == mExcludedStates.end()) {
			lBestState = i;
			break;
		}
	}
	
	if(mTargets.size() > 1) {
		//Find the best trajectory based on the smallest angle to target
		vector<double> lTheta(lTrajetories.size(),0);
		for(unsigned int i = 0; i < lTrajetories.size(); ++i) {
			if(find(mExcludedStates.begin(),mExcludedStates.end(),i) != mExcludedStates.end())
				continue;
			
			if(norm(lTrajetories[i]) == 0) {
				if( lCurrentOutput == mTargets ) {
					lTheta[i] = 0 ;
				} else {
					lTheta[i] = DBL_MAX;
				}
			} else {
				//Compute the angle between the trajectory at the current state and the target direction
				lTheta[i] = acos( dot(lTargetVector,lTrajetories[i]) / ( norm(lTargetVector) * norm(lTrajetories[i]) ) );	
			}
			
			//Get the best trajectory
			if(fabs(lTheta[i]) < (lTheta[lBestState])) {
				lBestState = i;
			}
		}
	} else {
		//Trajectory is meaning less in 1D. Only the distance to target should be used.
		for(unsigned int i = 0; i < lDistance2Target.size(); ++i) {
			if(lDistance2Target[i] < lDistance2Target[lBestState]) {
				lBestState = i;
			}
#ifdef DEBUG_CONTROLLER
			cout << "Distance to target: " << i << " : " << lDistance2Target[i] << endl;
#endif	
		}
	}
	
	//Assign the best establish control policy
#ifdef DEBUG_CONTROLLER
	cout << "Choosen state: " << lBestState << endl;
#endif		
	
	mCurrentState = lBestState;
	setCurrentState(lBestState);
}

//Version working of the state variables
//void LookaheadController::updateSwitchState(double inTime, const vector<double>& inInputs) {
//	vector<int> lExcludedStates;
//	vector<double> lStateVariables;
//	mBondGraph->getStateVariables(lStateVariables);
//	mBondGraph->getOutputVariables(lStateVariables);
//	
//	std::vector<double> lCurrentOutput;
//	mapState2Output(lStateVariables,lCurrentOutput);
//	
//	//cout << "Current levels: " << lCurrentOutput << endl;
//	//cout << "Current targets:" << mTargets << endl;
//	
//	assert(mTargets.size() == lCurrentOutput.size());
//	
//	vector<double> lTargetVector( mTargets.size() );
//	for(unsigned int i = 0; i < lStateVariables.size(); ++i) {
//		lTargetVector[i] = mTargets[i] - lCurrentOutput[i];
//	}
//	
//	int lNbState = getNbStates();//int( pow(2.0,int(this->size()) ) );
//		
//	//Get the direction of each state
//	vector<double> lDistance2Target(lNbState);
//	vector< vector<double> > lTrajetories(lNbState);
//	for(unsigned int i = 0; i < lNbState; ++i) {
//		try{
//			setCurrentState(i);
//		} catch(BG::CausalityException inError) {
//			lExcludedStates.push_back(i);
//			continue;
//		}
//		
//		vector<bool> lSwState;
//		mBondGraph->getSwitchState(lSwState);
//		////cout << "Switch state: " << lSwState << endl;
//		
//		//Simulation forward at current state
//		vector<double> lResults;
//		vector<double> lStateResults;
//		mBondGraph->simulateVirtual(lSwState,mSimTime, lResults, lStateResults);
//		
//		lTrajetories[i].resize(mTargets.size());
//		
//		std::vector<double> lExpectedOutput;
//		mapState2Output(lStateResults,lExpectedOutput);
//		//cout << i << ": " << lExpectedOutput << endl;
//		
//		for(unsigned int j = 0; j < mTargets.size(); ++j) {
//			lTrajetories[i][j] = lExpectedOutput[j] - lCurrentOutput[j];
//		}
//		
//		lDistance2Target[i] = norm(mTargets - lExpectedOutput);
//	}
//	
//	if(lExcludedStates.size() == lNbState) //All state has differential causality
//		throw BG::CausalityException("All switches state have differential causality");
//
//	//Find the best trajectory based on the smallest angle to target
//	vector<double> lTheta(lTrajetories.size(),0);
//	int lBestState = 0;
//	for(unsigned int i = 0; i < lTrajetories.size(); ++i) {
//		if(find(lExcludedStates.begin(),lExcludedStates.end(),i) != lExcludedStates.end())
//			continue;
//	
//		if(norm(lTrajetories[i]) == 0) {
//			if( lCurrentOutput == mTargets ) {
//				lTheta[i] = 0 ;
//			} else {
//				lTheta[i] = DBL_MAX;
//			}
//		} else {
//			//Compute the angle between the trajectory at the current state and the target direction
//			lTheta[i] = acos( dot(lTargetVector,lTrajetories[i]) / ( norm(lTargetVector) * norm(lTrajetories[i]) ) );	
//			//cout << i << ": " << lTheta[i] <<endl;
//		}
//		
//		//Get the best trajectory outside any cones in case no trajectory fall inside the cone
//		if(fabs(lTheta[i]) < (lTheta[lBestState])) {
//			lBestState = i;
//		}
//	}
//	
//	//Assign the best establish control policy
//	//cout << "State: " << lBestState << endl;
//	mCurrentState = lBestState;
//	setCurrentState(lBestState);
//}



void LookaheadController::setCurrentState(unsigned int inState) {
	for(unsigned int i = 0; i < this->size(); ++i) {
		(*this)[i]->setState( (inState >> i) & 1);
	}
}

