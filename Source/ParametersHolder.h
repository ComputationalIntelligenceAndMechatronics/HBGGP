/*
 *  ParametersHolder.h
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
 *  This file was created by Jean-Francois Dupuis on 16/05/10.
 */

#ifndef ParametersHolder_H
#define ParametersHolder_H

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Component.hpp"
#include <Component.h>

class ParametersHolder : public Beagle::Component,
public std::vector<BG::Component*> {
public:
    typedef Beagle::AllocatorT<ParametersHolder,Beagle::Component::Alloc> Alloc;
	typedef Beagle::PointerT<ParametersHolder,Beagle::Component::Handle> Handle;
	typedef Beagle::ContainerT<ParametersHolder,Beagle::Component::Bag>	Bag;
	
	explicit ParametersHolder();
	virtual ~ParametersHolder() { }
	
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
};

#endif
