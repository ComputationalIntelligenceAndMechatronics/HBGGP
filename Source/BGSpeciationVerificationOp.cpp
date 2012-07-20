/*
 *  BGSpeciationVerificationOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 21/06/10.
 */

#include "BGSpeciationVerificationOp.h"

#include <beagle/Deme.hpp>
#include <beagle/Individual.hpp>
#include "TreeSTag.h"
#include "BGSpeciesHolder.h"
#include "BGFitness.h"

using namespace Beagle;

void BGSpeciationVerificationOp::operate(Deme& ioDeme, Context& ioContext) {
	Beagle_StackTraceBeginM();
	
	Beagle::Component::Handle lHolderComponent = ioContext.getSystem().getComponent("BGSpeciesHolder");
	if(lHolderComponent==NULL) throw Beagle_RunTimeExceptionM("No specie holder component found in the system!");
	BGSpeciesHolder::Handle lSpeciesHolder = castHandleT<BGSpeciesHolder>(lHolderComponent);
	
	
	//Find corresponding species for all new breeded individual and update species count
	for(int i = 0; i < ioDeme.size(); ++i) {
		Individual::Handle lIndiv = ioDeme[i];
		TreeSTag::Handle lTree = castHandleT<TreeSTag>((*lIndiv)[0]);

		//Find a matching species
		Fitness::Handle lFitness = lIndiv->getFitness();
		bool lIsNew;
		BGSpecies* lSpecies = lSpeciesHolder->findSpecies( castHandleT<BGFitness>(lFitness)->getSimplifiedBondGraph(), ioContext, lIsNew);

		if(lTree->getStructureID() != lSpecies->getId() ) {
			Beagle_LogTraceM(
							 ioContext.getSystem().getLogger(),
							 "speciation", "BGSpeciationVerificationOp",
							 std::string("Invalid species for the ")+uint2ordinal(i+1)+std::string(" individual of the ")+uint2ordinal(ioContext.getDemeIndex()+1)
							 +std::string(" deme."));
			
			if(lIsNew) {
				Beagle_LogTraceM(
								 ioContext.getSystem().getLogger(),
								 "speciation", "BGSpeciationVerificationOp",
								 std::string("Tree structure id pointing to the ")+uint2ordinal(lTree->getStructureID()+1)
								 +std::string(" species, but no matching structure was found."));
			} else {
				Beagle_LogTraceM(
								 ioContext.getSystem().getLogger(),
								 "speciation", "BGSpeciationVerificationOp",
								 std::string("Tree structure id pointing to the ")+uint2ordinal(lTree->getStructureID()+1)
								 +std::string(" species, but structure match the ")+uint2ordinal(lSpecies->getId()+1)+std::string(" species."));
			}
			
			lTree->setStructureIDInvalid();
			exit(EXIT_FAILURE);
		}
	}
	
	Beagle_StackTraceEndM("void BGSpeciationVerificationOp::operate(Deme& ioDeme, Context& ioContext)");
}


void BGSpeciationVerificationOp::initialize(Beagle::System& ioSystem) {
	
	
}

void BGSpeciationVerificationOp::postInit(Beagle::System& ioSystem) {
	
}
