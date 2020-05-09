/*
 * RtMidiCommand.cpp
 *
 *  Created on: May 5, 2020
 *      Author: gil
 */

#include "RtMidiCommand.h"
#include <string>

namespace commands {

	RtMidiCommand::RtMidiCommand() {
		// TODO Auto-generated constructor stub

	}

	RtMidiCommand::~RtMidiCommand() {
		// TODO Auto-generated destructor stub
	}

	//test an inner class
	class MidiCommandSub2: public commands::RtMidiCommand{

	};
}

