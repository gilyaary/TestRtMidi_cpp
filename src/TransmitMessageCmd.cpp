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

bool is_number(const std::string& s);

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

/*
void TransmitMessageCmd::transmitMessage(char cmd [], int size) {

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
		int port_number = 0;
		try{
			 if(is_number(port_str)){
				 port_number = stoi(port_str);
			 }
			 else{
				 std::cout << "Invalid Port: " << port_str;
				 return;
			 }
		}catch(int e){

		}
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
					std::cout << "Exception when trying to open port. Ex=" << ex;
				}
			}
			if ( port_addr->isPortOpen() ){
				try{
					std::vector<unsigned char> message;
					// Note On: 144, 64, 90
					// Note On: 128, 64, 40
					//std::cout << "vector length: " << v.size() << "\n";
					for(int j=2; j<v.size(); j++){
						if(is_number(v[j])){
							int value = stoi(v[j]);
							//std::cout << "." << value;
							message.push_back(value);
						}
						else{
							std::cout << "Invalid Number in midi command: " << v[j] << "\n";
							return;
						}
					}
					//std::cout << "\n";
					port_addr->sendMessage(&message);
					//TODO: close port command
				}catch(int ex){
					//std::cout << "Exception Sending Midi. Ex=" << ex;
				}
			}
		}
	}
}
*/

void TransmitMessageCmd::transmitMessage(char cmd [], int size) {

	std::cout << "TX Midi Command: " << cmd << " Size: " << size << "\n";

	if(size >= 8){
		char api_char = cmd[0];
		char port = cmd[2];
		int port_number = port - 48;
		//std::cout << "api: " << api_char << "\n";
		//std::cout << "port: " << port_number << "\n";
		std::string api_string = api_char == 'J' ? "J" : api_char == 'A' ? "A" : "A";
		RtMidi::Api api = this->apiMap[api_string];
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
					port_addr->openPort(port_number, "RT_PORT_" + std::to_string(port_number));
				}catch(int ex){
					std::cout << "Exception when trying to open port. Ex=" << ex;
				}
			}
			if ( port_addr->isPortOpen() ){
				try{
					std::vector<unsigned char> message;
					// Note On: 144, 64, 90
					// Note On: 128, 64, 40

					int last_number [] = {-1,-1,-1};
					int last_number_index = 0;
					for(int k=4; k< size; k++){
						char c = cmd[k];
						int value = c - 48;
						if(value >=0 && value <= 9){
							//std::cout << value;
							last_number[last_number_index] = value;
							last_number_index ++;
						}
						if(c == '.' || k == size-1){
							int value = 0;
							for(int m=0; m<last_number_index;m++){
								int power_of_10 = last_number_index - m -1;
								int current_digit = last_number[m];
								//std::cout << "Digit: " << current_digit << "\n";
								for(int n=0; n<power_of_10; n++){
									current_digit *= 10;
								}
								//std::cout << "Digit Multiplied: " << current_digit << "\n";
								value += current_digit;
								//std::cout << "Accumulated Value: " << value << "\n";
							}
							//convert last_number to an int
							//append to command
							message.push_back(value);
							//std::cout << "Pushing Value: " << value << "\n";
							last_number_index = 0;
						}
					}
					//std::cout << last_number << "\n";
					port_addr->sendMessage(&message);
					//TODO: close port command
					last_number[0] = -1;
					last_number[1] = -1;
					last_number[2] = -1;
				}catch(int ex){
					//std::cout << "Exception Sending Midi. Ex=" << ex;
				}
			}
		}
	}
}


bool is_number(const std::string& s)
	{
	    std::string::const_iterator it = s.begin();
	    while (it != s.end() && std::isdigit(*it)) ++it;
	    return !s.empty() && it == s.end();
	}
}

