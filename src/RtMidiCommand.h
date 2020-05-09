/*
 * RtMidiCommand.h
 *
 *  Created on: May 5, 2020
 *      Author: gil
 */

#ifndef RTMIDICOMMAND_H_
#define RTMIDICOMMAND_H_

//#include "GetInfoCmd.h"
//#include "ReceiveMessagesCmd.h"
//#include "TransmitMessageCmd.h"
//#include <string>
namespace commands{

class RtMidiCommand {
	public:
		RtMidiCommand();
		virtual ~RtMidiCommand();
	private:
		enum commandType{GET_API_NAMES,GET_ALL_PORTS,SEND_MESSAGE, START_RECEIVEING_MESSAGES_ON_PORT, STOP_RECEIVEING_MESSAGES_ON_PORT};

	};
}

#endif /* RTMIDICOMMAND_H_ */
