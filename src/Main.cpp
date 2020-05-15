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
//this thread receives commands. It can answer the command,
//send a MIDI event or set something in the environment
//(open/close a port and set the ports that a sockets would receive events from)
void inputThreadFunction(int new_socket)
{
	int valread;
	vector<string> command;
	std:string commands_str;

	while(true){
		char in_buffer[1024] = {0};
		ssize_t valread;
		valread = read( new_socket , in_buffer, 1024);
		//std::cout << "   Read Input Size: " << valread << "\n";
		//printf("   %s\n",in_buffer );

		if (valread == -1 || (in_buffer[0] == 'Q' && in_buffer[1] == 'Q' && in_buffer[2] == 'Q')) {
			printf("client sent disconnect signal. Existing input thread\n");
			close(new_socket);
			return;
		}
		else{
			commands_str.append(in_buffer, valread);
		}
		int start = 0;
		for(int end=1;end<commands_str.size(); end++){
			char c = commands_str[end];
			if(c == '@'){
				//found the end
				char chars[end - start];
				commands_str.copy(chars, end-start, start);
				processCommand(new_socket, chars);
				start = end + 1;
				end = start;
				if(end == commands_str.size() -1){
					commands_str.clear();
				}
			}
			else{
				if(end == commands_str.size() -1){
					commands_str = commands_str.substr(start,commands_str.size());
				}
			}
		}

	} //end of while
}


void processCommand(int new_socket, char* buffer) {

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
		std::thread* t_out = new thread(outputThreadFunction,new_socket);
		std::thread* t_in = new thread(inputThreadFunction,new_socket);
		vector<string> prefs;
		socket_map[new_socket] = prefs;
		//std::thread::id id = outputThread->get_id();
    }

    //do we need to deleteall threads?

    return 0;
}
