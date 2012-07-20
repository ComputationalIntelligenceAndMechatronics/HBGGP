/*
 *  BondGraphEvalOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 14/05/09.
 */

#include "BondGraphEvalOp.h"
#include "LogFitness.h"

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator.
 */
BondGraphEvalOp::BondGraphEvalOp(std::string inName) :
#ifdef USE_MPI
MPI::GP::EvaluationOp(inName)
#else
Beagle::GP::EvaluationOp(inName)
#endif
{ 

}
//
///*!
// *  \brief Evaluate the fitness of the given individual.
// *  \param inIndividual Current individual to evaluate.
// *  \param ioContext Evolutionary context.
// *  \return Handle to the fitness value of the individual.
// */
//Fitness::Handle BondGraphEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
//{
//	Beagle_AssertM(inIndividual.size() == 1);
//	
//	LogFitness *lFitness = new LogFitness;
//		
//	return lFitness;
//}


/*!
 *  \brief Initialize the evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void BondGraphEvalOp::initialize(Beagle::System& ioSystem)
{
#ifdef USE_MPI
	Beagle::MPI::EvaluationOp::initialize(ioSystem);
#else
	Beagle::EvaluationOp::initialize(ioSystem);
#endif
	
}


/*!
 *  \brief Post-initialize the evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void BondGraphEvalOp::postInit(Beagle::System& ioSystem)
{
#ifdef USE_MPI
	Beagle::MPI::EvaluationOp::postInit(ioSystem);
#else
	Beagle::EvaluationOp::postInit(ioSystem);
#endif	
}


