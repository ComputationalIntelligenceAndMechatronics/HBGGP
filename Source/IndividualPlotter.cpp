/*
 *  IndividualPlotter.cpp
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
#ifndef WITHOUT_GRAPHVIZ

#include "IndividualPlotter.h"
#include <stdio.h>
#include <stdlib.h>

void plotIndividual(Beagle::GP::Individual &inIndividual, const std::string& inFilename) {
	Beagle::GP::Tree::Handle lTree = inIndividual[0];
	
	GVC_t* lContext = gvContext();
	Agraph_t *lGraph = agopen("lGraph", AGDIGRAPH);

	char lNameId [256];
	sprintf(lNameId,"%d",0);
	Agnode_t *lRoot = agnode(lGraph, lNameId);
	
	char* lName = new char[(*lTree)[0].mPrimitive->getName().size()+1];
	strcpy(lName,(*lTree)[0].mPrimitive->getName().c_str());

	agsafeset(lRoot, "label", lName, "");
	delete [] lName;
	
	unsigned int lNumberArguments = (*lTree)[0].mPrimitive->getNumberArguments();
	unsigned int lSubTreeSize = 1;
	for(unsigned int i=0; i<lNumberArguments; ++i) {
		lSubTreeSize += plotSubTree(lTree, lSubTreeSize, lGraph, lRoot);
	}

	gvLayout(lContext, lGraph, "dot");
	gvRenderFilename(lContext, lGraph, "svg", inFilename.c_str());
	gvFreeLayout(lContext, lGraph);
	agclose(lGraph);	
	gvFreeContext(lContext);
}

unsigned int plotSubTree(Beagle::GP::Tree::Handle inTree, unsigned int inN, Agraph_t *inGraph, Agnode_t *inParent) {
	
	char* lName = new char[(*inTree)[inN].mPrimitive->getName().size()+1];
	strcpy(lName,(*inTree)[inN].mPrimitive->getName().c_str());
	
	char lNameId [256];
	sprintf(lNameId,"%d",inN);
	Agnode_t *lChild = agnode(inGraph, lNameId);
	agsafeset(lChild, "label", lName, "");
	agedge(inGraph, inParent, lChild);
	
	delete [] lName;
	
	unsigned int lNumberArguments = (*inTree)[inN].mPrimitive->getNumberArguments();
	unsigned int lSubTreeSize = 1;
	for(unsigned int i=0; i<lNumberArguments; ++i) {
		lSubTreeSize += plotSubTree(inTree, (lSubTreeSize+inN), inGraph, lChild);
	}
	
	Beagle_AssertM(lSubTreeSize == (*inTree)[inN].mSubTreeSize);
	
	return lSubTreeSize;
}

#endif

