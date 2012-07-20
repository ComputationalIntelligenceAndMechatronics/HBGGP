/*
 *  VectorUtil.h
 *  Copyright 2008 Jean-Francois Dupuis.
 *
 *  This file is part of ThreeTanks.
 *  
 *  ThreeTanks is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ThreeTanks is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ThreeTanks.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  This file was created by Jean-Francois Dupuis on 13/11/08.
 */

#ifndef VectorUtil_H
#define VectorUtil_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cfloat>

template <class T>
std::ostream& operator<<(std::ostream& ioStream, const std::vector<T>& inVector){
	if(inVector.size() > 0) {
		for(unsigned int i = 0; i < inVector.size()-1; ++i) {
			ioStream << inVector[i] << ",";
		}
		ioStream << inVector[inVector.size()-1];
	}
	return ioStream;
}

template <class T>
std::istream& operator>>(std::istream& ioStream, std::vector<T>& inVector){
	inVector.resize(0);
	
	char lC;
	T lValue;
	do {
		lC='\0';
		lValue=DBL_MAX;
		ioStream >> lValue >> lC;
		if(lValue == DBL_MAX)
			throw std::runtime_error("Bad format of vector!");
		inVector.push_back(lValue);
	} while(lC == ',');
	
	return ioStream;
}

std::vector<double> cross(const std::vector<double>& inV1, const std::vector<double>& inV2);
double dot(const std::vector<double>& inV1, const std::vector<double>& inV2);
double norm(const std::vector<double>& inV1);

std::vector<double> operator*(const std::vector<double>& inV1, double inValue);
std::vector<double> operator/(const std::vector<double>& inV1, double inValue);

std::vector<double> operator+(const std::vector<double>& inV1, const std::vector<double>& inV2);
std::vector<double> operator-(const std::vector<double>& inV1, const std::vector<double>& inV2);
#endif
