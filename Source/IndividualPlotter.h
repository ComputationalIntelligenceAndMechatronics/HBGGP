/*
 *  IndividualPlotter.h
 *  Copyright 2009 Jean-Francois Dupuis.
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
 *  This file was created by Jean-Francois Dupuis on 30/11/09.
 */

#ifndef IndividualPlotter_H
#define IndividualPlotter_H
#ifndef WITHOUT_GRAPHVIZ

#include <graphviz/gvc.h>
#include <beagle/Beagle.hpp>
#include <beagle/Allocator.hpp>
#include <beagle/Pointer.hpp>
#include <beagle/Container.hpp>
#include <beagle/GP/Tree.hpp>
#include <beagle/GP/Individual.hpp>

unsigned int plotSubTree(Beagle::GP::Tree::Handle inTree, unsigned int inN, Agraph_t *inGraph, Agnode_t *inParent);
void plotIndividual(Beagle::GP::Individual &inIndividual, const std::string& inFilename);

#endif
#endif
