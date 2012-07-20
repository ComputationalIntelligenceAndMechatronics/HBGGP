
#include <iostream>
#include <beagle/Beagle.hpp>

#ifdef USE_MPI
#include <MPI_GA_EvolverBitString.hpp>
#endif

#include "DCDCBoost2xGAEvalOp.h"
#include "DCDCBoost2xGAController.h"
#include "LogFitness.h"

#include <stdexcept>
#include <numeric>

using namespace std;
using namespace Beagle;


int main (int argc, char** argv) {
	try {
		// 1. Build the system.
		System::Handle lSystem = new System;
		
		// 2. Build evaluation operator.
		DCDCBoost2xGAEvalOp::Handle lEvalOp = new DCDCBoost2xGAEvalOp;

		// 3. Instanciate the evolver.
		unsigned int lNbBits = (unsigned int)(NBSTATE*NBINPUTSYBMOLE + 1)*NBSWITCH;
#ifdef USE_MPI
		MPI::GA::EvolverBitString::Handle lEvolver = new MPI::GA::EvolverBitString(lEvalOp,lNbBits);
#else
		GA::EvolverBitString::Handle lEvolver = new GA::EvolverBitString(lEvalOp,lNbBits);
#endif

		// 4. Initialize the vivarium
		GA::BitString::Alloc::Handle lBSAlloc = new GA::BitString::Alloc;
		LogFitness::Alloc::Handle lFitAlloc = new LogFitness::Alloc;
		Vivarium::Handle lVivarium = new Vivarium(lBSAlloc, lFitAlloc);
		
		// 5. Initialize the evolver and evolve the vivarium.
		lEvolver->initialize(lSystem, argc, argv);
		
//////////////////////		
		//Read individual from file
		ostringstream lFilename;
		lFilename << "individual-ga-test.xml";
		
		Individual::Handle lIndividual = new Individual(lBSAlloc);
		lIndividual->readFromFile(lFilename.str(), *lSystem);
		
		Beagle::Context::Handle lContext = castHandleT<Beagle::Context>(lSystem->getContextAllocator().allocate());
		lContext->setSystemHandle(lSystem);
		lContext->setIndividualHandle(lIndividual);
		lContext->setIndividualIndex(0);
		
		LogFitness::Handle lFitness = castHandleT<LogFitness>(lEvalOp->evaluate(*lIndividual,*lContext));
		
		cout << "\nResulting fitness: " << lFitness->getValue() << endl;
		ofstream lFitnessStream("fitness.xml");
		lFitnessStream << lFitness->serialize() << endl;
		return 0;
/////////////////////
		
		lEvolver->evolve(lVivarium);
	}
	catch(Exception& inException) {
		inException.terminate(cerr);
	}
	catch(std::exception& inException) {
		cerr << "Standard exception catched:" << endl << flush;
		cerr << inException.what() << endl << flush;
		return 1;
	}
	return 0;
}
