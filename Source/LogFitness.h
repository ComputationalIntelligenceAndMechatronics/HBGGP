/*
 *  LogFitness.h
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
 *  This file was created by Jean-Francois Dupuis on 13/05/08.
 */

#ifndef LogFitness_H
#define LogFitness_H

#include <beagle/GP.hpp>
#include <beagle/FitnessSimple.hpp>
#include <string>
#include <vector>

class LogFitness : public Beagle::FitnessSimple {
protected:
	std::vector<std::string> mVariableData;
	std::vector<std::string> mVariableName;
	std::vector<unsigned int> mVariableDataSet;
	std::vector<unsigned int> mUncompressedSize;
	
	std::map<unsigned int, double> mDataSet;
	
	std::map<std::string, std::string> mInfo;
	
public:
	//! LogFitness allocator type.
	typedef Beagle::AllocatorT<LogFitness,Beagle::FitnessSimple::Alloc>
	Alloc;
	//!< LogFitness handle type.
	typedef Beagle::PointerT<LogFitness,Beagle::FitnessSimple::Handle>
	Handle;
	//!< LogFitness bag type.
	typedef Beagle::ContainerT<LogFitness,Beagle::FitnessSimple::Bag>
	Bag;
	
	LogFitness();
	explicit LogFitness(float inFitness);
	virtual ~LogFitness() { }
	
	LogFitness& operator=(const LogFitness& inRightFitness);
	
//	virtual void read(Beagle::XMLNode::Handle& inNode);
//	virtual void write(Beagle::XMLStreamer& ioStreamer, bool inIndent) const;
#ifdef XMLBEAGLE
	virtual void read(Beagle::XMLNode::Handle& inNode);
	virtual void write(Beagle::XMLStreamer& ioStreamer, bool inIndent = true) const;
#else
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
#endif
	
	std::vector<std::string> getData() { return mVariableData; }
	std::vector<std::string> getName() { return mVariableName; }
	void addData(const std::string &inName, const std::vector<double> &inData, unsigned int inDataSet=0);
	void addDataSet(unsigned int inID, float inFitness);
	
	void addInfo(const std::string &inName, const std::string &inData);
	
	void clearData() { mDataSet.clear(); mVariableDataSet.clear(); mUncompressedSize.clear(); mVariableData.clear(); mVariableName.clear(); mInfo.clear(); }
};

#endif
