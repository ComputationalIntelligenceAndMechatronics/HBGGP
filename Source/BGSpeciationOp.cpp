/*
 *  BGSpeciationOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 17/06/10.
 */

#include "BGSpeciationOp.h"
#include <beagle/Deme.hpp>
#include <beagle/Individual.hpp>
#include "TreeSTag.h"
#include "BGSpeciesHolder.h"
#include "BGFitness.h"

using namespace Beagle;

void BGSpeciationOp::operate(Deme& ioDeme, Context& ioContext) {
	Beagle_StackTraceBeginM();
	
//	{
//		int i = 0;
//		char hostname[256];
//		gethostname(hostname, sizeof(hostname));
//		printf("PID %d on %s ready for attach\n", getpid(), hostname);
//		fflush(stdout);
//		while (0 == i)
//			sleep(5);
//	}
	
	

	Beagle::Component::Handle lHolderComponent = ioContext.getSystem().getComponent("BGSpeciesHolder");
	if(lHolderComponent==NULL) throw Beagle_RunTimeExceptionM("No specie holder component found in the system!");
	BGSpeciesHolder::Handle lSpeciesHolder = castHandleT<BGSpeciesHolder>(lHolderComponent);
	
	lSpeciesHolder->clearCount(ioContext);

	//Find corresponding species for all new breeded individual and update species count
	for(int i = 0; i < ioDeme.size(); ++i) {
		Individual::Handle lIndiv = ioDeme[i];
		TreeSTag::Handle lTree = castHandleT<TreeSTag>((*lIndiv)[0]);
		
		if(!lTree->isStructureIDValid()) {
			//Find a matching species
			Fitness::Handle lFitness = lIndiv->getFitness();
			bool lIsNewSpecies;
			
			GrowingBG::Handle lBondGraph = castHandleT<BGFitness>(lFitness)->getSimplifiedBondGraph();
			if(lBondGraph == 0) {
				lBondGraph = castHandleT<BGFitness>(lFitness)->getBondGraph();
				lBondGraph->getBondGraph()->simplify();
			}
			
			BGSpecies* lSpecies = lSpeciesHolder->findSpecies(lBondGraph, ioContext,lIsNewSpecies);
			++(*lSpecies);
			
			Beagle_LogVerboseM(
							   ioContext.getSystem().getLogger(),
							   "speciation", "BGSpeciationOp",
							   std::string("Finding the matching species for the ")+uint2ordinal(i+1)+std::string(" individual. ")+
							   (lIsNewSpecies ? std::string("Create the new ") : std::string("Match the "))
							   +uint2ordinal(lSpecies->getId()+1)+std::string(" species.")
							   );
			
			lTree->setStructureID(lSpecies->getId());
			lTree->setStructureIDValid();
		} else {
			Beagle_LogVerboseM(
							   ioContext.getSystem().getLogger(),
							   "speciation", "BGSpeciationOp",
							   std::string("Incrementing the matching species for the ")+uint2ordinal(i+1)+std::string(" individual. Match the ")
							   +uint2ordinal(lTree->getStructureID()+1)+std::string(" species.")
							   );
			BGSpecies* lSpecies = (*lSpeciesHolder)[ioContext.getDemeIndex()][lTree->getStructureID()];
			++(*lSpecies);
		}
	}
	
	//Adjust fitness value after the correct number of individual per species is valid
	for(int i = 0; i < ioDeme.size(); ++i) {
		Individual::Handle lIndiv = ioDeme[i];
		TreeSTag::Handle lTree = castHandleT<TreeSTag>((*lIndiv)[0]);
		
		BGFitness::Handle lFitness = castHandleT<BGFitness>( (Fitness::Handle)lIndiv->getFitness() );
		BGSpecies* lSpecies = (*lSpeciesHolder)[ioContext.getDemeIndex()][lTree->getStructureID()];
		
		double lSpecieFactor = 1;
		double lAgingFactor = 1;
		
		if(mThreshold->getWrappedValue() >= 0) {
			if( lSpecies->getSize() > mThreshold->getWrappedValue()*mNbStructureSearch->getWrappedValue() ) {
				lSpecieFactor = pow( double(lSpecies->getSize()/mNbStructureSearch->getWrappedValue()), double(-mAlpha->getWrappedValue()) );
			}
		}	
				   
		if(mAgingThres->getWrappedValue() >= 0) {
			double lAge = ioContext.getGeneration() - lSpecies->getAge();
			if(lAge > mAgingThres->getWrappedValue())
				lAgingFactor = 1 - 1/mAgingSpeed->getWrappedValue()*(lAge - mAgingThres->getWrappedValue());
			if(lAgingFactor < 0)
				lAgingFactor = 0;
		}
				   
				   
		double lFactor = lSpecieFactor*lAgingFactor;
		
		if(lFactor > 1) 
			lFactor = 1;
		lFitness->setAdjustedValue( lFitness->getOriginalFitnessValue() * lFactor );
		Beagle_LogTraceM(
						   ioContext.getSystem().getLogger(),
						   "speciation", "BGSpeciationOp",
						   std::string("Adjusting fitness of the ")+uint2ordinal(i+1)+std::string(" individual of the ")
						   +uint2ordinal(ioContext.getDemeIndex()+1)+std::string(" deme belonging to the ")
						   +uint2ordinal(lTree->getStructureID()+1)+std::string(" species.")
						   );

		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "speciation", "BGSpeciationOp",
						   std::string("Species size: ")+dbl2str(lSpecies->getSize())+
						   std::string(", fitness value ")+dbl2str(lFitness->getOriginalFitnessValue())+std::string(" adjusted by a factor of ")+dbl2str(lFactor)+
						   std::string(" to ")+dbl2str(lFitness->getValue())
						   );
			
		
	}

	Beagle_StackTraceEndM("void BGSpeciationOp::operate(Deme& ioDeme, Context& ioContext)");
}


void BGSpeciationOp::initialize(Beagle::System& ioSystem) {
	
	
	// Add population size parameter
	if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
		mPopSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
	} else {
		mPopSize = new UIntArray(1,100);
		std::string lLongDescrip("Number of demes and size of each deme of the population. ");
		lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
		lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
		lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
		lLongDescrip += "deme.";
		Register::Description lDescription(
										   "Vivarium and demes sizes",
										   "UIntArray",
										   mPopSize->serialize(),
										   lLongDescrip
										   );
		ioSystem.getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("ec.sp.nbstructure")) {
		mNbStructureSearch = castHandleT<Int>(ioSystem.getRegister()["ec.sp.nbstructure"]);
	} else {
		mNbStructureSearch = new Int(20);
		Register::Description lDescription(
										   "Number of expected individual per structure type. ",
										   "Int",
										   mNbStructureSearch->serialize(),
										   "Number of expected individual per structure type."
										   );
		ioSystem.getRegister().addEntry("ec.sp.nbstructure", mNbStructureSearch, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("ec.sp.alpha")) {
		mAlpha = castHandleT<Float>(ioSystem.getRegister()["ec.sp.alpha"]);
	} else {
		mAlpha = new Float(1.5);
		Register::Description lDescription(
										   "Alpha value in the fitness sharing function.",
										   "Float",
										   mAlpha->serialize(),
										   "Alpha value in the fitness sharing function."
										   );
		ioSystem.getRegister().addEntry("ec.sp.alpha", mAlpha, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("ec.sp.threshold")) {
		mThreshold = castHandleT<Float>(ioSystem.getRegister()["ec.sp.threshold"]);
	} else {
		mThreshold = new Float(0.90);
		Register::Description lDescription(
										   "Threshold value in fitness sharing.If negative, no factor is applied.",
										   "Float",
										   mThreshold->serialize(),
										   "Threshold value in fitness sharing.If negative, no factor is applied."
										   );
		ioSystem.getRegister().addEntry("ec.sp.threshold", mThreshold, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("ec.sp.aging")) {
		mAgingThres = castHandleT<Float>(ioSystem.getRegister()["ec.sp.aging"]);
	} else {
		mAgingThres = new Float(15);
		Register::Description lDescription(
										   "Threshold value for aging factor.If negative, no aging factor.",
										   "Float",
										   mAgingThres->serialize(),
										   "Threshold value for aging factor.If negative, no aging factor."
										   );
		ioSystem.getRegister().addEntry("ec.sp.aging", mAgingThres, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("ec.sp.agingspeed")) {
		mAgingSpeed = castHandleT<Float>(ioSystem.getRegister()["ec.sp.agingspeed"]);
	} else {
		mAgingSpeed = new Float(10);
		Register::Description lDescription(
										   "Aging factor speed. ",
										   "Float",
										   mAgingSpeed->serialize(),
										   "Number of generation after threshold for the fitness to vanish."
										   );
		ioSystem.getRegister().addEntry("ec.sp.agingspeed", mAgingSpeed, lDescription);
	}
}

void BGSpeciationOp::postInit(Beagle::System& ioSystem) {
	BGSpeciesHolder::Handle lHolder = new BGSpeciesHolder;
	lHolder->resize(mPopSize->size());
	ioSystem.addComponent(lHolder);
}
