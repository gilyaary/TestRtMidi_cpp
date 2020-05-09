/*
 * GetInfoCmd.cpp
 *
 *  Created on: May 6, 2020
 *      Author: gil
 */

#include "GetInfoCmd.h"

namespace commands {

GetInfoCmd::GetInfoCmd() {
	// TODO Auto-generated constructor stub

}

GetInfoCmd::~GetInfoCmd() {
	// TODO Auto-generated destructor stub
}

std::string GetInfoCmd::exec() {
	std::map<int, std::string> apiMap;
	apiMap[RtMidi::MACOSX_CORE] = "OS-X CoreMIDI";
	apiMap[RtMidi::WINDOWS_MM] = "Windows MultiMedia";
	apiMap[RtMidi::UNIX_JACK] = "Jack Client";
	apiMap[RtMidi::LINUX_ALSA] = "Linux ALSA";
	apiMap[RtMidi::RTMIDI_DUMMY] = "RtMidi Dummy";

	std::vector<RtMidi::Api> apis;
	RtMidi::getCompiledApi(apis);
	std::string output = "{\"apis\":[\n";
	//Get the input and output ports for each API (Alsa, Jack).
	std::vector< RtMidi::Api > compiledApis;
	RtMidi :: getCompiledApi( compiledApis );

	for ( unsigned int i=0; i<compiledApis.size(); i++ ){
		RtMidiIn *midiin = 0;
		RtMidiOut *midiout = 0;
		try {
			output += "{\n";

			std::string apiName = apiMap[apis[i]];
			const std::string name = RtMidi::getApiName(apis[i]);
			const std::string displayName = RtMidi::getApiDisplayName(apis[i]);
			output += "\t\"number\":";
			output += std::to_string((int) apis[i]);
			output += ",\n\t\"name\":\"";
			output += name;
			output += "\",\n\t\"displayName\":\"";
			output += displayName;
			output += "\",\n\t\"apiName\":\"";
			output += apiName;
			output += "\",\n";

			// RtMidiIn constructor ... exception possible
			midiin = new RtMidiIn(apis[i]);
			//std::cout << "\nCurrent input API: " << apiMap[ midiin->getCurrentApi() ] << std::endl;
			// Check inputs.
			unsigned int nPorts = midiin->getPortCount();
			//std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
			output += "\n\t\"input_ports\":[\n";
			for ( unsigned i=0; i<nPorts; i++ ) {
				std::string portName = midiin->getPortName(i);
				output += "\t\t{\"portName\":\"";
				output += portName;
				output += "\"},\n";
			}
			output += "\t],";
			// RtMidiOut constructor ... exception possible
			midiout = new RtMidiOut(apis[i]);
			//std::cout << "\nCurrent output API: " << apiMap[ midiout->getCurrentApi() ] << std::endl;
			// Check outputs.
			nPorts = midiout->getPortCount();
			//std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
			output += "\n\t\"output_ports\":[\n";
			for ( unsigned i=0; i<nPorts; i++ ) {
				std::string portName = midiout->getPortName(i);
				output += "\t\t{\"portName\":\"";
				output += portName;
				output += "\"},\n";
			}
			output += "\t]\n";
			output += "},\n";
		} catch ( RtMidiError &error ) {
			error.printMessage();
		}

		delete midiin;
		delete midiout;
	}
	output += "]}\n";

	return output;
}

} /* namespace commands */
