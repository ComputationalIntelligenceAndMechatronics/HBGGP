/*
 *  AdSwitchE.h
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
 *  This file was created by Jean-Francois Dupuis on 06/04/10.
 */

#ifndef AddSwitchE_H
#define AddSwitchE_H

#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include "BGPrimitive.h"

/*! \brief Insert Junction Pair Base Class
 *  This class the the base class for junction pair insertion primitive.
 */
class AddSwitchE : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<AddSwitchE,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<AddSwitchE,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<AddSwitchE,BGPrimitive::Bag> Bag;
	
	AddSwitchE(std::string inName) : BGPrimitive(5, inName) {}
	~AddSwitchE() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const = 0;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const = 0;
	
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) = 0;
	
	void insert(Beagle::GP::Datum& inBond, BG::Junction::JunctionType inType1, BG::Junction::JunctionType inType2, Beagle::GP::Context& ioContext);
};


/*! \brief Insert a Junction Pair 0-1 with mutually exclusive switch.
 *  This primitive insert a pair of junction 0 and 1 between a junction 1 and a 
 *	junction 0. The new junction pair will be inserted in the middle of an existing
 *	bond. Then, an mutually exclusive switch pair will be added to the new junction.
 */
class AddSwitchE01 : public AddSwitchE {
public:
	typedef Beagle::AllocatorT<AddSwitchE01,AddSwitchE::Alloc> Alloc;
	typedef Beagle::PointerT<AddSwitchE01,AddSwitchE::Handle> Handle;
	typedef Beagle::ContainerT<AddSwitchE01,AddSwitchE::Bag> Bag;
	
	AddSwitchE01() : AddSwitchE("AddSwitchE01") {}
	~AddSwitchE01() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

/*! \brief Insert a Junction Pair 1-0 with mutually exclusive switch.
 *  This primitive insert a pair of junction 1 and 0 between a junction 0 and a 
 *	junction 1. The new junction pair will be inserted in the middle of an existing
 *	bond. Then, an mutually exclusive switch pair will be added to the new junction.
 */
class AddSwitchE10 : public AddSwitchE {
public:
	typedef Beagle::AllocatorT<AddSwitchE10,AddSwitchE::Alloc> Alloc;
	typedef Beagle::PointerT<AddSwitchE10,AddSwitchE::Handle> Handle;
	typedef Beagle::ContainerT<AddSwitchE10,AddSwitchE::Bag> Bag;
	
	AddSwitchE10() : AddSwitchE("AddSwitchE10") {}
	~AddSwitchE10() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

#endif
