/*
 *  DCDCBoost2xGAEvalOp.h
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

#ifndef DCDCBoost2xGAEvalOp_H
#define DCDCBoost2xGAEvalOp_H


#include <beagle/GA.hpp>
#include <vector>
#include "SimulationCase.h"
#include <BondGraph.h>


#ifdef USE_MPI
#include <MPI_EvaluationOp.hpp>
class DCDCBoost2xGAEvalOp : public Beagle::MPI::EvaluationOp {
public:
	
	//! DCDCBoost2xGAEvalOp allocator type.
	typedef Beagle::AllocatorT<DCDCBoost2xGAEvalOp,Beagle::MPI::EvaluationOp::Alloc>
	Alloc;
	//!< DCDCBoost2xGAEvalOp handle type.
	typedef Beagle::PointerT<DCDCBoost2xGAEvalOp,Beagle::MPI::EvaluationOp::Handle>
	Handle;
	//!< DCDCBoost2xGAEvalOp bag type.
	typedef Beagle::ContainerT<DCDCBoost2xGAEvalOp,Beagle::MPI::EvaluationOp::Bag>
	Bag;
#else
#include <beagle/EvaluationOp.hpp>
class DCDCBoost2xGAEvalOp : public Beagle::EvaluationOp {
public:
	
	//! DCDCBoost2xGAEvalOp allocator type.
	typedef Beagle::AllocatorT<DCDCBoost2xGAEvalOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//!< DCDCBoost2xGAEvalOp handle type.
	typedef Beagle::PointerT<DCDCBoost2xGAEvalOp,Beagle::EvaluationOp::Handle>
	Handle;
	//!< DCDCBoost2xGAEvalOp bag type.
	typedef Beagle::ContainerT<DCDCBoost2xGAEvalOp,Beagle::EvaluationOp::Bag>
	Bag;
#endif
	
	explicit DCDCBoost2xGAEvalOp();
	
	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
											 Beagle::Context& ioContext);
	virtual void initialize(Beagle::System& ioSystem);
	virtual void postInit(Beagle::System& ioSystem);
	
protected:
	double computeError(const BG::BondGraph* inBondGraph, std::map<std::string, std::vector<double> > &lSimulationLog);
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
	
	Beagle::Int::Handle mMaxNumberSwitch;
	Beagle::Int::Handle mAllowDifferentialCausality;
	
	unsigned int mNbStates;
	long int mIndividualCounter;
};

#endif
