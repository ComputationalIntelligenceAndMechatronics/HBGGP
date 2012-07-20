/*
 *  SplitBond.h
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

#ifndef SplitBond_H
#define SplitBond_H


#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include "BGPrimitive.h"

/*! \brief Insert Junction Pair in parallele to a bond class
 *  This class is the base class for junction pair insertion over a bond.
 */
class SplitBond : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<SplitBond,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<SplitBond,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<SplitBond,BGPrimitive::Bag> Bag;
	
	SplitBond(std::string inName) : BGPrimitive(6, inName) {}
	~SplitBond() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const = 0;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const = 0;
	
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) = 0;
	
	void insert(Beagle::GP::Datum& inBond, BG::Junction::JunctionType inType1, BG::Junction::JunctionType inType2, Beagle::GP::Context& ioContext);
};


/*! \brief Insert a Junction Pair 0-1
 *  This primitive insert a pair of junction 0 and 1 between a junction 1 and a 
 *	junction 0. The new junction pair will be inserted in paralelle of an existing
 *	bond. Thus, create a loop.
 */
class SplitBond01 : public SplitBond {
public:
	typedef Beagle::AllocatorT<SplitBond01,SplitBond::Alloc> Alloc;
	typedef Beagle::PointerT<SplitBond01,SplitBond::Handle> Handle;
	typedef Beagle::ContainerT<SplitBond01,SplitBond::Bag> Bag;
	
	SplitBond01() : SplitBond("SplitBond01") {}
	~SplitBond01() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

/*! \brief Insert a Junction Pair 1-0
 *  This primitive insert a pair of junction 1 and 0 between a junction 0 and a 
 *	junction 1. The new junction pair will be inserted in paralelle of an existing
 *	bond. Thus, create a loop.
 */
class SplitBond10 : public SplitBond {
public:
	typedef Beagle::AllocatorT<SplitBond10,SplitBond::Alloc> Alloc;
	typedef Beagle::PointerT<SplitBond10,SplitBond::Handle> Handle;
	typedef Beagle::ContainerT<SplitBond10,SplitBond::Bag> Bag;
	
	SplitBond10() : SplitBond("SplitBond10") {}
	~SplitBond10() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};


#endif
