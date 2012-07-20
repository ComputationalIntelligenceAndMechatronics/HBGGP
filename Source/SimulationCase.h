/*
 *  SimulationCase.h
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

#ifndef SimulationCase_H
#define SimulationCase_H

#include <vector>
#include <string>
#include <Util/Randomizer.hpp>

using namespace std;

class SimulationCase {
public:
	SimulationCase() {}
	~SimulationCase() {}
	
	void clear() { mTimes.clear(); mTargets.clear(); }

	void addTargets(double inTime, const vector<double> &inTargets, const vector<double> &inParameters) {
		mTargets.push_back(inTargets);
		mParameters.push_back(inParameters);
		mTimes.push_back(inTime); 
	}
	
	void addTargets(double inTime, const vector<double> &inTargets) {
		mTargets.push_back(inTargets);
		mTimes.push_back(inTime); 
	}
	
	unsigned int getSize() const { return mTimes.size(); }
	
	double getTime(unsigned int inIndex) const { return mTimes[inIndex]; }
	const vector<double>& getTargets(unsigned int inIndex) const { return mTargets[inIndex]; }
	const vector<double>& getParameters(unsigned int inIndex) const { return mParameters[inIndex]; }
				
	void write(ostream &inStream) const;
	
	void createRandomCase(PACC::Randomizer *inRadomizer, unsigned int inNumberOfTarget, vector<double> inLimits, vector<double> inTimes);
	
private:
	vector<double> mTimes;
	vector< vector<double> > mTargets;
	vector< vector<double> > mParameters;
};

void readSimulationCase(std::string inString, std::vector<SimulationCase> &outCases);

#endif
