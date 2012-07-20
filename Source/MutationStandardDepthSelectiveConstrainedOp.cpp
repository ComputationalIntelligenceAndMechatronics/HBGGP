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
 *  \file   beagle/GP/src/MutationStandardDepthSelectiveConstrainedOp.cpp
 *  \brief  Source code of class GP::MutationStandardDepthSelectiveConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4 $
 *  $Date: 2010/07/11 10:14:22 $
 */

#include "beagle/GP.hpp"
#include "MutationStandardDepthSelectiveConstrainedOp.hpp"
#include <algorithm>
#include <string>
#include "ArgType.h"

#include "TreeSTag.h"

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree standard mutation operator.
 *  \param inInitOp Initialization operator used to mutate the GP subtree.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inMaxRegenDepthName Maximum regeneration depth in mutation parameter name.
 *  \param inName Name of the operator.
 */
GP::MutationStandardDepthSelectiveConstrainedOp::MutationStandardDepthSelectiveConstrainedOp(GP::InitializationOp::Handle inInitOp,
																				   std::string inMutationPbName,
																				   std::string inMaxRegenDepthName,
																				   std::string inName) :
MutationStandardConstrainedOp(inInitOp, inMutationPbName, inMaxRegenDepthName, inName)
{ }


/*!
 *  \brief Register the parameters of the constrained GP tree standard mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationStandardDepthSelectiveConstrainedOp::initialize(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MutationStandardConstrainedOp::initialize(ioSystem);
	DepthDependentSelectionOp::initialize(ioSystem);
	
	Beagle_StackTraceEndM("void GP::MutationStandardDepthSelectiveConstrainedOp::registerParams(Beagle::System&)");
}


/*!
 *  \brief Standard mutate a constrained GP individual.
 *  \param ioIndividual GP individual to standard mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationStandardDepthSelectiveConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Individual& lIndividual        = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext              = castObjectT<GP::Context&>(ioContext);
	unsigned int lMaxTreeDepth         = mMaxTreeDepth->getWrappedValue();
	unsigned int lMaxRegenerationDepth = mMaxRegenerationDepth->getWrappedValue();
	
	//Select node to mutate
	unsigned int lChoosenTree = 0;
	unsigned int lChoosenNode = 0;
	
	bool lDoParameterSearch = (lContext.getSystem().getRandomizer().rollUniform(0.0, 1.0) <= mMutParameterPb->getWrappedValue());
	
	//Select primitive based on type
	for(unsigned int lTry = 0; lTry < 2; ++lTry) { //Do only twice
		std::vector<const std::type_info*> lDesiredTypes(1, ArgEph);
		RouletteT< std::pair<unsigned int,unsigned int> > lRoulette;
		DepthDependentSelectionOp::buildRoulette(lRoulette, lDesiredTypes, lIndividual, lContext,!lDoParameterSearch,lDoParameterSearch);
		
		if(lRoulette.size() == 0) {
			if(lDoParameterSearch) {
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
								   string("No EphemeralDouble node found.")
								   );
			}
			else {
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
								   string("No non EphemeralDouble node found.")
								   );
			}
			lDoParameterSearch = !lDoParameterSearch;
			if(lTry >= 1) {
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
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
						   "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
						   string("Mutation applied only on EphemeralDouble node.")
						   );
	}
	else {
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
						   string("Mutation applied on node that are not EphemeralDouble.")
						   );
	}
	
	
	unsigned int lOldGenotypeIndex = lContext.getGenotypeIndex();
	GP::Tree::Handle lOldGenotypeHandle = lContext.getGenotypeHandle();
	
	Beagle_LogDebugM(
					 ioContext.getSystem().getLogger(),
					 "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
					 std::string("Individual before GP standard mutation: ")+
					 ioIndividual.serialize()
					 );
	
	GP::Tree::Handle lActualTree = lIndividual[lChoosenTree];
	GP::Tree::Handle lNewTree    = castHandleT<GP::Tree>(lIndividual.getTypeAlloc()->allocate());
	lNewTree->setPrimitiveSetIndex(lActualTree->getPrimitiveSetIndex());
	lNewTree->setNumberArguments(lActualTree->getNumberArguments());
	unsigned int lChoosenNodeSubTreeSize = (*lActualTree)[lChoosenNode].mSubTreeSize;
	lNewTree->insert(lNewTree->end(), lActualTree->begin(), lActualTree->begin()+lChoosenNode);
	lContext.setGenotypeIndex(lChoosenTree);
	lContext.setGenotypeHandle(lActualTree);
	lContext.emptyCallStack();
	lActualTree->setContextToNode(lChoosenNode, lContext);
	lContext.popCallStack();
	const unsigned int lMaxSubTreeDepth =
    minOf<unsigned int>(lMaxTreeDepth - lContext.getCallStackSize(), lMaxRegenerationDepth);
	lIndividual[lChoosenTree] = lNewTree;
	lContext.setGenotypeHandle(lNewTree);
	
	unsigned int lAttempt=0;
	if(lMaxSubTreeDepth >= 1) {
		for(; lAttempt < mNumberAttempts->getWrappedValue(); lAttempt++) {
			if(mInitOp->initTree(*lNewTree, 1, lMaxSubTreeDepth, lContext) != 0) break;
		}
	} else {
		lAttempt = mNumberAttempts->getWrappedValue();
	}
	if(lAttempt == mNumberAttempts->getWrappedValue()) {
		lIndividual[lChoosenTree] = lActualTree;
		lContext.setGenotypeIndex(lOldGenotypeIndex);
		lContext.setGenotypeHandle(lOldGenotypeHandle);
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
						   "Unable to GP standard mutate the individual"
						   );
		return false;
	}
	
	if( !lDoParameterSearch ) {
		//Set structure id invalid
		castHandleT<TreeSTag>((lIndividual)[0])->setStructureIDInvalid();
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
						   std::string("Set structure id invalid") );
	}
	
	Beagle_LogVerboseM(
					   ioContext.getSystem().getLogger(),
					   "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
					   std::string("GP standard mutate the ")+uint2ordinal(lChoosenNode+1)+
					   std::string(" node, ")+ (*lActualTree)[lChoosenNode].mPrimitive->getName()
					   +std::string(", of the ")+uint2ordinal(lChoosenTree+1)+
					   std::string(" tree with max depth ")+uint2str(lMaxSubTreeDepth)
					   );
	
	lNewTree->insert(lNewTree->end(),
					 lActualTree->begin()+lChoosenNode+lChoosenNodeSubTreeSize,
					 lActualTree->end());
	unsigned int lDiffSize =
    (*lActualTree)[lChoosenNode].mSubTreeSize - (*lNewTree)[lChoosenNode].mSubTreeSize;
	for(unsigned int l=0; l<lContext.getCallStackSize(); l++) {
		(*lNewTree)[lContext.getCallStackElement(l)].mSubTreeSize -= lDiffSize;
	}
	
	lContext.setGenotypeIndex(lOldGenotypeIndex);
	lContext.setGenotypeHandle(lOldGenotypeHandle);
	
	Beagle_LogDebugM(
					 ioContext.getSystem().getLogger(),
					 "mutation", "Beagle::GP::MutationStandardDepthSelectiveConstrainedOp",
					 std::string("Individual after GP standard mutation: ")+
					 ioIndividual.serialize()
					 );
	
	return true;
	Beagle_StackTraceEndM("bool GP::MutationStandardDepthSelectiveConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)");
}





