/*
 *  AnalogFilterEmbryo.cpp
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
 *  This file was created by Jean-Francois Dupuis on 14/05/09.
 */

#include "AnalogFilterEmbryo.h"
#include "Defines.h"
#include "ArgType.h"
#include "BGContext.h"
#include "GrowingHybridBondGraph.h"

using namespace BG;

//#define WITH_PARAMETER

AnalogFilterEmbryo::AnalogFilterEmbryo() {
#ifdef WITH_PARAMETER
	setNumberArguments(5);
#else
	setNumberArguments(3);
#endif
}

const std::type_info* AnalogFilterEmbryo::getArgType(unsigned int inN, GP::Context& ioContext) const {
	//Define three modifable site type
	switch(inN) {
		case 0:
			return ArgJct;
		case 1:
			return ArgBond;
		case 2:
			return ArgJct;
#ifdef WITH_PARAMETER
		case 3:
		case 4:
			return ArgEph;
		default:
			throw ValidationException("AddComponent should have no more than 5 arguments");
#else
		default:
			throw ValidationException("AddComponent should have no more than 3 arguments");
#endif
	}
}


void AnalogFilterEmbryo::execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext) {
	RootReturn& lReturnID = castObjectT<RootReturn&>(outDatum);
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	GrowingHybridBondGraph::Handle lGrowingBondGraph = new GrowingHybridBondGraph();
	
	//Creating the bond graph embryo
	Source *lSe = new Source(Source::eEffort);
	lSe->setValue(1);
	lGrowingBondGraph->addComponent(lSe);
	
	Junction *lJ1_1 = new Junction(Junction::eOne);
	lGrowingBondGraph->addComponent(lJ1_1);
	
	Junction *lJ0_1 = new Junction(Junction::eZero);
	lGrowingBondGraph->addComponent(lJ0_1);
	
	//Embryo with R parameter part of the evolving process
	
#ifdef WITH_PARAMETER	
	//Get the component parameter value
	Beagle::Double lValue;
	getArgument(3,lValue,ioContext);
	//Check if the return value is zero, if so set it to a very small value;
	if(lValue == 0.0) lValue = ZEROVALUE;
	
	Beagle::Double lValue2;
	getArgument(4,lValue2,ioContext);
	//Check if the return value is zero, if so set it to a very small value;
	if(lValue == 0.0) lValue = ZEROVALUE;
#else
	//Get the component parameter value
	Beagle::Double lValue = 1000;
	Beagle::Double lValue2 = 1000;
#endif
	
	Passive *lR1 = new Passive(Passive::eResistor);
    lR1->setValue(lValue*RFactor);
	lGrowingBondGraph->addComponent(lR1);

	Passive *lR = new Passive(Passive::eResistor);
	lR->setValue(lValue*RFactor);
	lGrowingBondGraph->addComponent(lR);
	
	//Create the bonds
	lGrowingBondGraph->connect(lSe,lJ1_1);
	lGrowingBondGraph->connect(lJ1_1,lR1); 
	Bond* lBond = lGrowingBondGraph->connect(lJ1_1,lJ0_1);
	lGrowingBondGraph->connect(lJ0_1,lR);
	
	lReturnID.setValue(lR->getId());
	
	//For each modifable site a subtree can be parsed.
	JunctionPtr lJunction1Ptr(lJ1_1);
	JunctionPtr lJunction0Ptr(lJ0_1);
	BondPtr lBondPtr(lBond);
	
	lContext.setBondGraph(lGrowingBondGraph);
	
	getArgument(0,lJunction1Ptr,ioContext);
	getArgument(1,lBondPtr,ioContext);
	getArgument(2,lJunction0Ptr,ioContext);
}

