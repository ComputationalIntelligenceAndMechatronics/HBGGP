/*
 *  LogIndividualNoDataOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 11/07/10.
 */

#include "LogIndividualDataOp.h"
#include <beagle/Individual.hpp>
#include <beagle/IndividualBag.hpp>
#include <beagle/Deme.hpp>
#include <beagle/IsLessPointerPredicate.hpp>
#include <beagle/Logger.hpp>
#include <beagle/Context.hpp>
#include "LogFitness.h"

using namespace Beagle;

LogIndividualDataOp::LogIndividualDataOp(std::string inName) 
: LogIndividualOp(inName),mKeepData(NULL) {
	
}

void LogIndividualDataOp::initialize(Beagle::System& ioSystem) {
	LogIndividualOp::initialize(ioSystem);
	
	if(ioSystem.getRegister().isRegistered("log.individual.keepdata")) {
		mKeepData = castHandleT<Bool>(ioSystem.getRegister().getEntry("log.individual.keepdata"));
	} else {
		mKeepData = new Bool(1);
		std::string lLongDescrip("Keep data associated to the individual when logging. ");
		Register::Description lDescription(
										   "Keep individual data",
										   "Bool",
										   mNumberIndividualPerDem->serialize(),
										   lLongDescrip
										   );
		ioSystem.getRegister().addEntry("log.individual.keepdata", mKeepData, lDescription);
	}
}

void LogIndividualDataOp::operate(Deme& ioDeme, Context& ioContext) {
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
		Individual::Handle lIndividual = NULL;
		if( !mKeepData->getWrappedValue() ) {
			//Strip the simulation data of the individual
			lIndividual = castHandleT<Individual>(ioDeme.getTypeAlloc()->cloneData(*lTempPop[0]));
			LogFitness::Handle lFitness = castHandleT<LogFitness>(lIndividual->getFitness());
			lFitness->clearData();
			
		} else {
			lIndividual = lTempPop[0];
		}

		Beagle_LogObjectM(
						  ioContext.getSystem().getLogger(),
						  Logger::eStats,
						  "history", "Beagle::LogIndividualDataOp",
						  *lIndividual
						  );
		
		
		// STL heap pop of the best individual of the temporary buffer.
		std::pop_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());
		lTempPop.pop_back();
	}
	
	
	Beagle_StackTraceEndM("void LogIndividualDataOp::operate(Deme& ioDeme, Context& ioContext)");
}
