/*
 *  BGSpeciationVerificationOp.h
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
 *  This file was created by Jean-Francois Dupuis on 21/06/10.
 */

#ifndef BGSpeciationVerificationOp_H
#define BGSpeciationVerificationOp_H

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Int.hpp"
#include "beagle/Float.hpp"
#include "beagle/UInt.hpp"

class BGSpeciationVerificationOp : public Beagle::Operator {
public:
	typedef Beagle::AllocatorT<BGSpeciationVerificationOp,Beagle::Operator::Alloc> Alloc;
	typedef Beagle::PointerT<BGSpeciationVerificationOp,Beagle::Operator::Handle> Handle;
	typedef Beagle::ContainerT<BGSpeciationVerificationOp,Beagle::Operator::Bag> Bag;
	
	BGSpeciationVerificationOp() : Beagle::Operator("BGSpeciationVerificationOp") {}
	~BGSpeciationVerificationOp() {}
	
	virtual void initialize(Beagle::System& ioSystem);
	virtual void postInit(Beagle::System& ioSystem);
	
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	
private:
	Beagle::Int::Handle mNbStructureSearch;
	Beagle::Float::Handle mAlpha;
	Beagle::Float::Handle mThreshold;
	Beagle::UIntArray::Handle mPopSize;
};


#endif
