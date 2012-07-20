/*
 *  DCDCBoost2xGAEvalOp.cpp
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

#include "DCDCBoost2xGAEvalOp.h"
#include "beagle/GA.hpp"
#include "LogFitness.h"
#include "DCDCBoost2xGAController.h"
//#include "DCDCBoost2xGAManualController.h"
#include "Simulator.h"

#include "BGException.h"
#include <cmath>
#include <cfloat>
#include <vector>
#include <map>
#include "GrowingHybridBondGraph.h"
#include <assert.h>
#include "VectorUtil.h"

#define NBOUTPUTS 3
#define NBPARAMETERS 3

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator.
 */
DCDCBoost2xGAEvalOp::DCDCBoost2xGAEvalOp() :
#ifdef USE_MPI
MPI::EvaluationOp("DCDCBoost2xGAEvalOp")
#else
Beagle::EvaluationOp("DCDCBoost2xGAEvalOp")
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
	
	mIndividualCounter = 0;
}

Beagle::Fitness::Handle DCDCBoost2xGAEvalOp::evaluate(Beagle::Individual& inIndividual, Beagle::Context& ioContext) {
	Beagle_StackTraceBeginM();
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
	DCDCBoost2xGAController *lController = new DCDCBoost2xGAController;
	//DCDCBoost2xGAManualController *lController = new DCDCBoost2xGAManualController;
	HybridBondGraph lBondGraph;
	lController->createBondGraph(lBondGraph);
	
	LogFitness *lFitness = new LogFitness(-1);
	//BGFitness *lFitness = new BGFitness(-1);
	
	try {		
		//Check if the restriction of the number of switch is fullfilled
		if( (lBondGraph.getSwitches().size() > mMaxNumberSwitch->getWrappedValue()) && (mMaxNumberSwitch->getWrappedValue() != -1) ) {
			lFitness->setValue(0);
			return lFitness;
		}
		
		//Evaluate the bond graph
		//Initialize the simulation
		std::map<std::string, std::vector<double> > &lLogger = lBondGraph.getSimulationLog();
		lLogger.clear();
		
		if(mAllowDifferentialCausality->getWrappedValue() <= 1) {
			lBondGraph.setDifferentialCausalitySupport(false);
		}
		
		try {
			std::vector<double> lInitialOutput(1,0);
			
			//Simulate every target prior to this generation
			vector<double> lFitnessVector;
			unsigned int lTry = 0;
			for(int g = mSimulationCases.size()-1; g >= 0; --g) {
				bool lRun = false;
				if( (*mGenerationSteps)[0] < 0 )
					lRun = true;
				else if( ioContext.getGeneration() >= (*mGenerationSteps)[g] )
					lRun = true;
				if(lRun) {
					
					double lF = 0;
					bool lSimulationRan = false;
					
					for(unsigned int i = 0; i < mSimulationCases[g].getSize(); ++i) {
						
						if( mSimulationCases[g].getTime(i) >= mSimulationDuration->getWrappedValue() )
							throw Beagle_RunTimeExceptionM("DCDCBoostEvalOp : Applying control target later than simulation end");
						if(mSimulationCases[g].getTargets(i).size() != NBOUTPUTS-1)
							throw Beagle_RunTimeExceptionM("DCDCBoostEvalOp : There should be 1 target value for each control time");
						if(mSimulationCases[g].getParameters(i).size() != NBPARAMETERS)
							throw Beagle_RunTimeExceptionM("DCDCBoostEvalOp : There should be 1 parameter value for each control time");
						
						//Assign parameters
						const vector<double>& lParameters = mSimulationCases[g].getParameters(i);
						if(!lParameters.empty()) {
							lBondGraph.clearStateMatrix();
						}

						assert(lController->getParametricComponents().size() == lParameters.size());
						for(unsigned int k = 0; k < lParameters.size(); ++k) {
							(lController->getParametricComponents())[k]->setValue(lParameters[k]);
						}
						
						//Compute the current target
						vector<double> lTargets = mSimulationCases[g].getTargets(i);
						double lCurrentTarget = 0;
						for(unsigned int k = 0; k < lTargets.size(); ++k) {
							lCurrentTarget += lTargets[k]*lTargets[k]/(lParameters[0]*lParameters[k+1]);
						}
						lTargets.push_back(lCurrentTarget);
						lController->setTarget(lTargets);
						
						if(i == 0) {
							//Find a valid initial state						
							std::vector<double> lInitialStates(mInitialState->size());
							for(unsigned int i = 0; i< mInitialState->size(); ++i) {
								lInitialStates[i] = (*mInitialState)[i];
							}

							lController->initialize(&lBondGraph,lStartState,lInitialStates);
							lController->setTransitionTable(lTransitionTable);
							
							//Reset the bond graph
							lLogger.clear();
							lBondGraph.reset();
							
							//Remove transition state when test hand writen individual
							vector<double> lInitialState(3,0);
//							lInitialState[0] = 150e-6*lCurrentTarget;
//							lInitialState[1] = 800e-6*(lTargets[0]);
//							lInitialState[2] = 146.6e-6*(lTargets[1]);
							lBondGraph.setInitialStateVariable(lInitialState);
						}
						
#ifndef NOSIMULATION
						//Run the simulation
						lSimulationRan = true;
						if(i < mSimulationCases[g].getSize()-1) 
							lBondGraph.simulate(mSimulationCases[g].getTime(i+1),mContinuousTimeStep->getWrappedValue(),false);
						else
							lBondGraph.simulate(mSimulationDuration->getWrappedValue(),mContinuousTimeStep->getWrappedValue(),false);
#endif
						
					}
					
					//Evaluate the results
					if(lSimulationRan) {
						lF = computeError(&(lBondGraph),lLogger);
						
						//Clean logger
						for(map<std::string, vector<double> >::iterator lIter = lLogger.begin(); lIter != lLogger.end();) {
							if(lIter->first != "Output_0" &&
							   lIter->first != "Output_1" &&
							   lIter->first != "Output_2" &&
							   lIter->first != "Target_0" &&
							   lIter->first != "Target_1" &&
							   lIter->first != "Target_2" &&
							   lIter->first != "State" &&
							   lIter->first != "time" &&
							   lIter->first != "ControlSymbols" &&
							   lIter->first != "S1") {
								lLogger.erase(lIter++);
							} else {
								++lIter;		
							}
						}
						lBondGraph.writeSimulationLog(std::string("DCDCBoost2xGA_testcase_")+uint2str(g)+std::string(".csv"));
					} else {
						lF = 0;
					}
								
					//Log simulation data
					lFitnessVector.push_back(lF);
					lFitness->addDataSet(lTry, lF);
					for(map<std::string, std::vector<double> >::const_iterator lIter = lLogger.begin(); lIter != lLogger.end(); ++lIter) {
						lFitness->addData(lIter->first, lIter->second,lTry);
					}
					++lTry;
				}
			}
			
			//Take the average of all test case
			double lAvg = 0;
			for(unsigned int i = 0; i < lFitnessVector.size(); ++i) {
				lAvg += lFitnessVector[i];
			}
			lAvg = lAvg/lFitnessVector.size();
			lFitness->setValue(lAvg);
			
//			 //Look at the worst test case
//			double lMinF = DBL_MAX;
//			for(unsigned int i = 0; i < lFitnessVector.size(); ++i) {
//				if(lFitnessVector[i] < lMinF) {
//					lMinF = lFitnessVector[i];
//				}
//			}
//			 lFitness->setValue(lMinF);
			
			Beagle_LogTraceM(
							 ioContext.getSystem().getLogger(),
							 "evaluation", "DCDCBoostEvalOp",
							 std::string("Result of evaluation1: ")+
							 lFitness->serialize()
							 );
			
		} 
		catch(BG::CausalityException inError) {
			lFitness->setValue(0);
		}
		
	}
	catch(std::runtime_error inError) {
		std::cerr << "Error catched while evaluating the bond graph: " << inError.what() << std::endl;
		
		//Save bond graph for debuging
		std::ostringstream lFilename;
		lFilename << "bug/bondgraph_bug_" << ioContext.getGeneration() << "_" << mIndividualCounter++;//ioContext.getIndividualIndex();
#ifndef WITHOUT_GRAPHVIZ
		lBondGraph.plotGraph(lFilename.str()+std::string(".svg"));
#endif
		ofstream lFileStream((lFilename.str()+std::string(".xml")).c_str());
		PACC::XML::Streamer lStreamer(lFileStream);
		lBondGraph.write(lStreamer);
		inIndividual.write(lStreamer);
		lFileStream << endl;	
		//Assign null fitness
		lFitness->setValue(0);
		
#ifdef STOP_ON_ERROR
		exit(EXIT_FAILURE);
#endif
    }
	
	Beagle_LogTraceM(
					 ioContext.getSystem().getLogger(),
					 "evaluation", "DCDCBoostEvalOp",
					 std::string("Result of evaluation: ")+
					 lFitness->serialize()
					 );
	
	//delete lBondGraph;
	
#ifdef NOSIMULATION
	lFitness->setValue(ioContext.getSystem().getRandomizer().rollUniform(0,0.999));
#endif
	
	return lFitness;
	
	Beagle_StackTraceEndM("void DCDCBoostEvalOp::evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext)");
}

double DCDCBoost2xGAEvalOp::computeError(const BondGraph* inBondGraph, std::map<std::string, std::vector<double> > &inSimulationLog) {
	
	std::vector<double> lErrors(NBOUTPUTS,0);
	std::vector<bool> lZeroOutput(NBOUTPUTS,true);
	for(unsigned int k = 0; k < NBOUTPUTS; ++k) {
		const std::vector<double>& lTarget = inSimulationLog[std::string("Target_")+int2str(k)];
		std::vector<double>& lOutput = inSimulationLog[std::string("Output_")+int2str(k)];
		const std::vector<double>& lTime = inSimulationLog["time"];
		
		unsigned int lDataSize = lTime.size();
		assert(lTime.size() == lOutput.size());
		assert(lTime.size() == lTarget.size());
		assert(lDataSize != 0);
		
		std::vector<double> lError(lDataSize);
		for(unsigned int i = 0; i < lDataSize; ++i) {
			lError[i] = fabs(lOutput[i] - lTarget[i])/lTarget[i];// * lTime[i]/lTime.back()*2;
			
			if(lOutput[i] != 0) 
				lZeroOutput[k] = false;
			
//			if( k < 2 ) 
//				lError[i] = fabs(lOutput[i] - lTarget[i])/lTarget[i];	
//			else {
//				//Compute the error on the current by making sure it doesn't exceed a limit
//				double lCurrentLimit = 2;
//				lError[i] = (lOutput[i] - lCurrentLimit)/lCurrentLimit;
//				if( lError[i] < 0 )
//					lError[i] = 0;
//				else
//					lError[i] = fabs(lError[i] * 10); //If over the limits, greatly penalise
//			}
			
		}
		
		//Integrate the error vector
		for(unsigned int i = 0; i < lDataSize-1; ++i) {
			double dt = lTime[i+1] - lTime[i];
			lErrors[k] += (lError[i]+lError[i+1])/2*dt;
		}
	}
	
	
	//	//Take the mean errors of the two targets
	//	double lF = 0;
	//	for(unsigned int k = 0; k < lErrors.size(); ++k) {
	//		lF += lErrors[k];
	//	}
	//	lF = lF/lErrors.size();
	
	//cout << "Errors: " << lErrors << endl;
	
	//Take the worst 
	double lF = lErrors[0];
	for(unsigned int k = 1; k < lErrors.size(); ++k) {
		lF = max(lF,lErrors[k]);
	}
	
	//	//Look at the worst of the output voltage, disregard the current
	//	double lF = max(lErrors[0],lErrors[1]);
	
	
	
	if(lF != 0)
		lF = 1/lF;
	else
		lF = DBL_MAX;
	
	//Look if the output is null
	bool lZero = true;
	for(unsigned int i = 0 ; i < lZeroOutput.size(); ++i) {
		lZero = lZero && lZeroOutput[i];
		if(!lZero)
			break;
	}
	if(lZero)
		lF = 0;
	
	return lF;		
}


/*!
 *  \brief Initialize the evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void DCDCBoost2xGAEvalOp::initialize(Beagle::System& ioSystem)
{
#ifdef USE_MPI
	Beagle::MPI::EvaluationOp::initialize(ioSystem);
#else
	Beagle::EvaluationOp::initialize(ioSystem);
#endif
	
	if(ioSystem.getRegister().isRegistered("bg.allow.diffcausality")) {
		mAllowDifferentialCausality = castHandleT<Int>(ioSystem.getRegister()["bg.allow.diffcausality"]);
	} else {
		mAllowDifferentialCausality = new Int(0);
		Register::Description lDescription(
										   "Allow differential causality in the bond graph",
										   "Int",
										   mAllowDifferentialCausality->serialize(),
										   "Allow differential causality in the bond graph, 0: No causality allowed, 1: Only if no configuration whithout diff causality, 2: All causality allowed."
										   );
		ioSystem.getRegister().addEntry("bg.allow.diffcausality", mAllowDifferentialCausality, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("bg.max.switch")) {
		mMaxNumberSwitch = castHandleT<Int>(ioSystem.getRegister()["bg.max.switch"]);
	} else {
		mMaxNumberSwitch = new Int(-1);
		Register::Description lDescription(
										   "Maximum number of switch allowed in the bond graph, -1 is unlimited",
										   "Int",
										   mMaxNumberSwitch->serialize(),
										   "Maximum number of switch"
										   );
		ioSystem.getRegister().addEntry("bg.max.switch", mMaxNumberSwitch, lDescription);
	}
	
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
										   "Tank levels target for the controller"
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
		mInitialState = castHandleT<FloatArray>(ioSystem.getRegister()["sim.dynamic.initialstate"]);
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
void DCDCBoost2xGAEvalOp::postInit(Beagle::System& ioSystem)
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
		lCase.createRandomCase(&ioSystem.getRandomizer(),2,lLimits,lTimes);
		ostringstream lStream;
		lCase.write(lStream);
		mSimulationCases.push_back(lCase);
		mTargetString = new String(lStream.str());
	} 
	
	readSimulationCase(*mTargetString, mSimulationCases);
	
	
	
	if(mSimulationCases[0].getTime(0) != 0) {
		throw Beagle_RunTimeExceptionM("DCDCBoost2xGAEvalOp : Not applying control target at time 0");
	}
	
	if(mGenerationSteps->size() > 0) {
		if((*mGenerationSteps)[0] >= 0) {
			if((*mGenerationSteps)[0] != 0) {
				throw Beagle_RunTimeExceptionM("DCDCBoost2xGAEvalOp : Not applying control target at generation 0");
			}
		}
	}
	
}

