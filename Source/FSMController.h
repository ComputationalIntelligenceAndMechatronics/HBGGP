/*
 *  FSMController.h
 *  Copyright 2008 Jean-Francois Dupuis.
 *
 *  This file is part of EvoStateController.
 *  
 *  EvoStateController is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EvoStateController is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EvoStateController.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  This file was created by Jean-Francois Dupuis on 03/10/08.
 */

#ifndef FSMController_H
#define FSMController_H

#include <vector>
#include <map>
#include "Controller.h"

using namespace std;

class FSMController : public Controller {

public:
	FSMController(map<string, vector<double> > *inLogger) : Controller(inLogger), mInputSymbols(0), mLastInputSymbols(0) { }
	~FSMController() {}
	
	virtual void initialize(unsigned int inStartState, const vector<double>& inContinuousState) = 0;
	
	vector<double> getOutputs();
	void setInput(const vector<double>& inInputs);
	
	void setTarget(const vector<double>& inTargets) { mTargets = inTargets;	}
	
	void setTransitionTable(const vector< vector<unsigned int> > &inTransitionTable) { mTransitionTable = inTransitionTable; }
	virtual void writeLog() = 0;
	
protected:
	virtual void generateInputSymbols(vector<double> inInputs) = 0;
	virtual vector<double> generateOutputSymbols() = 0;
	virtual void processInput() = 0;
	
	unsigned int mNbStates;
	
	unsigned int mCurrentState;
	int mTransitionAction;
	
	vector<double> mOutputSymbols;
	vector<double> mInputSymbols;
	vector<double> mLastInputSymbols;

	bool mStart;
	
	vector< vector<unsigned int> > mTransitionTable; //< Transition table, first Index is the current state, second Index is the received inputs. The contenant is the new state
};

#endif
