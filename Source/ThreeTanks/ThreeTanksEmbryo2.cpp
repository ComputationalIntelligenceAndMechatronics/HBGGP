/*
 *  ThreeTanksEmbryo2.cpp
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
 *  This file was created by Jean-Francois Dupuis on 22/02/10.
 */

#include "ThreeTanksEmbryo2.h"

#include "Defines.h"
#include "ArgType.h"
#include "BGContext.h"
#include "GrowingHybridBondGraph.h"
#include "ThreeTanksLookaheadController.h"

using namespace BG;

//#define WITH_PARAMETER



ThreeTanksEmbryo2::ThreeTanksEmbryo2() : EmbryoPrimitive("ThreeTanksEmbryo2") {
	setNumberArguments(7);
	
	mSwR = 118.7630e3/10;
	mDrainR = 118.7630e3;
	mC = 1.570e-6;
	g = 9.81;
	mFluidDensity = 998.2;
	mF =  1.833e-3*4;
	mR = 118.7630e2;
}

const std::type_info* ThreeTanksEmbryo2::getArgType(unsigned int inN, GP::Context& ioContext) const {
	//Define three modifable site type
	switch(inN) {
		case 0:
		case 1:
		case 2:
			return ArgJct0;
		case 3:
		case 4:
			return ArgJct1;
		case 5:
		case 6:
			return ArgBond1;
		default:
			throw ValidationException("ThreeTanksEmbryo2 should have no more than 5 arguments");
	}
}

void ThreeTanksEmbryo2::execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext) {
	//RootReturn& lReturnID = castObjectT<RootReturn&>(outDatum);
	BGContext& lContext = castObjectT<BGContext&>(ioContext);
	GrowingHybridBondGraph::Handle lGrowingBondGraph = new GrowingHybridBondGraph();
	ThreeTanksLookaheadController *lController = new ThreeTanksLookaheadController;
	
	//Creating the bond graph embryo
	
	//Create the sources
	vector<Source*> lSf(2);
	for(unsigned int i = 0; i < lSf.size(); ++i) {
		lSf[i] = new Source(Source::eFlow);
		lSf[i]->setValue(mF);
		lGrowingBondGraph->addComponent(lSf[i]);
	}
	
	vector<Junction*> lJ0(3);
	for(unsigned int i = 0; i < lJ0.size(); ++i) {
		lJ0[i] = new Junction(Junction::eZero);
		lGrowingBondGraph->addComponent(lJ0[i]);
	}
	
	vector<Passive*> lC(3);
	for(unsigned int i = 0; i < lC.size(); ++i) {
		lC[i] = new Passive(Passive::eCapacitor);
		lC[i]->setValue(mC);
		lGrowingBondGraph->addComponent(lC[i]);
	}
	
	vector<Junction*> lJ1(2);
	for(unsigned int i = 0; i < lJ1.size(); ++i) {
		lJ1[i] = new Junction(Junction::eOne);
		lGrowingBondGraph->addComponent(lJ1[i]);
	}
	
	vector<Passive*> lR(2);
	for(unsigned int i = 0; i < lR.size(); ++i) {
		lR[i] = new Passive(Passive::eResistor);
		lR[i]->setValue(mR);
		lGrowingBondGraph->addComponent(lR[i]);
	}
	
	//Create the bonds
	vector<Bond*> lOutputBonds(3);
	vector<Bond*> lArgBonds(2);
	
	lGrowingBondGraph->connect(lSf[0],lJ0[0]);
	lGrowingBondGraph->connect(lSf[1],lJ0[1]);
	
	lGrowingBondGraph->connect(lJ1[0],lR[0]);
	lGrowingBondGraph->connect(lJ1[1],lR[1]);
	
	lOutputBonds[0] = lGrowingBondGraph->connect(lJ0[0],lC[0]);
	lOutputBonds[1] = lGrowingBondGraph->connect(lJ0[1],lC[1]);
	lOutputBonds[2] = lGrowingBondGraph->connect(lJ0[2],lC[2]);
	
	lGrowingBondGraph->connect(lJ0[0],lJ1[0]);
	lArgBonds[0] = lGrowingBondGraph->connect(lJ1[0],lJ0[1]);
	lGrowingBondGraph->connect(lJ0[1],lJ1[1]);
	lArgBonds[1] = lGrowingBondGraph->connect(lJ1[1],lJ0[2]);
	
		
	//lReturnID.setValue(mR->getId());
	lGrowingBondGraph->setOutputBonds(lOutputBonds,vector<Bond*>(0));
	
 	lGrowingBondGraph->addSwitchController(lController);
	lGrowingBondGraph->postConnectionInitialization();

	lContext.setBondGraph(lGrowingBondGraph);
	
	//For each modifable site a subtree can be parsed.
	vector<JunctionPtr> lJctPtr0(lJ0.size());
	for(unsigned int i = 0; i < lJctPtr0.size(); ++i) {
		lJctPtr0[i].setValue(lJ0[i]);
		getArgument(i,lJctPtr0[i],ioContext);
	}
	vector<JunctionPtr> lJctPtr1(lJ1.size());
	for(unsigned int i = 0; i < lJctPtr1.size(); ++i) {
		lJctPtr1[i].setValue(lJ1[i]);
		getArgument(i+lJctPtr0.size(),lJctPtr1[i],ioContext);
	}
	vector<BondPtr> lBondPtr(lArgBonds.size());
	for(unsigned int i = 0; i < lBondPtr.size(); ++i) {
		lBondPtr[i].setValue(lArgBonds[i]);
		getArgument(i+lJctPtr0.size()+lJctPtr1.size(),lBondPtr[i],ioContext);
	}
	
}
