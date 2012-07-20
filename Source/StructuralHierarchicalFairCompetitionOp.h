/*
 *  StructuralHierarchicalFairCompetitionOp.h
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
 *  This file was created by Jean-Francois Dupuis on 20/06/10.
 */

#ifndef StructuralHierarchicalFairCompetitionOp_H
#define StructuralHierarchicalFairCompetitionOp_H

#include "beagle/RouletteT.hpp"
#include "beagle/HierarchicalFairCompetitionOp.hpp"

/*! \brief Hierachical Fair Competition for structure flagged tree
 *	This operator redefine the  Beagle::HierarchicalFairCompetitionOp operate method
 *	so that the valid structure flag is set to invalid when the individual is migrated.
 */
class StructuralHierarchicalFairCompetitionOp : public Beagle::HierarchicalFairCompetitionOp {
public:
	typedef Beagle::AllocatorT<StructuralHierarchicalFairCompetitionOp,Beagle::HierarchicalFairCompetitionOp::Alloc> Alloc;
	typedef Beagle::PointerT<StructuralHierarchicalFairCompetitionOp,Beagle::HierarchicalFairCompetitionOp::Handle> Handle;
	typedef Beagle::ContainerT<StructuralHierarchicalFairCompetitionOp,Beagle::HierarchicalFairCompetitionOp::Bag> Bag;
	
	StructuralHierarchicalFairCompetitionOp(std::string inName="StructuralHierarchicalFairCompetitionOp");
	virtual ~StructuralHierarchicalFairCompetitionOp() { }
	
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
};


#endif
