/*
 * PortInfo.h
 *
 *  Created on: Jun 5, 2020
 *      Author: gil
 */

#ifndef PORTINFO_H_
#define PORTINFO_H_

#include <string>

enum ApiType {ALSA,JACK};
enum PortType {VIRTUAL, REAL};
enum PortDirection {IN, OUT};

class PortInfo {
public:
	PortInfo();
	PortInfo(ApiType apiType, PortType portType, PortDirection portDirection, int portNumber, char *name);
	virtual ~PortInfo();

	ApiType getApiType() const {
		return apiType;
	}

	void setApiType(ApiType apiType) {
		this->apiType = apiType;
	}

	const char* getName(){
		return this->name.c_str();
	}

	void setName(char* name) {
		this->name = std::string(name);
	}

	PortDirection getPortDirection() const {
		return portDirection;
	}

	void setPortDirection(PortDirection portDirection) {
		this->portDirection = portDirection;
	}

	int getPortNumber() const {
		return portNumber;
	}

	void setPortNumber(int portNumber = 0) {
		this->portNumber = portNumber;
	}

	PortType getPortType() const {
		return portType;
	}

	void setPortType(PortType portType) {
		this->portType = portType;
	}

private:
	ApiType apiType;
	PortType portType;
	PortDirection portDirection;
	int portNumber = 0;
	std::string name;



};


#endif /* PORTINFO_H_ */
