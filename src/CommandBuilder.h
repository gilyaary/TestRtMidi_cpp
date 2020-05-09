/*
 * CommandBuilder.h
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#ifndef COMMANDBUILDER_H_
#define COMMANDBUILDER_H_

#include "RtMidiCommand.h"
#include "GetInfoCmd.h"
#include "ReceiveMessagesCmd.h"
#include "TransmitMessageCmd.h"
#include <string>

namespace commands {

class CommandBuilder {
public:
	CommandBuilder();
	virtual ~CommandBuilder();
	static RtMidiCommand parseCommandString(char* str);
};

} /* namespace commands */

#endif /* COMMANDBUILDER_H_ */
