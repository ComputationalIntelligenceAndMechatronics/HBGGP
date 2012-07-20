/*
 *  DepthDependentSelectionOp.h
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

#ifndef DepthDependentSelectionOp_H
#define DepthDependentSelectionOp_H

#include <beagle/PointerT.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/GP/Tree.hpp>
#include <beagle/GP/Individual.hpp>
#include <beagle/RouletteT.hpp>

#include "SelectiveConstrainedSelectionOp.h"

class DepthDependentSelectionOp : public SelectiveConstrainedSelectionOp {
protected:
	Beagle::Float::Handle mMutDepthFactor;
	Beagle::Float::Handle mCxDepthFactor;
//	Beagle::UInt::Handle mMaxTreeDepth;
	
	double computeWeight(const Beagle::GP::Tree& inTree, unsigned int inNode, Beagle::GP::Context& ioContext) const;
public:
	explicit DepthDependentSelectionOp() : SelectiveConstrainedSelectionOp() {}
	~DepthDependentSelectionOp() {}
	
	virtual void initialize(Beagle::System& ioSystem);
	
	virtual void buildRoulette(Beagle::RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
							   bool inSelectABranch,
							   std::vector<const std::type_info*> inNodeReturnTypes,
							   Beagle::GP::Individual& inIndiv,
							   Beagle::GP::Context& ioContext,bool inIsExclusiveSet, bool inSelectSingleParameterSubTree);
	
	virtual void buildRoulette(Beagle::RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
							   std::vector<const std::type_info*> inNodeReturnTypes,
							   Beagle::GP::Individual& inIndiv,
							   Beagle::GP::Context& ioContext,bool inIsExclusiveSet, bool inSelectSingleParameterSubTree);
	
	virtual bool selectNodeToMateWithTypes(unsigned int& outSelectTreeIndex,
										   unsigned int& outSelectNodeIndex,
										   bool inSelectABranch,
										   std::vector<const std::type_info*> inNodeReturnTypes,
										   unsigned int inPrimitSetIndex,
										   unsigned int inMaxSubTreeDepth,
										   unsigned int inMaxSubTreeSize,
										   Beagle::GP::Individual& inIndividual,
										   Beagle::GP::Context& ioContext, bool inSelectSingleParameterSubTree) const;
	
	virtual unsigned int buildRouletteWithTypes(
												Beagle::RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
												bool inSelectABranch,
												std::vector<const std::type_info*> inNodeReturnTypes,
												unsigned int inMaxSubTreeDepth,
												unsigned int inMaxSubTreeSize,
												unsigned int inActualIndex,
												Beagle::GP::Tree& inTree,
												Beagle::GP::Context& ioContext) const;

};
#endif
