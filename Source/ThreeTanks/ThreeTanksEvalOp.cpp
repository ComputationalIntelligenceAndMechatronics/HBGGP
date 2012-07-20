/*
 *  ThreeTanksEvalOp.cpp
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

#include "ThreeTanksEvalOp.h"

#include "BGFitness.h"
#include <BondGraph.h>
#include <RootReturn.h>
#include "BGContext.h"
#include "GrowingHybridBondGraph.h"
#include "TreeSTag.h"
#include <cfloat>
#include "ThreeTanksLookaheadController.h"
#include "BGException.h"
#include "ParametersHolder.h"

using namespace Beagle;
using namespace BG;

//#define NBTANKS 3
#define NBOUTPUTS 3

//Beagle::Fitness::Handle ThreeTanksEvalOp::evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext) {
//	Beagle_AssertM(inIndividual.size() == 1);
//	
//	LogFitness *lFitness = new LogFitness;
//	
//	Double lResult = 0;
//	BondGraph* lBondGraph;
//	try{
//		PACC::Matrix lA,lB;
//		vector<double> lInitial;
//	
//		inIndividual.run(lResult, ioContext);
//		BGContext& lContext = castObjectT<BGContext&>(ioContext);
//		lBondGraph = lContext.getBondGraph();
//		lBondGraph->assignCausality();
//
//		lBondGraph->computeStateEquation();
//		lBondGraph->getStateMatrix(lA,lB);
//		
//		lFitness->setValue(ioContext.getSystem().getRandomizer().rollUniform(0,0.9));
//						   
//	} catch(std::runtime_error inError) {
//		std::cerr << "Error catched while evaluating the bond graph: " << inError.what() << std::endl;
//		lFitness->setValue(0);
//		
//		//Save bond graph for debuging
//		std::ostringstream lFilename;
//		lFilename << "bondgraph_bug_" << ioContext.getGeneration() << "_" << ioContext.getIndividualIndex();
//		lBondGraph->plotGraph(lFilename.str()+string(".svg"));
//		ofstream lFileStream((lFilename.str()+string(".xml")).c_str());
//		PACC::XML::Streamer lStreamer(lFileStream);
//		lBondGraph->write(lStreamer);
//	}
//		
//	return lFitness;
//}

ThreeTanksEvalOp::ThreeTanksEvalOp(std::string inName) : BondGraphEvalOp(inName)
{ 
	
}

ThreeTanksEvalOp::~ThreeTanksEvalOp() {
}

/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
//Beagle::Fitness::Handle ThreeTanksEvalOp::evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext) {
//	Beagle_StackTraceBeginM();
//	Beagle_AssertM(inIndividual.size() == 1);
//	
//	BGFitness *lFitness = new BGFitness(0);
//	GrowingHybridBondGraph::Handle lBondGraph;
//	TreeSTag::Handle lTree = castHandleT<TreeSTag>(inIndividual[0]);
//	
//	try {
////		vector<double> lInitial;
//		
//		//Run the individual to create the bond graph.
//		RootReturn lResult;
//		inIndividual.run(lResult, ioContext);
//		BGContext& lContext = castObjectT<BGContext&>(ioContext);
//		lBondGraph = castHandleT<GrowingHybridBondGraph>(lContext.getBondGraph());
//		lFitness->setBondGraph(lBondGraph);
//		
//		Beagle_LogDebugM(
//						 ioContext.getSystem().getLogger(),
//						 "evaluation", "ThreeTanksEvalOp",
//						 std::string("Evaluating bondgrap: ")+
//						 lBondGraph->BondGraph::serialize()
//						 );
//		//lBondGraph->plotGraph("BondGraph_ns.svg");
//		
//		lBondGraph->simplify();	
//		Beagle_LogDebugM(
//						 ioContext.getSystem().getLogger(),
//						 "evaluation", "ThreeTanksEvalOp",
//						 std::string("Evaluating simplified bondgrap: ")+
//						 lBondGraph->BondGraph::serialize()
//						 );
//		/*
//		lBondGraph->plotGraph("BondGraph.svg");
//		ofstream lFilestream("Bondgraph.xml");
//		PACC::XML::Streamer lStreamer(lFilestream);
//		lBondGraph->write(lStreamer);
//		*/
////		lFitness->setValue(ioContext.getSystem().getRandomizer().rollUniform(0,0.9));
////		return lFitness;
//		
//		//Check if the restriction of the number of switch is fullfilled
//		if( (lBondGraph->getSwitches().size() > mMaxNumberSwitch->getWrappedValue()) && (mMaxNumberSwitch->getWrappedValue() != -1) ) {
//			lFitness->setValue(0);
//			return lFitness;
//		}
//		
//		//Evaluate the bond graph
//		//Initialize the simulation
//		std::map<std::string, std::vector<double> > &lLogger = lBondGraph->getSimulationLog();
//		ThreeTanksLookaheadController *lController = dynamic_cast<ThreeTanksLookaheadController*>(lBondGraph->getControllers()[0]);
//		lController->setSimulationDuration(mContinuousTimeStep->getWrappedValue());
//
//#ifndef ALLOW_DIFFCAUSALITY 
//		lBondGraph->setDifferentialCausalitySupport(false);
//#endif
//		
//		double g = 9.81;
//		double lFluidDensity = 998.2;
//		std::vector<double> lInitialLevels(mTanksLevelsIni->size());
//		for(unsigned int i = 0; i< mTanksLevelsIni->size(); ++i) {
//			lInitialLevels[i] = (*mTanksLevelsIni)[i] *(g*lFluidDensity);
//		}
//		
//		try {
//
//			unsigned int lInitialSwitchState = 0;
//			lController->initialize(&(*lBondGraph),lInitialSwitchState,lInitialLevels);
//		 
//			//Simulate every target prior to this generation
//			vector<double> lFitnessVector;
//			unsigned int lTry = 0;
//			for(int g = mSimulationCases.size()-1; g >= 0; --g) {
//				bool lRun = false;
//				if( (*mGenerationSteps)[0] < 0 )
//					lRun = true;
//				else if( ioContext.getGeneration() >= (*mGenerationSteps)[g] )
//					lRun = true;
//				if(lRun) {
//					
//					lLogger.clear();
//					lBondGraph->reset();
//					//try{
//						//lBondGraph->setDifferentialCausalitySupport(true);
//						lController->initialize(&(*lBondGraph),lInitialSwitchState,lInitialLevels);
//						//lBondGraph->setDifferentialCausalitySupport(false);
//					//} catch(BG::CausalityException inError) {}
//					
//					//Run the simulation
//					for(unsigned int i = 0; i < mSimulationCases[g].getSize(); ++i) {
//						if( mSimulationCases[g].getTime(i) >= mSimulationDuration->getWrappedValue() )
//							throw Beagle_RunTimeExceptionM("ThreeTanksEvalOp : Applying control target later than simulation end");
//						if(mSimulationCases[g].getTargets(i).size() != NBTANKS)
//							throw Beagle_RunTimeExceptionM("ThreeTanksEvalOp : There should be a target tripple for each control time");
//						
//						lController->setTarget(mSimulationCases[g].getTargets(i));
//						
//						
//						if(i < mSimulationCases[g].getSize()-1) 
//							lBondGraph->simulate(mSimulationCases[g].getTime(i+1),mContinuousTimeStep->getWrappedValue(),false);
//						else
//							lBondGraph->simulate(mSimulationDuration->getWrappedValue(),mContinuousTimeStep->getWrappedValue(),false);
//					}
//					
//					//Evaluate the results
//					//The mean error on the two tanks is used as a the performance measure
//					std::vector<std::string> lLevelTarget(NBTANKS);
//					std::vector<std::string> lTankLevel(NBTANKS);
//					for(unsigned int i = 0; i < NBTANKS; ++i) {
//						lLevelTarget[i] = std::string("Target_")+int2str(i);
//						lTankLevel[i] = std::string("Level_")+int2str(i);
//					}
//					
//					
//					//////Should be replaced !!!!
//					std::vector<double> lErrors(NBTANKS,0);
//					for(int i = 0; i < lErrors.size(); ++i) {
//						for(int j = 0; j < lLogger[lTankLevel[i]].size()-1; ++j) {
//							double a = lLogger[lTankLevel[i]][j] - lLogger[lLevelTarget[i]][j];
//							double b = lLogger[lTankLevel[i]][j+1] - lLogger[lLevelTarget[i]][j+1];
//							double dt = lLogger["time"][j+1] - lLogger["time"][j];
//							lErrors[i] += abs( (a+b)/2*dt ) * abs( (a+b)/2*dt );;
//						}
//					}
//					
//		
//					//Take the worst of the two tanks
//					double lF = lErrors[0];
//					for(unsigned int i = 1; i < NBTANKS; ++i) {
//						lF = max(lF, lErrors[i]);
//					}
//					
//					if(lF != 0)
//						lF = 1/lF;
//					else
//						lF = DBL_MAX;
//					
//					//Clean logger
//					for(map<std::string, vector<double> >::iterator lIter = lLogger.begin(); lIter != lLogger.end();) {
//						if(lIter->first != "Level_1" &&
//						   lIter->first != "Level_2" &&
//						   lIter->first != "Level_0" &&
//						   lIter->first != "Target_0" &&
//						   lIter->first != "Target_1" &&
//						   lIter->first != "Target_2" &&
//						   lIter->first != "State" &&
//						   lIter->first != "time" &&
//						   lIter->first != "S1" &&
//						   lIter->first != "S2") {
//							lLogger.erase(lIter++);
//						} else {
//							++lIter;		
//						}
//					}
//					
//					//Log simulation data
//					lFitnessVector.push_back(lF);
//					lFitness->addDataSet(lTry, lF);
//					for(map<std::string, std::vector<double> >::const_iterator lIter = lLogger.begin(); lIter != lLogger.end(); ++lIter) {
//						lFitness->addData(lIter->first, lIter->second,lTry);
//					}
//					++lTry;
//				}
//			}
//
//			//Take the average of all test case
//			double lAvg = 0;
//			for(unsigned int i = 0; i < lFitnessVector.size(); ++i) {
//				lAvg += lFitnessVector[i];
//			}
//			lAvg = lAvg/lFitnessVector.size();
//			lFitness->setValue(lAvg);
//
//
//	/*	
//			//Look at the worst test case
//			double lMinF = DBL_MAX;
//			for(unsigned int i = 0; i < lFitnessVector.size(); ++i) {
//				if(lFitnessVector[i] < lMinF) {
//					lMinF = lFitnessVector[i];
//				}
//			}
//			
//			lFitness->setValue(lMinF);
//	*/		
//			Beagle_LogTraceM(
//							 ioContext.getSystem().getLogger(),
//							 "evaluation", "ThreeTanksEvalOp",
//							 std::string("Result of evaluation1: ")+
//							 lFitness->serialize()
//							 );
//			
//		} 
//		catch(BG::CausalityException inError) {
//			lFitness->setValue(0);
//		}
//		
//	}
//	catch(std::runtime_error inError) {
//		std::cerr << "Error catched while evaluating the bond graph: " << inError.what() << std::endl;
//		
//		//Save bond graph for debuging
//		std::ostringstream lFilename;
//		lFilename << "bug/bondgraph_bug_" << ioContext.getGeneration() << "_" << ioContext.getIndividualIndex();
//#ifndef WITHOUT_GRAPHVIZ
//		lBondGraph->plotGraph(lFilename.str()+std::string(".svg"));
//#endif
//		ofstream lFileStream((lFilename.str()+std::string(".xml")).c_str());
//		PACC::XML::Streamer lStreamer(lFileStream);
//		lBondGraph->write(lStreamer);
//		inIndividual.write(lStreamer);
//		
//		//Assign null fitness
//		lFitness->setValue(0);
//		
//		
//		
//#ifdef STOP_ON_ERROR
//		exit(EXIT_FAILURE);
//#endif
//    }
//	
//	Beagle_LogTraceM(
//					 ioContext.getSystem().getLogger(),
//					 "evaluation", "ThreeTanksEvalOp",
//					 std::string("Result of evaluation: ")+
//					 lFitness->serialize()
//					 );
//	
//	//delete lBondGraph;
//	return lFitness;
//	Beagle_StackTraceEndM("void ThreeTanksEvalOp::evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext)");
//}

Beagle::Fitness::Handle ThreeTanksEvalOp::evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext) {
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inIndividual.size() == 1);
	
	
	BGFitness *lFitness = new BGFitness(-1);
	GrowingHybridBondGraph::Handle lBondGraph;
	TreeSTag::Handle lTree = castHandleT<TreeSTag>(inIndividual[0]);
	
	
	
	try {
//		//Get the parameters
//		Beagle::Component::Handle lHolderComponent = ioContext.getSystem().getComponent("ParametersHolder");
//		if(lHolderComponent==NULL)
//			throw Beagle_RunTimeExceptionM("No ParameterHolder holder component found in the system!");
//		ParametersHolder::Handle lHolder = castHandleT<ParametersHolder>(lHolderComponent);
//		if(lHolder==NULL)
//			throw Beagle_RunTimeExceptionM("Component named \"ParameterHolder\" found is not of the good type!");
//		lHolder->clear();
		
		//Run the individual to create the bond graph.
		RootReturn lResult;
		inIndividual.run(lResult, ioContext);
		BGContext& lContext = castObjectT<BGContext&>(ioContext);
		lBondGraph = castHandleT<GrowingHybridBondGraph>(lContext.getBondGraph());
		
		GrowingHybridBondGraph::Handle lSaveNonSimplifiedBondGraph = new GrowingHybridBondGraph;
		*lSaveNonSimplifiedBondGraph = *lBondGraph;
		lFitness->setBondGraph(lSaveNonSimplifiedBondGraph);	
		
		Beagle_LogDebugM(
						 ioContext.getSystem().getLogger(),
						 "evaluation", "ThreeTanksEvalOp",
						 std::string("Evaluating bondgrap: ")+
						 lBondGraph->BondGraph::serialize()
						 );
		
#ifdef DEBUG
		ofstream lFilestream("Bondgraph_ns.xml");
		PACC::XML::Streamer lStreamer(lFilestream);
		lBondGraph->write(lStreamer);
		inIndividual.write(lStreamer);
		lBondGraph->plotGraph("BondGraph_ns.svg");
#endif
		lBondGraph->simplify();	
		lFitness->setSimplifiedBondGraph(lBondGraph);
		//lFitness->setBondGraph(lBondGraph);
		
		/*///////////////////
		if( ioContext.getSystem().getRandomizer().rollUniform(0,0.999) > 0.5 )
			lFitness->setValue(0);
		else
			lFitness->setValue(ioContext.getSystem().getRandomizer().rollUniform(0,0.999));
		return lFitness;
		/*//////////////////
		
		
		Beagle_LogDebugM(
						 ioContext.getSystem().getLogger(),
						 "evaluation", "DCDCBoostEvalOp",
						 std::string("Evaluating simplified bondgrap: ")+
						 lBondGraph->BondGraph::serialize()
						 );
#ifdef DEBUG
		lBondGraph->plotGraph("BondGraph.svg");
		ofstream lFilestream2("Bondgraph.xml");
		PACC::XML::Streamer lStreamer2(lFilestream2);
		lBondGraph->write(lStreamer2);
		inIndividual.write(lStreamer2);
		plotIndividual(inIndividual,"Individual.svg");
#endif
		
		//Check if the restriction of the number of switch is fullfilled
		if( (lBondGraph->getSwitches().size() > mMaxNumberSwitch->getWrappedValue()) && (mMaxNumberSwitch->getWrappedValue() != -1) ) {
			lFitness->setValue(0);
			return lFitness;
		}
		
		//Evaluate the bond graph
		//Initialize the simulation
		std::map<std::string, std::vector<double> > &lLogger = lBondGraph->getSimulationLog();
		ThreeTanksLookaheadController *lController = dynamic_cast<ThreeTanksLookaheadController*>(lBondGraph->getControllers()[0]);
		lController->setSimulationDuration(mContinuousTimeStep->getWrappedValue());
		
		if(mAllowDifferentialCausality->getWrappedValue() <= 1) {
			lBondGraph->setDifferentialCausalitySupport(false);
		}
		
		try {
			double g = 9.81;
			double lFluidDensity = 998.2;
			std::vector<double> lInitialLevels(mTanksLevelsIni->size());
			for(unsigned int i = 0; i< mTanksLevelsIni->size(); ++i) {
				lInitialLevels[i] = (*mTanksLevelsIni)[i] *(g*lFluidDensity);
			}
			
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
							throw Beagle_RunTimeExceptionM("ThreeTanksEvalOp : Applying control target later than simulation end");
						if(mSimulationCases[g].getTargets(i).size() != NBOUTPUTS)
							throw Beagle_RunTimeExceptionM("ThreeTanksEvalOp : There should be 1 target value for each control time");
//						if(mSimulationCases[g].getParameters(i).size() != NBPARAMETERS)
//							throw Beagle_RunTimeExceptionM("ThreeTanksEvalOp : There should be 1 parameter value for each control time");
//						
//						//Assign parameters
//						const vector<double>& lParameters = mSimulationCases[g].getParameters(i);
//						if(!lParameters.empty()) {
//							lBondGraph->clearStateMatrix();
//						}
//						assert(lHolder->size() == lParameters.size());
//						for(unsigned int k = 0; k < lParameters.size(); ++k) {
//							(*lHolder)[k]->setValue(lParameters[k]);
//						}
						
						//Set targets
						vector<double> lTargets = mSimulationCases[g].getTargets(i);
						lController->setTarget(lTargets);
						
						if(i == 0) {
							//Find a valid initial state
							unsigned int lInitialSwitchState = 0;
							double lMaxConfiguration = pow(2.0,int(lBondGraph->getSwitches().size()));
							for(;lInitialSwitchState < lMaxConfiguration; ++lInitialSwitchState) { 
								try{ 
									lController->initialize(&(*lBondGraph),lInitialSwitchState,lInitialLevels);
								} catch(BG::CausalityException inError) {
									continue;
								} 
								
								break;
							}
							
							//Check if a valid initial state have been found
							if(lInitialSwitchState == lMaxConfiguration) {
								if(mAllowDifferentialCausality->getWrappedValue() == 2) {
									lBondGraph->setDifferentialCausalitySupport(true);
									lInitialSwitchState = 0;
								} else {
									lSimulationRan = false;
									throw BG::CausalityException("No initial state found!");
									break;
								}
							}
							
							//Reset the bond graph
							lLogger.clear();
							lBondGraph->reset();
						}
						
#ifndef NOSIMULATION
						//Run the simulation
						lSimulationRan = true;
						if(i < mSimulationCases[g].getSize()-1) 
							lBondGraph->simulate(mSimulationCases[g].getTime(i+1),mContinuousTimeStep->getWrappedValue(),false);
						else
							lBondGraph->simulate(mSimulationDuration->getWrappedValue(),mContinuousTimeStep->getWrappedValue(),false);
#endif
						
					}
					
					//Evaluate the results
					if(lSimulationRan) {
						lF = computeError(&(*lBondGraph),lLogger);
						
						
						
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
							   lIter->first != "S1" &&
							   lIter->first != "S2") {
								lLogger.erase(lIter++);
							} else {
								++lIter;		
							}
						}
						//lBondGraph->writeSimulationLog(std::string("DCDCBoost_Lookahead_")+uint2str(g)+std::string(".csv"));
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
			
			/*	
			 //Look at the worst test case
			 double lMinF = DBL_MAX;
			 for(unsigned int i = 0; i < lFitnessVector.size(); ++i) {
			 if(lFitnessVector[i] < lMinF) {
			 lMinF = lFitnessVector[i];
			 }
			 }
			 
			 lFitness->setValue(lMinF);
			 */		
			//			Beagle_LogDebugM(
			//							 ioContext.getSystem().getLogger(),
			//							 "evaluation", "ThreeTanksEvalOp",
			//							 std::string("Result of evaluation: ")+
			//							 lFitness->serialize()
			//							 );
			
		} 
		catch(BG::CausalityException inError) {
			lFitness->setValue(0);
			Beagle_LogDetailedM(
							 ioContext.getSystem().getLogger(),
							 "evaluation", "ThreeTanksEvalOp",
							 std::string("Error occured during evaluation of fitness: ")+
							 inError.what()
							 );
		}
		
	}
	catch(std::runtime_error inError) {
		std::cerr << "Error catched while evaluating the bond graph: " << inError.what() << std::endl;
		
		//Save bond graph for debuging
		std::ostringstream lFilename;
		lFilename << "bug/bondgraph_bug_" << ioContext.getGeneration() << "_" << mIndividualCounter++;//ioContext.getIndividualIndex();
#ifndef WITHOUT_GRAPHVIZ
		lBondGraph->plotGraph(lFilename.str()+std::string(".svg"));
#endif
		ofstream lFileStream((lFilename.str()+std::string(".xml")).c_str());
		PACC::XML::Streamer lStreamer(lFileStream);
		lBondGraph->write(lStreamer);
		inIndividual.write(lStreamer);
		lFileStream << endl;	
		//Assign null fitness
		lFitness->setValue(0);
		
#ifdef STOP_ON_ERROR
		exit(EXIT_FAILURE);
#endif
    }
	
	
	
	//delete lBondGraph;
	
#ifdef NOSIMULATION
	lFitness->setValue(ioContext.getSystem().getRandomizer().rollUniform(0,0.999));
#endif
	Beagle_LogDebugM(
					 ioContext.getSystem().getLogger(),
					 "evaluation", "ThreeTanksEvalOp",
					 std::string("Result of evaluation: ")+
					 lFitness->serialize()
					 );
	
	Beagle_LogTraceM(
					 ioContext.getSystem().getLogger(),
					 "evaluation", "ThreeTanksEvalOp",
					 std::string("Result of evaluation: ")+
					 dbl2str(lFitness->getValue())
					 );
	
	return lFitness;
	
	Beagle_StackTraceEndM("void ThreeTanksEvalOp::evaluate(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext)");
}

double ThreeTanksEvalOp::computeError(const BondGraph* inBondGraph, std::map<std::string, std::vector<double> > &inSimulationLog) {
	
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
			
//			if(lOutput[i] != 0) 
//				lZeroOutput[k] = false;
		}
		
		//Integrate the error vector
		for(unsigned int i = 0; i < lDataSize-1; ++i) {
			double dt = lTime[i+1] - lTime[i];
			lErrors[k] += (lError[i]+lError[i+1])/2*dt;
		}
	}
	
	
	//	//Take the mean errors of the targets
	//	double lF = 0;
	//	for(unsigned int k = 0; k < lErrors.size(); ++k) {
	//		lF += lErrors[k];
	//	}
	//	lF = lF/lErrors.size();
	
	//Take the worst errors
	double lF = lErrors[0];
	for(unsigned int k = 1; k < lErrors.size(); ++k) {
		lF = max(lF,lErrors[k]);
	}
	
	if(lF != 0)
		lF = 1/lF;
	else
		lF = DBL_MAX;
	
//	//Look if the output is null
//	bool lZero = true;
//	for(unsigned int i = 0 ; i < lZeroOutput.size(); ++i) {
//		lZero = lZero && lZeroOutput[i];
//		if(!lZero)
//			break;
//	}
//	if(lZero)
//		lF = 0;
	
	return lF;		
}


void ThreeTanksEvalOp::initialize(Beagle::System& ioSystem) {
#ifdef USE_MPI
	Beagle::MPI::EvaluationOp::initialize(ioSystem);
#else
	Beagle::EvaluationOp::initialize(ioSystem);
#endif
	
	PACC::XML::Streamer lStreamer(std::cout);
	ioSystem.getRegister().write(lStreamer,true);
	
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
	
	if(ioSystem.getRegister().isRegistered("sim.dynamic.levelini")) {
		mTanksLevelsIni = castHandleT<FloatArray>(ioSystem.getRegister()["sim.tanks.levelini"]);
	} else {
		mTanksLevelsIni = new FloatArray(3,0.25);
		Register::Description lDescription(
										   "Initial tank levels",
										   "Float",
										   mTanksLevelsIni->serialize(),
										   "Initial tank levels"
										   );
		ioSystem.getRegister().addEntry("sim.dynamic.levelini", mTanksLevelsIni, lDescription);
	}
	
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
		mSimulationDuration = new Float(15);
		Register::Description lDescription(
										   "Simulation duration",
										   "Float",
										   mSimulationDuration->serialize(),
										   "Simulation duration"
										   );
		ioSystem.getRegister().addEntry("sim.duration.time", mSimulationDuration, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("bg.max.switch")) {
		mMaxNumberSwitch = castHandleT<Int>(ioSystem.getRegister()["bg.max.switch"]);
	} else {
		mMaxNumberSwitch = new Int(-1);
		Register::Description lDescription(
										   "Maximum number of switch allowed in the bond graph, -1 is unlimited",
										   "Int",
										   mTargetString->serialize(),
										   "Maximum number of switch"
										   );
		ioSystem.getRegister().addEntry("bg.max.switch", mMaxNumberSwitch, lDescription);
	}
}


/*!
 *  \brief Post-initialize the evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void ThreeTanksEvalOp::postInit(Beagle::System& ioSystem)
{
#ifdef USE_MPI
	Beagle::MPI::EvaluationOp::postInit(ioSystem);
#else
	Beagle::EvaluationOp::postInit(ioSystem);
#endif
	
	ioSystem.addComponent(new ParametersHolder);
	
	if(*mTargetString == Beagle::String("")) {
		SimulationCase lCase;
		vector<double> lLimits(2);	lLimits[0] = 0.1; lLimits[1] = 0.5;
		vector<double> lTimes(1,0);
		lCase.createRandomCase(&ioSystem.getRandomizer(),NBOUTPUTS,lLimits,lTimes);
		ostringstream lStream;
		lCase.write(lStream);
		mSimulationCases.push_back(lCase);
		mTargetString = new String(lStream.str());
	} 
	
	readSimulationCase(*mTargetString, mSimulationCases);
	
	if(mSimulationCases[0].getTime(0) != 0) {
		throw Beagle_RunTimeExceptionM("ThreeTanksEvalOp : Not applying control target at time 0");
	}
	
	if(mGenerationSteps->size() > 0) {
		if((*mGenerationSteps)[0] >= 0) {
			if((*mGenerationSteps)[0] != 0) {
				throw Beagle_RunTimeExceptionM("ThreeTanksEvalOp : Not applying control target at generation 0");
			}
		}
	}
}


