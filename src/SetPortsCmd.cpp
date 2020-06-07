/*
 * SetPortsCmd.cpp
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#include "SetPortsCmd.h"

namespace commands {

/*
 Midi set port command syntax:

 3#{I/O}.{A/J}.{V/R}.{PORT_NUMBER}.{NAME}
 I/O: Input Output
 A/J: Alsa. Jack
 V/R: Virtual or Real port
 PORT_NUMBER: Integer with port index number
 NAME: Any user identifier

 //TODO: we can either create a new Listener object for each input port and handle the callback
 //or use one listener for all ports

 //each midiin controls a single input port
 //it may be good to hold them in a Map or a vector.
 //Idea: defining Virtual Input and Output ports may be a much easier way to control the program
 //we can simply define these ports and then connect them to other ports

 RtMidiIn* midiin = new RtMidiIn(RtMidi::Api::LINUX_ALSA);
 midiin->openPort( 0 );
 midiin->setCallback( &mycallback );
 midiin->ignoreTypes( false, false, false );
 midiin->cancelCallback();
 midiin->closePort();
 midiin->getPortCount();
 midiin->getPortName(0);
 midiin->isPortOpen();
 midiin->openVirtualPort("GIL_IN_PORT");
 midiin->setErrorCallback(errCallbackFuntion);
 midiin->setPortName("NEW_NAME FOR_GIL_IN_PORT");
 */

int calcSize(char* str);

SetPortsCmd::SetPortsCmd() {
	// TODO Auto-generated constructor stub

}

SetPortsCmd::~SetPortsCmd() {
	// TODO Auto-generated destructor stub
}

std::string SetPortsCmd::exec(char midi_command[], int size, RtMidiIn::RtMidiCallback incomingMidiEventCallback ) {
	std::cout << "Received Command to set Port: " << midi_command << "\n";


	int valid_fields[] = { 0, 0, 0, 0, 0 };
	ApiType apiType = ALSA;
	PortType portType = VIRTUAL;
	PortDirection portDirection = IN;
	int portNumber = 0;
	char* name;

	char *token = strtok(midi_command, ".");
	for (int i = 0; token != NULL && i < 5; i++) {
		//printf("%s\n", token);
		switch (i) {
		case 0:
			if (token[0] == 'I') {
				valid_fields[i] = 1;
				portDirection = IN;
			} else if (token[0] == 'O') {
				valid_fields[i] = 1;
				portDirection = OUT;
			}
			break;
		case 1:
			if (token[0] == 'A') {
				valid_fields[i] = 1;
				apiType = ALSA;
			} else if (token[0] == 'J') {
				valid_fields[i] = 1;
				apiType = JACK;
			}
			break;
		case 2:
			if (token[0] == 'V') {
				valid_fields[i] = 1;
				portType = VIRTUAL;
			} else if (token[0] == 'R') {
				valid_fields[i] = 1;
				portType = REAL;
			}
			break;
		case 3:
			//parse port number

		{
			int portIndex = std::stoi(token);
			valid_fields[i] = 1;
			portNumber = portIndex;
			//std::cout << "Port Index: " << portIndex << "\n";
			break;
		}
		case 4:
		{//This is the port name
			valid_fields[i] = 1;
			//printf("Port Name: %s\n", token);
			name = token;
			break;
		}
		default:
			printf("Unknown field: %s\n", token);
		}
		token = strtok(NULL, ".");
	}

	if (valid_fields[0] && valid_fields[1] && valid_fields[2] && valid_fields[3]
			&& valid_fields[4]) {

		char name2 [calcSize(name)];
		strcpy(name2, name);
		PortInfo *portInfo = new PortInfo(apiType, portType, portDirection, portNumber, name);
		port_map[portNumber] = portInfo;

		std::map<int, PortInfo*>::iterator it;
		for ( it = port_map.begin(); it != port_map.end(); it++ )
		{
			std::cout << "Saved Name: " << it->second->getName() << "\n";
		}



		if (portDirection == IN) {
			RtMidiIn* midiin;
			if (apiType == ALSA) {
				midiin = new RtMidiIn(RtMidi::Api::LINUX_ALSA);
			} else if (apiType == JACK) {
				midiin = new RtMidiIn(RtMidi::Api::UNIX_JACK);
			} else {
				midiin = new RtMidiIn(RtMidi::Api::LINUX_ALSA);
			}
			if(portType == VIRTUAL){
				midiin->openVirtualPort(name);
			}else{
				midiin->openPort(portNumber, name);
			}

			midiin->setCallback(incomingMidiEventCallback , portInfo);
			midiin->ignoreTypes(false, false, false);
			portInfo->setMidiin(midiin);
		}
	}

	return "OK\n@@@\n";
}

void SetPortsCmd::remove( int portNumber ) {
	PortInfo* portInfo = this->port_map[portNumber];
	RtMidiIn* midiin = portInfo->getMidiin();
	midiin->cancelCallback();
	if(midiin->isPortOpen()){
		midiin->closePort();
		//midiin->getPortCount();
		//midiin->getPortName(0);
	}
	this->port_map.erase(portNumber);
}

int calcSize(char* str){
	int size = 0;
	for(;str[size] != '\0'; size++);
	return size+1;
}


} /* namespace commands */
