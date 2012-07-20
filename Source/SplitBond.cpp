/*
 *  SplitBond.cpp
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
 *  This file was created by Jean-Francois Dupuis on 11/05/10.
 */

#include "SplitBond.h"
#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include <beagle/ValidationException.hpp>
#include <beagle/GP/PrimitiveInline.hpp>

void SplitBond::insert(Beagle::GP::Datum& inBond, BG::Junction::JunctionType inType1, BG::Junction::JunctionType inType2, Beagle::GP::Context& ioContext) {
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	BondPtr& lInBond = castObjectT<BondPtr&>(inBond);
	GrowingBG::Handle lGrowingBondGraph = lContext.getBondGraph();
	BG::BondGraph* lBondGraph = lGrowingBondGraph->getBondGraph();
	
	BG::Junction* lNewJunction1 = new BG::Junction(inType1);
	BG::Junction* lNewJunction2 = new BG::Junction(inType2);
	
	BG::Bond* lNewBond0 = 0;
	BG::Bond* lNewBond1 = 0;
	BG::Bond* lNewBond2 = 0;
	
	//Get existing ports
	BG::Port *lFromPort  = lInBond.getValue()->getFromPort();
	BG::Port *lToPort = lInBond.getValue()->getToPort();
	
	BG::Component* lToComponent = lToPort->getComponent();
	BG::Component* lFromComponent = lFromPort->getComponent();
	
	if( (lToComponent->getComponentGroup() == BG::Component::eJ) && (lFromComponent->getComponentGroup() == BG::Component::eJ) ) {
		lBondGraph->addComponent(lNewJunction1);
		lBondGraph->addComponent(lNewJunction2);
		
		lNewBond0 = lBondGraph->connect(lFromComponent, lNewJunction1);
		lNewBond1 = lBondGraph->connect(lNewJunction1, lNewJunction2);
		lNewBond2 = lBondGraph->connect(lNewJunction2, lToComponent);
		
		BondPtr lNewBondPtr0(lNewBond0);
		BondPtr lNewBondPtr1(lNewBond1);
		BondPtr lNewBondPtr2(lNewBond2);
		JunctionPtr lNewJunctionPtr1(lNewJunction1);
		JunctionPtr lNewJunctionPtr2(lNewJunction2);
		
		//Continue parsing the tree
		getArgument(0,lInBond,ioContext);
		getArgument(1,lNewBondPtr0,ioContext);
		getArgument(2,lNewJunctionPtr1,ioContext);
		getArgument(3,lNewBondPtr1,ioContext);
		getArgument(4,lNewJunctionPtr2,ioContext);
		getArgument(5,lNewBondPtr2,ioContext);
		
		return;
	} 
	
	if(lToComponent->getComponentGroup() == BG::Component::eJ) {
		//lBondGraph->addComponent(lNewJunction1);
		lBondGraph->addComponent(lNewJunction2);
		
		lNewBond1 = lBondGraph->connect(lToComponent, lNewJunction2);
		lNewBond2 = lBondGraph->connect(lNewJunction2, lToComponent);
		
		//BondPtr lNewBondPtr0(lNewBond0);
		BondPtr lNewBondPtr1(lNewBond1);
		BondPtr lNewBondPtr2(lNewBond2);
		//JunctionPtr lNewJunctionPtr1(lNewJunction1);
		JunctionPtr lNewJunctionPtr2(lNewJunction2);
		
		//Continue parsing the tree
		getArgument(0,lInBond,ioContext);
		//getArgument(1,lNewBondPtr0,ioContext);
		//getArgument(2,lNewJunctionPtr1,ioContext);
		getArgument(3,lNewBondPtr1,ioContext);
		getArgument(4,lNewJunctionPtr2,ioContext);
		getArgument(5,lNewBondPtr2,ioContext);
		
		return;
	}
	
	if(lFromComponent->getComponentGroup() == BG::Component::eJ) {
		lBondGraph->addComponent(lNewJunction1);
		
		lNewBond0 = lBondGraph->connect(lFromComponent, lNewJunction1);
		lNewBond1 = lBondGraph->connect(lNewJunction1, lFromComponent);
		
		BondPtr lNewBondPtr0(lNewBond0);
		BondPtr lNewBondPtr1(lNewBond1);
		JunctionPtr lNewJunctionPtr1(lNewJunction1);
		
		//Continue parsing the tree
		getArgument(0,lInBond,ioContext);
		getArgument(1,lNewBondPtr0,ioContext);
		getArgument(2,lNewJunctionPtr1,ioContext);
		getArgument(3,lNewBondPtr1,ioContext);
		//getArgument(4,lNewJunctionPtr2,ioContext);
		//getArgument(5,lNewBondPtr2,ioContext);
		return;
	}
	
	
	
	

}

void SplitBond01::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	insert(inDatum, BG::Junction::eZero, BG::Junction::eOne, ioContext);
}

std::vector<const std::type_info*> SplitBond01::getReturnTypes(GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgBond);
	lTypes.push_back(ArgBond1);
	return lTypes;
}

std::vector<const std::type_info*> SplitBond01::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	switch(inN) {
		case 0:
			lTypes.push_back(ArgBond1);
			return lTypes;
		case 1:
			lTypes.push_back(ArgBond1);
			return lTypes;
		case 2:
			lTypes.push_back(ArgJct0);
			return lTypes;
		case 3:
			lTypes.push_back(ArgBond0);
			return lTypes;			
		case 4:
			lTypes.push_back(ArgJct1);
			return lTypes;
		case 5:
			lTypes.push_back(ArgBond1);
			return lTypes;
		default:
			throw ValidationException("SplitBond01 should have no more than 6 arguments");
	}
}

void SplitBond10::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	insert(inDatum, BG::Junction::eOne, BG::Junction::eZero, ioContext);
}

std::vector<const std::type_info*> SplitBond10::getReturnTypes(GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgBond);
	lTypes.push_back(ArgBond0);
	return lTypes;
}

std::vector<const std::type_info*> SplitBond10::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	switch(inN) {
		case 0:
			lTypes.push_back(ArgBond0);
			return lTypes;
		case 1:
			lTypes.push_back(ArgBond0);
			return lTypes;
		case 2:
			lTypes.push_back(ArgJct1);
			return lTypes;
		case 3:
			lTypes.push_back(ArgBond1);
			return lTypes;			
		case 4:
			lTypes.push_back(ArgJct0);
			return lTypes;
		case 5:
			lTypes.push_back(ArgBond0);
			return lTypes;
		default:
			throw ValidationException("SplitBond10 should have no more than 6 arguments");
	}
}
