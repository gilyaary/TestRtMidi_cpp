/*
 * CommandBuilder.cpp
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#include "CommandBuilder.h"

namespace commands {

CommandBuilder::CommandBuilder() {
	// TODO Auto-generated constructor stub

}

CommandBuilder::~CommandBuilder() {
	// TODO Auto-generated destructor stub
}

RtMidiCommand CommandBuilder::parseCommandString(char* str){
	int len = sizeof(str);
	if(len>=1){
		char c = str[0];
		switch(c){
			case '1':{
				GetInfoCmd* cmd1 = new GetInfoCmd();
				return *cmd1;
			}
			case '2':{
				TransmitMessageCmd* cmd2 = new TransmitMessageCmd();
				return *cmd2;
			}
			case '3':{
				ReceiveMessagesCmd* cmd3 = new ReceiveMessagesCmd();
				return *cmd3;
			}
		}
	}
}

} /* namespace commands */
