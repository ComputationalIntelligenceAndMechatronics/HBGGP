/*
 *  GrowingBondGraph.h
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
 *  This file was created by Jean-Francois Dupuis on 13/05/09.
 */

#ifndef GrowingBondGraph_H
#define GrowingBondGraph_H

#include <BondGraph.h>
#include "GrowingBG.h"
#include <beagle/PointerT.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/Matrix.hpp>
#include <beagle/GA/FloatVector.hpp>

class GrowingBondGraph : public GrowingBG, public BG::BondGraph {
public:
	//! BondGraph allocator type.
	typedef Beagle::AllocatorT<GrowingBondGraph,GrowingBG::Alloc> Alloc;
	//! BondGraph handle type.
	typedef Beagle::PointerT<GrowingBondGraph,GrowingBG::Handle> Handle;
	//! BondGraph bag type.
	typedef Beagle::ContainerT<GrowingBondGraph,GrowingBG::Object::Bag> Bag;
	
	explicit GrowingBondGraph();
	~GrowingBondGraph() {}
	
	GrowingBondGraph& operator=(const BG::BondGraph& inBondGraph);
	
	bool isCausalityValid() const { return mValidCausality; }
	void clearCausality();
	void assignCausality();
	
	void insertJunction(BG::Bond* inBond, BG::Junction* inJunction, BG::Bond*& outBond);
	void insertComponent(BG::Junction* inJunction, BG::Component* inComponent, BG::Bond*& outBond);
	//void replaceComponent(BG::Component* inOldComponent, BG::Component*& inNewComponent);

	virtual void read(PACC::XML::ConstIterator inIter) { BG::BondGraph::read(inIter); }
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const { BG::BondGraph::write(ioStreamer,inIndent); }
	
	void extractParameters(Beagle::GA::FloatVector& outParameters) const;
	void assignParameters(const Beagle::GA::FloatVector& outParameters);
	
	virtual BG::BondGraph* getBondGraph() { return this; }
	
//protected:
//	bool mValidCausality;	//!< Indicate that the previously computed causality is still valid.
	
};

#endif
