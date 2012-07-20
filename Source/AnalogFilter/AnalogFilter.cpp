//#include <iostream>
//#include <PACC/Math.hpp>
//#include <BondGraph.h>
//#include <HybridBondGraph.h>
//#include <stdexcept>
//
//using namespace BG;
//
//class HBG1Controller : public SwitchController {
//private:
//	double mX1;
//	double mX2;
//	double mPreviousTime;
//	bool mContactState;
//public:	
//	HBG1Controller() { 
//		mOutputs.resize(1,false); 
//		mContactState = false;
//	}
//	virtual const vector<bool>& getOutputs() { 
//		return mOutputs; 
//	}	
//	virtual void writeLog() {
//		(*mLogger)["Sw"].push_back(mOutputs[0]);
//	}
//	virtual void reset() { mX1 = 10; mX2 = 0; mPreviousTime = 0;}
//	virtual void setInput(double inTime, const vector<double>& inInputs) {
//		mInputs = inInputs;
//		
//		double lF2 = mInputs[0];
//		double lv1 = mInputs[1];
//		double lv2 = mInputs[2];
//		
//		mX1 += lv1*(inTime-mPreviousTime);
//		mX2 += lv2*(inTime-mPreviousTime);
//		mPreviousTime = inTime;
//		
//		double lX12 = mX1 - mX2;
//		
//		if(!mContactState) {
//			if(lX12 <= 0 && lv1 < 0) {
//				mOutputs[0] = true;
//				mContactState = true;
//			}
//		} else {
//			if(lF2 > 0) {
//				mOutputs[0] = false;
//				mContactState = false;
//			}
//		}
//		
//	}
//	
//	void createBondGraph(HybridBondGraph &ioBondGraph) {
//		double M = 2;
//		double g = 9.81;
//		double b = 2;
//		double k = 20;
//		
//		//Create the sources
//		Source *lSe = new Source(Source::eEffort);
//		lSe->setValue(-M*g);
//		ioBondGraph.addComponent(lSe);
//		
//		//Create a switch
//		Switch *lSw = new Switch;
//		ioBondGraph.addSwitch(lSw);
//		
//		//Create the jonctions
//		Junction *lJ0_1 = new Junction(Junction::eZero);
//		ioBondGraph.addComponent(lJ0_1);
//		
//		Junction *lJ1_1 = new Junction(Junction::eOne);
//		ioBondGraph.addComponent(lJ1_1);
//		
//		Junction *lJ1_2 = new Junction(Junction::eOne);
//		ioBondGraph.addComponent(lJ1_2);
//		
//		//Create the resistor
//		Passive *lR = new Passive(Passive::eResistor);
//		lR->setValue(b);
//		ioBondGraph.addComponent(lR);
//		
//		//Create the storage
//		Passive *lC = new Passive(Passive::eCapacitor);
//		lC->setValue(1/k);
//		ioBondGraph.addComponent(lC);
//		
//		Passive *lI = new Passive(Passive::eInductor);
//		lI->setValue(M);
//		ioBondGraph.addComponent(lI); 
//		
//		
//		//Create the bonds
//		ioBondGraph.connect(lSe,lJ1_1);
//		ioBondGraph.connect(lJ1_1,lI); 
//		Bond* lBond3 = ioBondGraph.connect(lJ0_1,lJ1_1);
//		ioBondGraph.connect(lJ0_1,lSw);
//		Bond* lBond5 = ioBondGraph.connect(lJ1_2,lJ0_1);
//		Bond* lBond6 = ioBondGraph.connect(lJ1_2,lC);	
//		ioBondGraph.connect(lJ1_2,lR);
//		
//		vector<Bond*> lEffortBonds,lFlowBonds;
//		lEffortBonds.push_back(lBond6);
//		lFlowBonds.push_back(lBond3);
//		lFlowBonds.push_back(lBond5);
//		
//		ioBondGraph.setOutputBonds(lEffortBonds, lFlowBonds);
//	}
//};
//
//int main (int argc, char * const argv[]) {
//	
//	PACC::XML::Streamer lStream(std::cout);
//	PACC::Matrix lA,lB;
//	vector<double> lInitial;
//	HBG1Controller lController;
//	HybridBondGraph lHBG(&lController);
//	lController.createBondGraph(lHBG);
//	try{
//		vector<bool> lInitialSwitchState(1,false);
//		vector<double> lInitialState(2,0);
//		lHBG.setInitialState(lInitialSwitchState,lInitialState);
//		lHBG.simulate(5, 0.0001);
//		lHBG.writeSimulationLog("hybridbondgraph_1.csv");
//		
//	} catch(std::runtime_error inError) {
//		std::cerr << "Error catched : " << inError.what() << std::endl;
//	}
//	
//	
//	return 0;
//}

//
//#include <iostream>
//#include <PACC/Math.hpp>
//#include <BondGraph.h>
//#include <HybridBondGraph.h>
//#include <stdexcept>
//
//using namespace BG;
//
//void BG51a(BondGraph &ioBondGraph) {
//	
//	double lValueM = 2.5;
//	double lValueR1 = 2000;
//	
//	double lValueC1 = 2e-4;
//	double lValueI1 = 0.001;
//	
//	double lValueSe = 20;
//	
//	Source *lSe = new Source(Source::eEffort);
//	lSe->setValue(lValueSe);
//	ioBondGraph.addComponent(lSe);
//	
//	Junction *lJ1_1 = new Junction(Junction::eOne);
//	ioBondGraph.addComponent(lJ1_1);
//	
//	Passive *lC1 = new Passive(Passive::eCapacitor);
//	lC1->setValue(lValueC1);
//	ioBondGraph.addComponent(lC1);
//	
//	Passive *lTF = new Passive(Passive::eTransformer);
//	lTF->setValue(lValueM);
//	ioBondGraph.addComponent(lTF);
//	
//	Junction *lJ0_1 = new Junction(Junction::eZero);
//	ioBondGraph.addComponent(lJ0_1);
//	
//	Passive *lI1 = new Passive(Passive::eInductor);
//	lI1->setValue(lValueI1);
//	ioBondGraph.addComponent(lI1);
//	
//	Passive *lR1 = new Passive(Passive::eResistor);
//	lR1->setValue(lValueR1);
//	ioBondGraph.addComponent(lR1);
//	
//	//Create the bonds
//	ioBondGraph.connect(lSe,lJ1_1);
//	ioBondGraph.connect(lJ1_1,lC1); 
//	ioBondGraph.connect(lJ1_1,lTF);
//	ioBondGraph.connect(lTF,lJ0_1);
//	ioBondGraph.connect(lJ0_1,lI1);
//	ioBondGraph.connect(lJ0_1,lR1);
//}
//
//int main(int argc, char *argv[]) {
//	BondGraph lBG4;
//	BG51a(lBG4);
//	PACC::XML::Streamer lStream(std::cout);
//	PACC::Matrix lA,lB;
//	vector<double> lInitial;
//	try{
//		lBG4.assignCausality();
//		lBG4.plotGraph("bondgraph_5_1a.svg");
//		lBG4.computeStateEquation();
//		lBG4.getStateMatrix(lA,lB);
//		lA.write(lStream); std::cout << std::endl;
//		lB.write(lStream); std::cout << std::endl;
//	} catch(std::runtime_error inError) {
//		std::cerr << "Error catched : " << inError.what() << std::endl;
//	}
//}
 
#include <beagle/Beagle.hpp>
#include <beagle/GP.hpp>
#include "RootReturn.h"
#include "ReplaceComponent.h"
#include "AddComponent.h"
#include "InsertJunction.h"
#include "InsertJunctionPair.h"
#include "End.h"
#include "EmbryoPrimitive.h" 
#include "BGFitness.h"
#include "AnalogFilterEmbryo.h"
#include "AnalogFilterEvalOp.h"
#include "BGContext.h"
#include "TreeSTag.h"
#include "MatlabInitializationOp.h"
#ifdef USE_MPI
#include "MPI_GP_Evolver.hpp"
#endif

using namespace Beagle;

int main(int argc, char *argv[]) {
	try {
		// 1: Build primitives.
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&(typeid(RootReturn)));

		lSet->insert(new AnalogFilterEmbryo);
		
		lSet->insert(new ReplaceR);
		lSet->insert(new ReplaceC);
		lSet->insert(new ReplaceI);
		
		lSet->insert(new AddR);
		lSet->insert(new AddC);
		lSet->insert(new AddI);

#ifdef USE_JUNCTIONPAIR
		lSet->insert(new InsertJ10);
		lSet->insert(new InsertJ01);
#else
		lSet->insert(new InsertJ0);
		lSet->insert(new InsertJ1);
#endif
		
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
		AnalogFilterEvalOp::Handle lEvalOp = new AnalogFilterEvalOp;
		
		// 4: Build an evolver and a vivarium.
		TreeSTag::Alloc::Handle lTreeAlloc = new TreeSTag::Alloc;
		BGFitness::Alloc::Handle lFitAlloc = new BGFitness::Alloc;
		GP::Vivarium::Handle lVivarium = new GP::Vivarium(lTreeAlloc,lFitAlloc);
		
#ifdef USE_MPI	
		MPI::GP::Evolver::Handle lEvolver = new MPI::GP::Evolver(lEvalOp);
#else
		GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
#endif
		lEvolver->addOperator(new MatlabInitializationOp);
		
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

