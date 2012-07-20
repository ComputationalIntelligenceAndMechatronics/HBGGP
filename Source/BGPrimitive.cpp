/*
 *  BGPrimitive.cpp
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
 *  This file was created by Jean-Francois Dupuis on 16/03/10.
 */

#include "BGPrimitive.h"
#include "Defines.h"
#include "ArgType.h"
#include "BGObject.h"
#include "BGContext.h"
#include <beagle/ValidationException.hpp>
#include <beagle/GP/PrimitiveInline.hpp>

std::vector<const std::type_info*> BGPrimitive::getParentArgumentTypes(GP::Context& ioContext) const {
	GP::Tree& lActualTree = ioContext.getGenotype();
		
	//Find which argument this primitive is
	unsigned int lParentIndex    = ioContext.getCallStackElement(ioContext.getCallStackSize()-2);
	unsigned int lChildIndex     = lParentIndex + 1;
	unsigned int lArgsIndexChild = 0;
	while(lChildIndex != ioContext.getCallStackTop()) {
		lChildIndex += lActualTree[lChildIndex].mSubTreeSize;
		Beagle_AssertM(lChildIndex < lActualTree.size());
		lArgsIndexChild++;
		Beagle_AssertM(lArgsIndexChild < lActualTree[lParentIndex].mPrimitive->getNumberArguments());
	}
	
	// Return parent argment cast
	unsigned int lActualNodeIndex = ioContext.getCallStackTop();
	ioContext.popCallStack();
	std::vector<const std::type_info*> lParentDesiredTypes = lActualTree[lParentIndex].mPrimitive->getArgTypes(lArgsIndexChild, ioContext);
	ioContext.pushCallStack(lActualNodeIndex);
	
	return lParentDesiredTypes;
}
