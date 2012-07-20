/*
 *  RemoveComponent.cpp
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

#include "RemoveComponent.h"
//#include <beagle/ValidationException.hpp>
//#include <beagle/GP/PrimitiveInline.hpp>
#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include "GrowingHybridBondGraph.h"


const std::type_info* RemoveComponent::getReturnType(GP::Context& ioContext) const {
	return ArgNode;
}

void RemoveComponent::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	ComponentPtr& lInComponent = castObjectT<ComponentPtr&>(inDatum);
	GrowingHybridBondGraph::Handle lGrowingBondGraph = castHandleT<GrowingHybridBondGraph>(lContext.getBondGraph());
	lGrowingBondGraph->setCausalityInvalid();
	
	//Insert switch
	lGrowingBondGraph->removeComponent(lInComponent.getValue());

}
