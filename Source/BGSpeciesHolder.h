/*
 *  BGSpeciesHolder.h
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

#ifndef BGSpeciesHolder_H
#define BGSpeciesHolder_H

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Component.hpp"
#include "GrowingBG.h"
#include <Component.h>

class BGSpecies : public Beagle::Object {
private:
	GrowingBG::Handle mBondGraph;
	unsigned int mAge;
	unsigned int mCount;
	unsigned int mId;
public:
	typedef Beagle::AllocatorT<BGSpecies,Beagle::Object::Alloc> Alloc;
	typedef Beagle::PointerT<BGSpecies,Beagle::Object::Handle> Handle;
	typedef Beagle::ContainerT<BGSpecies,Beagle::Object::Bag>	Bag;
	
	BGSpecies(GrowingBG::Handle inBondGraph, unsigned int inGeneration, unsigned int inId);
	
	void setId(unsigned int inId) { mId = inId; }
	unsigned int getId() const { return mId; }
	void clearCount() { mCount = 0; }
	unsigned int getSize() const { return mCount; }
	unsigned int getAge() const { return mAge; }
	
	GrowingBG::Handle getBondGraphObject() const { return mBondGraph; }
	
	BGSpecies& operator++() { ++mCount; return *this; }
	//BGSpecies& operator--() { if(mCount>0) --mCount; return *this; };
	
};



class BGSpeciesHolder : public Beagle::Component, public vector< std::map<unsigned int, BGSpecies*> >
{
private:
	unsigned int mIdCounter;

public:
    typedef Beagle::AllocatorT<BGSpeciesHolder,Beagle::Component::Alloc> Alloc;
	typedef Beagle::PointerT<BGSpeciesHolder,Beagle::Component::Handle> Handle;
	typedef Beagle::ContainerT<BGSpeciesHolder,Beagle::Component::Bag>	Bag;
	
	explicit BGSpeciesHolder();
	virtual ~BGSpeciesHolder() { }
	
	void clearCount(Beagle::Context& ioContext);
	
	BGSpecies* findSpecies(GrowingBG::Handle inBondGraph, Beagle::Context& ioContext);
	BGSpecies* findSpecies(GrowingBG::Handle inBondGraph, Beagle::Context& ioContext, bool& outIsNew);
	
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

#endif
