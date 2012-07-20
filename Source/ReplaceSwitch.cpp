/*
 *  ReplaceSwitch.cpp
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

#include "ReplaceSwitch.h"
//#include <beagle/ValidationException.hpp>
//#include <beagle/GP/PrimitiveInline.hpp>
#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include "GrowingHybridBondGraph.h"

const std::type_info* ReplaceSwitch::getArgType(unsigned int inN, GP::Context& ioContext) const {
	switch(inN) {
		case 0:
			return ArgNode;
		default:
			throw ValidationException("AddComponent should have no more than 2 arguments");
	}
}


const std::type_info* ReplaceSwitch::getReturnType(GP::Context& ioContext) const {
	return ArgNode;
}

void ReplaceSwitch::execute(Beagle::GP::Datum& inComponent, Beagle::GP::Context& ioContext) {
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	ComponentPtr& lInComponent = castObjectT<ComponentPtr&>(inComponent);
	GrowingHybridBondGraph::Handle lGrowingBondGraph = castHandleT<GrowingHybridBondGraph>(lContext.getBondGraph());
	lGrowingBondGraph->setCausalityInvalid();
	
	//Create the new component
	BG::Switch *lSw = new BG::Switch;
	
	//Replace component
	lGrowingBondGraph->replaceComponent(lInComponent.getValue(),(BG::Component*&)lSw);
	
	//Continue parsing the tree
	ComponentPtr lNewComponent(lSw);
	getArgument(0,lNewComponent,ioContext);
}
