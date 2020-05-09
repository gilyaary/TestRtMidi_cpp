/*
 * GetInfoCmd.h
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#ifndef GETINFOCMD_H_
#define GETINFOCMD_H_

#include "RtMidiCommand.h"
#include <string>
#include "RtMidi.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include "RtMidi.h"


namespace commands {

class GetInfoCmd: public RtMidiCommand {
public:
	GetInfoCmd();
	virtual ~GetInfoCmd();
	std::string exec();
};

} /* namespace commands */

#endif /* GETINFOCMD_H_ */
