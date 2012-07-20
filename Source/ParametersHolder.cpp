/*
 *  ParametersHolder.cpp
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
 *  This file was created by Jean-Francois Dupuis on 16/05/10.
 */

#include "ParametersHolder.h"
#include <sstream>
#include <beagle/macros.hpp>
#include <beagle/Exception.hpp>
#include <beagle/IOException.hpp>
#include "VectorUtil.h"

using namespace Beagle;

ParametersHolder::ParametersHolder() :
Beagle::Component("ParametersHolder")
{ }


void ParametersHolder::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
//	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="ParametersHolder"))
//		throw Beagle_IOExceptionNodeM(*inIter, "tag <ParametersHolder> expected!");
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
	Beagle_StackTraceEndM("void ParametersHolder::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)");
}


void ParametersHolder::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
//	ioStreamer.openTag("ParametersHolder", inIndent);
//	for(ParametersHolder::const_iterator lIterMap=begin(); lIterMap!=end(); ++lIterMap) {
//		ioStreamer.openTag("Values", inIndent);
//		std::ostringstream lStreamer;
//		lStreamer << this;
//		ioStreamer.insertStringContent(lStreamer.str());
//		ioStreamer.closeTag();
//	}
//	ioStreamer.closeTag();
	Beagle_StackTraceEndM("void ParametersHolder::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
