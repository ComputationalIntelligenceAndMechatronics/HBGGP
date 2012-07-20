/*
 *  AnalogFilterParameterEvalOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 19/08/09.
 */

#include "AnalogFilterParameterEvalOp.h"
#include "BGFitness.h"
#include <BondGraph.h>
#include <RootReturn.h>
#include "BGContext.h"
#include "libAnalogFilterDesignEval.h"
#include "SpeciesGA.h"
#include "TreeSTag.h"

using namespace Beagle;
using namespace BG;



AnalogFilterParameterEvalOp::AnalogFilterParameterEvalOp(std::string inName) : BondGraphParameterEvalOp(inName)
{ 
}

AnalogFilterParameterEvalOp::~AnalogFilterParameterEvalOp() {

}

/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
Beagle::Fitness::Handle AnalogFilterParameterEvalOp::evaluate(Beagle::Individual& inIndividual, Beagle::Context& ioContext) {
	Beagle_AssertM(inIndividual.size() == 1);
	
	Beagle_LogVerboseM(
					   ioContext.getSystem().getLogger(),
					   "evaluation", "AnalogFilterParameterEvalOp",
					   std::string("Evaluating individual: ")+
					   inIndividual.serialize()
					   );
	
	BGFitness *lFitness = new BGFitness(0);
	GrowingBondGraph::Handle lBondGraph;
	
	GA::FloatVector::Handle lParametersVector = castHandleT<GA::FloatVector>(inIndividual[0]);
	
	BGContext& lBGContext = castObjectT<BGContext&>(ioContext);
	
	SpeciesGA& lSpecies = castObjectT<SpeciesGA&>(ioContext.getDeme());
	RepresentantGP::Handle lIndividual = castHandleT<RepresentantGP>(lSpecies.getRepresentant());
	
	TreeSTag::Handle lTree = castHandleT<TreeSTag>((*lIndividual)[0]);
//	lTree->assignNewParameterVector(lParametersVector, lBGContext);
	lBondGraph = lTree->getBondGraph();
	lBondGraph->assignParameters(*lParametersVector);
	
	
	lIndividual->getFitness()->setInvalid();
	
	Beagle_LogDebugM(
					   ioContext.getSystem().getLogger(),
					   "evaluation", "AnalogFilterParameterEvalOp",
					   std::string("Individual after parameter assignment: ")+
					   lIndividual->serialize()
					   );

	try {
		vector<double> lInitial;
		
		//Run the individual to create the bond graph.
//		RootReturn lResult;
//		
//		lBGContext.setIndividualHandle(lIndividual);
//		lIndividual->run(lResult, lBGContext);
//		lBondGraph = lBGContext.getBondGraph();
//		lTree->setBondGraph(lBondGraph);
//		
//		//Set output bond
//		Bond* lOutputBond = lBondGraph->getComponents()[lResult.getValue()-1]->getPorts()[0]->getBond();
//		lBondGraph->setOutputBonds(lOutputBond, 0);
		
		Beagle_LogDebugM(
						 ioContext.getSystem().getLogger(),
						 "evaluation", "AnalogFilterParameterEvalOp",
						 std::string("Evaluating bondgrap: ")+
						 lBondGraph->BondGraph::serialize()
						 );
		//lBondGraph->simplify();		
		
		//Get state equations
		lBondGraph->assignCausality();
		lBondGraph->computeStateEquation();
		PACC::Matrix lA,lB,lB2,lC,lD,lD2;
		lBondGraph->getStateMatrix(lA,lB,lB2);
		lBondGraph->getOutputMatrix(lC,lD,lD2);
		
		lFitness->addStateMatrix(lA);
		lFitness->addStateMatrix(lB);
		lFitness->addStateMatrix(lC);
		lFitness->addStateMatrix(lD);
		
		//Check to see if the system is LTI
		if(lBondGraph->hasDeferentialCausality()) {
			//lFitness->setValue(ioContext.getSystem().getRandomizer().getFloat());
			lFitness->setValue(0);
			//delete lBondGraph;
			return lFitness;
		} else {
#ifndef DEBUG_NOMATLAB
			//Evalute the response in Matlab.
			
			// Cast the state matrix as input data
			// The mwArray::SetData is copying in column major order and the PACC::Matrix is a row major order
			// Therefore, the matrix need to be transposed.
			PACC::Matrix lAt,lBt,lCt,lDt;
			if(!lA.empty())
				lAt = lA.transpose();
			if(!lB.empty())
				lBt = lB.transpose();
			if(!lC.empty())
				lCt = lC.transpose();
			if(!lD.empty())
				lDt = lD.transpose();
			
			double *lValueA = new double[lAt.size()];
			std::copy(lAt.begin(), lAt.end(), lValueA);
			
			mwArray lArrayA(lAt.getCols(),lAt.getRows(), mxDOUBLE_CLASS, mxREAL);
			lArrayA.SetData(lValueA,lAt.size());
			
			double *lValueB = new double[lBt.size()];
			std::copy(lBt.begin(), lBt.end(), lValueB);
			mwArray lArrayB(lBt.getCols(),lBt.getRows(), mxDOUBLE_CLASS, mxREAL);
			lArrayB.SetData(lValueB,lBt.size());
			
			double *lValueC = new double[lCt.size()];
			std::copy(lCt.begin(), lCt.end(), lValueC);
			mwArray lArrayC(lCt.getCols(),lCt.getRows(), mxDOUBLE_CLASS, mxREAL);
			lArrayC.SetData(lValueC,lCt.size());
			
			double *lValueD = new double[lDt.size()];
			std::copy(lDt.begin(), lDt.end(), lValueD);
			mwArray lArrayD(lDt.getCols(),lDt.getRows(), mxDOUBLE_CLASS, mxREAL);
			lArrayD.SetData(lValueD,lDt.size());
			
		
			// Create output array
			mwArray loutArray;
			
			// Call the library function
			
			AnalogFilterEval(1, loutArray, lArrayA, lArrayB, lArrayC,lArrayD);
			
			// Extract the output
			int lNbOutput = loutArray.NumberOfElements();
			
			double* loutValues = new double[lNbOutput];
			loutArray.GetData(loutValues, lNbOutput);
			
			// Bundle the fitness
			lFitness->setValue(loutValues[0]);
			
						
			delete [] lValueA;
			delete [] lValueB;
			delete [] lValueC;
			delete [] lValueD;
			delete [] loutValues;
#else
			lFitness->setValue(ioContext.getSystem().getRandomizer().getFloat());
#endif

		}
		
	}
    catch (const mwException& inException)  {
        std::cerr << inException.what() << std::endl;
        
		PACC::Matrix lA,lB,lB2,lC,lD,lD2;
		lBondGraph->getStateMatrix(lA,lB,lB2);
		lBondGraph->getOutputMatrix(lC,lD,lD2);
		PACC::XML::Streamer lStream(cerr);
		lA.write(lStream); cerr << endl;
		lB.write(lStream); cerr << endl;
		lC.write(lStream); cerr << endl;
		lD.write(lStream); cerr << endl;
		
		//Save bond graph for debuging
		std::ostringstream lFilename;
		lFilename << "bug/bondgraph_bug_" << ioContext.getGeneration() << "_" << ioContext.getIndividualIndex();
#ifndef WITHOUT_GRAPHVIZ
		lBondGraph->plotGraph(lFilename.str()+std::string(".svg"));
#endif
		ofstream lFileStream((lFilename.str()+std::string(".xml")).c_str());
		PACC::XML::Streamer lStreamer(lFileStream);
		lBondGraph->write(lStreamer);
		
#ifdef STOP_ON_ERROR
		exit(EXIT_FAILURE);
#endif
	} 
	catch(std::runtime_error inError) {
		std::cerr << "Error catched while evaluating the bond graph: " << inError.what() << std::endl;
		
		//Save bond graph for debuging
		std::ostringstream lFilename;
		lFilename << "bug/bondgraph_bug_" << ioContext.getGeneration() << "_" << ioContext.getIndividualIndex();
#ifndef WITHOUT_GRAPHVIZ
		lBondGraph->plotGraph(lFilename.str()+std::string(".svg"));
#endif
		ofstream lFileStream((lFilename.str()+std::string(".xml")).c_str());
		PACC::XML::Streamer lStreamer(lFileStream);
		lBondGraph->write(lStreamer);
		
		//Assign null fitness
		lFitness->setValue(0);
		
#ifdef XMLBEAGLE
		XMLStreamer lStreamer2(std::cout);
		inIndividual.write(lStreamer2);
#else
		inIndividual.write(lStreamer);
#endif
		
#ifdef STOP_ON_ERROR
		exit(EXIT_FAILURE);
#endif
    }
	
	//delete lBondGraph;
	return lFitness;
}

void AnalogFilterParameterEvalOp::initialize(Beagle::System& ioSystem) {
	Beagle::EvaluationOp::initialize(ioSystem);
	
//	//if(!mIsInitialized) {
//		// Call application and library initialization. Perform this 
//		// initialization before calling any API functions or
//		// Compiler-generated libraries.
//		const char *pStrings[]={"-nojvm","-nojit"};
//		if (!mclInitializeApplication(pStrings,2) || !libAnalogFilterDesignEvalInitialize())
//		{
//			throw runtime_error("Could not initialize the Matlab library properly\n");
//		}
////		mIsInitialized = true;
//	//}
}


/*!
 *  \brief Post-initialize the ECP evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void AnalogFilterParameterEvalOp::postInit(Beagle::System& ioSystem)
{
	Beagle::EvaluationOp::postInit(ioSystem);
}

