/*
 *  GrowingBondGraph.cpp
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

#include "GrowingBondGraph.h"
#include "Defines.h"

using namespace BG;

GrowingBondGraph::GrowingBondGraph() : GrowingBG(), BG::BondGraph() { 
//	mValidCausality = false; 
}

GrowingBondGraph& GrowingBondGraph::operator=(const BG::BondGraph& inBondGraph) {
	Beagle_StackTraceBeginM();
	BG::BondGraph::operator=(inBondGraph);
	mValidCausality = false;
	return *this;
	Beagle_StackTraceEndM("GrowingBondGraph& GrowingBondGraph::operator=(const BG::BondGraph& inBondGraph)");
}

void GrowingBondGraph::clearCausality() {
	BondGraph::clearCausality();
	mValidCausality = false;
}

void GrowingBondGraph::assignCausality() {
	BondGraph::assignCausality();
	mValidCausality = true;
}

/*! \brief Insert a junction
 *  Insert a junction on a existing bond. If there is a non-jonction component attached to
 *	the bond, the existing bond will be attached to the non-junction component. If the bond
 *	is between two junctions, the new junction will be attached at the from port side of the existing
 *	bond.
 *  \param  inBond Insertion point.
 *  \param  inJunction New junction to be added.
 *  \param  outBond New bond created by the insertion.
 */
void GrowingBondGraph::insertJunction(BG::Bond* inBond, BG::Junction* inJunction, BG::Bond*& outBond) {
	//Add the new junction
	addComponent(inJunction);
	
	//Create a new bond
	outBond = new Bond();
	addBond(outBond);
	
	//Create new ports
	Port *lNewFromPort  = new Port(false);
	Port *lNewToPort = new Port(true);
	
	//Get existing ports
	Port *lFromPort  = inBond->getFromPort();
	Port *lToPort = inBond->getToPort();
	
	//Assign causality to the new port 
	lNewFromPort->setCausality(lFromPort->getCausality());
	lNewToPort->setCausality(lToPort->getCausality());
	
	//Reconnect component by keeping the original bond with the non-junction component
	if(lToPort->getComponent()->getComponentGroup() == BG::Component::eJ) {
		Component* lComponent = lToPort->getComponent();
		lComponent->disconnect(lToPort);
		
		//Connect the elements together
		inJunction->connect(lToPort);
		inJunction->connect(lNewFromPort);
		outBond->connect(lNewFromPort,lNewToPort);
		lComponent->connect(lNewToPort);
	} else {
		Component* lComponent = lFromPort->getComponent();
		lComponent->disconnect(lFromPort);
		
		//Connect the elements together
		inJunction->connect(lFromPort);
		inJunction->connect(lNewToPort);
		outBond->connect(lNewFromPort,lNewToPort);
		lComponent->connect(lNewFromPort);
	}
}

void GrowingBondGraph::insertComponent(BG::Junction* inJunction, BG::Component* inComponent, BG::Bond*& outBond) {
	addComponent(inComponent);
	outBond = connect(inJunction,inComponent);
	mValidCausality = false;
}

void GrowingBondGraph::extractParameters(Beagle::GA::FloatVector& outParameters) const {
	outParameters.resize(0);
	for(unsigned int i = 0; i < mComponents.size(); ++i) {
		if(mComponents[i] != 0) {
			if(mComponents[i]->getElementType() == BG::BondGraphElement::ePassive) {
				BG::Passive* lPassive = dynamic_cast<BG::Passive*>(mComponents[i]);
				switch(lPassive->getType()) {
					case BG::Passive::eResistor:
						outParameters.push_back(lPassive->getValue()/RFactor);
						break;
					case BG::Passive::eCapacitor:
						outParameters.push_back(lPassive->getValue()/CFactor);
						break;
					case BG::Passive::eInductor:
						outParameters.push_back(lPassive->getValue()/IFactor);
						break;
					default:
						break;
				}				
			}
		}
	}	
}

void GrowingBondGraph::assignParameters(const Beagle::GA::FloatVector& outParameters) {
	unsigned int lParamCounter = 0;
	for(unsigned int i = 0; i < mComponents.size(); ++i) {
		if(mComponents[i] != 0) {
			if(mComponents[i]->getElementType() == BG::BondGraphElement::ePassive) {
				BG::Passive* lPassive = dynamic_cast<BG::Passive*>(mComponents[i]);
				switch(lPassive->getType()) {
					case BG::Passive::eResistor:
						lPassive->setValue(outParameters[lParamCounter++]*RFactor);
						break;
					case BG::Passive::eCapacitor:
						lPassive->setValue(outParameters[lParamCounter++]*CFactor);
						break;
					case BG::Passive::eInductor:
						lPassive->setValue(outParameters[lParamCounter++]*IFactor);
						break;
					default:
						break;
				}				
			}
		}
	}	
}

