/*
 *  FSMController.cpp
 *  Copyright 2008 Jean-Francois Dupuis.
 *
 *  This file is part of EvoStateController.
 *  
 *  EvoStateController is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EvoStateController is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EvoStateController.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  This file was created by Jean-Francois Dupuis on 03/10/08.
 */

#include "FSMController.h"

vector<double> FSMController::getOutputs() { 
	return generateOutputSymbols(); 
}

void FSMController::setInput(const vector<double>& inInputs) { 
	generateInputSymbols(inInputs); 
	processInput();
}


