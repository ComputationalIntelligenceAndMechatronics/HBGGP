/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/GP/src/MutationShrinkSelectiveConstrainedOp.cpp
 *  \brief  Source code of class GP::MutationShrinkSelectiveConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2010/07/11 10:14:22 $
 */

#include "beagle/GP.hpp"

#include "MutationShrinkSelectiveConstrainedOp.hpp"

#include <algorithm>
#include <string>
#include "ArgType.h"

#include "TreeSTag.h"

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree shrink mutation operator.
 *  \param inMutationPbName Mutation shrink probability.
 *  \param inName Name of the operator.
 */
GP::MutationShrinkSelectiveConstrainedOp::MutationShrinkSelectiveConstrainedOp(std::string inMutationPbName,
																			   std::string inName) :
MutationShrinkConstrainedOp(inMutationPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the constrained GP tree shrink mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationShrinkSelectiveConstrainedOp::initialize(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MutationShrinkConstrainedOp::initialize(ioSystem);
	SelectiveConstrainedSelectionOp::initialize(ioSystem);
	Beagle_StackTraceEndM("void GP::MutationShrinkSelectiveConstrainedOp::registerParams(Beagle::System&)");
}


/*!
 *  \brief Shrink mutate a GP individual.
 *  \param ioIndividual GP individual to shrink mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationShrinkSelectiveConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Individual& lIndividual  = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext        = castObjectT<GP::Context&>(ioContext);
	unsigned int lNumberAttempts = mNumberAttempts->getWrappedValue();
	bool lMutationDone           = false;
	
	//Select node to mutate
	unsigned int lChoosenTree = 0;
	unsigned int lChoosenNode = 0;
	
	bool lDoParameterSearch = (lContext.getSystem().getRandomizer().rollUniform(0.0, 1.0) <= mMutParameterPb->getWrappedValue());
	
	RouletteT< std::pair<unsigned int,unsigned int> > lRoulette;
	
	//Select primitive based on type
	for(unsigned int lTry = 0; lTry < 2; ++lTry) { //Do only twice
		std::vector<const std::type_info*> lDesiredTypes(1, ArgEph);
		SelectiveConstrainedSelectionOp::buildRoulette(lRoulette, lDesiredTypes, lIndividual, lContext,!lDoParameterSearch,lDoParameterSearch);
		
		if(lRoulette.size() == 0) {
			if(lDoParameterSearch) {
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
								   string("No EphemeralDouble node found.")
								   );
			}
			else {
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
								   string("No non EphemeralDouble node found.")
								   );
			}
			lDoParameterSearch = !lDoParameterSearch;
			if(lTry >= 1) {
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
								   "Unable to GP standard mutate the individual"
								   );
				return false;
			}
		}
		else { 
			std::pair<unsigned int,unsigned int> lSelectedNode = lRoulette.select(ioContext.getSystem().getRandomizer());
			lChoosenTree = lSelectedNode.first;
			lChoosenNode = lSelectedNode.second;
			break;
		}
	}
	
	if(lDoParameterSearch) {
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
						   string("Mutation applied only on EphemeralDouble node.")
						   );
	}
	else {
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
						   string("Mutation applied on node that are not EphemeralDouble.")
						   );
	}
	
	GP::Tree::Handle lActualTree = lIndividual[lChoosenTree];
	if(lActualTree->size() < 2) return false;
	
	
	
	//Clean the roulette to include only the choosen tree
	if(lDoParameterSearch) {
		for(std::vector< std::pair<double, std::pair<unsigned int,unsigned int> > >::iterator lRouletteIter = lRoulette.begin(); lRouletteIter!=lRoulette.end();){
			if(lRouletteIter->second.first != lChoosenTree || (*lActualTree)[lRouletteIter->second.second].mPrimitive->getNumberArguments() == 0) {
				lRouletteIter = lRoulette.erase(lRouletteIter++);
			} else {
				++lRouletteIter;
			}
		}
	}

	
	if( !lDoParameterSearch || lRoulette.size() > 0 ) {
		
		GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
		unsigned int lOldTreeIndex = lContext.getGenotypeIndex();
		
		Beagle_LogDebugM(
						 ioContext.getSystem().getLogger(),
						 "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
						 std::string("Individual before constrained GP tree shrink mutation: ")+
						 ioIndividual.serialize()
						 );
		
		lIndividual[lChoosenTree] = castHandleT<GP::Tree>(lIndividual.getTypeAlloc()->allocate());
		lIndividual[lChoosenTree]->setPrimitiveSetIndex(lActualTree->getPrimitiveSetIndex());
		lIndividual[lChoosenTree]->setNumberArguments(lActualTree->getNumberArguments());
		
		
		for(unsigned int lAttempt=0; lAttempt<lNumberAttempts; lAttempt++) {
			//Find a node with arguments
			while((*lActualTree)[lChoosenNode].mPrimitive->getNumberArguments() == 0) {
				lChoosenNode = lRoulette.select(ioContext.getSystem().getRandomizer()).second;
			}
			
			lIndividual[lChoosenTree]->clear();
			lIndividual[lChoosenTree]->insert(lIndividual[lChoosenTree]->end(), lActualTree->begin(), lActualTree->begin()+lChoosenNode);
			unsigned int lChoosenArg = lContext.getSystem().getRandomizer().rollInteger(0, (*lActualTree)[lChoosenNode].mPrimitive->getNumberArguments()-1);
			unsigned int lChoosenArgIndex = lChoosenNode + 1;
			for(unsigned int k=0; k<lChoosenArg; k++) {
				lChoosenArgIndex += (*lActualTree)[lChoosenArgIndex].mSubTreeSize;
			}
			unsigned int lChoosenArgSubTreeSize = (*lActualTree)[lChoosenArgIndex].mSubTreeSize;
			lIndividual[lChoosenTree]->insert(lIndividual[lChoosenTree]->end(),
											  lActualTree->begin()+lChoosenArgIndex,
											  lActualTree->begin()+lChoosenArgIndex+lChoosenArgSubTreeSize);
			unsigned int lChoosenNodeSubTreeSize = (*lActualTree)[lChoosenNode].mSubTreeSize;
			lIndividual[lChoosenTree]->insert(lIndividual[lChoosenTree]->end(),
											  lActualTree->begin()+lChoosenNode+lChoosenNodeSubTreeSize,
											  lActualTree->end());
			lActualTree->setContextToNode(lChoosenNode, lContext);
			unsigned int lDiffSize =
			(*lActualTree)[lChoosenNode].mSubTreeSize - (*lActualTree)[lChoosenArgIndex].mSubTreeSize;
			for(unsigned int l=0; l<(lContext.getCallStackSize()-1); l++) {
				(*lIndividual[lChoosenTree])[lContext.getCallStackElement(l)].mSubTreeSize -= lDiffSize;
			}
			
			Beagle_LogVerboseM(
							   ioContext.getSystem().getLogger(),
							   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
							   std::string("Trying to replace the ")+uint2ordinal(lChoosenNode+1)+
							   std::string(" node, ")+ (*lActualTree)[lChoosenNode].mPrimitive->getName()
							   +std::string(", of the ")+uint2ordinal(lChoosenTree+1)+
							   std::string(" tree with its ")+uint2ordinal(lChoosenArg+1)+
							   std::string(" argument, that is the ")+
							   uint2ordinal(lChoosenArgIndex+1)+std::string(" node")
							   );
			
			lContext.setGenotypeHandle(lIndividual[lChoosenTree]);
			lContext.setGenotypeIndex(lChoosenTree);
			if(lIndividual[lChoosenTree]->validateSubTree(lChoosenNode, lContext)) {
				lMutationDone = true;
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
								   "Constrained GP tree shrink mutation valid"
								   );
				break;
			}
			else if(lAttempt == (lNumberAttempts-1)) {
				lIndividual[lChoosenTree] = lActualTree;
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
								   "Constrained GP tree shrink mutation invalid"
								   );
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
								   "Unable to shrink mutate the individual"
								   );
				break;
			}
			else {
				lChoosenNode = lRoulette.select(ioContext.getSystem().getRandomizer()).second;

				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
								   "Constrained GP tree shrink mutation invalid"
								   );
				continue;
			}
		}
		
		lContext.setGenotypeHandle(lOldTreeHandle);
		lContext.setGenotypeIndex(lOldTreeIndex);
		
	} else {
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
						   "Unable to shrink mutate the individual"
						   );
	}
		
	if(lMutationDone) {
		if( !lDoParameterSearch ) {
			//Set structure id invalid
			castHandleT<TreeSTag>((lIndividual)[0])->setStructureIDInvalid();
			Beagle_LogVerboseM(
							   ioContext.getSystem().getLogger(),
							   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
							   std::string("Set structure id invalid") );
		}
		
		Beagle_LogDebugM(
						 ioContext.getSystem().getLogger(),
						 "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
						 std::string("Individual after constrained GP tree shrink mutation: ")+
						 ioIndividual.serialize()
						 );
	}
	else {
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationShrinkSelectiveConstrainedOp",
						   "No constrained GP tree shrink mutation done"
						   );
	}
	
	return lMutationDone;
	Beagle_StackTraceEndM("bool GP::MutationShrinkSelectiveConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)");
}

