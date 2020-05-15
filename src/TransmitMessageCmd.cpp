/*
 * TransmitMessageCmd.cpp
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#include "TransmitMessageCmd.h"

namespace commands {

TransmitMessageCmd::TransmitMessageCmd() {
	// TODO Auto-generated constructor stub
	this->apiMap["J"] = RtMidi::UNIX_JACK;
	this->apiMap["A"] = RtMidi::LINUX_ALSA;
	this->apiMap["M"] = RtMidi::MACOSX_CORE;
	this->apiMap["W"] = RtMidi::WINDOWS_MM;
	this->apiMap["D"] = RtMidi::RTMIDI_DUMMY;

	//std::map<int,RtMidiOut*> *ports_map = new std::map<int,RtMidiOut*>();
	//(this->apiPorts).insert(std::pair<RtMidi::Api, std::map<int,RtMidiOut*>>(api,*ports_map));
	this->apiPorts[RtMidi::UNIX_JACK] = *(new std::map<int, RtMidiOut*>());
	this->apiPorts[RtMidi::LINUX_ALSA] = *(new std::map<int, RtMidiOut*>());
	this->apiPorts[RtMidi::MACOSX_CORE] = *(new std::map<int, RtMidiOut*>());
	this->apiPorts[RtMidi::WINDOWS_MM] = *(new std::map<int, RtMidiOut*>());
	this->apiPorts[RtMidi::RTMIDI_DUMMY] = *(new std::map<int, RtMidiOut*>());
}

TransmitMessageCmd::~TransmitMessageCmd() {
	/*
	 std::map<RtMidi::Api,std::map<int,RtMidiOut>>::iterator apiIterator = this->apiPorts.begin();

	 while (apiIterator != this->apiPorts.end()) {
	 RtMidi::Api api = apiIterator->first;
	 std::map<int,RtMidiOut> portsMap = apiIterator->second;
	 std::map<int,RtMidiOut>::iterator portsIterator = portsMap.begin();
	 while (portsIterator != portsMap.end()) {
	 int portNumber = portsIterator->first;
	 RtMidiOut portOutput = portsIterator->second;
	 portOutput.closePort();
	 delete &portOutput;
	 }
	 portsMap.clear();
	 delete &portsMap;
	 apiIterator++;
	 }
	 this->apiPorts.clear();
	 delete &this->apiPorts;
	 this->apiMap.clear();
	 delete &this->apiMap;
	 */
}

StringTokenizer st;

void TransmitMessageCmd::transmitMessage(std::string cmd) {

	//std::cout << "\nCommand: " << cmd << " Size: " << cmd.size() << "\n";
	std::vector<std::string> v;
	st.tokenize(cmd,&v);
	//std::cout << "Received Token Vector Size: " << v.size() << "\n";
	//v.push_back("A");
	//v.push_back("0");
	//v.push_back("1");

	if (v.size() >= 3) {
		std::string api_str = v[0];
		std::string port_str = v[1];
		int port_number = stoi(port_str);
		RtMidi::Api api = this->apiMap[api_str];
		if (api != NULL) {
			//std::cout << "Api Found " << api_str << "\n";
			RtMidiOut* port_addr = this->apiPorts[api][port_number];
			//std::cout << "Port address " << port_addr << "\n";
			if (port_addr == 0) {
				//std::cout << "Port Does NOT exist. Creating new Port";
				this->apiPorts[api][port_number] = new RtMidiOut(api, "client_1");
				port_addr = this->apiPorts[api][port_number];
			} else {
				//std::cout << "Port already exists.";
			}
			if ( ! port_addr->isPortOpen() ){
				//std::cout << "Port closed. Attempting to open";
				try{
					port_addr->openPort(port_number, "PORT-" + port_str);
				}catch(int ex){
					//std::cout << "Exception when trying to open port. Ex=" << ex;
				}
			}
			if ( port_addr->isPortOpen() ){
				try{
					std::vector<unsigned char> message;
					// Note On: 144, 64, 90
					// Note On: 128, 64, 40
					//std::cout << "vector length: " << v.size() << "\n";
					for(int j=2; j<v.size(); j++){
						int value = stoi(v[j]);
						std::cout << "." << value;
						message.push_back(value);
					}
					std::cout << "\n";
					port_addr->sendMessage(&message);
					//TODO: close port command
				}catch(int ex){
					//std::cout << "Exception Sending Midi. Ex=" << ex;
				}
			}
		}
	}
}
}

