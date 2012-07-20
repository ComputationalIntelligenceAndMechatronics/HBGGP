/*
 *  AdSwitchE.cpp
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
 *  This file was created by Jean-Francois Dupuis on 06/04/10.
 */

#include "AddSwitchE.h"

#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include <beagle/ValidationException.hpp>
#include <beagle/GP/PrimitiveInline.hpp>
#include "GrowingHybridBondGraph.h"
#include "SwitchME.h"


void AddSwitchE::insert(Beagle::GP::Datum& inBond, BG::Junction::JunctionType inType1, BG::Junction::JunctionType inType2, Beagle::GP::Context& ioContext) {
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	BondPtr& lInBond = castObjectT<BondPtr&>(inBond);
	GrowingHybridBondGraph::Handle lGrowingBondGraph = castHandleT<GrowingHybridBondGraph>(lContext.getBondGraph());
	
	
	BG::Junction* lNewJunction1 = new BG::Junction(inType1);
	BG::Junction* lNewJunction2 = new BG::Junction(inType2);
	
	BG::Bond* lNewBond1 = 0;
	BG::Bond* lNewBond2 = 0;
	
	lGrowingBondGraph->insertJunction(lInBond.getValue(),lNewJunction1,lNewBond1);
	lGrowingBondGraph->insertJunction(lNewBond1,lNewJunction2,lNewBond2);
	
	BondPtr lNewBondPtr1(lNewBond1);
	BondPtr lNewBondPtr2(lNewBond2);
	JunctionPtr lNewJunctionPtr1(lNewJunction1);
	JunctionPtr lNewJunctionPtr2(lNewJunction2);
	
	BG::Switch *lSw1 = new BG::Switch;
	BG::SwitchME *lSw2 = new BG::SwitchME(lSw1);
	
	BG::Bond* lNewBond = 0;
	lGrowingBondGraph->insertComponent(lNewJunction1,lSw1,lNewBond);
	lGrowingBondGraph->insertSwitch(lNewJunction2,lSw2,lNewBond);

	//Continue parsing the tree
	getArgument(0,lInBond,ioContext);
	getArgument(1,lNewJunctionPtr1,ioContext);
	getArgument(2,lNewBondPtr1,ioContext);
	getArgument(3,lNewJunctionPtr2,ioContext);
	getArgument(4,lNewBondPtr2,ioContext);
}

void AddSwitchE01::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	insert(inDatum, BG::Junction::eZero, BG::Junction::eOne, ioContext);
}

std::vector<const std::type_info*> AddSwitchE01::getReturnTypes(GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgBond);
	lTypes.push_back(ArgBond1);
	return lTypes;
}

std::vector<const std::type_info*> AddSwitchE01::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	switch(inN) {
		case 0:
			lTypes.push_back(ArgBond1);
			return lTypes;
		case 1:
			lTypes.push_back(ArgJct0);
			return lTypes;
		case 2:
			lTypes.push_back(ArgBond0);
			return lTypes;			
		case 3:
			lTypes.push_back(ArgJct1);
			return lTypes;
		case 4:
			lTypes.push_back(ArgBond1);
			return lTypes;
		default:
			throw ValidationException("InsertJ01 should have no more than 5 arguments");
	}
}

void AddSwitchE10::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	insert(inDatum, BG::Junction::eOne, BG::Junction::eZero, ioContext);
}

std::vector<const std::type_info*> AddSwitchE10::getReturnTypes(GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgBond);
	lTypes.push_back(ArgBond0);
	return lTypes;
}

std::vector<const std::type_info*> AddSwitchE10::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	switch(inN) {
		case 0:
			lTypes.push_back(ArgBond0);
			return lTypes;
		case 1:
			lTypes.push_back(ArgJct1);
			return lTypes;
		case 2:
			lTypes.push_back(ArgBond1);
			return lTypes;			
		case 3:
			lTypes.push_back(ArgJct0);
			return lTypes;
		case 4:
			lTypes.push_back(ArgBond0);
			return lTypes;
		default:
			throw ValidationException("InsertJ10 should have no more than 5 arguments");
	}
}
