/*
 *  ArgType.h
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
 *  This file was created by Jean-Francois Dupuis on 09/10/09.
 */

#ifndef ArgType_H
#define ArgType_H

#include <typeinfo>
#include <BondGraph.h>
#include <beagle/Double.hpp>
#include "BGObject.h"

//#define WITHOUT_REPLACE

class Bond0 : public BG::Bond {
public:
	Bond0(long int inId = -1) : BG::Bond(inId) {}
	~Bond0() {}
};

class Bond1 : public BG::Bond {
public:
	Bond1(long int inId = -1)  : BG::Bond(inId) {}
	~Bond1() {}
};

class Junction0 : public BG::Junction {
public:
	Junction0(long int inId = -1) : BG::Junction(eZero,inId) {}
	~Junction0() {}
};

class Junction1 : public BG::Junction {
public:
	Junction1(long int inId = -1)  : BG::Junction(eOne,inId) {}
	~Junction1() {}
};


typedef BGObjectT< Bond0* > Bond0Ptr;
typedef BGObjectT< Bond1* > Bond1Ptr;

typedef BGObjectT< Junction0* > Junction0Ptr;
typedef BGObjectT< Junction1* > Junction1Ptr;
#define ArgJct0  &typeid(Junction0Ptr)
#define ArgJct1  &typeid(Junction1Ptr)

typedef BGObjectT< BG::Bond* > BondPtr;
typedef BGObjectT< BG::Component * > ComponentPtr;
typedef BGObjectT< BG::Junction* > JunctionPtr;

#define ArgBond1 &typeid(Bond1Ptr)
#define ArgBond0 &typeid(Bond0Ptr)

#define ArgBond &typeid(BondPtr)
#define ArgNode &typeid(ComponentPtr)
#define ArgJct  &typeid(JunctionPtr)
#define ArgEph	&typeid(Beagle::Double)

#endif
