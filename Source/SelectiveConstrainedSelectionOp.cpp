/*
 *  SelectiveConstrainedSelectionOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 26/06/10.
 */

#include "SelectiveConstrainedSelectionOp.h"
#include <beagle/GP/Context.hpp>
#include <beagle/RunTimeException.hpp>
#include "ArgType.h"

using namespace Beagle;

void SelectiveConstrainedSelectionOp::initialize(Beagle::System& ioSystem) {
	if(ioSystem.getRegister().isRegistered("gp.mut.parampb")) {
		mMutParameterPb = castHandleT<Float>(ioSystem.getRegister()["gp.mut.parampb"]);
	} else {
		mMutParameterPb = new Float(0.8);
		Register::Description lDescription(
										   "Individual mutation probability of parameter over all node.",
										   "FloatArray",
										   mMutParameterPb->serialize(),
										   "Individual parameter node mutation probability. 1.0 means that parameter nodes will always be choosen."
										   );
		ioSystem.getRegister().addEntry("gp.mut.parampb", mMutParameterPb, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("gp.cx.parampb")) {
		mCxParameterPb = castHandleT<Float>(ioSystem.getRegister()["gp.cx.parampb"]);
	} else {
		mCxParameterPb = new Float(0.8);
		Register::Description lDescription(
										   "Individual crossover probability of parameter over all node.",
										   "FloatArray",
										   mCxParameterPb->serialize(),
										   "Individual parameter node crossover probability. 1.0 means that parameter nodes will always be choosen."
										   );
		ioSystem.getRegister().addEntry("gp.cx.parampb", mCxParameterPb, lDescription);
	}
}

void SelectiveConstrainedSelectionOp::findParameterSubTreeRoots(Beagle::GP::Individual& inIndiv, Beagle::GP::Context& ioContext, std::vector< std::pair<unsigned int,unsigned int> > &outNodes) const {
	Beagle_StackTraceBeginM();
	
	//Go throught all nodes
	for(unsigned int t = 0; t < inIndiv.size(); ++t) {
		GP::Tree& lTree = *inIndiv[t];
		for(unsigned int i = 0; i < lTree.size(); ++i) {
			std::vector<const std::type_info*> lNodeTypes = lTree[i].mPrimitive->getReturnTypes(ioContext);
			//If the node is not returning a double, then one of it arguments might be the root of the parameter tree
			if( find(lNodeTypes.begin(), lNodeTypes.end(), ArgEph) == lNodeTypes.end() ) {
				for(unsigned int j = 0; j < lTree[i].mPrimitive->getNumberArguments(); ++j) {
					try{
						std::vector<const std::type_info*> lArgType = lTree[i].mPrimitive->getArgTypes(j,ioContext);
						if( find(lArgType.begin(), lArgType.end(), ArgEph) != lArgType.end() ) {
							//The argument is the root a parameter tree, add to the return nodes
							
							//Get argument index
							unsigned int lArgIndex = i+1;
							for(unsigned int k = 0; k < j; ++k) lArgIndex += lTree[lArgIndex].mSubTreeSize;
							
							//Add to the return node list
							outNodes.push_back( make_pair(t,lArgIndex) );
							
							//Could break here if it's assumed that primitive has only one double arguments.
						}
					}
					catch(Beagle::RunTimeException inError) {} //Catch the exeption when trying to access parent time with empty call stack
					//Could do a better job handling the callstack
				}
			}		
		}
	}
	
//	if(outNodes.size() > 0) {
//		std::ostringstream lStream;
//		lStream << (*inIndiv[outNodes[0].first])[outNodes[0].second].mPrimitive->getName();
//		for(unsigned int i = 1; i < outNodes.size(); ++i) {
//			lStream << "," << (*inIndiv[outNodes[i].first])[outNodes[i].second].mPrimitive->getName();
//		}
//		Beagle_LogDebugM(
//						   ioContext.getSystem().getLogger(),
//						   "selection", "SelectiveConstrainedSelectionOp::findParameterSubTreeRoots",
//						   std::string("Parameter root nodes: ")+lStream.str()
//						   );
//	}
		
	Beagle_StackTraceEndM("SelectiveConstrainedSelectionOp::findParameterSubTreeRoots(Beagle::GP::Individual& inIndiv, Beagle::GP::Context& ioContext, std::vector< std::pair<unsigned int,unsigned int> > &outNodes)");
}

bool SelectiveConstrainedSelectionOp::selectNodeToMateWithTypes(unsigned int& outSelectTreeIndex,
														   unsigned int& outSelectNodeIndex,
														   bool inSelectABranch,
														   std::vector<const std::type_info*> inNodeReturnTypes,
														   unsigned int inPrimitSetIndex,
														   unsigned int inMaxSubTreeDepth,
														   unsigned int inMaxSubTreeSize,
														   GP::Individual& inIndividual,
														   GP::Context& ioContext, bool inSelectSingleParameterSubTree) const
{
	Beagle_StackTraceBeginM();
	RouletteT< std::pair<unsigned int,unsigned int> > lRoulette;
	GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
	const unsigned int lOldTreeIndex = ioContext.getGenotypeIndex();
	ioContext.emptyCallStack();
	
	std::vector< std::pair<unsigned int,unsigned int> > lParameterRoots;

	if(inSelectSingleParameterSubTree) { 
		//Find all parameter root nodes
		findParameterSubTreeRoots(inIndividual, ioContext, lParameterRoots);
		if(lParameterRoots.size() == 0)
			return false;
		
		//If the roulette is empty, try another parameter root
		while(lParameterRoots.size() > 0) {
			std::pair<unsigned int,unsigned int> lChosenRoot = lParameterRoots[ioContext.getSystem().getRandomizer().rollInteger(0.0, lParameterRoots.size()-1)];
			GP::Tree::Handle lChosenTree = inIndividual[lChosenRoot.first];
			unsigned int lChosenRootIndex = lChosenRoot.second;
			
			if(lChosenTree->getPrimitiveSetIndex() != inPrimitSetIndex) {
				//Remove the root
				std::vector< std::pair<unsigned int,unsigned int> >::iterator lIter = find(lParameterRoots.begin(), lParameterRoots.end(), lChosenRoot);
				lParameterRoots.erase(lIter);
				continue;
			}
			
			ioContext.setGenotypeHandle(lChosenTree);
			ioContext.setGenotypeIndex(lChosenRoot.first);
			buildRouletteWithTypes(lRoulette,
								   inSelectABranch,
								   inNodeReturnTypes,
								   inMaxSubTreeDepth,
								   inMaxSubTreeSize,
								   lChosenRootIndex,
								   *lChosenTree,
								   ioContext);
			
			if(lRoulette.size() == 0) {
				std::vector< std::pair<unsigned int,unsigned int> >::iterator lIter = find(lParameterRoots.begin(), lParameterRoots.end(), lChosenRoot);
				lParameterRoots.erase(lIter);
			} else {
				break;
			}
		}
	} 
	else {
		for(unsigned int i=0; i<inIndividual.size(); ++i) {
			if(inIndividual[i]->getPrimitiveSetIndex() != inPrimitSetIndex) continue;
			ioContext.setGenotypeHandle(inIndividual[i]);
			ioContext.setGenotypeIndex(i);
			buildRouletteWithTypes(lRoulette,
								   inSelectABranch,
								   inNodeReturnTypes,
								   inMaxSubTreeDepth,
								   inMaxSubTreeSize,
								   0,
								   *inIndividual[i],
								   ioContext);
		}
	}
	
	ioContext.setGenotypeIndex(lOldTreeIndex);
	ioContext.setGenotypeHandle(lOldTreeHandle);
	if(lRoulette.size() == 0) return false;
	std::pair<unsigned int,unsigned int> lSelectedNode = lRoulette.select(ioContext.getSystem().getRandomizer());
	outSelectTreeIndex = lSelectedNode.first;
	outSelectNodeIndex = lSelectedNode.second;
	return true;
	Beagle_StackTraceEndM("bool SelectiveConstrainedSelectionOp::selectNodeToMateWithType(unsigned int& outSelectTreeIndex, unsigned int& outSelectNodeIndex, bool inSelectABranch, const std::type_info* inNodeReturnType, unsigned int inPrimitSetIndex, unsigned int inMaxSubTreeDepth, unsigned int inMaxSubTreeSize, GP::Individual& inIndividual, GP::Context& ioContext, bool inSelectSingleParameterSubTree) const");
}

/*!
 *  \brief Build a roulette of nodes that can be selected following the constraints penalties.
 *  \param ioRoulette Roulette of nodes that can be selected following the constraints given.
 *  \param inSelectABranch True if node to select must be a branch, false if it must a leaf.
 *  \param inNodeReturnType Desired return type for the nodes to be selected.
 *  \param inMaxSubTreeDepth Maximum sub tree depth allowed of the node to be selected.
 *  \param inMaxSubTreeSize Maximum sub tree size allowed of the node to be selected.
 *  \param inActualIndex Index in actual tree of the node processed.
 *  \param inTree Tree processed.
 *  \param ioContext Evolutionary context.
 *  \return Max depth of subtree processed.
 */
unsigned int SelectiveConstrainedSelectionOp::buildRouletteWithTypes(
																RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
																bool inSelectABranch,
																std::vector<const std::type_info*> inNodeReturnTypes,
																unsigned int inMaxSubTreeDepth,
																unsigned int inMaxSubTreeSize,
																unsigned int inActualIndex,
																GP::Tree& inTree,
																GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	const unsigned int lNbArgs = inTree[inActualIndex].mPrimitive->getNumberArguments();
	const unsigned int lSubTreeSize = inTree[inActualIndex].mSubTreeSize;
	const bool lGoodArity = ((inTree.size()==1) || ((lNbArgs==0) != inSelectABranch));
	ioContext.pushCallStack(inActualIndex);
	std::vector<const std::type_info*> lNodeTypes = inTree[inActualIndex].mPrimitive->getReturnTypes(ioContext);
	
	bool lCompatibleTyping = ((inNodeReturnTypes[0]==NULL) || (lNodeTypes[0]==NULL) );
	for(unsigned int k = 0; k < lNodeTypes.size() && !lCompatibleTyping; ++k) {
		lCompatibleTyping = std::find(inNodeReturnTypes.begin(),inNodeReturnTypes.end(),lNodeTypes[k]) != inNodeReturnTypes.end();
	}
	
	unsigned int lChildIndex = inActualIndex+1;
	unsigned int lMaxDepthDown = 0;
	for(unsigned int i=0; i<lNbArgs; ++i) {
		unsigned int lChildDepth = buildRouletteWithTypes(ioRoulette,
														  inSelectABranch,
														  inNodeReturnTypes,
														  inMaxSubTreeDepth,
														  inMaxSubTreeSize,
														  lChildIndex,
														  inTree,
														  ioContext);
		lChildIndex += inTree[lChildIndex].mSubTreeSize;
		if(lChildDepth > lMaxDepthDown) lMaxDepthDown = lChildDepth;
	}
	++lMaxDepthDown;
	const unsigned int lMaxDepthUp = ioContext.getCallStackSize();
	ioContext.popCallStack();
	if(lGoodArity && lCompatibleTyping && (lSubTreeSize<=inMaxSubTreeSize) &&
	   (lMaxDepthDown<=inMaxSubTreeDepth) && (lMaxDepthUp<=inMaxSubTreeDepth)) {
		std::pair<unsigned int,unsigned int> lPair(ioContext.getGenotypeIndex(), inActualIndex);
		ioRoulette.insert(lPair, 1.0);
	}
	return lMaxDepthDown;
	Beagle_StackTraceEndM("unsigned int SelectiveConstrainedSelectionOp::buildRouletteWithTypes(RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette, bool inSelectABranch, const std::type_info* inNodeReturnType, unsigned int inMaxSubTreeDepth, unsigned int inMaxSubTreeSize, unsigned int inActualIndex, GP::Tree& inTree, GP::Context& ioContext) const");
}


/*! \brief Build a roulette for node selection
 *  The roulette is built so that the node type match the desired one. 
 *  \param  ioRoulette Resulting roulette
 *	\param	inSelectABranch Specify to select a branch.
 *  \param  inNodeReturnTypes Return argument type that is desired
 *  \param  inIndiv Individual to inspect
 *  \param  ioContext Beagle context
 *  \param  inIsExclusiveSet If set to true, the search is inversed to exclude the desired type from the set.
 */
void SelectiveConstrainedSelectionOp::buildRoulette(
													Beagle::RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
													bool inSelectABranch,
													std::vector<const std::type_info*> inNodeReturnTypes,
													Beagle::GP::Individual& inIndiv, Beagle::GP::Context& ioContext,
													bool inIsExclusiveSet, bool inSelectSingleParameterSubTree) 
{
	Beagle_StackTraceBeginM();
	
	//Compute total number of node of the individual including all trees
	unsigned int     lSizeIndiv     = 0;
	for(unsigned int i=0; i<inIndiv.size(); i++) lSizeIndiv += inIndiv[i]->size();
	
	unsigned int lNodeSearchStart = 0;
	unsigned int lNodeSearchEnd = lSizeIndiv;
	
	if(inSelectSingleParameterSubTree) { 
		//Find all parameter root nodes
		std::vector< std::pair<unsigned int,unsigned int> > lParameterRoots;
		findParameterSubTreeRoots(inIndiv, ioContext, lParameterRoots);
		
		//Choose one parameter sub-tree
		std::pair< unsigned int,unsigned int> lChosenRoot;
		if(lParameterRoots.size() > 0)
			lChosenRoot = lParameterRoots[ioContext.getSystem().getRandomizer().rollInteger(0.0, lParameterRoots.size()-1)];
		else
			return; //Empty roulette
		
		GP::Tree& lChosenTree = *inIndiv[lChosenRoot.first];
		unsigned int lChosenRootIndex = lChosenRoot.second;
		
		lNodeSearchStart = lChosenRootIndex;
		lNodeSearchEnd = lChosenRootIndex + lChosenTree[lChosenRootIndex].mSubTreeSize;
	} 
	
	
	for(unsigned int lChoosenNode = lNodeSearchStart; lChoosenNode < lNodeSearchEnd; ++lChoosenNode) {
		// Get the tree in which the choosen node is. Change the global node index to the tree's index.
		unsigned int lChoosenTree = 0;
		for(; lChoosenTree<inIndiv.size(); lChoosenTree++) {
			if(lChoosenNode < inIndiv[lChoosenTree]->size()) 
				break;
			Beagle_AssertM(lChoosenNode >= inIndiv[lChoosenTree]->size());
			lChoosenNode -= inIndiv[lChoosenTree]->size();
		}
		Beagle_AssertM(lChoosenTree < inIndiv.size());
		
		GP::Tree& lTree = *inIndiv[lChoosenTree];
		
		//Get the number of arguments
		const unsigned int lNbArgs = lTree[lChoosenNode].mPrimitive->getNumberArguments();
		const bool lGoodArity = ((lTree.size()==1) || ((lNbArgs==0) != inSelectABranch));
		if(!lGoodArity) continue;
		
		//Get compatibility of types
		std::vector<const std::type_info*> lNodeTypes = lTree[lChoosenNode].mPrimitive->getReturnTypes(ioContext);
		
		bool lCompatibleTyping = ((inNodeReturnTypes[0]==NULL) || (lNodeTypes[0]==NULL) );
		for(unsigned int k = 0; k < lNodeTypes.size() && !lCompatibleTyping; ++k) {
			if(inIsExclusiveSet)
				lCompatibleTyping = std::find(inNodeReturnTypes.begin(),inNodeReturnTypes.end(),lNodeTypes[k]) == inNodeReturnTypes.end();
			else
				lCompatibleTyping = std::find(inNodeReturnTypes.begin(),inNodeReturnTypes.end(),lNodeTypes[k]) != inNodeReturnTypes.end();
			
		}
		
		//if all condition are good, add to the roulette
		if(lGoodArity && lCompatibleTyping) {
			std::pair<unsigned int,unsigned int> lPair(lChoosenTree, lChoosenNode);
			ioRoulette.insert(lPair, 1.0);
		}
	}	
	
	Beagle_StackTraceEndM("void SelectiveConstrainedSelectionOp::buildRoulette()");
}

/*! \brief Build a roulette for node selection
 *  The roulette is built so that the node type match the desired one. 
 *  \param  ioRoulette Resulting roulette
 *  \param  inNodeReturnTypes Return argument type that is desired
 *  \param  inIndiv Individual to inspect
 *  \param  ioContext Beagle context
 *  \param  inIsExclusiveSet If set to true, the search is inversed to exclude the desired type from the set.
 */
void SelectiveConstrainedSelectionOp::buildRoulette(Beagle::RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
													std::vector<const std::type_info*> inNodeReturnTypes,
													Beagle::GP::Individual& inIndiv, Beagle::GP::Context& ioContext,
													bool inIsExclusiveSet, bool inSelectSingleParameterSubTree ) 
{
	Beagle_StackTraceBeginM();
	
	ioRoulette.clear();
	
	//Compute total number of node of the individual including all trees
	unsigned int     lSizeIndiv     = 0;
	for(unsigned int i=0; i<inIndiv.size(); i++) lSizeIndiv += inIndiv[i]->size();
	
	unsigned int lNodeSearchStart = 0;
	unsigned int lNodeSearchEnd = lSizeIndiv;
	
	if(inSelectSingleParameterSubTree) { 
		//Find all parameter root nodes
		std::vector< std::pair<unsigned int,unsigned int> > lParameterRoots;
		findParameterSubTreeRoots(inIndiv, ioContext, lParameterRoots);

		//Choose one parameter sub-tree
		std::pair< unsigned int,unsigned int> lChosenRoot;
		if(lParameterRoots.size() > 0)
			lChosenRoot = lParameterRoots[ioContext.getSystem().getRandomizer().rollInteger(0.0, lParameterRoots.size()-1)];
		else
			return;
		
		GP::Tree& lChosenTree = *inIndiv[lChosenRoot.first];
		unsigned int lChosenRootIndex = lChosenRoot.second;
		
		lNodeSearchStart = lChosenRootIndex;
		lNodeSearchEnd = lChosenRootIndex + lChosenTree[lChosenRootIndex].mSubTreeSize;
	} 

	
	for(unsigned int lChoosenNode = lNodeSearchStart; lChoosenNode < lNodeSearchEnd; ++lChoosenNode) {
		// Get the tree in which the choosen node is. Change the global node index to the tree's index.
		unsigned int lChoosenTree = 0;
		for(; lChoosenTree<inIndiv.size(); lChoosenTree++) {
			if(lChoosenNode < inIndiv[lChoosenTree]->size()) 
				break;
			Beagle_AssertM(lChoosenNode >= inIndiv[lChoosenTree]->size());
			lChoosenNode -= inIndiv[lChoosenTree]->size();
		}
		Beagle_AssertM(lChoosenTree < inIndiv.size());
		
		GP::Tree& lTree = *inIndiv[lChoosenTree];
		
		//Get compatibility of types
		std::vector<const std::type_info*> lNodeTypes = lTree[lChoosenNode].mPrimitive->getReturnTypes(ioContext);
		
		bool lCompatibleTyping = ((inNodeReturnTypes[0]==NULL) || (lNodeTypes[0]==NULL) );
		for(unsigned int k = 0; k < lNodeTypes.size() && !lCompatibleTyping; ++k) {
			if(inIsExclusiveSet)
				lCompatibleTyping = std::find(inNodeReturnTypes.begin(),inNodeReturnTypes.end(),lNodeTypes[k]) == inNodeReturnTypes.end();
			else
				lCompatibleTyping = std::find(inNodeReturnTypes.begin(),inNodeReturnTypes.end(),lNodeTypes[k]) != inNodeReturnTypes.end();
		}
		
		//if all condition are good, add to the roulette
		if(lCompatibleTyping) {
			std::pair<unsigned int,unsigned int> lPair(lChoosenTree, lChoosenNode);
			ioRoulette.insert(lPair, 1.0);
		}
	}
	
	
	
//	if(ioRoulette.size() > 0) {
//		std::ostringstream lStream;
//		lStream << ioRoulette[0].second.first << ":" << ioRoulette[0].second.second << ":" << (*(inIndiv[ioRoulette[0].second.first]))[ioRoulette[0].second.second].mPrimitive->getName();
//		for(unsigned int i = 1; i < ioRoulette.size(); ++i) {
//			lStream << "," << ioRoulette[i].second.first << ":" << ioRoulette[i].second.second << ":" << (*(inIndiv[ioRoulette[i].second.first]))[ioRoulette[i].second.second].mPrimitive->getName();
//		}
//		Beagle_LogDebugM(
//						   ioContext.getSystem().getLogger(),
//						   "selection", "SelectiveConstrainedSelectionOp::buildRoulette",
//						   std::string("Possible node: ")+lStream.str()
//						   );
//	}
	
	Beagle_StackTraceEndM("void SelectiveConstrainedSelectionOp::buildRoulette()");
}


/*! \brief Check if the desired argument exist in the individual
 *  Look for the existance of at least one node having the desired argument type.
 */
bool SelectiveConstrainedSelectionOp::hasOutputTypes(std::vector<const std::type_info*> inNodeReturnTypes, Beagle::GP::Individual& inIndiv,
													 Beagle::GP::Context& ioContext, bool inIsExclusiveSet) {
	Beagle_StackTraceBeginM();
	//Find at least one node with inNodeReturnTypes
	for(unsigned int i=0; i<inIndiv.size(); i++) {
		GP::Tree& lTree = *inIndiv[i];
		for(unsigned int j=0; j<lTree.size(); j++) {
			//Get compatibility of types
			std::vector<const std::type_info*> lNodeTypes = lTree[j].mPrimitive->getReturnTypes(ioContext);
			
			bool lCompatibleTyping = ((inNodeReturnTypes[0]==NULL) || (lNodeTypes[0]==NULL) );
			for(unsigned int k = 0; k < lNodeTypes.size() && !lCompatibleTyping; ++k) {
				if(inIsExclusiveSet)
					lCompatibleTyping = std::find(inNodeReturnTypes.begin(),inNodeReturnTypes.end(),lNodeTypes[k]) == inNodeReturnTypes.end();
				else
					lCompatibleTyping = std::find(inNodeReturnTypes.begin(),inNodeReturnTypes.end(),lNodeTypes[k]) != inNodeReturnTypes.end();
			}
			
			if(lCompatibleTyping) {
				return true;
			}
		}
	}
	
	return false;
	
	Beagle_StackTraceEndM("bool SelectiveConstrainedSelectionOp::hasOutputTypes()");
}



