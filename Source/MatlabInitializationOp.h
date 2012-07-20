/*
 *  MatlabInitialisationOp.h
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

#ifndef MatlabInitializationOp_H
#define MatlabInitializationOp_H

#include "beagle/Operator.hpp"

class MatlabInitializationOp : public Beagle::Operator {
public:
	
	//! MatlabInitializationOp allocator type.
	typedef Beagle::AllocatorT<MatlabInitializationOp,Beagle::Operator::Alloc> Alloc;
	//! MatlabInitializationOp handle type.
	typedef Beagle::PointerT<MatlabInitializationOp,Beagle::Operator::Handle>	Handle;
	//! MatlabInitializationOp bag type.
	typedef Beagle::ContainerT<MatlabInitializationOp,Beagle::Operator::Bag> Bag;
	
	explicit MatlabInitializationOp(std::string inName="MatlabInitializationOp");
	virtual ~MatlabInitializationOp() { }
	
	virtual void initialize(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext) {}
	
	
protected:
	bool mIsInitialized;
};


#endif
