/*
 *  GrowingHybridBondGraph.h
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

#ifndef GrowingHybridBondGraph_H
#define GrowingHybridBondGraph_H

#include <HybridBondGraph.h>
#include "GrowingBG.h"
#include <beagle/PointerT.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/Matrix.hpp>
#include <beagle/GA/FloatVector.hpp>

class GrowingHybridBondGraph : public GrowingBG, public BG::HybridBondGraph {
public:
	//! BondGraph allocator type.
	typedef Beagle::AllocatorT<GrowingHybridBondGraph,GrowingBG::Alloc> Alloc;
	//! BondGraph handle type.
	typedef Beagle::PointerT<GrowingHybridBondGraph,GrowingBG::Handle> Handle;
	//! BondGraph bag type.
	typedef Beagle::ContainerT<GrowingHybridBondGraph,GrowingBG::Bag> Bag;
	
	explicit GrowingHybridBondGraph();
	~GrowingHybridBondGraph() {	}
	
	GrowingHybridBondGraph& operator=(const BG::HybridBondGraph& inHybridBondGraph);
	
	bool isCausalityValid() const { return mValidCausality; }
	void clearCausality();
	void assignCausality();
	
	void insertSwitch(BG::Junction* inJunction, BG::Switch* inComponent, BG::Bond*& outBond);
	
	void insertJunction(BG::Bond* inBond, BG::Junction* inJunction, BG::Bond*& outBond);
	void insertComponent(BG::Junction* inJunction, BG::Component* inComponent, BG::Bond*& outBond);
	//void replaceComponent(BG::Component* inOldComponent, BG::Component*& inNewComponent);

	virtual void read(PACC::XML::ConstIterator inIter) { BG::HybridBondGraph::read(inIter); }
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const { BG::HybridBondGraph::write(ioStreamer,inIndent); }
	
	void extractParameters(Beagle::GA::FloatVector& outParameters) const;
	void assignParameters(const Beagle::GA::FloatVector& outParameters);
	
	virtual BG::BondGraph* getBondGraph() { return this; }
	
//protected:
//	bool mValidCausality;	//!< Indicate that the previously computed causality is still valid.
	
};

#endif
