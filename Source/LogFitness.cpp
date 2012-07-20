/*
 *  LogFitness.cpp
 *  Copyright 2008 Jean-Francois Dupuis.
 *
 *  This is free software: you can redistribute it and/or modify
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

#include "LogFitness.h"
#include <beagle/IOException.hpp>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include "stringcompression.h"
#include <assert.h>

using namespace Beagle;

LogFitness::LogFitness() : FitnessSimple() {}
LogFitness::LogFitness(float inFitness) : FitnessSimple(inFitness) {}


void LogFitness::read(PACC::XML::ConstIterator inIter) {	
	Beagle_StackTraceBeginM();
	
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Fitness"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Fitness> expected!");
	
	std::string lValid = inIter->getAttribute("valid").c_str();
	
	if(lValid.empty() || (lValid == "yes")) {
		std::string lType = inIter->getAttribute("type").c_str();
		if(lType != "LogFitness")
			throw Beagle_IOExceptionNodeM((*inIter), "fitness type mismatch!");
		
		mUncompressedSize.clear();
		mVariableData.clear();
		mVariableName.clear();
		mInfo.clear();
		
		for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; lChild=lChild->getNextSibling()) {
			if(lChild->getValue() == "Obj") {
				PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
				if(!lChild2) throw Beagle_IOExceptionNodeM(*inIter, "no fitness value present!");
				if(lChild2->getType() != PACC::XML::eString)
					throw Beagle_IOExceptionNodeM(*inIter, "no fitness value present!");

				mFitness = str2dbl(lChild2->getValue().c_str());

			}
			else if(lChild->getValue() == "Data") {
				mVariableName.push_back(lChild->getAttribute("Name"));
				mUncompressedSize.push_back( str2uint(lChild->getAttribute("usize")) );
				if(str2uint(lChild->getAttribute("usize")) != 0) {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) {
						throw Beagle_IOExceptionNodeM((*lChild), "needed a CDATA value in the <Path> tag!");
					}
					if(lChild2->getType() != PACC::XML::eString) {
						throw Beagle_IOExceptionNodeM((*lChild2), "needed a string value in the <Path> tag!");
					}
					mVariableData.push_back(lChild2->getValue());
				} else {
					mVariableData.push_back("");
				}
			}
			else if(lChild->getValue() == "DataSet") {
				unsigned int lID = str2uint(lChild->getAttribute("ID"));
				double lFitness = 0;
				for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; lChild2=lChild2->getNextSibling()) {
					if(lChild2->getValue() == "Obj") {
						PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
						if(!lChild3) throw Beagle_IOExceptionNodeM(*inIter, "no fitness value present!");
						if(lChild3->getType() != PACC::XML::eString)
							throw Beagle_IOExceptionNodeM(*inIter, "no fitness value present!");
						lFitness = str2dbl(lChild3->getValue());
					}
					else if(lChild2->getValue() == "Data") {
						mVariableName.push_back(lChild2->getAttribute("Name"));
						mVariableDataSet.push_back(lID);
						mUncompressedSize.push_back( str2uint(lChild2->getAttribute("usize")) );
						if(str2uint(lChild2->getAttribute("usize")) != 0) {
							PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
							if(!lChild3) {
								throw Beagle_IOExceptionNodeM((*lChild2), "needed a CDATA value in the <Path> tag!");
							}
							if(lChild3->getType() != PACC::XML::eString) {
								throw Beagle_IOExceptionNodeM((*lChild3), "needed a string value in the <Path> tag!");
							}
							mVariableData.push_back(lChild3->getValue());
						} else {
							mVariableData.push_back("");
						}
					}
				}
				
				mDataSet[lID] = lFitness;
			}
			else if(lChild->getValue() == "Info") {
				for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; lChild2=lChild2->getNextSibling()) {
					std::string lInfoName = lChild2->getValue();
					PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
					if(!lChild3) throw Beagle_IOExceptionNodeM(*inIter, "info data present!");
					if(lChild3->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*inIter, "info data present!");
					std::string lInfoData = lChild3->getValue();
					
					mInfo[lInfoName] = lInfoData;
				}
			}
			
			else //Ignore anything else
				continue;
			
		}
		setValid();
	}
	else if(lValid == "no") setInvalid();
	else throw Beagle_IOExceptionNodeM((*inIter), "bad fitness validity value!");
	Beagle_StackTraceEndM("void LogFitness::read(PACC::XML::ConstIterator inIter)");
	
}


void LogFitness::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Fitness", inIndent);
	ioStreamer.insertAttribute("type", "LogFitness");
	if(isValid()) {
		ioStreamer.openTag("Obj", false);

		ioStreamer.insertStringContent(dbl2str(mFitness).c_str());

		ioStreamer.closeTag();
		if(mDataSet.size() == 0) {
			for(unsigned int i = 0; i < mVariableData.size(); ++i) {
				ioStreamer.openTag("Data", false);
				ioStreamer.insertAttribute("Name", mVariableName[i]);
				ioStreamer.insertAttribute("usize", uint2str(mUncompressedSize[i]));
				ioStreamer.insertStringContent(mVariableData[i]);
				ioStreamer.closeTag();
			}
		} else {
			for(std::map<unsigned int, double>::const_iterator lIter = mDataSet.begin(); lIter != mDataSet.end(); ++lIter) {
				ioStreamer.openTag("DataSet", inIndent);
				ioStreamer.insertAttribute("ID", lIter->first);
				ioStreamer.openTag("Obj", false);
				ioStreamer.insertStringContent( dbl2str(lIter->second).c_str() );
				ioStreamer.closeTag();
				for(unsigned int i = 0; i < mVariableData.size(); ++i) {
					if(lIter->first == mVariableDataSet[i]) {
						ioStreamer.openTag("Data", false);
						ioStreamer.insertAttribute("Name", mVariableName[i]);
						ioStreamer.insertAttribute("usize", uint2str(mUncompressedSize[i]));
						ioStreamer.insertStringContent(mVariableData[i]);
						ioStreamer.closeTag();
					}
				}
				ioStreamer.closeTag();
			}
		}
		if(!mInfo.empty()) {
			ioStreamer.openTag("Info", inIndent);
			for(std::map<std::string,std::string>::const_iterator lIter = mInfo.begin(); lIter != mInfo.end(); ++lIter) {
				ioStreamer.openTag(lIter->first, false);
				ioStreamer.insertStringContent( lIter->second);
				ioStreamer.closeTag();
			}		
			ioStreamer.closeTag();
		}
		
	}
	else ioStreamer.insertAttribute("valid", "no");
	ioStreamer.closeTag();
	Beagle_StackTraceEndM("void LogFitness::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

void LogFitness::addData(const std::string &inName, const std::vector<double> &inData, unsigned int inDataSet) {
	ostringstream lDataStream;
	assert(inData.size() > 0);
	for(unsigned int i = 0; i < inData.size() - 1; ++i) {
		lDataStream << inData[i] << " ";
	}
	lDataStream << inData.back();
	
	std::string lCompressedStr;
	compressString(lDataStream.str(), lCompressedStr);
	string2hex(lCompressedStr);
	
	
	mUncompressedSize.push_back(lDataStream.str().size());
	mVariableData.push_back(lCompressedStr);
	mVariableName.push_back(inName);
	mVariableDataSet.push_back(inDataSet);
}

void LogFitness::addDataSet(unsigned int inID, float inFitness) {
	mDataSet[inID] = inFitness;
}

void LogFitness::addInfo(const std::string &inName, const std::string &inData) {
	mInfo[inName] = inData;
}

LogFitness& LogFitness::operator=(const LogFitness& inRightFitness) {
	mVariableData = inRightFitness.mVariableData;
	mVariableName = inRightFitness.mVariableName;
	mVariableDataSet = inRightFitness.mVariableDataSet;
	mUncompressedSize = inRightFitness.mUncompressedSize;
	mDataSet = inRightFitness.mDataSet;
	mInfo = inRightFitness.mInfo;
	
	setValue(inRightFitness.getValue());
	if(inRightFitness.isValid()) setValid();
	else setInvalid();

	return *this;
}
