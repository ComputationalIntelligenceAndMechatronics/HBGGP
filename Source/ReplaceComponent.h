/*
 *  ReplaceComponent.h
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

#ifndef ReplaceComponent_H
#define ReplaceComponent_H

#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include "BGPrimitive.h"

using namespace Beagle;

class ReplaceComponent : public BGPrimitive {
public:
	typedef AllocatorT<ReplaceComponent,BGPrimitive::Alloc> Alloc;
	typedef PointerT<ReplaceComponent,BGPrimitive::Handle> Handle;
	typedef ContainerT<ReplaceComponent,BGPrimitive::Bag>	Bag;
	
	ReplaceComponent(std::string inName) : BGPrimitive(2, inName) {}
	~ReplaceComponent() {}
	
	
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
	
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) = 0;
	void replace(Beagle::GP::Datum& inComponent, BG::Passive::PassiveType inNewType, Beagle::GP::Context& ioContext, double inFactor);
};

class ReplaceR : public ReplaceComponent {
public:
	typedef AllocatorT<ReplaceR,ReplaceComponent::Alloc> Alloc;
	typedef PointerT<ReplaceR,ReplaceComponent::Handle> Handle;
	typedef ContainerT<ReplaceR,ReplaceComponent::Bag> Bag;
	
	ReplaceR() : ReplaceComponent("ReplaceR") {}
	~ReplaceR() {}
	
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

class ReplaceC : public ReplaceComponent {
public:
	typedef AllocatorT<ReplaceC,ReplaceComponent::Alloc> Alloc;
	typedef PointerT<ReplaceC,ReplaceComponent::Handle> Handle;
	typedef ContainerT<ReplaceC,ReplaceComponent::Bag> Bag;
	
	ReplaceC() : ReplaceComponent("ReplaceC") {}
	~ReplaceC() {}
	
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

class ReplaceI : public ReplaceComponent {
public:
	typedef AllocatorT<ReplaceI,ReplaceComponent::Alloc> Alloc;
	typedef PointerT<ReplaceI,ReplaceComponent::Handle> Handle;
	typedef ContainerT<ReplaceI,ReplaceComponent::Bag> Bag;
	
	ReplaceI() : ReplaceComponent("ReplaceI") {}
	~ReplaceI() {}
	
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};


#endif
