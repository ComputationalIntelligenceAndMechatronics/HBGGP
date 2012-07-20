/*
 *  FlipBond.cpp
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

#include "FlipBond.h"
#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include <beagle/ValidationException.hpp>
#include <beagle/GP/PrimitiveInline.hpp>

void FlipBond::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	BondPtr& lInBond = castObjectT<BondPtr&>(inDatum);
	GrowingBG::Handle lGrowingBondGraph = lContext.getBondGraph();
	
	//Get existing ports
	BG::Port *lFromPort  = lInBond.getValue()->getFromPort();
	BG::Port *lToPort = lInBond.getValue()->getToPort();
	
	lFromPort->setPowerStroke(!lFromPort->isPowerSide());
	lToPort->setPowerStroke(!lToPort->isPowerSide());
	
	getArgument(0,lInBond,ioContext);
}

std::vector<const std::type_info*> FlipBond::getReturnTypes(GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgBond);
	lTypes.push_back(ArgBond0);
	lTypes.push_back(ArgBond1);
	return lTypes;
}

std::vector<const std::type_info*> FlipBond::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgBond);
	return lTypes;
}
