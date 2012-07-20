/*
 *  FlipBond.h
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
 *  This file was created by Jean-Francois Dupuis on 11/05/10.
 */

#ifndef FlipBond_H
#define FlipBond_H

#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include "BGPrimitive.h"

class FlipBond : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<FlipBond,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<FlipBond,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<FlipBond,BGPrimitive::Bag> Bag;
	
	FlipBond(std::string inName="FlipBond") : BGPrimitive(1, inName) {}
	~FlipBond() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
	
};

#endif
