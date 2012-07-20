/*
 *  .cpp
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
 *  This file was created by Jean-Francois Dupuis on 25/03/10.
 */

#include "DCDCBoostEmbryo.h"

#include "Defines.h"
#include "ArgType.h"
#include "BGContext.h"
#include "GrowingHybridBondGraph.h"
#include "DCDCBoostLookaheadController.h"
#include "ParametersHolder.h"

using namespace BG;

DCDCBoostEmbryo::DCDCBoostEmbryo() : EmbryoPrimitive("DCDCBoostEmbryo") {
#ifdef SINGLE_OUTPUT
	setNumberArguments(3);
#else
	setNumberArguments(4);
#endif
}

const std::type_info* DCDCBoostEmbryo::getArgType(unsigned int inN, GP::Context& ioContext) const {
	//Define three modifable site type
	switch(inN) {
		case 0:
			return ArgJct0;
		case 1:
		case 2:
		case 3:
			return ArgBond0;
		default:
			throw ValidationException("DCDCBoostEmbryo should have no more than 3 arguments");
	}
}

void DCDCBoostEmbryo::execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext) {
	//RootReturn& lReturnID = castObjectT<RootReturn&>(outDatum);
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	GrowingHybridBondGraph::Handle lGrowingBondGraph = new GrowingHybridBondGraph();
	DCDCBoostLookaheadController *lController = new DCDCBoostLookaheadController;
	
	
	//Get the parameters
	Beagle::Component::Handle lHolderComponent = ioContext.getSystem().getComponent("ParametersHolder");
	if(lHolderComponent==NULL)
		throw Beagle_RunTimeExceptionM("No CMA holder component found in the system!");
	ParametersHolder::Handle lHolder = castHandleT<ParametersHolder>(lHolderComponent);
	if(lHolder==NULL)
		throw Beagle_RunTimeExceptionM("Component named \"ParameterHolder\" found is not of the good type!");
	
#ifdef SINGLE_OUTPUT
	//Creating the bond graph embryo
	
	//Create the sources
	std::vector<Source*> lS(1);
	for(unsigned int i = 0; i < lS.size(); ++i) {
		lS[i] = new Source(Source::eEffort);
		lS[i]->setValue(1.5);
		lHolder->push_back(lS[i]);
		lGrowingBondGraph->addComponent(lS[i]);
	}
	
	std::vector<Junction*> lJ0(1);
	for(unsigned int i = 0; i < lJ0.size(); ++i) {
		lJ0[i] = new Junction(Junction::eZero);
		lGrowingBondGraph->addComponent(lJ0[i]);
	}
	
	std::vector<Passive*> lR(1);
	for(unsigned int i = 0; i < lR.size(); ++i) {
		lR[i] = new Passive(Passive::eResistor);
		lHolder->push_back(lR[i]);
		lGrowingBondGraph->addComponent(lR[i]);
	}
	lR[0]->setValue(6);
	
	//Create the bonds
	std::vector<Bond*> lEffortOutputBonds(1);
	std::vector<Bond*> lFlowtOutputBonds(1);
	std::vector<Bond*> lArgBonds(2);
	
	lArgBonds[0] = lGrowingBondGraph->connect(lS[0],lJ0[0]);
	lArgBonds[1] = lGrowingBondGraph->connect(lJ0[0],lR[0]);
	lEffortOutputBonds[0] = lArgBonds[1];
	lFlowtOutputBonds[0] = lArgBonds[0];
#else
	//Creating the bond graph embryo for the double output
	
	//Create the sources
	std::vector<Source*> lS(1);
	for(unsigned int i = 0; i < lS.size(); ++i) {
		lS[i] = new Source(Source::eEffort);
		lS[i]->setValue(1.5);
		lHolder->push_back(lS[i]);
		lGrowingBondGraph->addComponent(lS[i]);
	}
	
	std::vector<Junction*> lJ0(1);
	for(unsigned int i = 0; i < lJ0.size(); ++i) {
		lJ0[i] = new Junction(Junction::eZero);
		lGrowingBondGraph->addComponent(lJ0[i]);
	}
	
	std::vector<Passive*> lR(2);
	for(unsigned int i = 0; i < lR.size(); ++i) {
		lR[i] = new Passive(Passive::eResistor);
		lHolder->push_back(lR[i]);
		lGrowingBondGraph->addComponent(lR[i]);
	}
	lR[0]->setValue(6.25);
	lR[1]->setValue(34.1);
	
	//Create the bonds
	std::vector<Bond*> lEffortOutputBonds(2);
	std::vector<Bond*> lFlowtOutputBonds(1);
	std::vector<Bond*> lArgBonds(3);
	
	lArgBonds[0] = lGrowingBondGraph->connect(lS[0],lJ0[0]);
	lArgBonds[1] = lGrowingBondGraph->connect(lJ0[0],lR[0]);
	lArgBonds[2] = lGrowingBondGraph->connect(lJ0[0],lR[1]);
	lEffortOutputBonds[0] = lArgBonds[1];
	lEffortOutputBonds[1] = lArgBonds[2];
	lFlowtOutputBonds[0] = lArgBonds[0];
	
#endif
	
	lGrowingBondGraph->setOutputBonds(lEffortOutputBonds,lFlowtOutputBonds);
		
 	lGrowingBondGraph->addSwitchController(lController);
	lGrowingBondGraph->postConnectionInitialization();
	
	lContext.setBondGraph(lGrowingBondGraph);
	
	//For each modifable site a subtree can be parsed.
	std::vector<JunctionPtr> lJctPtr(1);
	for(unsigned int i = 0; i < lJctPtr.size(); ++i) {
		lJctPtr[i].setValue(lJ0[i]);
		getArgument(i,lJctPtr[i],ioContext);
	}	
	std::vector<BondPtr> lBondPtr(lArgBonds.size());
	for(unsigned int i = 0; i < lBondPtr.size(); ++i) {
		lBondPtr[i].setValue(lArgBonds[i]);
		getArgument(i+lJctPtr.size(),lBondPtr[i],ioContext);
	}
	
}
