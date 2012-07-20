/*
 *  InsertJunction.h
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

#ifndef InsertJunction_H
#define InsertJunction_H

#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include "BGPrimitive.h"


//using namespace Beagle;

class InsertJunction : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<InsertJunction,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<InsertJunction,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<InsertJunction,BGPrimitive::Bag> Bag;
	
	InsertJunction(std::string inName) : BGPrimitive(3, inName) {}
	~InsertJunction() {}

	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	//virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
	
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) = 0;
	
	void insert(Beagle::GP::Datum& inBond, BG::Junction::JunctionType inType, Beagle::GP::Context& ioContext);
};

class InsertJ0 : public InsertJunction {
public:
	typedef Beagle::AllocatorT<InsertJ0,InsertJunction::Alloc> Alloc;
	typedef Beagle::PointerT<InsertJ0,InsertJunction::Handle> Handle;
	typedef Beagle::ContainerT<InsertJ0,InsertJunction::Bag> Bag;
	
	InsertJ0(std::string inName="InsertJ0") : InsertJunction(inName) {}
	~InsertJ0() {}
	
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

class InsertJ1 : public InsertJunction {
public:
	typedef Beagle::AllocatorT<InsertJ1,InsertJunction::Alloc> Alloc;
	typedef Beagle::PointerT<InsertJ1,InsertJunction::Handle> Handle;
	typedef Beagle::ContainerT<InsertJ1,InsertJunction::Bag> Bag;
	
	InsertJ1(std::string inName="InsertJ1") : InsertJunction(inName) {}
	~InsertJ1() {}
	
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};


#endif
