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
 *  \file   beagle/GP/src/MutationSwapSelectiveConstrainedOp.cpp
 *  \brief  Source code of class GP::MutationSwapSelectiveConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2010/07/11 10:14:22 $
 */

#include "beagle/GP.hpp"
#include "MutationSwapSelectiveConstrainedOp.hpp"

#include <algorithm>
#include <string>
#include "ArgType.h"

#include "TreeSTag.h"

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree swap mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inDistribPbName Swap mutation distribution probability parameter name.
 *  \param inName Name of the operator.
 */
GP::MutationSwapSelectiveConstrainedOp::MutationSwapSelectiveConstrainedOp(std::string inMutationPbName,
																		   std::string inDistribPbName,
																		   std::string inName) :
MutationSwapConstrainedOp(inMutationPbName, inDistribPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the constrained GP tree swap mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationSwapSelectiveConstrainedOp::initialize(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MutationSwapConstrainedOp::initialize(ioSystem);
	SelectiveConstrainedSelectionOp::initialize(ioSystem);
	Beagle_StackTraceEndM("void GP::MutationSwapSelectiveConstrainedOp::registerParams(Beagle::System&)");
}


/*!
 *  \brief Swap mutate a constrained GP individual.
 *  \param ioIndividual GP individual to swap mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationSwapSelectiveConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Individual& lIndividual  = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext        = castObjectT<GP::Context&>(ioContext);
	double lDistrProba           = mDistributionProba->getWrappedValue();
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
								   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
								   string("No EphemeralDouble node found.")
								   );
			}
			else {
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
								   string("No non EphemeralDouble node found.")
								   );
			}
			lDoParameterSearch = !lDoParameterSearch;
			if(lTry >= 1) {
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
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
						   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
						   string("Mutation applied only on EphemeralDouble node.")
						   );
	}
	else {
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
						   string("Mutation applied on node that are not EphemeralDouble.")
						   );
	}
	
	GP::Tree& lTree = *lIndividual[lChoosenTree];
	if(lTree.size() == 0) return false;
	
	GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
	unsigned int lOldTreeIndex = lContext.getGenotypeIndex();
	lContext.setGenotypeHandle(lIndividual[lChoosenTree]);
	lContext.setGenotypeIndex(lChoosenTree);
	
	Beagle_LogDebugM(
					 ioContext.getSystem().getLogger(),
					 "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
					 std::string("Individual before constrained GP tree swap mutation: ")+ioIndividual.serialize()
					 );
	
	if(lTree.size() > 1) {
		bool lTypeNode = (lContext.getSystem().getRandomizer().rollUniform(0., 1.) < lDistrProba);
		//Clean the roulette to include only the choosen tree
		for(std::vector< std::pair<double, std::pair<unsigned int,unsigned int> > >::iterator lRouletteIter = lRoulette.begin(); lRouletteIter!=lRoulette.end();){
			if(lRouletteIter->second.first != lChoosenTree || (lTree[lRouletteIter->second.second].mPrimitive->getNumberArguments() != 0) != lTypeNode ) {
				lRouletteIter = lRoulette.erase(lRouletteIter++);
			} else {
				++lRouletteIter;
			}
		}
		if(lRoulette.size() > 0) 
			lChoosenNode = lRoulette.select(ioContext.getSystem().getRandomizer()).second;
	}
	
	if( !lDoParameterSearch || lRoulette.size() > 0 ) {
	
		Primitive::Handle lOriginalPrimitive = lTree[lChoosenNode].mPrimitive;
		
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
						   std::string("Trying to constrained GP tree swap mutate the ")+uint2ordinal(lChoosenNode+1)+
						   std::string(" node (primitive: \"")+lOriginalPrimitive->getName()+
						   std::string("\" nb args: ")+uint2str(lOriginalPrimitive->getNumberArguments())+
						   std::string(") of the ")+uint2ordinal(lChoosenTree+1)+std::string(" tree")
						   );
		
		GP::PrimitiveSet& lPrimitiveSet = lTree.getPrimitiveSet(lContext);
		unsigned int lNbArgsPrimit = lTree[lChoosenNode].mPrimitive->getNumberArguments();
		lTree.setContextToNode(lChoosenNode, lContext);
		for(unsigned int lAttempt=0; lAttempt < lNumberAttempts; ++lAttempt) {
			Primitive::Handle lChoosenPrimitive = lPrimitiveSet.select(lNbArgsPrimit, lContext);
			if(lChoosenPrimitive==NULL) break;
			
			lTree[lChoosenNode].mPrimitive = lChoosenPrimitive->giveReference(lNbArgsPrimit, lContext);
			
			Beagle_LogVerboseM(
							   ioContext.getSystem().getLogger(),
							   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
							   std::string("Trying the primitive \"")+lChoosenPrimitive->getName()+
							   std::string("\"")
							   );
			
			if(lTree.validateSubTree(lChoosenNode, lContext)) {
				lMutationDone = true;
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
								   "Constrained GP tree swap mutation valid"
								   );
				break;
			}
			else {
				lTree[lChoosenNode].mPrimitive = lOriginalPrimitive;
				Beagle_LogVerboseM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
								   "Constrained GP tree swap mutation invalid"
								   );
			}
		}
	}
	
	lContext.setGenotypeHandle(lOldTreeHandle);
	lContext.setGenotypeIndex(lOldTreeIndex);
	
	if(lMutationDone) {
		if( !lDoParameterSearch ) {
			//Set structure id invalid
			castHandleT<TreeSTag>((lIndividual)[0])->setStructureIDInvalid();
			Beagle_LogVerboseM(
							   ioContext.getSystem().getLogger(),
							   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
							   std::string("Set structure id invalid") );
		}
		
		Beagle_LogDebugM(
						 ioContext.getSystem().getLogger(),
						 "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
						 std::string("Individual after constrained GP swap mutation: ")+
						 ioIndividual.serialize()
						 );
	}
	else {
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GP::MutationSwapSelectiveConstrainedOp",
						   "Unable to swap mutate the constrained individual"
						   );
	}
	
	return lMutationDone;
	Beagle_StackTraceEndM("bool GP::MutationSwapSelectiveConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)");
}



