/*
 * GetInfoCmd.h
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#ifndef SETPORTSCMD_H_
#define SETPORTSCMD_H_

#include "RtMidiCommand.h"
#include <string.h>
#include "RtMidi.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include "RtMidi.h"
#include <stdio.h>
#include <map>
#include <vector>
#include "PortInfo.h"





namespace commands {

class SetPortsCmd: public RtMidiCommand {
public:
	SetPortsCmd();
	virtual ~SetPortsCmd();
	std::string exec(char cmd [], int size, RtMidiIn::RtMidiCallback incomingMidiEventCallback);
};

} /* namespace commands */

#endif /* SETPORTSCMD_H_ */
