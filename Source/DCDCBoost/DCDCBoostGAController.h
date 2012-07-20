/*
 *  DCDCBoostBG.h
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

#ifndef DCDCBoostBG_H
#define DCDCBoostBG_H

#include "HybridBondGraph.h"
#include "GASwitchController.h"

using namespace BG;

#define NBSTATE 2
#define NBSWITCH 2
#define NBINPUTSYBMOLE 9

class DCDCBoostGAController : public GASwitchController {
public:	
	DCDCBoostGAController() { mBondGraph = 0; }
	
	virtual void initialize() {}
	void initialize(HybridBondGraph *inBondGraph, unsigned int inInitialSwState, const vector<double> &inStateValues);
							
	virtual void writeLog();
	virtual void reset();
	virtual void updateSwitchState(double inTime, const vector<double>& inInputs);
	
	virtual void createBondGraph(HybridBondGraph &ioBondGraph);

	void setCurrentState(unsigned int inState);
	
	double computeError();
	
protected:
	virtual void generateInputSymbols(vector<double> inInputs);
	
	vector<double> mInputSymbols;
	vector<double> mLastInputSymbols;
	
	Bond* mOutBond;
	Bond* mIBond;
	
	bool mStart;
	
	unsigned int mInitialSwState;
	vector<double> mInitialState;
	
	HybridBondGraph *mBondGraph;
	
	///
	double mP;
	double mPrevP;

};

#endif
