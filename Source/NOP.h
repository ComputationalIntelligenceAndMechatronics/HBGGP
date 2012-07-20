/*
 *  NOP.h
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
 *  This file was created by Jean-Francois Dupuis on 24/09/09.
 */

#ifndef NOP_H
#define NOP_H

#include <beagle/GP.hpp>
#include "Defines.h"
#include "ArgType.h"
#include <BondGraph.h>
#include "BGPrimitive.h"


/*! \brief No Operation Primitive
 *  Used to enable correct initialization of the tree. When the tree ask for more depth and only an ephemeral
 *	is usable. The primitive can increase the required depth of the ephemeral.
 */
class NOP : public BGPrimitive {
public:
	typedef Beagle::AbstractAllocT<NOP,BGPrimitive::Alloc>	Alloc;
	typedef Beagle::PointerT<NOP,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<NOP,BGPrimitive::Bag> Bag;
	
	NOP(std::string inName="NOP") : BGPrimitive(1, inName) {}
	~NOP() {}
	
	virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const {	return ArgEph;	}
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const {	return ArgEph;	}
	
	/*! Starting point of the bond graph construction. 
	 *	Building of the initial embryo bond graph should be done in this function.
	 *	Need to be application defined according to the specific modifiable site.
	 */
	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);
};

#endif
