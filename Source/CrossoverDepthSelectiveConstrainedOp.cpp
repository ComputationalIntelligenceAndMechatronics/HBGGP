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


#include "CrossoverDepthSelectiveConstrainedOp.hpp"

#include <algorithm>
#include <string>
#include "ArgType.h"

#include "TreeSTag.h"

using namespace Beagle;




/*!
 *  \brief Construct a GP constrained tree crossover operator.
 *  \param inMatingPbName Individual mating probability parameter name used in register.
 *  \param inDistribPbName Distribution probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GP::CrossoverDepthSelectiveConstrainedOp::CrossoverDepthSelectiveConstrainedOp(Beagle::string inMatingPbName,
																	 Beagle::string inDistribPbName,
																	 Beagle::string inName) :
Beagle::GP::CrossoverConstrainedOp(inMatingPbName, inDistribPbName, inName)
{ 
	//mGenerationCounter = 0;
}

/*!
 *  \brief Initialize the GP crossover operator.
 *  \param ioSystem System of the evolution.
 */
void GP::CrossoverDepthSelectiveConstrainedOp::initialize(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	
	Beagle::GP::CrossoverConstrainedOp::initialize(ioSystem);
	DepthDependentSelectionOp::initialize(ioSystem);
	
	
	Beagle_StackTraceEndM("void GP::CrossoverDepthSelectiveConstrainedOp::initialize(Beagle::System& ioSystem)");
}

/*!
 *  \brief Mate two GP individuals for a constrained tree crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool GP::CrossoverDepthSelectiveConstrainedOp::mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
											   Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2)
{
	Beagle_StackTraceBeginM();
	// Initial parameters checks
	Beagle_AssertM(ioIndiv1.size() > 0);
	//Beagle_AssertM(ioIndiv1.size() == ioIndiv2.size());
	Beagle_ValidateParameterM(mNumberAttempts->getWrappedValue()>0,"gp.try",">0");
	
	// Cast method arguments.
	GP::Individual& lIndiv1   = castObjectT<GP::Individual&>(ioIndiv1);
	GP::Individual& lIndiv2   = castObjectT<GP::Individual&>(ioIndiv2);
	GP::Context&    lContext1 = castObjectT<GP::Context&>(ioContext1);
	GP::Context&    lContext2 = castObjectT<GP::Context&>(ioContext2);
	
	// Get parameters in local values, with the total number of nodes of an individual.
	bool             lMatingDone     = false;
	float            lDistrProba     = mDistributionProba->getWrappedValue();
	unsigned int     lMaxTreeDepth   = mMaxTreeDepth->getWrappedValue();
	GP::Tree::Handle lOldTreeHandle1 = lContext1.getGenotypeHandle();
	unsigned int     lOldTreeIndex1  = lContext1.getGenotypeIndex();
	GP::Tree::Handle lOldTreeHandle2 = lContext2.getGenotypeHandle();
	unsigned int     lOldTreeIndex2  = lContext2.getGenotypeIndex();
	unsigned int     lSizeIndiv1     = 0;
	for(unsigned int i=0; i<lIndiv1.size(); i++) lSizeIndiv1 += lIndiv1[i]->size();
	
	Beagle_LogDebugM(  
					 ioContext1.getSystem().getLogger(),
					 "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
					 string("First individual to mate (before constrained GP crossover): ")+
					 lIndiv1.serialize()
					 );
	Beagle_LogDebugM(
					 ioContext1.getSystem().getLogger(),
					 "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
					 string("Second individual to mate (before constrained GP crossover): ")+
					 lIndiv2.serialize()
					 );
	
	bool lFailedParameterOnly = false;
	
	//bool lDoParameterSearch = false;
	bool lDoParameterSearch = (lContext1.getSystem().getRandomizer().rollUniform(0.0, 1.0) <= mCxParameterPb->getWrappedValue());
	
	// Crossover loop. Try the given number of attempts to mate two individuals.
	for(unsigned int lAttempt=0; lAttempt < mNumberAttempts->getWrappedValue(); ++lAttempt) {
		
		//Deside either to mate a branch of a leaf
		bool lTypeNode1 = (lContext1.getSystem().getRandomizer().rollUniform(0.0, 1.0) < lDistrProba);
		
		unsigned int lChoosenTree1 = 0;
		unsigned int lChoosenNode1 = 0;
		
		//Select primitive based on type
		for(unsigned int lTry = 0; lTry < 2; ++lTry) { //Do only twice
			std::vector<const std::type_info*> lDesiredTypes(1, ArgEph);
			RouletteT< std::pair<unsigned int,unsigned int> > lRoulette1;
			DepthDependentSelectionOp::buildRoulette(lRoulette1, lTypeNode1, lDesiredTypes, lIndiv1, lContext1,!lDoParameterSearch,lDoParameterSearch);
			if(lRoulette1.size() == 0) {
				//Try again by changing the leaf/branche requirement
				DepthDependentSelectionOp::buildRoulette(lRoulette1, !lTypeNode1, lDesiredTypes, lIndiv1, lContext1,!lDoParameterSearch,lDoParameterSearch);
			}
			
			bool lIndiv2HasTyping = false;
			if(lRoulette1.size() != 0)
				lIndiv2HasTyping = hasOutputTypes(lDesiredTypes, lIndiv2, lContext2,!lDoParameterSearch);
		
			if(lRoulette1.size() == 0 || !lIndiv2HasTyping) {
				if(lDoParameterSearch) {
					Beagle_LogVerboseM(
									   ioContext1.getSystem().getLogger(),
									   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
									   string("No EphemeralDouble node found in at least one of the two individual.")
									   );
				}
				else {
					Beagle_LogVerboseM(
									   ioContext1.getSystem().getLogger(),
									   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
									   string("No non EphemeralDouble node found in at least one of the two individual.")
									   );
				}
				lDoParameterSearch = !lDoParameterSearch;
				if(lTry >= 1) {
					Beagle_LogVerboseM(
									   ioContext1.getSystem().getLogger(),
									   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
									   "Unable to GP standard mate the individuals"
									   );
					return false;
				}
			}
			else { 
				std::pair<unsigned int,unsigned int> lSelectedNode = lRoulette1.select(ioContext1.getSystem().getRandomizer());
				lChoosenTree1 = lSelectedNode.first;
				lChoosenNode1 = lSelectedNode.second;
				break;
			}
		}
		
		if(lDoParameterSearch) {
			Beagle_LogVerboseM(
							   ioContext1.getSystem().getLogger(),
							   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
							   string("Crossover applied only on EphemeralDouble node.")
							   );
		}
		else {
			Beagle_LogVerboseM(
							   ioContext1.getSystem().getLogger(),
							   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
							   string("Crossover applied on node that are not EphemeralDouble.")
							   );
		}
		
		GP::Tree& lTree1 = *lIndiv1[lChoosenTree1];
		const unsigned int lPrimitiveSetIndex1 = lTree1.getPrimitiveSetIndex();
		
		// Choose type of node (branch or leaf) for the second node.
		const bool lTypeNode2 =	(lContext2.getSystem().getRandomizer().rollUniform(0.0, 1.0) < lDistrProba);
		
		// Compute max depth allowable.
		lTree1.setContextToNode(lChoosenNode1, lContext1);
		const unsigned int lTmpMaxDepth1 = lMaxTreeDepth - lContext1.getCallStackSize();
		const unsigned int lTmpMaxDepth2 = lMaxTreeDepth - lTree1.getTreeDepth(lChoosenNode1);
		const unsigned int lMaxDepthTree2 = minOf(lTmpMaxDepth1, lTmpMaxDepth2);
		
		// Select a node in second individual for the crossover.
		unsigned int lChoosenTree2=0;
		unsigned int lChoosenNode2=0;

		std::vector<const std::type_info*> lDesiredType = lTree1[lChoosenNode1].mPrimitive->getReturnTypes(lContext1);
		bool lGoodSelect = DepthDependentSelectionOp::selectNodeToMateWithTypes(lChoosenTree2,
													 lChoosenNode2,
													 lTypeNode2,
													 lDesiredType,
													 lPrimitiveSetIndex1,
													 lMaxDepthTree2,
													 UINT_MAX,
													 lIndiv2,
													 lContext2,lDoParameterSearch);

		
		// Check to see that there is at least one node that can be selected
		if(lGoodSelect==false) {
			
			//Try again by changing the leaf/branche requirement
			lGoodSelect = DepthDependentSelectionOp::selectNodeToMateWithTypes(lChoosenTree2,
													lChoosenNode2,
													!lTypeNode2,
													lDesiredType,
													lPrimitiveSetIndex1,
													lMaxDepthTree2,
													UINT_MAX,
													lIndiv2,
													lContext2,lDoParameterSearch);
			
			if(lGoodSelect==false) {
				if( lDoParameterSearch ) lFailedParameterOnly = true; //Don't retry parameters only as it's sure to fail.
				Beagle_LogVerboseM(
								   ioContext1.getSystem().getLogger(),
								   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
								   string("Crossover attempt failed: it seems there is no corresponding nodes in second ")+
								   string("individual that would meet all the constraints")
								   );
				
				continue;
			}
		}
		
		// Get reference to the tree the choosen node is in.
		Beagle_AssertM(lChoosenTree2 < lIndiv2.size());
		GP::Tree& lTree2 = *lIndiv2[lChoosenTree2];
		lTree2.setContextToNode(lChoosenNode2, lContext2);
		
		// Mate the trees.
		Beagle_LogVerboseM(
						   ioContext1.getSystem().getLogger(),
						   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
						   string("Trying to exchange the ")+uint2ordinal(lChoosenNode1+1)+
						   string(" node of the ")+uint2ordinal(lChoosenTree1+1)+
						   string(" tree of the first individual with the ")+uint2ordinal(lChoosenNode2+1)+
						   string(" node of the ")+uint2ordinal(lChoosenTree2+1)+
						   string(" tree of the second individual")
						   );
		
		mateTrees(lTree1, lChoosenNode1, lContext1, lTree2, lChoosenNode2, lContext2);
		
		// If one tree is not valid, undo the crossover and do a new crossover attempt. 
		lContext1.setGenotypeHandle(lIndiv1[lChoosenTree1]);
		lContext1.setGenotypeIndex(lChoosenTree1);
		lContext2.setGenotypeHandle(lIndiv2[lChoosenTree2]);
		lContext2.setGenotypeIndex(lChoosenTree2);
		if(lTree1.validateSubTree(lChoosenNode1,lContext1) &&
		   lTree2.validateSubTree(lChoosenNode2,lContext2)) {
			lMatingDone = true;
			Beagle_LogVerboseM(
							   ioContext1.getSystem().getLogger(),
							   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
							   "Constrained tree GP crossover valid"
							   );
			break; // The crossover is valid.
		}
		else {   // Undo crossover.
			Beagle_LogVerboseM(
							   ioContext1.getSystem().getLogger(),
							   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
							   "Crossover attempt failed because one of the resulting trees was invalid"
							   );
			mateTrees(lTree1, lChoosenNode1, lContext1, lTree2, lChoosenNode2, lContext2);
			continue;
		}
	}
	
	
	
	// Replace the contexts.
	lContext1.setGenotypeHandle(lOldTreeHandle1);
	lContext1.setGenotypeIndex(lOldTreeIndex1);
	lContext2.setGenotypeHandle(lOldTreeHandle2);
	lContext2.setGenotypeIndex(lOldTreeIndex2);
	
	if(lMatingDone) {
		Beagle_LogDebugM(
						 ioContext1.getSystem().getLogger(),
						 "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
						 string("First individual mated (after constrained tree GP crossover): ")+
						 lIndiv1.serialize()
						 );
		Beagle_LogDebugM(
						 ioContext1.getSystem().getLogger(),
						 "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
						 string("Second individual mated (after constrained tree GP crossover): ")+
						 lIndiv2.serialize()
						 );
		if( !lDoParameterSearch ) {
			//Set structure id invalid
			castHandleT<TreeSTag>((lIndiv1)[0])->setStructureIDInvalid();
			castHandleT<TreeSTag>((lIndiv2)[0])->setStructureIDInvalid();
			Beagle_LogVerboseM(
							   ioContext1.getSystem().getLogger(),
							   "mutation", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
							   std::string("Set structure id invalid") );
		}
	}
	else {
		Beagle_LogVerboseM(
						   ioContext1.getSystem().getLogger(),
						   "crossover", "Beagle::GP::CrossoverDepthSelectiveConstrainedOp",
						   "No constrained tree GP crossover done"
						   );
	}
	
	return lMatingDone;
	Beagle_StackTraceEndM("bool GP::CrossoverDepthSelectiveConstrainedOp::mate(Individual& ioIndiv1, Context& ioContext1, Individual& ioIndiv2, Context& ioContext2)");
}

