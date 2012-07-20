/***************************************************************************
 *   Copyright (C) 2004 by Jean-Francois Dupuis                            *
 *   jfdupuis@gel.ulaval.ca                                                *
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
#ifndef NOCOMPRESS 

#include "stringcompression.h"

#include <zlib.h>
#include <stdexcept>
#include <sstream>

#include <iostream>

using namespace std;

void compressString(const string& inString, string& outString, unsigned int inCompressionLevel)
{
	if(inCompressionLevel == 0) 
		outString = inString;
	else
	{
		// size of buffer must be at least 0.1% + 12 bytes larger than the message size
		unsigned long lBufLength = (inString.size() + inString.size()/1000 + 1) + 12;
		outString.resize(lBufLength);
		// compress message
		int lReturn = compress2((Bytef *)outString.data(), (uLongf*)&lBufLength, (const Bytef *)inString.data(), inString.size(), inCompressionLevel);
		// check for error
		if(lReturn != Z_OK)
		{
			outString.clear();
			throw runtime_error("compressString : Unable to compress message!");
		}
		// resize message to size of compressed message
		outString.resize(lBufLength);
	}
}

void uncompressString(string& ioString, unsigned long inUncompressedSize)
{
	string lUncompressedMessage;
	lUncompressedMessage.resize(inUncompressedSize);
	int lReturn = uncompress((Bytef*)lUncompressedMessage.data(), (uLong*)&inUncompressedSize, (const Bytef*)ioString.data(), ioString.size());
	if(lReturn != Z_OK)
	{
		throw runtime_error("uncompressString : Unable to uncompress message!");
	}
	ioString = lUncompressedMessage;
}

void string2hex(string& ioString) {
	
	string lResult;
	lResult.resize(ioString.size() * 2);
	
	char* lDataIn = (char*)ioString.data();
	char* lDataOut = (char*)lResult.data();
	
	for(int i = 0, j = 0; i < ioString.size(); ++i) {
		char c;
		c = (lDataIn[i] >> 4) & 0xf;
		c = (c>9) ? c+'a'-10 : c + '0';
		lDataOut[j++] = c;
		c = lDataIn[i] & 0xf;
		c = (c>9) ? c+'a'-10 : c + '0';
		lDataOut[j++] = c;
	}
	
	ioString = lResult;
}

void hex2string(string& ioString) {
	string Out;
	string Temp = ioString;
	string ffStr = "ff";
	while(!Temp.empty()) {
		Out += (char )strtol(Temp.substr(0, 2).c_str(), 0, 16);
		Temp = Temp.substr(2);
	}
	ioString = Out;
} 

#endif
