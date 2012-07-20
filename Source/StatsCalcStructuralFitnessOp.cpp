/*
 *  StatsCalcStructuralFitnessOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 19/06/10.
 */


#include "StatsCalcStructuralFitnessOp.h"
#include "BGSpeciesHolder.h"

using namespace Beagle;

/*!
 *  \brief Construct a calculate stats operator.
 *  \param inName Name of the operator.
 */
StatsCalcStructuralFitnessOp::StatsCalcStructuralFitnessOp(Beagle::string inName) :
StatsCalcFitnessSimpleOp(inName)
{ }


/*!
 *  \brief Calculate statistics of a given deme.
 *  \param outStats Evaluated statistics.
 *  \param ioDeme Deme to evalute the statistics.
 *  \param ioContext Context of the evolution.
 */
void StatsCalcStructuralFitnessOp::calculateStatsDeme(Stats& outStats,
                                                  Deme& ioDeme,
                                                  Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	
	StatsCalcFitnessSimpleOp::calculateStatsDeme(outStats,ioDeme,ioContext);
		
	if(ioDeme.size() == 0) {
		outStats.addItem("StructNum",0);
		outStats.addItem("StructAvg",0);
		return;
	}
	
	if(ioDeme.size() == 1) {
		outStats.addItem("StructNum",1);
		outStats.addItem("StructAvg",1);
		return;
	}
	
	Beagle::Component::Handle lHolderComponent = ioContext.getSystem().getComponent("BGSpeciesHolder");
	if(lHolderComponent==NULL) throw Beagle_RunTimeExceptionM("No specie holder component found in the system!");
	const BGSpeciesHolder::Handle lSpeciesHolder = castHandleT<BGSpeciesHolder>(lHolderComponent);
	
	unsigned int lDeme = ioContext.getDemeIndex();
	double lSum = 0;
	double lNbStructure = 0;
	double lSizeAvg;
	if(lSpeciesHolder->size() > 0) {
		for(std::map<unsigned int, BGSpecies*>::const_iterator lIter = (*lSpeciesHolder)[lDeme].begin(); lIter != (*lSpeciesHolder)[lDeme].end(); ++lIter) {
			if( lIter->second->getSize() > 0) {
				lSum += lIter->second->getSize();
				lNbStructure++;
			}
		}
		if(lNbStructure != 0)
			lSizeAvg = lSum/lNbStructure;
		else
			lSizeAvg = 0;
			
	}
	
	
	
	outStats.addItem("StructCount",lNbStructure);
	outStats.addItem("StructSizeAvg",lSizeAvg);
	
	Beagle_StackTraceEndM("void StatsCalcStructuralFitnessOp::calculateStatsDeme(Stats& outStats, Deme& ioDeme, Context& ioContext) const");
}

void StatsCalcStructuralFitnessOp::calculateStatsVivarium(Stats& outStats,
                                              Vivarium& ioVivarium,
                                              Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	outStats.clear();
	outStats.clearItems();
	
	unsigned int lPopSize = 0;
	unsigned int lProcessed = 0;
	unsigned int lTotalProcessed = 0;
	for(unsigned int i=0; i<ioVivarium.size(); ++i) {
		lPopSize += ioVivarium[i]->getStats()->getPopSize();
		if(ioVivarium[i]->getStats()->existItem("processed")) {
			lProcessed += (unsigned int)ioVivarium[i]->getStats()->getItem("processed");
		}
		if(ioVivarium[i]->getStats()->existItem("total-processed")) {
			lTotalProcessed += (unsigned int)ioVivarium[i]->getStats()->getItem("total-processed");
		}
	}
	outStats.setGenerationValues("vivarium", ioContext.getGeneration(), lPopSize, true);
	outStats.addItem("processed", lProcessed);
	outStats.addItem("total-processed", lTotalProcessed);
	
	if(ioVivarium.size() == 0) return;
	
	outStats.resize(ioVivarium[0]->getStats()->size());
	for(unsigned int i=0; i<outStats.size(); ++i) {
		double lSumXi     = 0.0;
		double lSumXiPow2 = 0.0;
		double lMax       = (*ioVivarium[0]->getStats())[i].mMax;
		double lMin       = (*ioVivarium[0]->getStats())[i].mMin;
		
		for(unsigned int j=0; j<ioVivarium.size(); ++j) {
			Beagle_AssertM(outStats.size() == ioVivarium[j]->getStats()->size());
			const Measure& lMeasure  = (*ioVivarium[j]->getStats())[i];
			unsigned int   lDemeSize = ioVivarium[j]->getStats()->getPopSize();
			if(lDemeSize != 0) {
				double lSumFit  =  lMeasure.mAvg * lDemeSize;
				double lTmpVar1 =  pow2Of<double>(lSumFit) / lDemeSize;
				double lTmpVar2 =  pow2Of<double>(lMeasure.mStd) * (lDemeSize-1);
				lSumXi          += lSumFit;
				lSumXiPow2      += lTmpVar1 + lTmpVar2;
				lMax            =  maxOf<double>(lMax, lMeasure.mMax);
				lMin            =  minOf<double>(lMin, lMeasure.mMin);
			}
		}
		
		outStats[i].mId  = (*ioVivarium[0]->getStats())[i].mId;
		if(lPopSize == 0) {
			outStats[i].mAvg = 0.0;
			outStats[i].mStd = 0.0;
			outStats[i].mMax = 0.0;
			outStats[i].mMin = 0.0;
		}
		else if(lPopSize == 1) {
			outStats[i].mAvg = lSumXi;
			outStats[i].mStd = 0.0;
			outStats[i].mMax = lMax;
			outStats[i].mMin = lMin;
		}
		else {
			double lAverage  = lSumXi / lPopSize;
			double lVariance =
			lSumXiPow2 - (pow2Of<double>(lAverage*((double)lPopSize)) / ((double)lPopSize));
			double lStdError = sqrt(lVariance / (lPopSize - 1));
			outStats[i].mAvg = lAverage;
			outStats[i].mStd = lStdError;
			outStats[i].mMax = lMax;
			outStats[i].mMin = lMin;
		}
	}
	
	//Compute structure related stats
	double lNbStructure = 0;
	double lStructureAvg = 0;
	for(unsigned int j=0; j<ioVivarium.size(); ++j) {
		lNbStructure += (*ioVivarium[j]->getStats()).getItem("StructCount");
		lStructureAvg += (*ioVivarium[j]->getStats()).getItem("StructSizeAvg");
		
	}
	
	outStats.addItem("StructCount",lNbStructure/ioVivarium.size());
	outStats.addItem("StructSizeAvg",lStructureAvg/ioVivarium.size());
	
	Beagle_StackTraceEndM("void StatsCalcStructuralFitnessOp::calculateStatsVivarium(Stats& outStats, Vivarium& ioVivarium, Context& ioContext) const");
}
