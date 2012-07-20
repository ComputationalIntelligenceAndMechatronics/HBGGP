/*
 *  BGSpeciesHolder.cpp
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

#include "BGSpeciesHolder.h"

#include <sstream>
#include <beagle/macros.hpp>
#include <beagle/Exception.hpp>
#include <beagle/IOException.hpp>
#include <beagle/Context.hpp>
#include "VectorUtil.h"
#include <assert.h>

using namespace Beagle;

BGSpecies::BGSpecies(GrowingBG::Handle inBondGraph, unsigned int inGeneration, unsigned int inId) 
: mAge(inGeneration), mCount(0), mId(inId) {
	mBondGraph = inBondGraph;
}

BGSpeciesHolder::BGSpeciesHolder() : Beagle::Component("BGSpeciesHolder"), mIdCounter(0)
{ }

BGSpecies* BGSpeciesHolder::findSpecies(GrowingBG::Handle inBondGraph, Beagle::Context& ioContext){
	bool lDummy;
	return findSpecies(inBondGraph, ioContext, lDummy);
}

/*! \brief Find a matching specie
 *  If there is no matching specie, a new one is created
 *  \param  inBondGraph Bond graph for which a matching specie is needed
 *  \return Assiociated specie
 */
BGSpecies* BGSpeciesHolder::findSpecies(GrowingBG::Handle inBondGraph, Beagle::Context& ioContext, bool& outIsNew){
	assert(inBondGraph != 0);
	unsigned int lDeme = ioContext.getDemeIndex();
	assert(this->size() > lDeme);
	
	for(std::map<unsigned int, BGSpecies*>::iterator lIter = (*this)[lDeme].begin(); lIter != (*this)[lDeme].end(); ++lIter) {
		GrowingBG::Handle lBondGraphObj = lIter->second->getBondGraphObject();
		if( lIter->second->getBondGraphObject()->getBondGraph()->compare(*inBondGraph->getBondGraph()) ) {
			assert(lIter->second != 0);
			outIsNew = false;
			return lIter->second;
		}
	}
	
	//No matching specie found
	outIsNew = true;
	pair<map<unsigned int,BGSpecies*>::iterator,bool> lNewElement;
	++mIdCounter;
	lNewElement = (*this)[lDeme].insert( make_pair(mIdCounter, new BGSpecies(inBondGraph, ioContext.getGeneration(), mIdCounter) ) );
	assert(lNewElement.first->second != 0);
	return lNewElement.first->second;	
}

void BGSpeciesHolder::clearCount(Beagle::Context& ioContext) {
	unsigned int lDeme = ioContext.getDemeIndex();
	if(this->size() > lDeme) {
		for(std::map<unsigned int, BGSpecies*>::iterator lIterMap=(*this)[lDeme].begin(); lIterMap!=(*this)[lDeme].end(); ++lIterMap) {
			assert(lIterMap->second != 0);
			lIterMap->second->clearCount();
		}
	}
}

void BGSpeciesHolder::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	//	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="BGSpeciesHolder"))
	//		throw Beagle_IOExceptionNodeM(*inIter, "tag <BGSpeciesHolder> expected!");
	//	clear();
	//	
	//	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
	//		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Values")) {
	//			PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
	//			if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value present!");
	//			if(lChild2->getType() != PACC::XML::eString)
	//				throw Beagle_IOExceptionNodeM(*lChild2, "no value present!");
	//			
	//			std::istringstream lStream(lChild2->getValue().c_str());
	//			lStream >> *this;
	//		}
	//	}
	Beagle_StackTraceEndM("void BGSpeciesHolder::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)");
}


void BGSpeciesHolder::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("BGSpeciesHolder", inIndent);
	for(unsigned int lDeme = 0; lDeme < this->size(); ++lDeme) {
		ioStreamer.openTag("Deme", inIndent);
		ioStreamer.insertAttribute("id", lDeme);
		for(std::map<unsigned int, BGSpecies*>::const_iterator lIterMap=(*this)[lDeme].begin(); lIterMap!=(*this)[lDeme].end(); ++lIterMap) {
			if(lIterMap->second->getSize() > 0) {
				ioStreamer.openTag("Species", false);
				ioStreamer.insertAttribute("id", lIterMap->first);
				ioStreamer.insertAttribute("size", lIterMap->second->getSize());
				ioStreamer.insertAttribute("age", lIterMap->second->getAge());
				ioStreamer.closeTag();
			}
		}
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM("void BGSpeciesHolder::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
