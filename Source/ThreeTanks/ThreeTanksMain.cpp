/*
 *  ThreeTanksMain.cpp
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

#include <beagle/Beagle.hpp>
#include <beagle/GP.hpp>

#include "ThreeTanksEmbryo.h"
#include "ThreeTanksEvalOp.h"
#include "RootReturn.h"
#include "ReplaceComponent.h"
#include "ReplaceSwitch.h"
#include "AddComponent.h"
#include "RemoveComponent.h"
#include "AddSwitch.h"
#include "AddSwitchE.h"
#include "InsertJunction.h"
#include "InsertJunctionPair.h"
#include "End.h"
#include "FlipBond.h"
#include "SplitBond.h"
#include "SplitBondSame.h"
#include "BGFitness.h"
#include "BGContext.h"
#include "TreeSTag.h"
#include "GrowingHybridBondGraph.h"
#include "LogIndividualDataOp.h"
#include "BGSpeciationOp.h"
#include "BGSpeciationVerificationOp.h"
#include "StructuralHierarchicalFairCompetitionOp.h"
#include "StatsCalcStructuralFitnessOp.h"
#include "CrossoverSelectiveConstrainedOp.hpp"
#include "MutationStandardSelectiveConstrainedOp.hpp"
#include "MutationShrinkSelectiveConstrainedOp.hpp"
#include "MutationSwapSelectiveConstrainedOp.hpp"
#include "CrossoverDepthSelectiveConstrainedOp.hpp"
#include "MutationStandardDepthSelectiveConstrainedOp.hpp"
#include "MutationShrinkDepthSelectiveConstrainedOp.hpp"
#include "MutationSwapDepthSelectiveConstrainedOp.hpp"

#ifdef USE_MPI
#include "MPI_GP_Evolver.hpp"
#endif

using namespace Beagle;

int main(int argc, char *argv[]) {
	try {
		// 1: Build primitives.
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&(typeid(RootReturn)));
		
		lSet->insert(new ThreeTanksEmbryo);
		
		lSet->insert(new RemoveComponent,0.05);
		
		lSet->insert(new ReplaceR);
		//lSet->insert(new ReplaceC,0.10);
		//lSet->insert(new ReplaceI,0.10);
		lSet->insert(new ReplaceSwitch,0.10);
		
		lSet->insert(new AddSwitch,5);
		//lSet->insert(new AddSwitchE01);
		//lSet->insert(new AddSwitchE10);
		
		lSet->insert(new AddR);
		//lSet->insert(new AddC,5);
		//lSet->insert(new AddI,5);
		
		//lSet->insert(new FlipBond);
		//lSet->insert(new SplitBond01,0.5);
		//lSet->insert(new SplitBond10,0.5);
		//lSet->insert(new SplitBond0,0.5);
		//lSet->insert(new SplitBond1,0.5);
		
		lSet->insert(new InsertJ10,0.5);
		lSet->insert(new InsertJ01,0.5);
		lSet->insert(new InsertJ10j);
		lSet->insert(new InsertJ01j);
		
		lSet->insert(new EndBond);
		//lSet->insert(new EndEph);
		lSet->insert(new EndNode);
		lSet->insert(new EndJct);
		
		lSet->insert(new GP::MultiplyT<Double>);
		lSet->insert(new GP::DivideT<Double>);
		lSet->insert(new GP::AddT<Double>);
		lSet->insert(new GP::SubtractT<Double>);
		lSet->insert(new GP::EphemeralDouble,2);
		
		
		// 2: Build a system.
		BGContext::Alloc::Handle lContextAlloc = new BGContext::Alloc;
		
		GP::System::Handle lSystem = new GP::System(lSet,lContextAlloc);
		
		// 3: Build evaluation operator.
		ThreeTanksEvalOp::Handle lEvalOp = new ThreeTanksEvalOp;
		
		// 4: Build an evolver and a vivarium.
		TreeSTag::Alloc::Handle lTreeAlloc = new TreeSTag::Alloc;
		BGFitness::Alloc::Handle lFitAlloc = new BGFitness::Alloc;
		GP::Vivarium::Handle lVivarium = new GP::Vivarium(lTreeAlloc,lFitAlloc);
		
		
#ifdef USE_MPI	
		MPI::GP::Evolver::Handle lEvolver = new MPI::GP::Evolver(lEvalOp);
#else
		GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
#endif
		lEvolver->addOperator(new LogIndividualDataOp);
		lEvolver->addOperator(new BGSpeciationVerificationOp);
		lEvolver->addOperator(new StatsCalcStructuralFitnessOp);
		lEvolver->addOperator(new BGSpeciationOp);
		lEvolver->addOperator(new StructuralHierarchicalFairCompetitionOp);
		lEvolver->addOperator(new Beagle::GP::CrossoverSelectiveConstrainedOp);
		lEvolver->addOperator(new Beagle::GP::MutationStandardSelectiveConstrainedOp);
		lEvolver->addOperator(new Beagle::GP::MutationShrinkSelectiveConstrainedOp);
		lEvolver->addOperator(new Beagle::GP::MutationSwapSelectiveConstrainedOp);
		lEvolver->addOperator(new Beagle::GP::CrossoverDepthSelectiveConstrainedOp);
		lEvolver->addOperator(new Beagle::GP::MutationStandardDepthSelectiveConstrainedOp);
		lEvolver->addOperator(new Beagle::GP::MutationShrinkDepthSelectiveConstrainedOp);
		lEvolver->addOperator(new Beagle::GP::MutationSwapDepthSelectiveConstrainedOp);
		
		// 5: Initialize and evolve the vivarium.
		lEvolver->initialize(lSystem, argc, argv);
		lEvolver->evolve(lVivarium);
		
		
		
		//		ofstream lFile("Seed.xml");
		//		XMLStreamer lStreamer(lFile);
		//		lSystem->getRandomizer().write(lStreamer);
		
	}
	
	catch(Exception& inException) {
		inException.terminate();
	}
	catch(exception& inException) {
		cerr << "Standard exception catched:" << endl;
		cerr << inException.what() << endl << flush;
		return 1;
	}
	catch(...) {
		cerr << "Unknown exception catched!" << endl << flush;
		return 1;
	}
	return 0;
} 
