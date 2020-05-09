/*
 * TransmitMessageCmd.h
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#ifndef TRANSMITMESSAGECMD_H_
#define TRANSMITMESSAGECMD_H_

#include "RtMidiCommand.h"
#include <string>
#include "RtMidi.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include "StringTokenizer.h"

// Platform-dependent sleep routines.
#if defined(WIN32)
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif


namespace commands {

class TransmitMessageCmd: public RtMidiCommand {
public:
	TransmitMessageCmd();
	virtual ~TransmitMessageCmd();
	void transmitMessage(std::string str);
private:
	std::map<RtMidi::Api,std::map<int,RtMidiOut*>> apiPorts;
	std::map<std::string, RtMidi::Api> apiMap;
};

} /* namespace commands */

#endif /* TRANSMITMESSAGECMD_H_ */
