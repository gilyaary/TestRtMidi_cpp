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
void processCommand(char cmd [], int size, int new_socket);


void inputThreadFunction(int new_socket)
{
	printf("entering modified input thread function\n");
	char in_buffer[8092] = {0};
	ssize_t byte_count;

	do{
		byte_count = read( new_socket , in_buffer, 8092);
		if (byte_count == -1 || (in_buffer[0] == 'Q' && in_buffer[1] == 'Q' && in_buffer[2] == 'Q')) {
			printf("client sent disconnect signal. Existing input thread\n");
			close(new_socket);
			return;
		}
		//printf(in_buffer);printf("\n");
		if( byte_count >= 2 ){

			int start_from = 0;
			for(int i=0; i<byte_count; i++){
				if(in_buffer[i] == '@'){
					//std::cout << "found delimiter\n";
					int command_size = i-start_from;
					if(command_size > 0){
						char command [command_size];
						for(int k=start_from; k<i; k++){
							command[k-start_from] = in_buffer[k];
						}
						//std::cout << midi_command[i-2];
						try{
							processCommand(command, command_size, new_socket);
							std::cout << "command: " << command << "\n";
						}catch(int e){
							std::cout << "Exception when transmitting midi message: " << e;
						}
					}
					start_from = i+1;
				}
			}
		}
	} while( byte_count > 0 );//end of while
	std::cout << "Exit Input loop\n";
}

void processCommand(char cmd [], int size, int new_socket){
	char command = cmd[0];
	if ( command == '1' ){
		std::string info = getInfoCmd.exec();
		const void* infoStr = info.c_str();
		int len = info.size();
		int sentBytes = send(new_socket, infoStr, len, 0);
	} else if ( command == '2' ) {
		char midi_command [size-2];
		for(int i=2; i<size;i++){
			midi_command[i-2] = cmd[i];
		}
		transmitMessageCmd.transmitMessage(midi_command, size);
	}
	else{
		std::cout << "incorrect command: " << cmd << "\n";
	}

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
			std::this_thread::sleep_for (std::chrono::seconds(1000));
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
