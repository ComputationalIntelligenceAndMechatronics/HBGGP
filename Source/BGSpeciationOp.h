/*
 *  BGSpeciationOp.h
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
 *  This file was created by Jean-Francois Dupuis on 17/06/10.
 */

#ifndef BGSpeciationOp_H
#define BGSpeciationOp_H

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Int.hpp"
#include "beagle/Float.hpp"
#include "beagle/UInt.hpp"

class BGSpeciationOp : public Beagle::Operator {
public:
	typedef Beagle::AllocatorT<BGSpeciationOp,Beagle::Operator::Alloc> Alloc;
	typedef Beagle::PointerT<BGSpeciationOp,Beagle::Operator::Handle> Handle;
	typedef Beagle::ContainerT<BGSpeciationOp,Beagle::Operator::Bag> Bag;
	
	BGSpeciationOp() : Beagle::Operator("BGSpeciationOp") {}
	~BGSpeciationOp() {}
	
	virtual void initialize(Beagle::System& ioSystem);
	virtual void postInit(Beagle::System& ioSystem);
	
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	
private:
	Beagle::Int::Handle mNbStructureSearch;
	Beagle::Float::Handle mAlpha;
	Beagle::Float::Handle mThreshold;
	Beagle::UIntArray::Handle mPopSize;
	
	Beagle::Float::Handle mAgingThres;
	Beagle::Float::Handle mAgingSpeed;
};

#endif
