/*
 *  StructuralHierarchicalFairCompetitionOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 20/06/10.
 */

#include "StructuralHierarchicalFairCompetitionOp.h"
#include <cmath>
#include <algorithm>
#include "beagle/Beagle.hpp"
#include "TreeSTag.h"

using namespace Beagle;

StructuralHierarchicalFairCompetitionOp::StructuralHierarchicalFairCompetitionOp(std::string inName) 
: HierarchicalFairCompetitionOp(inName) {}

void StructuralHierarchicalFairCompetitionOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// Verify fitness threshold values.
	Beagle_AssertM(mPopSize->size()!=0);
	if(ioContext.getGeneration()==0) mFitnessThresholds.resize(0);
	if(mFitnessThresholds.size()!=(mPopSize->size()-1)) mFitnessThresholds.resize(mPopSize->size()-1);
	
	// Situations where HFC migration should not be applied.
	if(ioDeme.size() == 0) return;
	if(mPopSize->size() < 2) return;
	if(mMigrationInterval->getWrappedValue() == 0) return;
	if((ioContext.getGeneration() % mMigrationInterval->getWrappedValue()) != 0) return;
	
	// Log some information.
	Beagle_LogTraceM(
					 ioContext.getSystem().getLogger(),
					 "migration", "Beagle::HierarchicalFairCompetitionOp",
					 std::string("Applying HFC migration to the ")+uint2ordinal(ioContext.getDemeIndex())+
					 std::string(" deme")
					 );
	
	// If the deme is changed.
	bool lChanged = false;
	
	// Update fitness threshold.
	if(ioContext.getDemeIndex() != 0) {
		Beagle_AssertM(mHFCPercentile->getWrappedValue() < 1.0);
		const unsigned int lThresholdIndex =
		(unsigned int)std::ceil((1.0-mHFCPercentile->getWrappedValue()) * float(ioDeme.size()-1));
		std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
		for(unsigned int i=0; i<lThresholdIndex; ++i) {
			std::pop_heap(ioDeme.begin(), ioDeme.end()-i, IsLessPointerPredicate());
		}
		mFitnessThresholds[ioContext.getDemeIndex()-1] =
		castHandleT<Fitness>(ioDeme[0]->getFitnessAlloc()->clone(*ioDeme[0]->getFitness()));
	}
	
	// Insert migrating individuals from previous deme.
	if(ioContext.getDemeIndex() != 0) {
		Individual::Bag& lInMigBuffer =
		ioContext.getVivarium()[ioContext.getDemeIndex()-1]->getMigrationBuffer();
		for(unsigned int i=0; i<lInMigBuffer.size(); ++i) {
			Beagle_LogDebugM(
							 ioContext.getSystem().getLogger(),
							 "migration", "Beagle::HierarchicalFairCompetitionOp",
							 std::string("Individual migrated into the ")+uint2ordinal(ioContext.getDemeIndex())+
							 std::string(" deme: ")+lInMigBuffer[i]->serialize()
							 );
			castHandleT<TreeSTag>((*lInMigBuffer[i])[0])->setStructureIDInvalid();
			ioDeme.push_back(lInMigBuffer[i]);
			lChanged = true;
		}
		lInMigBuffer.resize(0);
	}
	
	// Migrating individual out of this deme.
	if((ioContext.getDemeIndex() != (mPopSize->size()-1)) &&
	   (mFitnessThresholds[ioContext.getDemeIndex()] != NULL)) {
		Individual::Bag& lOutMigBuffer = ioDeme.getMigrationBuffer();
		lOutMigBuffer.resize(0);
		Fitness::Handle lThreshold = mFitnessThresholds[ioContext.getDemeIndex()];
		std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
		while(ioDeme.size() > 0) {
			if(ioDeme[0]->getFitness()->isLess(*lThreshold)) break;
			std::pop_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
			lOutMigBuffer.push_back(castHandleT<Individual>(ioDeme.back()));
			ioDeme.pop_back();
			lChanged = true;
		}
	}
	
	// Fill the population with randomly generated individuals, if the population is too small.
	if(ioDeme.size() < (*mPopSize)[ioContext.getDemeIndex()]) {
		const unsigned int lNbNewInd = (*mPopSize)[ioContext.getDemeIndex()] - ioDeme.size();
		Individual::Bag lNewIndividuals = generateIndividuals(lNbNewInd, ioDeme, ioContext);
		ioDeme.insert(ioDeme.end(), lNewIndividuals.begin(), lNewIndividuals.end());
		lChanged = true;
	}
	
	// Delete worse individuals if the population is too big.
	if(ioDeme.size() > (*mPopSize)[ioContext.getDemeIndex()]) {
		const unsigned int lNbDeletedInd = ioDeme.size() - (*mPopSize)[ioContext.getDemeIndex()];
		std::make_heap(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
		for(unsigned int i=0; i<lNbDeletedInd; ++i) {
			std::pop_heap(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
			Beagle_LogDebugM(
							 ioContext.getSystem().getLogger(),
							 "migration", "Beagle::HierarchicalFairCompetitionOp",
							 std::string("Individual erased from the last deme: ")+
							 ioDeme.back()->serialize()
							 );
			ioDeme.pop_back();
		}
		lChanged = true;
	}
	
	// If changed, reset statistics.
	if(lChanged) {
		ioDeme.getStats()->setInvalid();
		ioContext.getVivarium().getStats()->setInvalid();
	}
	Beagle_StackTraceEndM("void HierarchicalFairCompetitionOp::operate(Deme& ioDeme, Context& ioContext)");
}
