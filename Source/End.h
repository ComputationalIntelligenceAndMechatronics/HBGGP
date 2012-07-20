/*
 *  End.h
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

#ifndef End_H
#define End_H

#include <BondGraph.h>
#include <beagle/PointerT.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/ContainerT.hpp>
#include "BGPrimitive.h"
#include "Defines.h"
#include "ArgType.h"


class EndBond : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<EndBond,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<EndBond,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<EndBond,BGPrimitive::Bag>	Bag;
	
	EndBond() : BGPrimitive(0, "EndBond") {}
	~EndBond() {}

	virtual std::vector<const std::type_info*> getReturnTypes(GP::Context& ioContext) const {
		std::vector<const std::type_info*> lTypes; lTypes.push_back(ArgBond); lTypes.push_back(ArgBond0); lTypes.push_back(ArgBond1);
		return lTypes;	
	}
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {}
};

class EndNode : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<EndNode,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<EndNode,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<EndNode,BGPrimitive::Bag>	Bag;
	
	EndNode() : BGPrimitive(0, "EndNode") {}
	~EndNode() {}
	

	virtual const std::type_info* getReturnType(GP::Context& ioContext) const {	return ArgNode;	}
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {}
};

class EndJct : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<EndJct,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<EndJct,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<EndJct,BGPrimitive::Bag>	Bag;
	
	EndJct() : BGPrimitive(0, "EndJct") {}
	~EndJct() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(GP::Context& ioContext) const {
		std::vector<const std::type_info*> lTypes; lTypes.push_back(ArgJct); lTypes.push_back(ArgJct0); lTypes.push_back(ArgJct1);
		return lTypes;	
	}
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {}
};

//class EndEph : public BGPrimitive {
//public:
//	typedef AllocatorT<EndEph,BGPrimitive::Alloc> Alloc;
//	typedef PointerT<EndEph,BGPrimitive::Handle> Handle;
//	typedef ContainerT<EndEph,BGPrimitive::Bag>	Bag;
//	
//	EndEph() : BGPrimitive(0, "EndEph") {}
//	~EndEph() {}
//	
//	virtual std::string  getReturnType() const { return ArgEph; }
//	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {}
//};


#endif
