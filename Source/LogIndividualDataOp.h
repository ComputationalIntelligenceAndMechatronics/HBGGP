/*
 *  LogIndividualDataOp.h
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
 *  This file was created by Jean-Francois Dupuis on 11/07/10.
 */

#ifndef LogIndividualDataOp_H
#define LogIndividualDataOp_H

#include "LogIndividualOp.h"
#include <beagle/Bool.hpp>

class LogIndividualDataOp : public LogIndividualOp {
public:
	
	//! LogIndividualDataOp allocator type.
	typedef Beagle::AllocatorT<LogIndividualDataOp,LogIndividualOp::Alloc> Alloc;
	//! LogIndividualDataOp handle type.
	typedef Beagle::PointerT<LogIndividualDataOp,LogIndividualOp::Handle>	Handle;
	//! LogIndividualDataOp bag type.
	typedef Beagle::ContainerT<LogIndividualDataOp,LogIndividualOp::Bag> Bag;
	
	explicit LogIndividualDataOp(std::string inName="LogIndividualDataOp");
	virtual ~LogIndividualDataOp() { }
	
	virtual void initialize(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	
	
protected:
	Beagle::Bool::Handle mKeepData;
};

#endif
