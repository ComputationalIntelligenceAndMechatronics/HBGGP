/*
 *  AnalogFilterEvalOp.h
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
 *  This file was created by Jean-Francois Dupuis on 14/05/09.
 */

#ifndef AnalogFilterEvalOp_H
#define AnalogFilterEvalOp_H


#include <beagle/GP.hpp>
#include "BondGraphEvalOp.h"

class AnalogFilterEvalOp : public BondGraphEvalOp {
public:
	typedef Beagle::AllocatorT<AnalogFilterEvalOp,BondGraphEvalOp::Alloc> Alloc;
	typedef Beagle::PointerT<AnalogFilterEvalOp,BondGraphEvalOp::Handle> Handle;
	typedef Beagle::ContainerT<AnalogFilterEvalOp,BondGraphEvalOp::Bag> Bag;
	
	AnalogFilterEvalOp(std::string inName="AnalogFilterEvalOp");
	~AnalogFilterEvalOp();
	
	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext);
	
	virtual void initialize(Beagle::System& ioSystem);
	virtual void postInit(Beagle::System& ioSystem);

	
private:
	static bool mIsInitialized;
};

#endif
