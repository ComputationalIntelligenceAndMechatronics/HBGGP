/*
 *  TreeSTag.cpp
 *  Copyright 2009 Jean-Francois Dupuis.
 *
 *  This file is part of EvoBG.
 *  
 *  EvoBG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EvoBG is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EvoBG.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  This file was created by Jean-Francois Dupuis on 10/08/09.
 */

#include "TreeSTag.h"
#include "Defines.h"
#include "ArgType.h"
#include "beagle/GP.hpp"
#include "GrowingHybridBondGraph.h"

using namespace Beagle;

TreeSTag::~TreeSTag() { }


void TreeSTag::setStructureIDInvalid() {
	mStructureID = 0;
	mValidStructure = false;
}

void TreeSTag::assignNewParameterVector(Beagle::GA::FloatVector::Handle inParameters, Beagle::GP::Context& ioContext) {
	mParametersVector = inParameters;
	mAssignmentIndex = 0;
	mValidStructure = false;
	parseSubTreeAssign(0, ioContext);
	return;
}

unsigned int TreeSTag::parseSubTreeAssign(unsigned int inN, GP::Context& ioContext) {
	unsigned int lNumberArguments = (*this)[inN].mPrimitive->getNumberArguments();
	
	//Identify the primitive
	if( /*((*this)[inN].mPrimitive->getReturnType(ioContext) == ArgEph) &&*/ ((*this)[inN].mPrimitive->getName() == "E")) {
		Double lValue = (*mParametersVector)[mAssignmentIndex++];
		(*this)[inN].mPrimitive->setValue(lValue);
	}
	
	//Parse all child
	unsigned int lSubTreeSize = 1;
	for(unsigned int i=0; i<lNumberArguments; ++i) {
		lSubTreeSize += parseSubTreeAssign((lSubTreeSize+inN), ioContext);
	}
	return lSubTreeSize;
}

GA::FloatVector::Handle TreeSTag::computeParameterVector(GP::Context& ioContext){
	mParametersVector->resize(0);
	parseSubTree(0, ioContext);
	return mParametersVector;
}

unsigned int TreeSTag::parseSubTree(unsigned int inN, GP::Context& ioContext) {

	unsigned int lNumberArguments = (*this)[inN].mPrimitive->getNumberArguments();
	
	//Identify the primitive
	if( /*((*this)[inN].mPrimitive->getReturnType(ioContext) == ArgEph) &&*/ ((*this)[inN].mPrimitive->getName() == "E")) {
		Double lValue;
		if((*this)[inN].mPrimitive->haveValue()) {
			(*this)[inN].mPrimitive->getValue(lValue);
			mParametersVector->push_back(lValue);
		}
	}
	
	//Parse all child
	unsigned int lSubTreeSize = 1;
	for(unsigned int i=0; i<lNumberArguments; ++i) {
		lSubTreeSize += parseSubTree((lSubTreeSize+inN), ioContext);
	}
	return lSubTreeSize;
}


TreeSTag::TreeSTag(unsigned int inSize, unsigned int inPrimitiveSetIndex, unsigned int inNumberArguments) {
	Beagle::GP::Tree(inSize,inPrimitiveSetIndex,inNumberArguments);
	mStructureID = 0;
	mValidStructure = false;
	mParametersVector = new GA::FloatVector;
}

bool TreeSTag::isEqual(const Object& inRightObj) const {
	const TreeSTag& lRightTree = castObjectT<const TreeSTag&>(inRightObj);
	if(lRightTree.getStructureID() != mStructureID) 
		return false;
	return(Tree::isEqual(inRightObj));
}

void TreeSTag::readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext){
	Beagle_StackTraceBeginM();
	GP::Context& lGPContext = castObjectT<GP::Context&>(ioContext);
	
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Genotype"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");
	
	std::string lType = inIter->getAttribute("type").c_str();
	if(lType.empty())
		throw Beagle_IOExceptionNodeM(*inIter, "GP tree type of the genotype is not present!");
	if(lType != "gptreetag")
		throw Beagle_IOExceptionNodeM(*inIter, std::string("type of genotype mismatch, expected ")
									  +std::string("\"gptreetag\" but read \"")+lType+std::string("\" instead!"));
	
	//Get structure id
	std::string lStructureID = inIter->getAttribute("structureid").c_str();
	mStructureID = str2uint(lStructureID);
	
	// Get primitive set index
	std::string lPrimitiveSetIndexText = inIter->getAttribute("primitiveSetIndex").c_str();
	if(lPrimitiveSetIndexText.empty()) {
		// No primitive set index was specified.  This must be an old
		// tree.  Assume index is equal to tree's index
		unsigned int lGenotypeIndex = ioContext.getGenotypeIndex();
		if(lGenotypeIndex >= lGPContext.getSystem().getPrimitiveSuperSet().size()) {
			throw Beagle_RunTimeExceptionM(std::string("In GP::Tree::readWithContext(): The ")+
										   std::string("'primitiveSetIndex' attribute was missing from an individual's genotype. ")+
										   std::string("It would normally be assumed that such a tree was to be mapped to the ")+
										   std::string("primitive set of the same index as the genotype. In this case that would ")+
										   std::string("result in an incorrect mapping because there are not enough primitive sets ")+
										   std::string("in the System. Perhaps this individual was not intended to be read with the ")+
										   std::string("current set of primitive sets?"));
		}
		setPrimitiveSetIndex(lGenotypeIndex);
	}
	else {
		// primitiveSetIndex is a valid attribute.
		unsigned int lPrimitiveSetIndex = str2uint(lPrimitiveSetIndexText);
		if(lPrimitiveSetIndex >= lGPContext.getSystem().getPrimitiveSuperSet().size()) {
			std::string lMessage = std::string("In GP::Tree::readWithContext(): The 'primitiveSetIndex' ")+
			std::string("attribute contained the value '") + lPrimitiveSetIndexText + 
			std::string("' which was read as the number '") + uint2str(lPrimitiveSetIndex) + 
			std::string("'. This value is incorrect as there are not enough primitive sets in the ")+
			std::string("System. Perhaps this individual was not intended to be read with the current ")+
			std::string("set of primitive sets?");
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		setPrimitiveSetIndex(lPrimitiveSetIndex);
	}
	
	// Get numberArguments attribute
	std::string lNumberArgumentsText = inIter->getAttribute("numberArguments").c_str();
	if(lNumberArgumentsText.empty()) {
		// numberArguments attribute wasn't defined.  This must be an old
		// tree.  Assume the number of arguments is zero.
		setNumberArguments(0);
	}
	else {
		// numberArguments is a valid attribute.
		setNumberArguments(str2uint(lNumberArgumentsText));
	}
	
	// Get size attribute and reserve size accordingly
	std::string lSizeText = inIter->getAttribute("size").c_str();
	if(lSizeText.empty()==false) reserve(str2uint(lSizeText));
	
	PACC::XML::ConstIterator lChild = inIter->getFirstChild();
	if((!lChild) || (lChild->getType()!=PACC::XML::eData))
		throw Beagle_IOExceptionNodeM(*lChild, "expected a XML tag for the GP tree!");
	
	clear();
	readSubTree(lChild, lGPContext);
	
	//Read BondGraph if any
	const PACC::XML::Node* lParent = inIter->getParent();
	
	PACC::XML::ConstFinder lIndivFinder = PACC::XML::ConstIterator(lParent);
	PACC::XML::ConstIterator lBGTag = lIndivFinder.find("//BondGraph");
	if(!lBGTag) { 
		return;
	}
	
//	mBondGraph = new GrowingHybridBondGraph;
//	mBondGraph->read(lBGTag);	
	
	Beagle_StackTraceEndM("void GP::Tree::readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext)");
}

void TreeSTag::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	Beagle_StackTraceBeginM();
	
	ioStreamer.openTag("Genotype", inIndent);
	ioStreamer.insertAttribute("type", "gptreetag");
	ioStreamer.insertAttribute("structureid", uint2str(mStructureID));
	ioStreamer.insertAttribute("size", uint2str(size()));
	ioStreamer.insertAttribute("depth", uint2str(getTreeDepth()));
	ioStreamer.insertAttribute("primitSetId", uint2str(getPrimitiveSetIndex()));
	ioStreamer.insertAttribute("nbArgs", uint2str(getNumberArguments()));
	if(size() > 0) {
		unsigned int lSizeSubTree = writeSubTree(ioStreamer, 0, inIndent);
		Beagle_AssertM(lSizeSubTree == size());
	}
	ioStreamer.closeTag();
	
//	if(mBondGraph!=NULL) {
//		mBondGraph->write(ioStreamer,inIndent);
//	} 
	
	Beagle_StackTraceEndM("void GP::Tree::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

bool TreeSTag::compareTopology(const TreeSTag& inRightTree) const {
	if(this->size() != inRightTree.size())
		return false;
	
	for(unsigned int i = 0; i < (*this).size(); ++i) {
		if(((*this)[i].mPrimitive->getName() != inRightTree[i].mPrimitive->getName()) || ((*this)[i].mSubTreeSize != inRightTree[i].mSubTreeSize) )
			return false;
	}
	
	return true;
}


bool TreeSTag::findMatchingTopology(Deme& ioDeme, Context& ioContext, TreeSTag::Handle& outTree) {
	
	for(unsigned int i=0; i<ioDeme.size(); ++i) {
		TreeSTag::Handle lTree = castHandleT<TreeSTag>((*(ioDeme)[i])[0]);
		if(lTree != this) {
			if(this->compareTopology(*lTree)) {
				outTree = lTree;
				return true;
			}
		}
	}
	return false;
}

void TreeSTag::removeNOPLoop(unsigned int inN, GP::Context& ioContext) {
	GP::Tree& lActualTree = ioContext.getGenotype();
	
	if(lActualTree[inN].mPrimitive->getName() == "NOP") {
		//Decrease the subtree size of the call stack
		for(unsigned i = 0; i < ioContext.getCallStackSize(); ++i) {
			lActualTree[ioContext.getCallStack()[i]].mSubTreeSize -= 1;
		}
		
		//Delete the primitive from the tree
		std::vector< GP::Node,BEAGLE_STLALLOCATOR<GP::Node> >::iterator lPrimitiveIter = lActualTree.begin();
		lPrimitiveIter += inN;
		lActualTree.erase(lPrimitiveIter);

		
//		cout << "Callstack: " << ioContext.getCallStack()[0];
//		for(unsigned int i = 1; i < ioContext.getCallStackSize() ; ++i) {
//			cout << ", " << ioContext.getCallStack()[0];
//		} cout << endl;
//		for(unsigned int i = 0; i < lActualTree.size(); ++i) {
//			cout << i << " : " << lActualTree[i].mPrimitive->getName() << " : " << lActualTree[i].mSubTreeSize << endl;
//		}
		
		removeNOPLoop(inN, ioContext);
		
	} else {
		//Parse all arguments
		ioContext.pushCallStack(inN);
		for(unsigned int i = 0; i < lActualTree[inN].mPrimitive->getNumberArguments(); ++i) {
			removeNOPLoop(lActualTree[inN].mPrimitive->getChildrenNodeIndex(i,ioContext), ioContext);
		}
		ioContext.popCallStack();
	}
}


void TreeSTag::removeNOP(Individual& inIndividual, GP::Context& ioContext) {
//	Beagle_LogDebugM(
//					 ioContext.getSystem().getLogger(),
//					 "individual", "TreeSTag",
//					 std::string("Individual before NOP removal: ")+inIndividual.serialize()
//					 );
	
	
	TreeSTag::Handle lTree = castHandleT<TreeSTag>((inIndividual)[0]);
	GP::Individual& lIndividual = castObjectT<GP::Individual&>(inIndividual);
	
	GP::Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
	unsigned int lOldGenotypeIndex = ioContext.getGenotypeIndex();
	GP::Tree::Handle lOldGenotypeHandle = ioContext.getGenotypeHandle();	
	
	ioContext.setIndividualHandle(&lIndividual);
	ioContext.setGenotypeIndex(0);
	ioContext.setGenotypeHandle(lIndividual[0]);
	
	ioContext.getCallStack().clear();
	removeNOPLoop(0,ioContext);
	
	ioContext.setIndividualHandle(lOldIndividualHandle);
	ioContext.setGenotypeIndex(lOldGenotypeIndex);
	ioContext.setGenotypeHandle(lOldGenotypeHandle);
	
//	Beagle_LogDebugM(
//					 ioContext.getSystem().getLogger(),
//					 "individual", "TreeSTag",
//					 std::string("Individual after NOP removal: ")+inIndividual.serialize()
//					 );
}
