/*
 *  BGContext.h
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

#ifndef BGContext_H
#define BGContext_H

#include <beagle/IndividualBag.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/PointerT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/GP/Context.hpp>
#include "GrowingBG.h"

using namespace Beagle;
		
class BGContext : public Beagle::GP::Context {
public:
	typedef AllocatorT< BGContext, GP::Context::Alloc > Alloc;
	typedef PointerT< BGContext, GP::Context::Handle > Handle;
	typedef ContainerT< BGContext, GP::Context::Bag >	Bag;
	
	BGContext() { mSubGeneration = -1; }
	BGContext(Beagle::Context &inContext) { (Beagle::Context)(*this) = inContext; mSubGeneration = -1; }
	virtual ~BGContext() { }
	
	void setBondGraph( GrowingBG::Handle inBondGraph) { mBondGraph = inBondGraph; }
	GrowingBG::Handle getBondGraph() { return mBondGraph; }

	void setSubGeneration(int inGeneration) { mSubGeneration = inGeneration; }
	int getSubGeneration() const { return mSubGeneration; }
	
	void setSubContinueFlag(bool inFlag) { mSubContinueFlag = inFlag; }
	bool getSubContinueFlag() const { return mSubContinueFlag; }
	
	/*!
	 *  \brief Return a constant reference to the vivarium.
	 *  \return Evolution vivarium constant reference.
	 */
	inline const Vivarium& getGAVivarium() const
	{
		Beagle_StackTraceBeginM();
		return *mVivariumHandle;
		Beagle_StackTraceEndM("const Vivarium& Context::getVivarium() const");
	}
	
	/*!
	 *  \brief Return a reference to the vivarium.
	 *  \return Evolution vivarium reference.
	 */
	inline Vivarium& getGAVivarium()
	{
		Beagle_StackTraceBeginM();
		return *mVivariumHandle;
		Beagle_StackTraceEndM("Vivarium& Context::getVivarium()");
	}
	
protected:
	GrowingBG::Handle mBondGraph;
	int mSubGeneration;
	bool mSubContinueFlag;
};


#endif
