/*
 *  ReplaceSwitch.h
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

#ifndef ReplaceSwitch_H
#define ReplaceSwitch_H

#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include "BGPrimitive.h"

using namespace Beagle;

class ReplaceSwitch : public BGPrimitive {
public:
	typedef AllocatorT<ReplaceSwitch,BGPrimitive::Alloc> Alloc;
	typedef PointerT<ReplaceSwitch,BGPrimitive::Handle> Handle;
	typedef ContainerT<ReplaceSwitch,BGPrimitive::Bag>	Bag;
	
	ReplaceSwitch(std::string inName="ReplaceSwitch") : BGPrimitive(1, inName) {}
	~ReplaceSwitch() {}
	
	
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
	
	virtual void execute(Beagle::GP::Datum& inComponent, Beagle::GP::Context& ioContext);
};

#endif
