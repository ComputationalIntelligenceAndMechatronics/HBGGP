/*
 *  DepthDependentSelectionOp.cpp
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

#include "DepthDependentSelectionOp.h"
#include <beagle/GP/Context.hpp>

using namespace Beagle;

void DepthDependentSelectionOp::initialize(Beagle::System& ioSystem) {
	SelectiveConstrainedSelectionOp::initialize(ioSystem);
	
	if(ioSystem.getRegister().isRegistered("gp.mut.depthfactor")) {
		mMutDepthFactor = castHandleT<Float>(ioSystem.getRegister()["gp.mut.depthfactor"]);
	} else {
		mMutDepthFactor = new Float(1.0);
		Register::Description lDescription(
										   "Node mutation probability adjustment factor relative to subtree depth.",
										   "FloatArray",
										   mMutDepthFactor->serialize(),
										   "Subtree factor node mutation."
										   );
		ioSystem.getRegister().addEntry("gp.mut.depthfactor", mMutDepthFactor, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("gp.cx.depthfactor")) {
		mCxDepthFactor = castHandleT<Float>(ioSystem.getRegister()["gp.cx.depthfactor"]);
	} else {
		mCxDepthFactor = new Float(1.0);
		Register::Description lDescription(
										   "Node crossover probability adjustment factor relative to subtree depth.",
										   "FloatArray",
										   mCxDepthFactor->serialize(),
										   "Subtree factor node crossover."
										   );
		ioSystem.getRegister().addEntry("gp.cx.depthfactor", mCxDepthFactor, lDescription);
	}
	
//	if(ioSystem.getRegister().isRegistered("gp.tree.maxdepth")) {
//		mMaxTreeDepth = castHandleT<UInt>(ioSystem.getRegister()["gp.tree.maxdepth"]);
//	} else {
//		mMaxTreeDepth = new UInt(17);
//		Register::Description lDescription(
//										   "Maximum tree depth",
//										   "UInt",
//										   mMaxTreeDepth->serialize(),
//										   "Maximum allowed depth for the trees."
//										   );
//		ioSystem.getRegister().addEntry("gp.tree.maxdepth", mMaxTreeDepth, lDescription);
//	}
}

double DepthDependentSelectionOp::computeWeight(const GP::Tree& inTree, unsigned int inNode,Beagle::GP::Context& ioContext) const {
//	unsigned int lSizeIndividual = 0;
//	for(unsigned int i=0; i<inIndividual.size(); i++) lSizeIndividual += inIndividual[i]->size();
	
	//Compute node depth
	unsigned int lDepth = 0;
	for(unsigned int lNode = 0; lNode < inNode; ) {
		if( inTree[lNode].mSubTreeSize + lNode - 1 < inNode ) { 
			//inNode is not in subtree
			lNode += inTree[lNode].mSubTreeSize;
		}else { 
			//inNode is in subtree, get on step deeper
			++lNode;
			++lDepth;
		}
	}
	
	double lFactor = mMutDepthFactor->getWrappedValue();
	double lSubTreeSizeFactor = inTree[inNode].mSubTreeSize/double(inTree.size());
	double lDepthFactor = lDepth/double(inTree.getTreeDepth());
	
	double lWeight = 1 - lFactor*lDepthFactor*lSubTreeSizeFactor;
//	Beagle_LogDebugM(
//					   ioContext.getSystem().getLogger(),
//					   "selection", "DepthDependentSelectionOp::computeWeight",
//					   std::string("Node depth: ")+uint2str(lDepth)+
//					   std::string(", sub-tree size: ")+uint2str(inTree[inNode].mSubTreeSize)+
//					   std::string(", tree size: ")+uint2str(inTree.size())
//					   );	
//	Beagle_LogDebugM(
//					 ioContext.getSystem().getLogger(),
//					 "selection", "DepthDependentSelectionOp::computeWeight",
//					 std::string("Computed node weigh of ")+inTree[inNode].mPrimitive->getName()+std::string(" is ")+dbl2str(lWeight)
//					 );
	
	return lWeight;
}

bool DepthDependentSelectionOp::selectNodeToMateWithTypes(unsigned int& outSelectTreeIndex,
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
			SelectiveConstrainedSelectionOp::buildRouletteWithTypes(lRoulette,
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
			DepthDependentSelectionOp::buildRouletteWithTypes(lRoulette,
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
	Beagle_StackTraceEndM("bool DepthDependentSelectionOp::selectNodeToMateWithType(unsigned int& outSelectTreeIndex, unsigned int& outSelectNodeIndex, bool inSelectABranch, const std::type_info* inNodeReturnType, unsigned int inPrimitSetIndex, unsigned int inMaxSubTreeDepth, unsigned int inMaxSubTreeSize, GP::Individual& inIndividual, GP::Context& ioContext, bool inSelectSingleParameterSubTree) const");
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
unsigned int DepthDependentSelectionOp::buildRouletteWithTypes(
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
		unsigned int lChildDepth = DepthDependentSelectionOp::buildRouletteWithTypes(ioRoulette,
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
		
		double lWeight = 1;
		lWeight = computeWeight(ioContext.getGenotype(),inActualIndex,ioContext);
		ioRoulette.insert(lPair, lWeight);

	}
	return lMaxDepthDown;
	Beagle_StackTraceEndM("unsigned int DepthDependentSelectionOp::buildRouletteWithTypes(RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette, bool inSelectABranch, const std::type_info* inNodeReturnType, unsigned int inMaxSubTreeDepth, unsigned int inMaxSubTreeSize, unsigned int inActualIndex, GP::Tree& inTree, GP::Context& ioContext) const");
}


/*! \brief Build a roulette for node selection
 *  The roulette is built so that the node type match the desired one. This function also 
 *	modify the selection probability of the node according to it's subtree size.
 *  \param  ioRoulette Resulting roulette
 *	\param	inSelectABranch Specify to select a branch.
 *  \param  inNodeReturnTypes Return argument type that is desired
 *  \param  inIndiv Individual to inspect
 *  \param  ioContext Beagle context
 *  \param  inIsExclusiveSet If set to true, the search is inversed to exclude the desired type from the set.
 */
void DepthDependentSelectionOp::buildRoulette(
											  Beagle::RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
											  bool inSelectABranch,
											  std::vector<const std::type_info*> inNodeReturnTypes,
											  Beagle::GP::Individual& inIndiv,
											  Beagle::GP::Context& ioContext,bool inIsExclusiveSet, bool inSelectSingleParameterSubTree) 
{
	Beagle_StackTraceBeginM();
	
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
			double lWeight = 1;
			if(!inSelectSingleParameterSubTree) 
				lWeight = computeWeight(lTree,lChoosenNode,ioContext);
			ioRoulette.insert(lPair, lWeight);
		}
	}	
	
	Beagle_StackTraceEndM("void DepthDependentSelectionOp::buildRoulette()");
}


/*! \brief Build a roulette for node selection
 *  The roulette is built so that the node type match the desired one. This function also 
 *	modify the selection probability of the node according to it's subtree size.
 *  \param  ioRoulette Resulting roulette
 *  \param  inNodeReturnTypes Return argument type that is desired
 *  \param  inIndiv Individual to inspect
 *  \param  ioContext Beagle context
 *  \param  inIsExclusiveSet If set to true, the search is inversed to exclude the desired type from the set.
 */
void DepthDependentSelectionOp::buildRoulette(
											  Beagle::RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
											  std::vector<const std::type_info*> inNodeReturnTypes,
											  Beagle::GP::Individual& inIndiv,
											  Beagle::GP::Context& ioContext,bool inIsExclusiveSet, bool inSelectSingleParameterSubTree) 
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
			double lWeight = 1;
			if(!inSelectSingleParameterSubTree) 
				lWeight = computeWeight(lTree,lChoosenNode,ioContext);
			ioRoulette.insert(lPair, lWeight);
		}
	}
	
	
//	if(ioRoulette.size() > 0) {
//		std::ostringstream lStream;
//		
//		lStream << ioRoulette[0].second.first << ":" << ioRoulette[0].second.second << ":" << (*(inIndiv[ioRoulette[0].second.first]))[ioRoulette[0].second.second].mPrimitive->getName();
//		for(unsigned int i = 1; i < ioRoulette.size(); ++i) {
//			lStream << "," << ioRoulette[i].second.first << ":" << ioRoulette[i].second.second << ":" << (*(inIndiv[ioRoulette[i].second.first]))[ioRoulette[i].second.second].mPrimitive->getName();
//		}
//		Beagle_LogVerboseM(
//						   ioContext.getSystem().getLogger(),
//						   "selection", "DepthDependentSelectionOp::buildRoulette",
//						   std::string("Possible node: ")+lStream.str()
//						   );
//	}
	
	Beagle_StackTraceEndM("void DepthDependentSelectionOp::buildRoulette()");
}



