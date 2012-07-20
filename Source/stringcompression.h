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
#ifndef STRINGCOMPRESSION_H
#define STRINGCOMPRESSION_H
#ifndef NOCOMPRESS
#include <string>

using namespace std;

/**
@author Jean-Francois Dupuis
*/

void compressString(const string& inString, string& outString, unsigned int inCompressionLevel = 9);
void uncompressString(string& ioString, unsigned int inUncompressedSize);

void string2hex(string& ioString);
void hex2string(string& ioString);
		

#endif
#endif
