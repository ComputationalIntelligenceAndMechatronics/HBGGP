/*
 *  GrowingBG.h
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
 *  This file was created by Jean-Francois Dupuis on 10/02/10.
 */

#ifndef GrowingBG_H
#define GrowingBG_H

#include <BondGraph.h>
#include <beagle/PointerT.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/Matrix.hpp>
#include <beagle/GA/FloatVector.hpp>

class GrowingBG : public Beagle::Object {
public:
	//! BondGraph allocator type.
	typedef Beagle::AllocatorT<GrowingBG,Beagle::Object::Alloc> Alloc;
	//! BondGraph handle type.
	typedef Beagle::PointerT<GrowingBG,Beagle::Object::Handle> Handle;
	//! BondGraph bag type.
	typedef Beagle::ContainerT<GrowingBG,Beagle::Object::Bag> Bag;
	
	explicit GrowingBG();
	~GrowingBG() {}
	
	void setCausalityInvalid() { mValidCausality = false; }
	bool isCausalityValid() const { return mValidCausality; }
	
	virtual void clearCausality() = 0;
	virtual void assignCausality() = 0;
	
	virtual void insertJunction(BG::Bond* inBond, BG::Junction* inJunction, BG::Bond*& lOutBond) = 0;
	virtual void insertComponent(BG::Junction* inJunction, BG::Component* inComponent, BG::Bond*& lOutBond) = 0;
//	virtual void replaceComponent(BG::Component* inOldComponent, BG::Component*& inNewComponent) = 0;
	
	virtual void read(PACC::XML::ConstIterator inIter)  = 0;
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const  = 0;
	
	virtual void extractParameters(Beagle::GA::FloatVector& outParameters) const = 0;
	virtual void assignParameters(const Beagle::GA::FloatVector& outParameters) = 0;
	
	virtual BG::BondGraph* getBondGraph() = 0;
	
protected:
	bool mValidCausality;	//!< Indicate that the previously computed causality is still valid.
	
};

#endif
