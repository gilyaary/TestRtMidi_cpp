// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <vector>
#include "GetInfoCmd.h"
#include "TransmitMessageCmd.h"
#include "ReceiveMessagesCmd.h"

using namespace std;

#define PORT 8888

map<int, vector<std::string>> socket_map;

commands::GetInfoCmd getInfoCmd;
commands::TransmitMessageCmd transmitMessageCmd;
commands::ReceiveMessagesCmd receiveMessagesCmd;
void processCommand(int new_socket, char* buffer);
//this thread sends MIDI events to the client socket
//It holds a vector of ports to listen to


void inputThreadFunction(int new_socket)
{
	printf("entering modified input thread function\n");
	vector<string> command;
	std:string commands_str;
	char in_buffer[8092] = {0};
	ssize_t byte_count;
	char last_tx_7th_char = '2';
	int sentBytes = 0;

	do{
		byte_count = read( new_socket , in_buffer, 8092);
		if (byte_count == -1 || (in_buffer[0] == 'Q' && in_buffer[1] == 'Q' && in_buffer[2] == 'Q')) {
			printf("client sent disconnect signal. Existing input thread\n");
			close(new_socket);
			return;
		}
		//printf(in_buffer);printf("\n");
		if( byte_count >= 2 ){
			char command = in_buffer[0];
			if ( command == '1' ){
				std::string info = getInfoCmd.exec();
				const void* infoStr = info.c_str();
				int len = info.size();
				sentBytes = send(new_socket, infoStr, len, 0);
			} else if ( command == '2' ) {
				if ( in_buffer[byte_count-1] == '@' ) {
					char midi_command [byte_count-3];
					for(int i=2; i<byte_count-1; i++){
						midi_command[i-2] = in_buffer[i];
						//std::cout << midi_command[i-2];
					}
					//std::cout << "\n";
					try{
						transmitMessageCmd.transmitMessage(midi_command);
					}catch(int e){
						std::cout << "Exception when transmitting midi message: " << e;
					}
				}
				else{
					std::cout << "incorrect command: " << in_buffer << "\n";
				}
			}
		}

	} while( byte_count > 0 );//end of while
}



void outputThreadFunction(int new_socket)
{
	char *hello = "Hello from server\r\n";
	ssize_t sentBytes;
	while (true) {

		sentBytes = send(new_socket , hello , strlen(hello) , 0 );
    	if(sentBytes >= 0){
			cout << "sending data to socket: ";
			cout << new_socket;
			cout << "\n";
			std::this_thread::sleep_for (std::chrono::seconds(10000));
    	}
    	else{
    		printf("client socket disconnected. Existing output thread\n");
    		close(new_socket);
    		return;
    	}

    }
}

void processCommand(int new_socket, char* buffer) {
	std::cout << buffer << "\n";
}

void processCommand2(int new_socket, char* buffer) {

	int commandLength = 0;
	vector<string> command;
	if(true) {
		//TODO: This is a command parser. We should implement a class for that
		char *token = strtok(buffer, "#");
		while (token != NULL) {
			command.push_back(token);
			token = strtok(NULL, "#");
		}
		commandLength = command.size();
		//create a command object from this string(command builder)
		if (commandLength > 0) {
			if (command[0].compare("1") == 0) {
				//std::cout << "received a GetInfo command\n";
				//this is get info
				string info = getInfoCmd.exec();
				//std::cout << "InfoString Is: " << info << "\n";
				const void* infoStr = info.c_str();
				int len = info.size();
				//std::cout << "Len Is: " << len << "\n";
				//TODO: we may have to add it to the sending thread buffer
				int sentBytes = 0;
				sentBytes = send(new_socket, infoStr, len, 0);

			} else if (command[0].compare("2") == 0) {
				//this is send event
				if (commandLength >= 2) {
					try {
						transmitMessageCmd.transmitMessage(command[1].substr(0, commandLength - 3));
					} catch (int ex) {
						//std::cout << "Exception: " << ex;
					}
				}
			} else if (command[0].compare("3") == 0) {
				//this is receive events
				if (commandLength >= 2) {
					receiveMessagesCmd.receiveMessages(command[1]);
				}
			} else {

			}
			command.clear();

		}

	}
}



int main(int argc, char const *argv[])
{

	int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    //char *hello = "200 OK Hello from server\r\n";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept incoming connections
    while(true){
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						   (socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

		//calling the constractor for thread class
		//std::thread* t_out = new thread(outputThreadFunction,new_socket);
		std::thread* t_in = new thread(inputThreadFunction,new_socket);
		vector<string> prefs;
		socket_map[new_socket] = prefs;
		//std::thread::id id = outputThread->get_id();
		//inputThreadFunction(new_socket);
    }

    //do we need to deleteall threads?

    return 0;
}
