/*
 *  StatsCalcStructuralFitnessOp.h
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
 *  This file was created by Jean-Francois Dupuis on 19/06/10.
 */

#ifndef StatsCalcStructuralFitness_H
#define StatsCalcStructuralFitness_H

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/Context.hpp"
#include "beagle/GP/StatsCalcFitnessSimpleOp.hpp"

class StatsCalcStructuralFitnessOp : public Beagle::GP::StatsCalcFitnessSimpleOp {
	
public:
	
	//! StatsCalcStructuralFitnessOp allocator type
	typedef Beagle::AllocatorT<StatsCalcStructuralFitnessOp,Beagle::StatsCalcFitnessSimpleOp::Alloc> Alloc;
	//! StatsCalcStructuralFitnessOp handle type.
	typedef Beagle::PointerT<StatsCalcStructuralFitnessOp,Beagle::StatsCalcFitnessSimpleOp::Handle> Handle;
	//! StatsCalcStructuralFitnessOp bag type.
	typedef Beagle::ContainerT<StatsCalcStructuralFitnessOp,Beagle::StatsCalcFitnessSimpleOp::Bag> Bag;
	
	explicit StatsCalcStructuralFitnessOp(std::string inName="StatsCalcStructuralFitnessOp");
	virtual ~StatsCalcStructuralFitnessOp() { }
	
	virtual void calculateStatsDeme(Beagle::Stats& outStats, Beagle::Deme& ioDeme, Beagle::Context& ioContext) const;
	virtual void calculateStatsVivarium(Beagle::Stats& outStats, Beagle::Vivarium& ioVivarium, Beagle::Context& ioContext) const;
};

#endif
