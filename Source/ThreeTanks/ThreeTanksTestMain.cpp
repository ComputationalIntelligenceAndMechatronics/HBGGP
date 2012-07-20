/*
 *  ThreeTanksTestMain.cpp
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
 *  This file was created by Jean-Francois Dupuis on 02/03/10.
 */

#include <beagle/Beagle.hpp>
#include <beagle/GP.hpp>

#include "RootReturn.h"
#include "ReplaceComponent.h"
#include "AddComponent.h"
#include "AddSwitch.h"
#include "InsertJunction.h"
#include "InsertJunctionPair.h"
#include "End.h"
#include "FlipBond.h"
#include "SplitBond.h"
#include "EmbryoPrimitive.h" 
#include "BGFitness.h"
#include "ThreeTanksEmbryo.h"
#include "ThreeTanksEvalOp.h"
#include "BGContext.h"
#include "TreeSTag.h"
#include "GrowingHybridBondGraph.h"

#ifdef USE_MPI
#include "MPI_GP_Evolver.hpp"
#endif

using namespace Beagle;

int main(int argc, char *argv[]) {
	try {
		
		// 1: Build primitives.
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&(typeid(RootReturn)));
		
		lSet->insert(new ThreeTanksEmbryo);
		
		lSet->insert(new ReplaceR);
		//		lSet->insert(new ReplaceC);
		//		lSet->insert(new ReplaceI);
		
		lSet->insert(new AddSwitch);
		lSet->insert(new AddR);
		//		lSet->insert(new AddC);
		//		lSet->insert(new AddI);
		
		lSet->insert(new InsertJ01);
		lSet->insert(new InsertJ10);
		
		lSet->insert(new EndBond);
		//lSet->insert(new EndEph);
		lSet->insert(new EndNode);
		lSet->insert(new EndJct);
		
		lSet->insert(new GP::MultiplyT<Double>);
		lSet->insert(new GP::DivideT<Double>);
		lSet->insert(new GP::AddT<Double>);
		lSet->insert(new GP::SubtractT<Double>);
		lSet->insert(new GP::EphemeralDouble);
		
		
		// 2: Build a system.
		BGContext::Alloc::Handle lContextAlloc = new BGContext::Alloc;
		
		GP::System::Handle lSystem = new GP::System(lSet,lContextAlloc);
		
		// 3: Build evaluation operator.
		ThreeTanksEvalOp::Handle lEvalOp = new ThreeTanksEvalOp;
		
		// 4: Build an evolver and a vivarium.
		TreeSTag::Alloc::Handle lTreeAlloc = new TreeSTag::Alloc;
		//GP::Tree::Alloc::Handle lTreeAlloc = new GP::Tree::Alloc;
		BGFitness::Alloc::Handle lFitAlloc = new BGFitness::Alloc;
		GP::Vivarium::Handle lVivarium = new GP::Vivarium(lTreeAlloc,lFitAlloc);
		
		GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
		lEvolver->initialize(lSystem, argc, argv);
		
		//Read individual from file
		ostringstream lFilename;
		lFilename << "individual-best153.xml";

		GP::Individual::Handle lIndividual = new GP::Individual(lTreeAlloc);
		lIndividual->readFromFile(lFilename.str(), *lSystem);
		
		BGContext::Handle lContext = castHandleT<BGContext>(lSystem->getContextAllocator().allocate());
		lContext->setSystemHandle(lSystem);
		lContext->setIndividualHandle(lIndividual);
		lContext->setIndividualIndex(0);
		
		BGFitness::Handle lFitness = castHandleT<BGFitness>(lEvalOp->evaluate(*lIndividual,*lContext));
		
		//TreeSTag::Handle lTree = castHandleT<TreeSTag>((*lIndividual)[0]);
		GrowingHybridBondGraph::Handle lBondGraph = castHandleT<GrowingHybridBondGraph>(lFitness->getBondGraph());
		//lBondGraph->simplify();
		lBondGraph->plotGraph("bondgraph.svg",false);
		
		cout << "\nResulting fitness: " << endl;
		cout << lFitness->serialize() << endl;
		
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
