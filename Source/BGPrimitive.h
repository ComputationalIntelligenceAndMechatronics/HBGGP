/*
 *  BGPrimitive.h
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
 *  This file was created by Jean-Francois Dupuis on 16/03/10.
 */

#ifndef BGPrimitive_H
#define BGPrimitive_H

#include <beagle/Beagle.hpp>
#include <beagle/GP/Primitive.hpp>

class BGPrimitive : public Beagle::GP::Primitive {
public:
	typedef Beagle::AllocatorT<BGPrimitive,Beagle::GP::Primitive::Alloc> Alloc;
	typedef Beagle::PointerT<BGPrimitive,Beagle::GP::Primitive::Handle> Handle;
	typedef Beagle::ContainerT<BGPrimitive,Beagle::GP::Primitive::Bag>	Bag;
	

	BGPrimitive(unsigned int inN, std::string inName) : Beagle::GP::Primitive(inN, inName) {}
	~BGPrimitive() {}
	
	std::vector<const std::type_info*> getParentArgumentTypes(Beagle::GP::Context& ioContext) const;
		
	virtual void execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) = 0;
};

#endif
