/*
 *  LogIndividualOp.h
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
 *  This file was created by Jean-Francois Dupuis on 10/07/10.
 */

#ifndef LogIndividualOp_H
#define LogIndividualOp_H

#include <beagle/Operator.hpp>
#include <beagle/UInt.hpp>
#include <beagle/Bool.hpp>

class LogIndividualOp : public Beagle::Operator {
public:
	
	//! LogIndividualOp allocator type.
	typedef Beagle::AllocatorT<LogIndividualOp,Beagle::Operator::Alloc> Alloc;
	//! LogIndividualOp handle type.
	typedef Beagle::PointerT<LogIndividualOp,Beagle::Operator::Handle>	Handle;
	//! LogIndividualOp bag type.
	typedef Beagle::ContainerT<LogIndividualOp,Beagle::Operator::Bag> Bag;
	
	explicit LogIndividualOp(std::string inName="LogIndividualOp");
	virtual ~LogIndividualOp() { }
	
	virtual void initialize(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	
	
protected:
	Beagle::Bool::Handle mOnlyVivarium;
	Beagle::UInt::Handle mNumberIndividualPerDem;
	
	Beagle::UIntArray::Handle  mPopSize;              //!< Population size for the evolution.
	unsigned int       mNbDemesCalculated;    //!< Number of demes for which stats is computed.
	unsigned int       mGenerationCalculated; //!< Actual generation for which stats is computed.

};

#endif
