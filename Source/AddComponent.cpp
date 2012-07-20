/*
 *  AddComponent.cpp
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
 *  This file was created by Jean-Francois Dupuis on 13/05/09.
 */

#include "AddComponent.h"
#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include <beagle/ValidationException.hpp>
#include <beagle/GP/PrimitiveInline.hpp>

std::vector<const std::type_info*> AddComponent::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	
	//getParentArgumentTypes(ioContext);
	
	std::vector<const std::type_info*> lTypes;
	switch(inN) {
		case 0:
			lTypes.push_back(ArgEph);
			return lTypes;
		case 1:
			return getParentArgumentTypes(ioContext);
		case 2:
			lTypes.push_back(ArgBond);
			return lTypes;
		case 3:
			lTypes.push_back(ArgNode);
			return lTypes;
		default:
			throw ValidationException("AddComponent should have no more than 4 arguments");
	}
}

//vector<const std::type_info*> AddComponent::getParentArgumentTypes(GP::Context& ioContext) const {
//	GP::Tree& lActualTree = ioContext.getGenotype();
//	
//	//Find which argument this primitive is
//	unsigned int lParentIndex    = ioContext.getCallStackElement(ioContext.getCallStackSize()-2);
//	unsigned int lChildIndex     = lParentIndex + 1;
//	unsigned int lArgsIndexChild = 0;
//	while(lChildIndex != ioContext.getCallStackTop()) {
//		lChildIndex += lActualTree[lChildIndex].mSubTreeSize;
//		Beagle_AssertM(lChildIndex < lActualTree.size());
//		lArgsIndexChild++;
//		Beagle_AssertM(lArgsIndexChild < lActualTree[lParentIndex].mPrimitive->getNumberArguments());
//	}
//	
//	// Return parent argment cast
//	unsigned int lActualNodeIndex = ioContext.getCallStackTop();
//	ioContext.popCallStack();
//	std::vector<const std::type_info*> lParentDesiredTypes = lActualTree[lParentIndex].mPrimitive->getArgTypes(lArgsIndexChild, ioContext);
//	ioContext.pushCallStack(lActualNodeIndex);
//	
//	return lParentDesiredTypes;
//}

std::vector<const std::type_info*> AddComponent::getReturnTypes(GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgJct);
	lTypes.push_back(ArgJct0);
	lTypes.push_back(ArgJct1);
	return lTypes;
}

void AddComponent::add(Beagle::GP::Datum& inJunction, BG::Component* inComponent, Beagle::GP::Context& ioContext, double inFactor) {
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	JunctionPtr& lInJunction = castObjectT<JunctionPtr&>(inJunction);
	GrowingBG::Handle lGrowingBondGraph = lContext.getBondGraph();
	
	//Get the component parameter value
	Beagle::Double lValue;
	getArgument(0,lValue,ioContext);
	//Check if the return value is zero, if so set it to a very small value;
	if(lValue == 0.0) lValue = ZEROVALUE; 
	inComponent->setValue(fabs(lValue)*inFactor);
	
	BG::Bond* lNewBond = 0;
	lGrowingBondGraph->insertComponent(lInJunction.getValue(),inComponent,lNewBond);

	BondPtr lNewBondPtr(lNewBond);
	ComponentPtr lNewComponent(inComponent);
	
	//Continue parsing the tree
	getArgument(1,lInJunction,ioContext);
	getArgument(2,lNewBondPtr,ioContext);
#ifndef WITHOUT_REPLACE
	getArgument(3,lNewComponent,ioContext);
#endif
}


void AddR::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	//Create the new component
	BG::Passive *lR = new BG::Passive(BG::Passive::eResistor);
	add(inDatum,lR,ioContext,RFactor);

}

void AddC::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	//Create the new component
	BG::Passive *lC = new BG::Passive(BG::Passive::eCapacitor);
	add(inDatum,lC,ioContext,CFactor);
}

void AddI::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	//Create the new component
	BG::Passive *lI = new BG::Passive(BG::Passive::eInductor);
	add(inDatum,lI,ioContext,IFactor);
}
