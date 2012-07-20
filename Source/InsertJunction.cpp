/*
 *  InsertJunction.cpp
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

#include "InsertJunction.h"
#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include <beagle/ValidationException.hpp>
#include <beagle/GP/PrimitiveInline.hpp>


//const std::type_info* InsertJunction::getReturnType(GP::Context& ioContext) const {
//	return ArgBond;
//}

std::vector<const std::type_info*> InsertJunction::getReturnTypes(GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgBond);
	lTypes.push_back(ArgBond1);
	lTypes.push_back(ArgBond0);
	return lTypes;
}

void InsertJunction::insert(Beagle::GP::Datum& inBond, BG::Junction::JunctionType inType, Beagle::GP::Context& ioContext) {
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	BondPtr& lInBond = castObjectT<BondPtr&>(inBond);
	GrowingBG::Handle lGrowingBondGraph = lContext.getBondGraph();
	
	BG::Junction* lNewJunction = new BG::Junction(inType);
	BG::Bond* lNewBond = 0;

	lGrowingBondGraph->insertJunction(lInBond.getValue(),lNewJunction,lNewBond);
	
	BondPtr lNewBondPtr(lNewBond);
	JunctionPtr lNewJunctionPtr(lNewJunction);
	
	//Continue parsing the tree
	getArgument(0,lInBond,ioContext);
	getArgument(1,lNewBondPtr,ioContext);
	getArgument(2,lNewJunctionPtr,ioContext);
}


void InsertJ0::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	insert(inDatum, BG::Junction::eZero, ioContext);
}

std::vector<const std::type_info*> InsertJ0::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	switch(inN) {
		case 0:
			lTypes.push_back(ArgBond);
			return lTypes;
		case 1:
			lTypes.push_back(ArgBond);
			return lTypes;			
		case 2:
			lTypes.push_back(ArgJct0);
			return lTypes;
		default:
			throw ValidationException("InsertJ0 should have no more than 3 arguments");
	}
}

void InsertJ1::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	insert(inDatum, BG::Junction::eOne, ioContext);
}

std::vector<const std::type_info*> InsertJ1::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	switch(inN) {
		case 0:
			lTypes.push_back(ArgBond);
			return lTypes;
		case 1:
			lTypes.push_back(ArgBond);
			return lTypes;
		case 2:
			lTypes.push_back(ArgJct1);
			return lTypes;
		default:
			throw ValidationException("InsertJ1 should have no more than 3 arguments");
	}
}
