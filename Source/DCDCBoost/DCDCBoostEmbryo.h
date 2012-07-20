/*
 *  DCDCBoostEmbryo.h
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
 *  This file was created by Jean-Francois Dupuis on 25/03/10.
 */

#ifndef DCDCBoostEmbryo_H
#define DCDCBoostEmbryo_H

#include <beagle/Beagle.hpp>
#include "EmbryoPrimitive.h"

class DCDCBoostEmbryo : public EmbryoPrimitive {
public:
	typedef Beagle::AllocatorT<DCDCBoostEmbryo,EmbryoPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<DCDCBoostEmbryo,EmbryoPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<DCDCBoostEmbryo,EmbryoPrimitive::Bag> Bag;
	
	DCDCBoostEmbryo();
	~DCDCBoostEmbryo() {}
	
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	
	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);
	
};

#endif
