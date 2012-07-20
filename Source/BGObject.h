/*
 *  BGObjectT.h
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
 *  This file was created by Jean-Francois Dupuis on 13/05/09.
 */

#ifndef BGObject_H
#define BGObject_H

#include <beagle/GP.hpp>
#include <beagle/PointerT.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/GP/Datum.hpp>

template <class T>
class BGObjectT : public Beagle::GP::Datum {
public:
	typedef Beagle::AllocatorT< BGObjectT<T>, Beagle::GP::Datum::Alloc > Alloc;
	typedef Beagle::PointerT< BGObjectT<T>, Beagle::GP::Datum::Handle > Handle;
	typedef Beagle::ContainerT< BGObjectT<T>, Beagle::GP::Datum::Bag >	Bag;
	
	BGObjectT(const T& inValue=NULL) : mValue(inValue) {}
	~BGObjectT() {}
	
	inline T& getValue() const { return mValue; }
	inline T& getValue() { return mValue; }
	
	inline void setValue(const T& inValue) { mValue = inValue; };
	
protected:
	T mValue;
};

#endif
