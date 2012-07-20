/*
 *  LogIndividualOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 10/07/10.
 */

#include "LogIndividualOp.h"

#include <beagle/Individual.hpp>
#include <beagle/IndividualBag.hpp>
#include <beagle/Deme.hpp>
#include <beagle/IsLessPointerPredicate.hpp>
#include <beagle/Logger.hpp>
#include <beagle/Context.hpp>

using namespace Beagle;

LogIndividualOp::LogIndividualOp(std::string inName) 
: Beagle::Operator(inName),
mPopSize(NULL),
mOnlyVivarium(NULL),
mNumberIndividualPerDem(NULL),
mNbDemesCalculated(0),
mGenerationCalculated(0) {
}

void LogIndividualOp::initialize(Beagle::System& ioSystem) {
	if(ioSystem.getRegister().isRegistered("log.individual.vivariumonly")) {
		mOnlyVivarium = castHandleT<Bool>(ioSystem.getRegister().getEntry("log.individual.vivariumonly"));
	} else {
		mOnlyVivarium = new Bool(true);
		std::string lLongDescrip("Log only the best individual in the vivarium.");
		Register::Description lDescription(
										   "Log individual for the vivarium only",
										   "Bool",
										   mOnlyVivarium->serialize(),
										   lLongDescrip
										   );
		ioSystem.getRegister().addEntry("log.individual.vivariumonly", mOnlyVivarium, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("log.individual.size")) {
		mNumberIndividualPerDem = castHandleT<UInt>(ioSystem.getRegister().getEntry("log.individual.size"));
	} else {
		mNumberIndividualPerDem = new UInt(1);
		std::string lLongDescrip("Number of individual per demes to log. ");
		Register::Description lDescription(
										   "Number of individual per demes to log.",
										   "UInt",
										   mNumberIndividualPerDem->serialize(),
										   lLongDescrip
										   );
		ioSystem.getRegister().addEntry("log.individual.size", mNumberIndividualPerDem, lDescription);
	}
	if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
		mPopSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
	} else {
		mPopSize = new UIntArray(1,100);
		string lLongDescrip("Number of demes and size of each deme of the population. ");
		lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
		lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
		lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
		lLongDescrip += "deme.";
		Register::Description lDescription(
										   "Vivarium and demes sizes",
										   "UIntArray",
										   "100",
										   lLongDescrip
										   );
		ioSystem.getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
	}
}

void LogIndividualOp::operate(Deme& ioDeme, Context& ioContext) {
	Beagle_StackTraceBeginM();
	
	if(mNumberIndividualPerDem->getWrappedValue() <= 0) {
		return;
	}
	
	// Temporary buffer of individuals.
	Individual::Bag lTempPop;
	
	if(ioContext.getGeneration() != mGenerationCalculated) {
		mGenerationCalculated = ioContext.getGeneration();
		mNbDemesCalculated = 0;
	}
	
	if(++mNbDemesCalculated == mPopSize->size() && mOnlyVivarium->getWrappedValue()) {
		//Make heap of all individual in the vivarium
		for( unsigned int i = 0; i < ioContext.getVivarium().size(); ++i) {
			lTempPop.insert(lTempPop.end(), ioContext.getVivarium()[i]->begin(), ioContext.getVivarium()[i]->end());
		}
	} else if(mOnlyVivarium->getWrappedValue()){
		return;
	} else {
		//Process only this deme
		// Insert pointer of all the individuals of the deme in the buffer.
		lTempPop.insert(lTempPop.end(), ioDeme.begin(), ioDeme.end());
	}
	// Make the buffer a STL heap with the fittest individual on the top.
	std::make_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());	
	
	for(unsigned int i = 0; i < mNumberIndividualPerDem->getWrappedValue(); ++i) {
		Beagle_LogObjectM(
						  ioContext.getSystem().getLogger(),
						  Logger::eStats,
						  "history", "Beagle::LogIndividualDataOp",
						  *lTempPop[0]
						  );
		
		
		// STL heap pop of the best individual of the temporary buffer.
		std::pop_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());
		lTempPop.pop_back();
	}
	
	
	Beagle_StackTraceEndM("void LogIndividualOp::operate(Deme& ioDeme, Context& ioContext)");
}
