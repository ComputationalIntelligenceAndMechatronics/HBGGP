/***************************************************************************
 *   Copyright (C) 2004 by Jean-Francois Dupuis                            *
 *   jfdupuis@gel.ulaval.ca                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef BEAGLEROOTRETURN_H
#define BEAGLEROOTRETURN_H

#include <beagle/GP.hpp>
#include <beagle/config.hpp>
#include <beagle/macros.hpp>
#include <beagle/Object.hpp>
#include <beagle/castObjectT.hpp>
#include <beagle/PointerT.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/IOException.hpp>
#include <beagle/Context.hpp>


using namespace Beagle;

/**
@author Jean-Francois Dupuis
*/
class RootReturn : public Object
{

public:
  //! RootReturn allocator type.
	typedef AllocatorT< RootReturn, Object::Alloc > Alloc;
  //! RootReturn handle type.
	typedef PointerT< RootReturn, Object::Handle > Handle;
  //! RootReturn bag type.
	typedef ContainerT< RootReturn, Object::Bag > Bag;

	RootReturn() { mValue = 0; }
	RootReturn(const int &inValue) {mValue = inValue;}
	virtual ~RootReturn() {};

	virtual bool isEqual(const RootReturn& inRightObj) const;
	virtual bool isLess(const RootReturn& inRightObj) const;

	virtual void readWithContext(PACC::XML::ConstIterator inNode, Beagle::Context& ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent) const;

	int getValue() const { return mValue; }
	void setValue(int inValue) { mValue = inValue; }

private:
	int mValue;
};

#endif
