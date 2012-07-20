/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Jean-Francois Dupuis
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef Beagle_GP_CrossoverSelectiveConstrainedOp_hpp
#define Beagle_GP_CrossoverSelectiveConstrainedOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/Int.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Tree.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/CrossoverOp.hpp"
#include "beagle/GP/CrossoverConstrainedOp.hpp"

#include <beagle/PointerT.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/ContainerT.hpp>

#include "SelectiveConstrainedSelectionOp.h"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

namespace Beagle {
namespace GP {

class CrossoverSelectiveConstrainedOp : public Beagle::GP::CrossoverConstrainedOp, public SelectiveConstrainedSelectionOp {
	
public:
	
	typedef Beagle::AllocatorT<CrossoverSelectiveConstrainedOp, Beagle::GP::CrossoverConstrainedOp::Alloc> Alloc;
	typedef Beagle::PointerT<CrossoverSelectiveConstrainedOp, Beagle::GP::CrossoverConstrainedOp::Handle> Handle;
	typedef Beagle::ContainerT<CrossoverSelectiveConstrainedOp, Beagle::GP::CrossoverConstrainedOp::Bag> Bag;
	
	explicit CrossoverSelectiveConstrainedOp(string inMatingPbName="gp.cx.indpb",
											 string inDistribPbName="gp.cx.distrpb",
											 string inName="GP-CrossoverSelectiveConstrainedOp");
	virtual ~CrossoverSelectiveConstrainedOp() { }
	
	virtual bool mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
					  Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2);
	
	virtual void initialize(Beagle::System& ioSystem);


};

}
}

#endif // Beagle_GP_CrossoverConstrainedOp_hpp
