/*
 *  DCDCBoostGAEvalOp.cpp
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

#include "DCDCBoostGAEvalOp.h"
#include "beagle/GA.hpp"
#include "LogFitness.h"
#include "DCDCBoostGAController.h"
#include "Simulator.h"

#include <cmath>
#include <cfloat>
#include <vector>
#include <map>

#define NBTANKS 2

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator.
 */
DCDCBoostGAEvalOp::DCDCBoostGAEvalOp() :
#ifdef USE_MPI
MPI::EvaluationOp("DCDCBoostGAEvalOp")
#else
Beagle::EvaluationOp("DCDCBoostGAEvalOp")
#endif
{ 
	//The number of bit is determine as follow. Each cell of the table need a number of bits
	//equal to NBSWICTH. The table dimensions are 2^NBSWICTH x NBINPUTSYBMOLE. Also, the first 
	//NBSWICTH bits are used to define the initial state.
	//mNbStates = (unsigned int)(pow(double(2),NBSWITCH));
	mNbStates = NBSTATE;
	for(unsigned int i=0; i< (mNbStates*NBINPUTSYBMOLE + 1); i++) {
		GA::BitString::DecodingKey lKey(0, mNbStates-1, NBSWITCH);
		mDecodingKeys.push_back(lKey);
	}
}

/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
Fitness::Handle DCDCBoostGAEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_AssertM(inIndividual.size() == 1);
	
	GA::BitString::Handle lBitString = castHandleT<GA::BitString>(inIndividual[0]);
	
	//Create the transition table
	Beagle::DoubleArray lValues;
	lBitString->decode(mDecodingKeys, lValues);
	std::vector< std::vector<unsigned int> > lTransitionTable(mNbStates);
	unsigned int lStartState = (unsigned int)lValues.back();
	for(unsigned int i = 0; i < mNbStates; ++i) {
		for(unsigned int j = 0; j < NBINPUTSYBMOLE; ++j) {
			lTransitionTable[i].push_back((unsigned int)(lValues[j+NBINPUTSYBMOLE*i]));
		}
	}
	
	//Initialize the simulation
	DCDCBoostGAController *lController = new DCDCBoostGAController;
	HybridBondGraph lHBG;
	lController->createBondGraph(lHBG);
	
	std::map<std::string, std::vector<double> >& lLogger = lHBG.getSimulationLog();
	
	std::vector<double> lInitialStates(mInitialState->size());
	for(unsigned int i = 0; i< mInitialState->size(); ++i) {
		lInitialStates[i] = (*mInitialState)[i];
	}

	lController->initialize(&lHBG,lStartState,lInitialStates);
	lController->setTransitionTable(lTransitionTable);
	
	//Simulate every target prior to this generation
	vector<double> lFitnessVector;
	LogFitness *lFitness = new LogFitness;
	unsigned int lTry = 0;
	for(int g = mSimulationCases.size()-1; g >= 0; --g) {
		bool lRun = false;
		if( (*mGenerationSteps)[0] < 0 )
			lRun = true;
		else if( ioContext.getGeneration() >= (*mGenerationSteps)[g] )
			lRun = true;
		if(lRun) {
			
			//Reinitialize
			lLogger.clear();
			lController->reset();
			
			//Run the simulation
			for(unsigned int i = 0; i < mSimulationCases[g].getSize(); ++i) {
				if( mSimulationCases[g].getTime(i) >= mSimulationDuration->getWrappedValue() )
					throw Beagle_RunTimeExceptionM("DCDCBoostGAEvalOp : Applying control target later than simulation end");
				if(mSimulationCases[g].getTargets(i).size() != 2)
					throw Beagle_RunTimeExceptionM("DCDCBoostGAEvalOp : There should be only two target for each control time");

				lController->setTarget(mSimulationCases[g].getTargets(i));
				
				if(i < mSimulationCases[g].getSize()-1) 
					lHBG.simulate(mSimulationCases[g].getTime(i+1), mContinuousTimeStep->getWrappedValue());
				else
					lHBG.simulate(mSimulationDuration->getWrappedValue(), mContinuousTimeStep->getWrappedValue());
				
//				lHBG.writeSimulationLog("simulation.csv");
//				exit(0);
				
			}
			
			//Evaluate the results
			double lF = lController->computeError();
	
			//Log simulation data
			lFitnessVector.push_back(lF);
			lFitness->addDataSet(lTry, lF);
			for(map<std::string, std::vector<double> >::const_iterator lIter = lLogger.begin(); lIter != lLogger.end(); ++lIter) {
				lFitness->addData(lIter->first, lIter->second,lTry);
			}
			++lTry;
		}
	}
	
//	//Compute mean of fitness
//	double lMeanF = 0;
//	for(unsigned int i = 0; i < lFitnessVector.size(); ++i) {
//		lMeanF += lFitnessVector[i];
//	}
//	lMeanF /= lFitnessVector.size();
//	lFitness->setValue(lMeanF);	
	
	//Look at the worst test case
	double lMinF = DBL_MAX;
	for(unsigned int i = 0; i < lFitnessVector.size(); ++i) {
		if(lFitnessVector[i] < lMinF) {
			lMinF = lFitnessVector[i];
		}
	}
	lFitness->setValue(lMinF);
	
	return lFitness;
}


/*!
 *  \brief Initialize the evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void DCDCBoostGAEvalOp::initialize(Beagle::System& ioSystem)
{
#ifdef USE_MPI
	Beagle::MPI::EvaluationOp::initialize(ioSystem);
#else
	Beagle::EvaluationOp::initialize(ioSystem);
#endif
	
	if(ioSystem.getRegister().isRegistered("sim.control.generationstep")) {
		mGenerationSteps = castHandleT<FloatArray>(ioSystem.getRegister()["sim.control.generationstep"]);
	} else {
		mGenerationSteps = new FloatArray(1,-1);
		Register::Description lDescription(
										   "Generation for control target transition, -1 mean no transition",
										   "FloatArray",
										   mGenerationSteps->serialize(),
										   "Generation for control target transition"
										   );
		ioSystem.getRegister().addEntry("sim.control.generationstep", mGenerationSteps, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("sim.control.target")) {
		mTargetString = castHandleT<String>(ioSystem.getRegister()["sim.control.target"]);
	} else {
		mTargetString = new String;
		Register::Description lDescription(
										   "Controller target, by pair for each control time",
										   "String",
										   mTargetString->serialize(),
										   "Output target for the controller"
										   );
		ioSystem.getRegister().addEntry("sim.control.target", mTargetString, lDescription);
	}
	
//	if(ioSystem.getRegister().isRegistered("sim.control.tol")) {
//		mTargetTolerances = castHandleT<Float>(ioSystem.getRegister()["sim.control.tol"]);
//	} else {
//		mTargetTolerances = new Float(0.005);
//		Register::Description lDescription(
//										   "Controller target tolerance",
//										   "Float",
//										   mTargetTolerances->serialize(),
//										   "Tank levels tolerance for the controller"
//										   );
//		ioSystem.getRegister().addEntry("sim.control.tol", mTargetTolerances, lDescription);
//	}
	
	if(ioSystem.getRegister().isRegistered("sim.dynamic.initialstate")) {
		mInitialState = castHandleT<FloatArray>(ioSystem.getRegister()["sim.tanks.initialstate"]);
	} else {
		mInitialState = new FloatArray(2,0.0);
		Register::Description lDescription(
										   "Initial state",
										   "Float",
										   mInitialState->serialize(),
										   "Initial state"
										   );
		ioSystem.getRegister().addEntry("sim.dynamic.initialstate", mInitialState, lDescription);
	}
	
//	if(ioSystem.getRegister().isRegistered("sim.control.timestep")) {
//		mDiscreteTimeStep = castHandleT<Float>(ioSystem.getRegister()["sim.control.timestep"]);
//	} else {
//		mDiscreteTimeStep = new Float(1e-3);
//		Register::Description lDescription(
//										   "Controller time step",
//										   "Float",
//										   mDiscreteTimeStep->serialize(),
//										   "Controller time step"
//										   );
//		ioSystem.getRegister().addEntry("sim.control.timestep", mDiscreteTimeStep, lDescription);
//	}
	
	if(ioSystem.getRegister().isRegistered("sim.dynamic.timestep")) {
		mContinuousTimeStep = castHandleT<Float>(ioSystem.getRegister()["sim.dynamic.timestep"]);
	} else {
		mContinuousTimeStep = new Float(1e-4);
		Register::Description lDescription(
										   "Dynamic time step",
										   "Float",
										   mContinuousTimeStep->serialize(),
										   "Dynamic time step"
										   );
		ioSystem.getRegister().addEntry("sim.dynamic.timestep", mContinuousTimeStep, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("sim.duration.time")) {
		mSimulationDuration = castHandleT<Float>(ioSystem.getRegister()["sim.duration.time"]);
	} else {
		mSimulationDuration = new Float(5);
		Register::Description lDescription(
										   "Simulation duration",
										   "Float",
										   mSimulationDuration->serialize(),
										   "Simulation duration"
										   );
		ioSystem.getRegister().addEntry("sim.duration.time", mSimulationDuration, lDescription);
	}
}


/*!
 *  \brief Post-initialize the evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void DCDCBoostGAEvalOp::postInit(Beagle::System& ioSystem)
{
#ifdef USE_MPI
	Beagle::MPI::EvaluationOp::postInit(ioSystem);
#else
	Beagle::EvaluationOp::postInit(ioSystem);
#endif
	
	
	if(*mTargetString == Beagle::String("")) {
		SimulationCase lCase;
		vector<double> lLimits(2);	lLimits[0] = 0.1; lLimits[1] = 0.5;
		vector<double> lTimes(1,0);
		lCase.createRandomCase(&ioSystem.getRandomizer(),NBTANKS,lLimits,lTimes);
		ostringstream lStream;
		lCase.write(lStream);
		mSimulationCases.push_back(lCase);
		mTargetString = new String(lStream.str());
	} 
	
	readSimulationCase(*mTargetString, mSimulationCases);
	
//	for(unsigned int i = 0; i < mSimulationCases.size(); ++i) {
//		std::cout << "Case " << i << ":";
//		mSimulationCases[i].write(std::cout);
//	}
	
	//	if(mSimulationCases.empty()) {
	//		
	//	}
	
	if(mSimulationCases[0].getTime(0) != 0) {
		throw Beagle_RunTimeExceptionM("DCDCBoostGAEvalOp : Not applying control target at time 0");
	}
	
	if(mGenerationSteps->size() > 0) {
		if((*mGenerationSteps)[0] >= 0) {
			if((*mGenerationSteps)[0] != 0) {
				throw Beagle_RunTimeExceptionM("DCDCBoostGAEvalOp : Not applying control target at generation 0");
			}
		}
	}
}

