/*
 *  ThreeTanksEmbryo2.h
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
 *  This file was created by Jean-Francois Dupuis on 22/02/10.
 */

#ifndef ThreeTanksEmbryo2_H
#define ThreeTanksEmbryo2_H

#include <beagle/Beagle.hpp>
#include "EmbryoPrimitive.h"

class ThreeTanksEmbryo2 : public EmbryoPrimitive {
public:
	typedef Beagle::AllocatorT<ThreeTanksEmbryo2,EmbryoPrimitive::Alloc> Alloc;
	typedef Beagle::PointerT<ThreeTanksEmbryo2,EmbryoPrimitive::Handle> Handle;
	typedef Beagle::ContainerT<ThreeTanksEmbryo2,EmbryoPrimitive::Bag> Bag;
	
	ThreeTanksEmbryo2();
	~ThreeTanksEmbryo2() {}
	

	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	
	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

private:
	double mSwR;
	double mDrainR;
	double mC;
	double mR;
	double g;
	double mFluidDensity;
	double mF;
	
};

#endif
