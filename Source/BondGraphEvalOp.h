/*
 *  BondGraphEvalOp.h
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

#ifndef BondGraphEvalOp_H
#define BondGraphEvalOp_H

#include <beagle/GP.hpp>
#include <stdexcept>

#ifdef USE_MPI
#include <MPI_GP_EvaluationOp.hpp>
class BondGraphEvalOp : public Beagle::MPI::GP::EvaluationOp {
public:
	
	//! BondGraphEvalOp allocator type.
	typedef Beagle::AbstractAllocT<BondGraphEvalOp,Beagle::MPI::EvaluationOp::Alloc> Alloc;
	//!< BondGraphEvalOp handle type.
	typedef Beagle::PointerT<BondGraphEvalOp,Beagle::MPI::EvaluationOp::Handle>	Handle;
	//!< BondGraphEvalOp bag type.
	typedef Beagle::ContainerT<BondGraphEvalOp,Beagle::MPI::EvaluationOp::Bag> Bag;
#else
#include <beagle/GP/EvaluationOp.hpp>
class BondGraphEvalOp : public Beagle::GP::EvaluationOp {
public:
	
	//! BondGraphEvalOp allocator type.
	typedef Beagle::AllocatorT<BondGraphEvalOp,Beagle::EvaluationOp::Alloc> Alloc;
	//!< BondGraphEvalOp handle type.
	typedef Beagle::PointerT<BondGraphEvalOp,Beagle::EvaluationOp::Handle> Handle;
	//!< BondGraphEvalOp bag type.
	typedef Beagle::ContainerT<BondGraphEvalOp,Beagle::EvaluationOp::Bag> Bag;
#endif
	
	explicit BondGraphEvalOp(std::string inName="BondGraphEvalOp");

	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext) { throw std::runtime_error("Undefined BondGraphEvalOp::evaluate"); }
	
	virtual void initialize(Beagle::System& ioSystem);
	virtual void postInit(Beagle::System& ioSystem);


};


#endif
