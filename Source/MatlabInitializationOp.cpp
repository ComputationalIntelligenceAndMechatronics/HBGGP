/*
 *  MatlabInitialisationOp.cpp
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
 *  This file was created by Jean-Francois Dupuis on 16/09/09.
 */

#include "MatlabInitializationOp.h"
#include "libAnalogFilterDesignEval.h"

MatlabInitializationOp::MatlabInitializationOp(std::string inName) 
: Beagle::Operator(inName) {
	mIsInitialized = false;
}

void MatlabInitializationOp::initialize(Beagle::System& ioSystem) {
#ifndef DEBUG_NOMATLAB
	if(!mIsInitialized) {
		// Call application and library initialization. Perform this 
		// initialization before calling any API functions or
		// Compiler-generated libraries.
		const char *pStrings[]={"-nojvm","-nojit"};
		if (!mclInitializeApplication(pStrings,2) || !libAnalogFilterDesignEvalInitialize())
		{
			throw std::runtime_error("Could not initialize the Matlab library properly\n");
		}
		mIsInitialized = true;
	}
#endif
}
