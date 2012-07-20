/*
 *  AddComponent.h
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

#ifndef AddComponent_H
#define AddComponent_H

#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include <beagle/GP/Primitive.hpp>
#include "ArgType.h"
#include "BGPrimitive.h"

class AddComponent : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<AddComponent,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<AddComponent,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<AddComponent,BGPrimitive::Bag>	Bag;
	
#ifndef WITHOUT_REPLACE
	AddComponent(std::string inName) : BGPrimitive(4, inName) {}
#else
	AddComponent(std::string inName) : BGPrimitive(3, inName) {}
#endif

	~AddComponent() {}

	//std::vector<const std::type_info*> getParentArgumentTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) = 0;
	void add(Beagle::GP::Datum& inJunction, BG::Component* inComponent, Beagle::GP::Context& ioContext, double inFactor);
};

class AddR : public AddComponent {
public:
	typedef Beagle::AllocatorT<AddR,AddComponent::Alloc> Alloc;
	typedef Beagle::PointerT<AddR,AddComponent::Handle> Handle;
	typedef Beagle::ContainerT<AddR,AddComponent::Bag> Bag;
	
	AddR() : AddComponent("AddR") {}
	~AddR() {}
	
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

class AddC : public AddComponent {
public:
	typedef Beagle::AllocatorT<AddC,AddComponent::Alloc> Alloc;
	typedef Beagle::PointerT<AddC,AddComponent::Handle> Handle;
	typedef Beagle::ContainerT<AddC,AddComponent::Bag> Bag;
	
	AddC() : AddComponent("AddC") {}
	~AddC() {}
	
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

class AddI : public AddComponent {
public:
	typedef Beagle::AllocatorT<AddI,AddComponent::Alloc> Alloc;
	typedef Beagle::PointerT<AddI,AddComponent::Handle> Handle;
	typedef Beagle::ContainerT<AddI,AddComponent::Bag> Bag;
	
	AddI() : AddComponent("AddI") {}
	~AddI() {}
	
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};


#endif
