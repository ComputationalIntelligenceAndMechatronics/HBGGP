/*
 *  DCDCBoostEvalOp.h
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
 *  This file was created by Jean-Francois Dupuis on 25/03/10.
 */

#ifndef DCDCBoostEvalOp_H
#define DCDCBoostEvalOp_H

#include <beagle/GP.hpp>
#include "BondGraph.h"
#include "BondGraphEvalOp.h"
#include "SimulationCase.h"
#include <map>
#include <vector>
#include <string>


class DCDCBoostEvalOp : public BondGraphEvalOp {
public:
	typedef Beagle::AllocatorT<DCDCBoostEvalOp,BondGraphEvalOp::Alloc> Alloc;
	typedef Beagle::PointerT<DCDCBoostEvalOp,BondGraphEvalOp::Handle> Handle;
	typedef Beagle::ContainerT<DCDCBoostEvalOp,BondGraphEvalOp::Bag> Bag;
	
	DCDCBoostEvalOp(std::string inName="DCDCBoostEvalOp");
	~DCDCBoostEvalOp();
	
	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext);
	
	virtual void initialize(Beagle::System& ioSystem);
	virtual void postInit(Beagle::System& ioSystem);
	
	
private:
	double computeError(const BG::BondGraph* inBondGraph, std::map<std::string, std::vector<double> > &lSimulationLog);
	static bool mIsInitialized;
	
	Beagle::String::Handle mTargetString;
	std::vector<SimulationCase> mSimulationCases;
	Beagle::FloatArray::Handle mGenerationSteps;
	
	//	Beagle::Float::Handle mTargetTolerances;
	Beagle::FloatArray::Handle mTanksLevelsIni;
	Beagle::Float::Handle mSimulationDuration;
	Beagle::Float::Handle mContinuousTimeStep;
	//	Beagle::Float::Handle mDiscreteTimeStep;
	
//	Beagle::FloatArray::Handle mCapacitance;
//	Beagle::Float::Handle mResistance;
	
	double mSourceValue;

	Beagle::Float::Handle mPenaltyFactor;
	
	Beagle::Int::Handle mMaxNumberSwitch;
	Beagle::Int::Handle mAllowDifferentialCausality;
	
	std::vector<Beagle::FloatArray::Handle> mTargetArrays;
	
	long int mIndividualCounter;
};


#endif
