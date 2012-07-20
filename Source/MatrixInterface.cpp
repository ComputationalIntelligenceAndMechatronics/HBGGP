/*
 *  MatrixInterface.cpp
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
 *  This file was created by Jean-Francois Dupuis on 27/05/09.
 */
#ifdef XML_BEAGLE

#include "MatrixInterface.h"
#include <sstream>
#include <beagle/IOException.hpp>
#include <PACC/Util/StringFunc.hpp>

void MatrixInterface::read(PACC::Matrix& inMatrix, Beagle::XMLNode& inNode) {
//	if(inNode == NULL) throw runtime_error("Matrix::read() nothing to read!");
	if(inNode.getType() == Beagle::XMLNode::eTag) {
		// read first format
		bool lStringParsed = false;
		for(Beagle::XMLNode::Handle lChild = inNode.getFirstChild(); lChild != NULL; lChild=lChild->getNextSibling()) {
			if(lChild->getType() == Beagle::XMLNode::eString) {
				if(lStringParsed) 
					throw Beagle_IOExceptionNodeM(inNode,"Matrix::read() invalid format, matrix contains multiple strings!");
				parse(lChild->getValue());
				lStringParsed = true;
			}
		}
//		if(inNode.isDefined("rows")) {
			// validate number of rows
			int lRows = PACC::String::convertToInteger(inNode.getTagAttribute("rows"));
			if((int)mRows != lRows) 
				throw Beagle_IOExceptionNodeM(inNode,"Matrix::read() invalid 'rows' attribute!");
//		}
//		if(inNode.isDefined("cols")) {
			// validate number of cols
			int lCols = PACC::String::convertToInteger(inNode.getTagAttribute("cols"));
			if((int)mCols != lCols) 
				throw Beagle_IOExceptionNodeM(inNode,"Matrix::read() invalid 'cols' attribute!");
//		}
	} else if(inNode.getType() == Beagle::XMLNode::eString) {
		// read second format
		parse(inNode.getValue());
	} else {
		// unsupported node type
		throw Beagle_IOExceptionNodeM(inNode,"Matrix::read() node type must be XML::eData or XML::eString!");
	}
//	if(inNode.isDefined("name"))
		mName = inNode.getTagAttribute("name");
	
	inMatrix = PACC::Matrix(*this);
}

void MatrixInterface::write(const PACC::Matrix& inMatrix, Beagle::XMLStreamer& ioStreamer) {
	std::ostringstream lStringStream;
	PACC::XML::Streamer lPaccStreamer(lStringStream);
	inMatrix.write(lPaccStreamer);
	ioStreamer.insertString(lStringStream.str());
}

#endif
