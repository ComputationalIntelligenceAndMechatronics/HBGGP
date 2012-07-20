/*
 *  VectorUtil.cpp
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

#include "VectorUtil.h"
#include <math.h>
#include <assert.h>

std::vector<double> cross(const std::vector<double>& inV1, const std::vector<double>& inV2) {
	assert(inV1.size() == inV2.size());
	assert(inV1.size() <= 3 && inV1.size() >= 2);
	
	std::vector<double> lV1(3);
	std::vector<double> lV2(3);
	if(inV1.size() == 2) {
		lV1[2] = 0;
		lV2[2] = 0;
	} else {
		lV1 = inV1;
		lV2 = inV2;
	}
	
	
	std::vector<double> lResult(3);
	lResult[0] = lV2[2]*lV1[1] - lV2[1]*lV1[2];
	lResult[1] = lV2[0]*lV1[2] - lV2[2]*lV1[0];
	lResult[2] = lV2[1]*lV1[0] - lV2[0]*lV1[1];
	
	return lResult;
}

double dot(const std::vector<double>& inV1, const std::vector<double>& inV2) {
	assert(inV1.size() == inV2.size());
	double lResult = 0;
	for(unsigned int i = 0; i < inV1.size(); ++i) {
		lResult += inV1[i]*inV2[i];
	}
	return lResult;
}

double norm(const std::vector<double>& inV1) {
	double lResult = 0;
	for(unsigned int i = 0; i < inV1.size(); ++i) {
		lResult += inV1[i]*inV1[i];
	}
	return sqrt(lResult);
}

std::vector<double> operator*(const std::vector<double>& inV1, double inValue)  {
	std::vector<double> lResult = inV1;
	for(unsigned int i = 0; i < lResult.size(); ++i) 
		lResult[i] *= inValue;
	return lResult;
}

std::vector<double> operator/(const std::vector<double>& inV1, double inValue)  {
	assert(inValue != 0);
	std::vector<double> lResult = inV1;
	for(unsigned int i = 0; i < lResult.size(); ++i) 
		lResult[i] /= inValue;
	return lResult;
}

std::vector<double> operator+(const std::vector<double>& inV1, const std::vector<double>& inV2) {
	assert(inV1.size() == inV2.size());
	std::vector<double> lResult(inV1.size());
	for(unsigned int i = 0; i < inV1.size(); ++i) {
		lResult[i] = inV1[i]+inV2[i];
	}
	return lResult;
}

std::vector<double> operator-(const std::vector<double>& inV1, const std::vector<double>& inV2) {
	assert(inV1.size() == inV2.size());
	std::vector<double> lResult(inV1.size());
	for(unsigned int i = 0; i < inV1.size(); ++i) {
		lResult[i] = inV1[i]-inV2[i];
	}
	return lResult;
}
