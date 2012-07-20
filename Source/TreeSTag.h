/*
 *  TreeSTag.h
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

#ifndef TreeSTag_H
#define TreeSTag_H

#include <beagle/GP.hpp>
#include <beagle/GA.hpp>
#include "GrowingBG.h"

class TreeSTag : public Beagle::GP::Tree {
public:
	//! Allocator type.
	typedef Beagle::AllocatorT<TreeSTag,Beagle::GP::Tree::Alloc> Alloc;
	//! Handle type.
	typedef Beagle::PointerT<TreeSTag,Beagle::GP::Tree::Handle> Handle;
	//! Bag type.
	typedef Beagle::ContainerT<TreeSTag,Beagle::GP::Tree::Bag> Bag;
	
	explicit TreeSTag(unsigned int inSize=0,
				  unsigned int inPrimitiveSetIndex=UINT_MAX,
				  unsigned int inNumberArguments=0);
	virtual ~TreeSTag(); 
	
	virtual bool isEqual(const Beagle::Object& inRightObj) const;
	virtual void readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	
	unsigned int getStructureID() const { return mStructureID; }
	void setStructureID(unsigned int inStructureID) { mStructureID = inStructureID; }
	
	unsigned int getAge() const { return mAge; }
	void setAge(unsigned int inAge) { mAge = inAge; }
	
	void assignNewParameterVector(Beagle::GA::FloatVector::Handle inParameters, Beagle::GP::Context& ioContext);
	Beagle::GA::FloatVector::Handle computeParameterVector(Beagle::GP::Context& ioContext);
	//Beagle::GA::FloatVector getParameterVector() const { return mParametersVector; }
	
//	void setBondGraph(GrowingBG::Handle& inBondGraph) { mBondGraph = inBondGraph; }
//	GrowingBG::Handle& getBondGraph() { return mBondGraph; }
//	const GrowingBG::Handle& getBondGraph() const { return mBondGraph; }
	
	bool compareTopology(const TreeSTag& inRightTree) const;
	bool findMatchingTopology(Beagle::Deme& ioDeme, Beagle::Context& ioContext, TreeSTag::Handle& outTree);
	
	
	static void removeNOP(Beagle::Individual& inIndividual, Beagle::GP::Context& ioContext);
	
	bool isStructureIDValid() const { return mValidStructure; }
	void setStructureIDInvalid();
	void setStructureIDValid() { mValidStructure = true; }
	
protected:
	static void removeNOPLoop(unsigned int inN, Beagle::GP::Context& ioContext);
	
	//GrowingBG::Handle mBondGraph;
	unsigned int mAge;
	bool mValidStructure;
	unsigned int mStructureID;

	unsigned int mAssignmentIndex;
	Beagle::GA::FloatVector::Handle mParametersVector;
	unsigned int parseSubTreeAssign(unsigned int inN, Beagle::GP::Context& ioContext);
	unsigned int parseSubTree(unsigned int inN, Beagle::GP::Context& ioContext);
};

#endif
