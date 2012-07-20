/*
 *  InsertJunctionAlternate.h
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

#ifndef InsertJunctionPair_H
#define InsertJunctionPair_H

#include <BondGraph.h>
#include <beagle/Beagle.hpp>
#include "BGPrimitive.h"

/*! \brief Insert Junction Pair Base Class
 *  This class the the base class for junction pair insertion primitive.
 */
class InsertJunctionPair : public BGPrimitive {
public:
	typedef Beagle::AllocatorT<InsertJunctionPair,BGPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<InsertJunctionPair,BGPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<InsertJunctionPair,BGPrimitive::Bag> Bag;
	
	InsertJunctionPair(std::string inName) : BGPrimitive(5, inName) {}
	~InsertJunctionPair() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const = 0;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const = 0;
	
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) = 0;
	
	void insert(Beagle::GP::Datum& inBond, BG::Junction::JunctionType inType1, BG::Junction::JunctionType inType2, Beagle::GP::Context& ioContext);
	void insertAtJunction(Beagle::GP::Datum& inJunction, BG::Junction::JunctionType inType1, BG::Junction::JunctionType inType2, Beagle::GP::Context& ioContext);
};


/*! \brief Insert a Junction Pair 0-1
 *  This primitive insert a pair of junction 0 and 1 between a junction 1 and a 
 *	junction 0. The new junction pair will be inserted in the middle of an existing
 *	bond.
 */
class InsertJ01 : public InsertJunctionPair {
public:
	typedef Beagle::AllocatorT<InsertJ01,InsertJunctionPair::Alloc> Alloc;
	typedef Beagle::PointerT<InsertJ01,InsertJunctionPair::Handle> Handle;
	typedef Beagle::ContainerT<InsertJ01,InsertJunctionPair::Bag> Bag;
	
	InsertJ01() : InsertJunctionPair("InsertJ01") {}
	~InsertJ01() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

/*! \brief Insert a Junction Pair 1-0
 *  This primitive insert a pair of junction 1 and 0 between a junction 0 and a 
 *	junction 1. The new junction pair will be inserted in the middle of an existing
 *	bond.
 */
class InsertJ10 : public InsertJunctionPair {
public:
	typedef Beagle::AllocatorT<InsertJ10,InsertJunctionPair::Alloc> Alloc;
	typedef Beagle::PointerT<InsertJ10,InsertJunctionPair::Handle> Handle;
	typedef Beagle::ContainerT<InsertJ10,InsertJunctionPair::Bag> Bag;
	
	InsertJ10() : InsertJunctionPair("InsertJ10") {}
	~InsertJ10() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

/*! \brief Insert a Terminal Junction Pair 0-1
 *  This primitive insert a pair of junction 0 and 1 at a junction 1.
 *	The new junction pair will be inserted like a component at the existing
 *	junction 1. Bond graph simplification will be required before computing the
 *	state matrix to remove any terminal junction.
 */
class InsertJ01j : public InsertJunctionPair {
public:
	typedef Beagle::AllocatorT<InsertJ01j,InsertJunctionPair::Alloc> Alloc;
	typedef Beagle::PointerT<InsertJ01j,InsertJunctionPair::Handle> Handle;
	typedef Beagle::ContainerT<InsertJ01j,InsertJunctionPair::Bag> Bag;
	
	InsertJ01j() : InsertJunctionPair("InsertJ01j") {}
	~InsertJ01j() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};

/*! \brief Insert a Terminal Junction Pair 1-0
 *  This primitive insert a pair of junction 1 and 0 at a junction 0.
 *	The new junction pair will be inserted like a component at the existing
 *	junction 0. Bond graph simplification will be required before computing the
 *	state matrix to remove any terminal junction.
 */
class InsertJ10j : public InsertJunctionPair {
public:
	typedef Beagle::AllocatorT<InsertJ10j,InsertJunctionPair::Alloc> Alloc;
	typedef Beagle::PointerT<InsertJ10j,InsertJunctionPair::Handle> Handle;
	typedef Beagle::ContainerT<InsertJ10j,InsertJunctionPair::Bag> Bag;
	
	InsertJ10j() : InsertJunctionPair("InsertJ10j") {}
	~InsertJ10j() {}
	
	virtual std::vector<const std::type_info*> getReturnTypes(Beagle::GP::Context& ioContext) const;
	virtual std::vector<const std::type_info*> getArgTypes(unsigned int inN, Beagle::GP::Context& ioContext) const;
	void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext);
};


#endif
