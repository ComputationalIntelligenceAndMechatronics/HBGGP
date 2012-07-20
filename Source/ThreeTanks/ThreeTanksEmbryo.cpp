/*
 *  ThreeTanksEmbryo.cpp
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

#include "ThreeTanksEmbryo.h"

#include "Defines.h"
#include "ArgType.h"
#include "BGContext.h"
#include "GrowingHybridBondGraph.h"
#include "ThreeTanksLookaheadController.h"

using namespace BG;

//#define WITH_PARAMETER



ThreeTanksEmbryo::ThreeTanksEmbryo() : EmbryoPrimitive("ThreeTanksEmbryo") {
	setNumberArguments(5);
	
	mSwR = 118.7630e3/10;
	mDrainR = 118.7630e3;
	mC = 1.570e-6;
	g = 9.81;
	mFluidDensity = 998.2;
	mF =  1.833e-3*4;
}

const std::type_info* ThreeTanksEmbryo::getArgType(unsigned int inN, GP::Context& ioContext) const {
	//Define three modifable site type
	switch(inN) {
		case 0:
		case 1:
		case 2:
			return ArgJct0;
		case 3:
		case 4:
			return ArgBond0;
		default:
			throw ValidationException("ThreeTanksEmbryo should have no more than 5 arguments");
	}
}

void ThreeTanksEmbryo::execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext) {
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
	
	//Create the bonds
	vector<Bond*> lOutputBonds(3);
	vector<Bond*> lArgBonds(2);
	
	lGrowingBondGraph->connect(lSf[0],lJ0[0]);
	lGrowingBondGraph->connect(lSf[1],lJ0[1]);
	lOutputBonds[0] = lGrowingBondGraph->connect(lJ0[0],lC[0]);
	lOutputBonds[1] = lGrowingBondGraph->connect(lJ0[1],lC[1]);
	lOutputBonds[2] = lGrowingBondGraph->connect(lJ0[2],lC[2]);
	
	lArgBonds[0] = lGrowingBondGraph->connect(lJ0[0],lJ0[1]);
	lArgBonds[1] = lGrowingBondGraph->connect(lJ0[1],lJ0[2]);
	
	//lReturnID.setValue(mR->getId());
	lGrowingBondGraph->setOutputBonds(lOutputBonds,vector<Bond*>(0));
	
 	lGrowingBondGraph->addSwitchController(lController);
	lGrowingBondGraph->postConnectionInitialization();

	lContext.setBondGraph(lGrowingBondGraph);
	
	//For each modifable site a subtree can be parsed.
	vector<JunctionPtr> lJctPtr(3);
	for(unsigned int i = 0; i < lJctPtr.size(); ++i) {
		lJctPtr[i].setValue(lJ0[i]);
		getArgument(i,lJctPtr[i],ioContext);
	}	
	vector<BondPtr> lBondPtr(2);
	for(unsigned int i = 0; i < lBondPtr.size(); ++i) {
		lBondPtr[i].setValue(lArgBonds[i]);
		getArgument(i+lJctPtr.size(),lBondPtr[i],ioContext);
	}
	
}


//
////Argument = 1
//void ThreeTanksEmbryo::execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext) {
//	//RootReturn& lReturnID = castObjectT<RootReturn&>(outDatum);
//	BGContext& lContext = castObjectT<BGContext&>(ioContext);
//	GrowingHybridBondGraph::Handle lGrowingBondGraph = new GrowingHybridBondGraph();
//	ThreeTanksLookaheadController *lController = new ThreeTanksLookaheadController;
//	
//	//Creating the bond graph embryo
//
//	//Create the sources
//	vector<Source*> lSf(2);
//	for(unsigned int i = 0; i < lSf.size(); ++i) {
//		lSf[i] = new Source(Source::eFlow);
//		lSf[i]->setValue(mF);
//		lGrowingBondGraph->addComponent(lSf[i]);
//	}
//	
//	vector<Junction*> lJ0(1);
//	for(unsigned int i = 0; i < lJ0.size(); ++i) {
//		lJ0[i] = new Junction(Junction::eZero);
//		lGrowingBondGraph->addComponent(lJ0[i]);
//	}
//	
//	vector<Passive*> lC(3);
//	for(unsigned int i = 0; i < lC.size(); ++i) {
//		lC[i] = new Passive(Passive::eCapacitor);
//		lC[i]->setValue(mC);
//		lGrowingBondGraph->addComponent(lC[i]);
//	}
//	
//	//Create the bonds
//	vector<Bond*> lOutputBonds(3);
//	
//	lGrowingBondGraph->connect(lSf[0],lJ0[0]);
//	lGrowingBondGraph->connect(lSf[1],lJ0[0]);
//	lOutputBonds[0] = lGrowingBondGraph->connect(lJ0[0],lC[0]);
//	lOutputBonds[1] = lGrowingBondGraph->connect(lJ0[0],lC[1]);
//	lOutputBonds[2] = lGrowingBondGraph->connect(lJ0[0],lC[2]);
//	
//	//lReturnID.setValue(mR->getId());
//	lGrowingBondGraph->setOutputBonds(lOutputBonds,vector<Bond*>(0));
//	
// 	lGrowingBondGraph->addSwitchController(lController);
//	lGrowingBondGraph->postConnectionInitialization();
//	
//	//For each modifable site a subtree can be parsed.
//	JunctionPtr lJunction0Ptr(lJ0[0]);
//	
//	lContext.setBondGraph(lGrowingBondGraph);
//	
//	getArgument(0,lJunction0Ptr,ioContext);
//
//}

//#define NBSWITCH 4
//void ThreeTanksEmbryo::execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext) {
//	//RootReturn& lReturnID = castObjectT<RootReturn&>(outDatum);
//	BGContext& lContext = castObjectT<BGContext&>(ioContext);
//	GrowingHybridBondGraph::Handle lGrowingBondGraph = new GrowingHybridBondGraph();
//	ThreeTanksLookaheadController *lController = new ThreeTanksLookaheadController;
//	
//	
//	//Creating the bond graph embryo
//	double lSwR = 118.7630e3/10;
//	double lDrainR = 118.7630e3;
//	
//	//Create the sources
//	vector<Source*> mSf(2);
//	for(unsigned int i = 0; i < mSf.size(); ++i) {
//		mSf[i] = new Source(Source::eFlow);
//		mSf[i]->setValue(mF);
//		lGrowingBondGraph->addComponent(mSf[i]);
//	}
//	
//	//Create a switch
//	vector<Switch*> lSw(NBSWITCH);
//	for(unsigned int i = 0; i < lSw.size(); ++i) {
//		lSw[i] = new Switch;
//		lController->push_back(lSw[i]);
//	}
//	
//	//Create the jonctions
//	vector<Junction*> lJ0(4);
//	for(unsigned int i = 0; i < lJ0.size(); ++i) {
//		lJ0[i] = new Junction(Junction::eZero);
//		lGrowingBondGraph->addComponent(lJ0[i]);
//	}
//	vector<Junction*> lJ1(4);
//	for(unsigned int i = 0; i < lJ1.size(); ++i) {
//		lJ1[i] = new Junction(Junction::eOne);
//		lGrowingBondGraph->addComponent(lJ1[i]);
//	}
//	
//	//Create component
//	vector<Passive*> lC(3);
//	for(unsigned int i = 0; i < lC.size(); ++i) {
//		lC[i] = new Passive(Passive::eCapacitor);
//		lC[i]->setValue(mC);
//		lGrowingBondGraph->addComponent(lC[i]);
//	}
//	vector<Passive*> lR(4);
//	for(unsigned int i = 0; i < lR.size(); ++i) {
//		lR[i] = new Passive(Passive::eResistor);
//		lR[i]->setValue(lSwR);
//		lGrowingBondGraph->addComponent(lR[i]);
//	}
//	lR[3]->setValue(lDrainR);
//	
//	
//	//Create the bonds
//	vector<Bond*> lOutputBonds(3);
//	lGrowingBondGraph->connect(mSf[0],lJ0[0]);
//	
//	lOutputBonds[0] = lGrowingBondGraph->connect(lJ0[0],lC[0]);
//	lGrowingBondGraph->connect(lJ0[0],lJ1[0]);
//	lGrowingBondGraph->connect(lJ1[0],lSw[0]);
//	lGrowingBondGraph->connect(lJ1[0],lR[0]);
//	
//	lGrowingBondGraph->connect(mSf[1],lJ0[1]);
//	lOutputBonds[1] = lGrowingBondGraph->connect(lJ0[1],lC[1]);
//	lGrowingBondGraph->connect(lJ0[1],lJ1[1]);
//	lGrowingBondGraph->connect(lJ1[1],lSw[1]);
//	lGrowingBondGraph->connect(lJ1[1],lR[1]);
//	
//	lOutputBonds[2] = lGrowingBondGraph->connect(lJ0[2],lC[2]);
//	//	lGrowingBondGraph->connect(lJ0[2],lR[3]);
//	
//	lGrowingBondGraph->connect(lJ1[2],lJ0[2]);
//	lGrowingBondGraph->connect(lJ1[2],lSw[2]);
//	lGrowingBondGraph->connect(lJ1[2],lR[2]);
//	
//	lGrowingBondGraph->connect(lJ1[0],lJ0[3]);
//	lGrowingBondGraph->connect(lJ1[1],lJ0[3]);
//	lGrowingBondGraph->connect(lJ0[3],lJ1[2]);
//	
//	lGrowingBondGraph->connect(lJ0[3],lJ1[3]);
//	lGrowingBondGraph->connect(lJ1[3],lSw[3]);
//	lGrowingBondGraph->connect(lJ1[3],lR[3]);
//	
//
//	lGrowingBondGraph->addSwitchController(lController);
//	lGrowingBondGraph->postConnectionInitialization();
//	
//	//lReturnID.setValue(mR->getId());
//	lGrowingBondGraph->setOutputBonds(lOutputBonds,vector<Bond*>(0));
//	
//	//For each modifable site a subtree can be parsed.
//	lContext.setBondGraph(lGrowingBondGraph);
//	
//
//}
