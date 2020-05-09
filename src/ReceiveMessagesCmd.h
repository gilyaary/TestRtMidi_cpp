/*
 * ReceiveMessagesCmd.h
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#ifndef RECEIVEMESSAGESCMD_H_
#define RECEIVEMESSAGESCMD_H_

#include "RtMidiCommand.h"
#include <string>

namespace commands {

class ReceiveMessagesCmd: public RtMidiCommand {
public:
	ReceiveMessagesCmd();
	virtual ~ReceiveMessagesCmd();
	void receiveMessages(std::string cmd);
};

} /* namespace commands */

#endif /* RECEIVEMESSAGESCMD_H_ */
