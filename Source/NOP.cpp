/*
 *  NOP.cpp
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
 *  This file was created by Jean-Francois Dupuis on 24/09/09.
 */

#include "NOP.h"
#include <beagle/Double.hpp>
#include <beagle/Beagle.hpp>
#include <beagle/GP/Primitive.hpp>

void NOP::execute(Beagle::GP::Datum& inDatum, Beagle::GP::Context& ioContext) {
	get1stArgument(inDatum, ioContext);
}
