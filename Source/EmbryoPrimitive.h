/*
 *  EmbryoPrimitive.h
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
 *  This file was created by Jean-Francois Dupuis on 14/05/09.
 */

#ifndef EmbryoPrimitive_H
#define EmbryoPrimitive_H

#include <beagle/GP.hpp>
#include "RootReturn.h"
#include "BGPrimitive.h"
//#include <beagle/PointerT.hpp>
//#include <beagle/AllocatorT.hpp>
//#include <beagle/ContainerT.hpp>
//#include <beagle/GP/Primitive.hpp>

class EmbryoPrimitive : public BGPrimitive {
public:
	typedef Beagle::AbstractAllocT<EmbryoPrimitive,BGPrimitive::Alloc>	Alloc;
	typedef Beagle::PointerT<EmbryoPrimitive,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<EmbryoPrimitive,BGPrimitive::Bag> Bag;
	
	EmbryoPrimitive(std::string inName="Embryo") : BGPrimitive(0, inName) {}
	~EmbryoPrimitive() {}
	
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const = 0;
	const std::type_info* getReturnType(GP::Context& ioContext) const {	return &typeid(RootReturn);	}
	
	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext) = 0;
};

#endif
