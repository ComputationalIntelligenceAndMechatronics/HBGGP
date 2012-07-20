/*
 *  SplitBondSame.h
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

#ifndef SplitBondSame_H
#define SplitBondSame_H


#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include "BGPrimitive.h"

/*! \brief Insert two junction in parallele to a bond class
 *  This class is the base class for junction pair insertion over a bond. This class
 *	require that both split and merge point be the same junction type.
 */
class SplitBondSame : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<SplitBondSame,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<SplitBondSame,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<SplitBondSame,BGPrimitive::Bag> Bag;
	
	SplitBondSame(std::string inName) : BGPrimitive(6, inName) {}
	~SplitBondSame() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const = 0;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const = 0;
	
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) = 0;
	
	void insert(Beagle::GP::Datum& inBond, BG::Junction::JunctionType inType1, BG::Junction::JunctionType inType2, Beagle::GP::Context& ioContext);
};

/*! \brief Insert a 1-junction in paralelle between two 0-junction
 *  This primitive insert a junction 1 between a two junction 0. The new junction 
 *	will be inserted in paralelle of an existing bond. Thus, create a loop.
 */
class SplitBond0 : public SplitBondSame {
public:
	typedef Beagle::AllocatorT<SplitBond0,SplitBondSame::Alloc> Alloc;
	typedef Beagle::PointerT<SplitBond0,SplitBondSame::Handle> Handle;
	typedef Beagle::ContainerT<SplitBond0,SplitBondSame::Bag> Bag;
	
	SplitBond0() : SplitBondSame("SplitBond0") {}
	~SplitBond0() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

/*! \brief Insert a two 0-junction in paralelle between two 1-junction
 *  This primitive insert a junction 1 between a two junction 1. The new junction 
 *	will be inserted in paralelle of an existing bond. Thus, create a loop.
 */
class SplitBond1 : public SplitBondSame {
public:
	typedef Beagle::AllocatorT<SplitBond1,SplitBondSame::Alloc> Alloc;
	typedef Beagle::PointerT<SplitBond1,SplitBondSame::Handle> Handle;
	typedef Beagle::ContainerT<SplitBond1,SplitBondSame::Bag> Bag;
	
	SplitBond1() : SplitBondSame("SplitBond1") {}
	~SplitBond1() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

#endif
