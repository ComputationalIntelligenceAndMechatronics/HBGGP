/*
 *  AnalogFilterParameterEvalOp.h
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
 *  This file was created by Jean-Francois Dupuis on 19/08/09.
 */

#ifndef AnalogFilterParameterEvalOp_H
#define AnalogFilterParameterEvalOp_H

#include "BondGraphParameterEvalOp.h"

class AnalogFilterParameterEvalOp : public BondGraphParameterEvalOp {
public:
	typedef Beagle::AllocatorT<AnalogFilterParameterEvalOp,BondGraphParameterEvalOp::Alloc> Alloc;
	typedef Beagle::PointerT<AnalogFilterParameterEvalOp,BondGraphParameterEvalOp::Handle> Handle;
	typedef Beagle::ContainerT<AnalogFilterParameterEvalOp,BondGraphParameterEvalOp::Bag> Bag;
	
	AnalogFilterParameterEvalOp(std::string inName="AnalogFilterParameterEvalOp");
	~AnalogFilterParameterEvalOp();
	
	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual, Beagle::Context& ioContext);

	virtual void initialize(Beagle::System& ioSystem);
	virtual void postInit(Beagle::System& ioSystem);
};


#endif
