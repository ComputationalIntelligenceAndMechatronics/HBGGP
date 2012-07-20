/*
 *  AddSwitch.cpp
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
 *  This file was created by Jean-Francois Dupuis on 25/02/10.
 */

#include "AddSwitch.h"

#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include <beagle/ValidationException.hpp>
#include <beagle/GP/PrimitiveInline.hpp>
#include "GrowingHybridBondGraph.h"

std::vector<const std::type_info*> AddSwitch::getArgTypes(unsigned int inN, GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	switch(inN) {
		case 0:
			//lTypes.push_back(ArgJct1);
			return getParentArgumentTypes(ioContext);
			//return lTypes;
		case 1:
			lTypes.push_back(ArgBond);
			return lTypes;
		case 2:
			lTypes.push_back(ArgNode);
			return lTypes;
		default:
			throw ValidationException("AddSwitch should have no more than 1 arguments");
	}
}

std::vector<const std::type_info*> AddSwitch::getReturnTypes(GP::Context& ioContext) const {
	std::vector<const std::type_info*> lTypes;
	lTypes.push_back(ArgJct);
	lTypes.push_back(ArgJct0);
	lTypes.push_back(ArgJct1);
	return lTypes;
}

void AddSwitch::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	//Create the new component
	BG::Switch *lSw = new BG::Switch;
	
	
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	JunctionPtr& lInJunction = castObjectT<JunctionPtr&>(inDatum);
	GrowingHybridBondGraph::Handle lGrowingBondGraph = castHandleT<GrowingHybridBondGraph>(lContext.getBondGraph());
	
	//Insert R
	BG::Bond* lNewBond = 0;
#ifdef INSERT_RESISTANCE_WITH_SWITCH
	BG::Passive *lR = new BG::Passive(BG::Passive::eResistor);
	lGrowingBondGraph->insertComponent(lInJunction.getValue(),lR,lNewBond);
#endif
	//Insert switch
	lGrowingBondGraph->insertSwitch(lInJunction.getValue(),lSw,lNewBond);
	
	BondPtr lNewBondPtr(lNewBond);
	ComponentPtr lNewComponent(lSw);
	
	//Continue parsing the tree
	getArgument(0,lInJunction,ioContext);
	getArgument(1,lNewBondPtr,ioContext);
	getArgument(2,lNewComponent,ioContext);
	
#ifdef INSERT_RESISTANCE_WITH_SWITCH
	//Adjust R value factor
	lR->setValue(1e-4);
#endif
}
