/*
 *  DCDCBoostGAEvalOp.h
 *  Copyright 2008 Jean-Francois Dupuis.
 *
 *  This file is part of EvoStateController.
 *  
 *  EvoStateController is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EvoStateController is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EvoStateController.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  This file was created by Jean-Francois Dupuis on 02/10/08.
 */

#ifndef DCDCBoostGAEvalOp_H
#define DCDCBoostGAEvalOp_H


#include <beagle/GA.hpp>
#include <vector>
#include "SimulationCase.h"



#ifdef USE_MPI
#include <MPI_EvaluationOp.hpp>
class DCDCBoostGAEvalOp : public Beagle::MPI::EvaluationOp {
public:
	
	//! DCDCBoostGAEvalOp allocator type.
	typedef Beagle::AllocatorT<DCDCBoostGAEvalOp,Beagle::MPI::EvaluationOp::Alloc>
	Alloc;
	//!< DCDCBoostGAEvalOp handle type.
	typedef Beagle::PointerT<DCDCBoostGAEvalOp,Beagle::MPI::EvaluationOp::Handle>
	Handle;
	//!< DCDCBoostGAEvalOp bag type.
	typedef Beagle::ContainerT<DCDCBoostGAEvalOp,Beagle::MPI::EvaluationOp::Bag>
	Bag;
#else
#include <beagle/EvaluationOp.hpp>
class DCDCBoostGAEvalOp : public Beagle::EvaluationOp {
public:
	
	//! DCDCBoostGAEvalOp allocator type.
	typedef Beagle::AllocatorT<DCDCBoostGAEvalOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//!< DCDCBoostGAEvalOp handle type.
	typedef Beagle::PointerT<DCDCBoostGAEvalOp,Beagle::EvaluationOp::Handle>
	Handle;
	//!< DCDCBoostGAEvalOp bag type.
	typedef Beagle::ContainerT<DCDCBoostGAEvalOp,Beagle::EvaluationOp::Bag>
	Bag;
#endif
	
	explicit DCDCBoostGAEvalOp();
	
	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
											 Beagle::Context& ioContext);
	virtual void initialize(Beagle::System& ioSystem);
	virtual void postInit(Beagle::System& ioSystem);
	
protected:
	Beagle::GA::BitString::DecodingKeyVector mDecodingKeys;

	Beagle::String::Handle mTargetString;
	std::vector<SimulationCase> mSimulationCases;
	
//	Beagle::FloatArray::Handle mTargetsTime;
//	Beagle::FloatArray::Handle mTargets;
	
	Beagle::FloatArray::Handle mGenerationSteps;
//	Beagle::FloatArray::Handle mTargetsGen0;
//	Beagle::FloatArray::Handle mTargetsGen1;
//	Beagle::FloatArray::Handle mTargetsGen2;
	
//	Beagle::Float::Handle mTargetTolerances;
	Beagle::FloatArray::Handle mInitialState;
	Beagle::Float::Handle mSimulationDuration;
	Beagle::Float::Handle mContinuousTimeStep;
//	Beagle::Float::Handle mDiscreteTimeStep;
	
	Beagle::FloatArray::Handle mCapacitance;
	Beagle::Float::Handle mResistance;
	
	std::vector<Beagle::FloatArray::Handle> mTargetArrays;
	
	unsigned int mNbStates;
};

#endif
