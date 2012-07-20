/*
 *  stringutil.h
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
 *  This file was created by Jean-Francois Dupuis on 17/11/09. The code
 *	is come from OpenBeagle sources.
 */

#ifndef stringutil_H
#define stringutil_H

#include <sstream>
#include <string>

/*!
 *  \brief Test if a floating-point number is in a not-a-number state.
 *  \param inValue Value to evaluate not-a-number state.
 *  \return True if the floating-point value is not-a-number, otherwise false.
 */
template <class T>
inline bool isNaN(const T& inValue)
{
	return ((inValue == inValue) == false);
}


/*!
 *  \brief Test if a floating-point number is infinite.
 *  \param inValue Value to evaluate finiteness.
 *  \return True if the floating-point value is equal to infinity, otherwise false.
 */
template <class T>
inline bool isInfinity(const T& inValue)
{
#ifdef HAVE_NUMERIC_LIMITS
	if(std::numeric_limits<T>::has_infinity) {
		return ((inValue == std::numeric_limits<T>::infinity()) ||
				(inValue == -std::numeric_limits<T>::infinity()));
	}
#endif
	T lZero(0.0);
	T lInfinity(1.0 / lZero);
	return ((inValue == lInfinity) || (inValue == -lInfinity));
}

/*!
 *  \brief  Convert an integer into a string.
 *  \param  inInteger Integer to convert into a string.
 *  \return string containing conversion of the integer.
 */
inline std::string int2str(long inInteger)
{
	std::ostringstream lStringOS;
	lStringOS << inInteger;
	return lStringOS.str().c_str();
}


/*!
 *  \brief  Convert a string into a integer.
 *  \param  inString String to convert into an integer.
 *  \return Long containing the conversion of the string.
 */
inline long str2int(const std::string& inString)
{
	std::string lStr(inString.c_str());
	std::istringstream lStringIS(lStr);
	long lInteger;
	lStringIS >> lInteger;
	return lInteger;
}


/*!
 *  \brief  Convert an unsigned integer into a string.
 *  \param  inInteger Integer to convert into a string.
 *  \return string containing conversion of the integer.
 */
inline std::string uint2str(unsigned long inInteger)
{
	std::ostringstream lStringOS;
	lStringOS << inInteger;
	return lStringOS.str().c_str();
}


/*!
 *  \brief  Convert a string into an unsigned integer.
 *  \param  inString String to convert into an integer.
 *  \return Unsigned long containing the conversion of the string.
 */
inline unsigned long str2uint(const std::string& inString)
{
	std::istringstream lStringIS(inString.c_str());
	unsigned long lInteger;
	lStringIS >> lInteger;
	return lInteger;
}


/*!
 *  \brief  Convert a double into a string.
 *  \param  inDouble Double to convert into a string.
 *  \return string containing conversion of the double.
 */
inline std::string dbl2str(double inDouble)
{
	std::ostringstream lStringOS;
	if(isNaN(inDouble)) lStringOS << "nan";
	else if(isInfinity(inDouble)) {
		if(inDouble < 0.0) lStringOS << "-inf";
		else lStringOS << "inf";
	}
	else lStringOS << inDouble;
	return lStringOS.str().c_str();
}


/*!
 *  \brief  Convert a double into a string.
 *  \param  inDouble Double to convert into a string.
 *  \param  inWidth Width of the written double.
 *  \return string containing conversion of the double.
 */
inline std::string dbl2str(double inDouble, unsigned int inWidth)
{
	using namespace std;
	ostringstream lStringOS;
	if(inWidth > 1) lStringOS.precision(inWidth-1);
	lStringOS.setf(ostringstream::left);
	lStringOS.width(inWidth);
	if(isNaN(inDouble)) lStringOS << "nan" << std::flush;
	else if(isInfinity(inDouble)) {
		if(inDouble < 0.0) lStringOS << "-inf" << std::flush;
		else lStringOS << "inf" << std::flush;
	}
	else lStringOS << inDouble << std::flush;
	std::string lString = lStringOS.str().c_str();
	if(lString.size() > inWidth) {
		std::string::size_type lPosE = lString.find('e', 0);
		if(lPosE < lString.size()) {
			std::string lStringE(lString, lPosE, lString.size());
			if(lStringE.size() < inWidth)
				lString.replace((inWidth-lStringE.size()), inWidth, lStringE);
		}
	}
	if(lString.size() != inWidth) lString.resize(inWidth, ' ');
	return lString;
}


/*!
 *  \brief  Convert a string into a double.
 *  \param  inString String to convert into a double.
 *  \return Double containing the conversion of the string.
 */
inline double str2dbl(const std::string& inString)
{
	if(inString == "nan") {
		double lZero(0.0);
		return (lZero / lZero);
	}
	else if(inString == "inf") {
#ifdef HAVE_NUMERIC_LIMITS
		if(std::numeric_limits<double>::has_infinity) {
			return std::numeric_limits<double>::infinity();
		}
#endif // HAVE_NUMERIC_LIMITS
		double lZero(0.0);
		return (1.0 / lZero);
	}
	else if(inString == "-inf") {
#ifdef HAVE_NUMERIC_LIMITS
		if(std::numeric_limits<double>::has_infinity) {
			return -std::numeric_limits<double>::infinity();
		}
#endif // HAVE_NUMERIC_LIMITS
		double lZero(0.0);
		return (-1.0 / lZero);
	}
	
	std::istringstream lStringIS(inString.c_str());
	double lDouble;
	lStringIS >> lDouble;
	return lDouble;
}


/*!
 *  \brief Convert a integer to its ordinal form , i.e. 1 -> 1st, 22 -> 22nd.
 *  \param inNumber Integer to convert.
 *  \return Ordinal form of the number.
 */
inline std::string uint2ordinal(unsigned int inNumber)
{
	// Write number to string
	std::ostringstream outStream;
	outStream << inNumber;
	std::string lString = outStream.str();
	
	// Insert commas for easier reading
	if(inNumber>9999) {
		unsigned int lCount=0;
		for(unsigned int i=lString.length(); i>1; --i) {
			++lCount;
			if(lCount==3) {
				lString.insert(i-1,",");
				lCount=0;
			}
		}
	}
	
	// Choose appropriate suffix
	std::string lSuffix = "th";
	switch(inNumber % 10) {
		case 1: {lSuffix = "st"; break;}
		case 2: {lSuffix = "nd"; break;}
		case 3: {lSuffix = "rd"; break;}
	}
	switch(inNumber % 100) {
		case 11:
		case 12:
		case 13: {lSuffix = "th"; break;}
	}
	lString.append(lSuffix);
	
	return lString;
}



#endif
