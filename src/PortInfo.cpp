/*
 * PortInfo.cpp
 *
 *  Created on: Jun 5, 2020
 *      Author: gil
 */

#include "PortInfo.h"
PortInfo::PortInfo(){

}
PortInfo::PortInfo(ApiType apiType, PortType portType, PortDirection portDirection, int portNumber, char *name) {
	this->apiType = apiType;
	this->portType = portType;
	this->portDirection = portDirection;
	this->portNumber = portNumber;
	this->name = name;

}

PortInfo::~PortInfo() {
	// TODO Auto-generated destructor stub
}

