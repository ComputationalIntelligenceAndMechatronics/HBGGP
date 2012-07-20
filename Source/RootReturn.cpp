/***************************************************************************
 *   Copyright (C) 2004 by Jean-Francois Dupuis                            *
 *   jfdupuis@gel.ulaval.ca                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "RootReturn.h"

bool RootReturn::isEqual(const RootReturn& inRightObj) const {
	const RootReturn& lRightRootReturn = castObjectT<const RootReturn&>(inRightObj);
	return mValue == lRightRootReturn.mValue;
}

bool RootReturn::isLess(const RootReturn& inRightObj) const {
	const RootReturn& lRightRootReturn = castObjectT<const RootReturn&>(inRightObj);
	return mValue < lRightRootReturn.mValue;
}

void RootReturn::readWithContext(PACC::XML::ConstIterator inNode, Context& ioContext) {
	Beagle_StackTraceBeginM();
	if(inNode->getType() != PACC::XML::eString)
		throw Beagle_IOExceptionNodeM(*inNode, "expected string to read RootReturn");
	std::istringstream lISS(inNode->getValue());
	lISS >> mValue;
	Beagle_StackTraceEndM("void RootReturn::read(PACC::XML::ConstIterator inNode)");
}
void RootReturn::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	Beagle_StackTraceBeginM();
	
	std::ostringstream lOSS;
	lOSS << mValue << std::flush;
	ioStreamer.insertStringContent(lOSS.str());
	
	Beagle_StackTraceEndM("void RootReturn::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}




