/*
 *  SimulationCase.cpp
 *  Copyright 2008 Jean-Francois Dupuis.
 *
 *  This file is part of ThreeTanks.
 *  
 *  ThreeTanks is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ThreeTanks is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ThreeTanks.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  This file was created by Jean-Francois Dupuis on 28/11/08.
 */

#include "SimulationCase.h"
#include <sstream>
#include <Util.hpp>
#include "VectorUtil.h"

void SimulationCase::write(ostream &inStream) const {
	for(unsigned int i = 0; i < mTimes.size(); ++i) {
		inStream << mTimes[i];
		
		if(mParameters.size() > 0 && !mParameters[i].empty()) {
			inStream << "[" << mParameters[i] << "]";
		}
		inStream << "(" << mTargets[i] << ")";
		
		if(i != mTimes.size()-1)
			inStream << ",";
	}
}

void readSimulationCase(string inString, vector<SimulationCase> &outCases) {
	//Parse the targets of the form T0(t1,t2,...),T1(t1,t2,...),T2(t1,t2,...); T0(t1,t2,...), ...
	// or
	//Parse the targets of the form T0[p1,p2,..](t1,t2,...),T1[p1,p2,..](t1,t2,...),T2[p1,p2,..](t1,t2,...); T0(t1,t2,...)(p1,p2,..), ...
	
	outCases.clear();
			
	istringstream lCaseStream(inString);
	
	PACC::Tokenizer lCaseTokenizer(lCaseStream);
	lCaseTokenizer.setDelimiters(";","");
	string lCaseToken;
	while(lCaseTokenizer.getNextToken(lCaseToken)) {
		SimulationCase lCase;
	
		istringstream lStream(lCaseToken);
		PACC::Tokenizer lTokenizer(lStream);
		lTokenizer.setDelimiters(" \n\r\t,()[]", "");
		string lToken;
		
		bool lLastValue = false;
		bool lReadTime = false;
		bool lReadTarget = false;
		bool lReadTargetNext = false;
		vector<double> lTargets;
		vector<double> lParameters;
		double lTime;
	
		while(lTokenizer.getNextToken(lToken)) {
			int lDelimiter = lTokenizer.peekNextChar();
			if(lDelimiter == '(') {
				lReadTime = true;
				lReadTargetNext = true;
			} else if(lDelimiter == '[') {
				lReadTime = true;
				lReadTargetNext = false;
			} else if(lDelimiter == ')') {
				lReadTime = false;
				lReadTarget = true;
				lLastValue = true;
			} else if(lDelimiter == ']') {
				lReadTime = false;
				lReadTarget = false;
				lReadTargetNext = true;
			} else {
				lReadTime = false;
				if(lReadTargetNext)
					lReadTarget = true;
				else
					lReadTarget = false;
			}
			
			if(lReadTime) {
				lTime = atof(lToken.c_str());
			} else if(lReadTarget) {
				lTargets.push_back(atof(lToken.c_str()));
				if(	lLastValue) {
					lCase.addTargets(lTime, lTargets, lParameters);
					lTargets.clear();	
					lParameters.clear();
					lLastValue = false;
				}
			} else {
				lParameters.push_back(atof(lToken.c_str()));
			}
		}
		outCases.push_back(lCase);
	}
}

void SimulationCase::createRandomCase(PACC::Randomizer *inRadomizer, unsigned int inNumberOfTarget, vector<double> inLimits, vector<double> inTimes) {
	for(unsigned int i = 0; i < inTimes.size(); ++i) {
		vector<double> lTargets(inNumberOfTarget);
		for(unsigned int j = 0; j < inNumberOfTarget; ++j) {
			lTargets[j] = inRadomizer->getFloat(inLimits[0], inLimits[1]);
		}
		this->addTargets(inTimes[i], lTargets);
	}
}
