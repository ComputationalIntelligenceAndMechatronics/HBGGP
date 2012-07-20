/*
 *  MatrixInterface.h
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

#ifndef MatrixInterface_H
#define MatrixInterface_H

#ifdef XML_BEAGLE

#include <beagle/XMLNode.hpp>
#include <beagle/XMLStreamer.hpp>
#include <PACC/Math.hpp>

class MatrixInterface : public PACC::Matrix {
public:
	void read(PACC::Matrix& inMatrix, Beagle::XMLNode& inNode);
	void write(const PACC::Matrix& inMatrix, Beagle::XMLStreamer& ioStreamer);
};
#endif
#endif
