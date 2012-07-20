/*
 *  ReplaceComponent.cpp
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

#include "ReplaceComponent.h"
//#include <beagle/ValidationException.hpp>
//#include <beagle/GP/PrimitiveInline.hpp>
#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include "GrowingHybridBondGraph.h"

const std::type_info* ReplaceComponent::getArgType(unsigned int inN, GP::Context& ioContext) const {
	switch(inN) {
		case 0:
			return ArgEph;
		case 1:
			return ArgNode;
		default:
			throw ValidationException("AddComponent should have no more than 2 arguments");
	}
}


const std::type_info* ReplaceComponent::getReturnType(GP::Context& ioContext) const {
	return ArgNode;
}

//void ReplaceComponent::replace(Beagle::GP::Datum& inComponent, BG::Passive::PassiveType inNewType, Beagle::GP::Context& ioContext, double inFactor) {
//	BGContext& lContext = castObjectT<BGContext&>(ioContext);
//	ComponentPtr& lInComponent = castObjectT<ComponentPtr&>(inComponent);
//	GrowingHybridBondGraph::Handle lGrowingBondGraph = castHandleT<GrowingHybridBondGraph>(lContext.getBondGraph());
//	lGrowingBondGraph->setCausalityInvalid();
//	
//		
//	//Get the component parameter value
//	Beagle::Double lValue;
//	getArgument(0,lValue,ioContext);
//	//Check if the return value is zero, if so set it to a very small value;
//	if(lValue == 0.0) lValue = ZEROVALUE; 
//	lValue = fabs(lValue)*inFactor);
//	
//	if( typeid(*(lInComponent.getValue())).name() == typeid(BG::Passive).name() ) {
//		//Just change the component type
//		BG::Passive* lPassive = dynamic_cast<BG::Passive*> (lInComponent.getValue());
//		lPassive->setType(inNewType);
//	} else {
//		BG::Passive* lPassive = new BG::Passive(inNewType,lValue);
//		lGrowingBondGraph->replaceComponent(lInComponent.getValue(),lPassive);
//	}
//	
//	//Continue parsing the tree
//	getArgument(1,lInComponent,ioContext);
//}

void ReplaceComponent::replace(Beagle::GP::Datum& inComponent, BG::Passive::PassiveType inNewType, Beagle::GP::Context& ioContext, double inFactor) {
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	ComponentPtr& lInComponent = castObjectT<ComponentPtr&>(inComponent);
	GrowingHybridBondGraph::Handle lGrowingBondGraph = castHandleT<GrowingHybridBondGraph>(lContext.getBondGraph());
	lGrowingBondGraph->setCausalityInvalid();
	
	Beagle::Double lValue;
	getArgument(0,lValue,ioContext);
	//Check if the return value is zero, if so set it to a very small value;
	if(lValue == 0.0) lValue = ZEROVALUE; 
	lValue = fabs(lValue)*inFactor;
	
	//Create the new component
	BG::Passive* lPassive = new BG::Passive(inNewType,lValue);
	
	//Replace component
	lGrowingBondGraph->replaceComponent(lInComponent.getValue(),(BG::Component*&)lPassive);

	//Continue parsing the tree
	ComponentPtr lNewComponent(lPassive);
	getArgument(1,lNewComponent,ioContext);
}

void ReplaceR::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	replace(inDatum, BG::Passive::eResistor, ioContext, RFactor);
}

void ReplaceC::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	replace(inDatum, BG::Passive::eCapacitor, ioContext, CFactor);
}

void ReplaceI::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	replace(inDatum, BG::Passive::eInductor, ioContext, IFactor);
}

