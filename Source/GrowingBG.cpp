/*
 *  GrowingBG.cpp
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
 *  This file was created by Jean-Francois Dupuis on 10/02/10.
 */

#include "GrowingBG.h"

#include "Defines.h"

using namespace BG;

GrowingBG::GrowingBG() : Beagle::Object() { 
	mValidCausality = false; 
}

//void GrowingBG::clearCausality() {
//	this->BondGraph::clearCausality();
//	mValidCausality = false;
//}
//
//void GrowingBG::assignCausality() {
//	BondGraph::assignCausality();
//	mValidCausality = true;
//}
//
//void GrowingBG::insertJunction(BG::Bond* inBond, BG::Junction* inJunction, BG::Bond*& lOutBond) {
//	//Add the new junction
//	addComponent(inJunction);
//	
//	//Create a new bond
//	lOutBond = new Bond();
//	addBond(lOutBond);
//	
//	//Create new ports
//	Port *lNewFromPort  = new Port(false);
//	Port *lNewToPort = new Port(true);
//	
//	//Get existing ports
//	Port *lFromPort  = inBond->getFromPort();
//	Port *lToPort = inBond->getToPort();
//	
//	//Get the destination component of the original bond
//	Component* lComponent = lToPort->getComponent();
//	lComponent->disconnect(lToPort);
//	
//	//Assign causality to the new port 
//	lFromPort->setCausality(lFromPort->getCausality());
//	lToPort->setCausality(lToPort->getCausality());
//	
//	//Connect the elements together
//	inJunction->connect(lToPort);
//	inJunction->connect(lNewFromPort);
//	lOutBond->connect(lNewFromPort,lNewToPort);
//	lComponent->connect(lNewToPort);
//	
//	
//	//	ostringstream lStream;
//	//	lStream << "Bondgraph_" << mComponents.size() << ".svg";
//	//	plotGraph(lStream.str());
//}
//
//void GrowingBG::insertComponent(BG::Junction* inJunction, BG::Component* inComponent, BG::Bond*& lOutBond) {
//	addComponent(inComponent);
//	lOutBond = connect(inJunction,inComponent);
//	mValidCausality = false;
//	
//	//	ostringstream lStream;
//	//	lStream << "Bondgraph_" << mComponents.size() << ".svg";
//	//	plotGraph(lStream.str());
//}
//
//void GrowingBG::replaceComponent(BG::Component* inOldComponent, BG::Component*& inNewComponent) {
//	//Connect the new component to the same ports
//	vector<Port*> lPorts = inOldComponent->getPorts();
//	for(unsigned int i = 0; i < lPorts.size(); ++i) {
//		inNewComponent->connect(lPorts[i]);
//	}
//	
//	//Delete the OldComponent
//	delete inOldComponent;
//	inOldComponent = 0;
//	
//	mValidCausality = false;
//}
//
//void GrowingBG::extractParameters(Beagle::GA::FloatVector& outParameters) const {
//	outParameters.resize(0);
//	for(unsigned int i = 0; i < mComponents.size(); ++i) {
//		if(mComponents[i] != 0) {
//			if(mComponents[i]->getElementType() == BG::BondGraphElement::ePassive) {
//				BG::Passive* lPassive = dynamic_cast<BG::Passive*>(mComponents[i]);
//				switch(lPassive->getType()) {
//					case BG::Passive::eResistor:
//						outParameters.push_back(lPassive->getValue()/RFactor);
//						break;
//					case BG::Passive::eCapacitor:
//						outParameters.push_back(lPassive->getValue()/CFactor);
//						break;
//					case BG::Passive::eInductor:
//						outParameters.push_back(lPassive->getValue()/IFactor);
//						break;
//					default:
//						break;
//				}				
//			}
//		}
//	}	
//}
//
//void GrowingBG::assignParameters(const Beagle::GA::FloatVector& outParameters) {
//	unsigned int lParamCounter = 0;
//	for(unsigned int i = 0; i < mComponents.size(); ++i) {
//		if(mComponents[i] != 0) {
//			if(mComponents[i]->getElementType() == BG::BondGraphElement::ePassive) {
//				BG::Passive* lPassive = dynamic_cast<BG::Passive*>(mComponents[i]);
//				switch(lPassive->getType()) {
//					case BG::Passive::eResistor:
//						lPassive->setValue(outParameters[lParamCounter++]*RFactor);
//						break;
//					case BG::Passive::eCapacitor:
//						lPassive->setValue(outParameters[lParamCounter++]*CFactor);
//						break;
//					case BG::Passive::eInductor:
//						lPassive->setValue(outParameters[lParamCounter++]*IFactor);
//						break;
//					default:
//						break;
//				}				
//			}
//		}
//	}	
//}
