// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>
#include <thread>
using namespace std;

#define PORT 8888

void foo(int Z)
{
    for (int i = 0; i < Z; i++) {
        cout << "Thread using function"
               " pointer as callable\n";
    }
}

//class thread_obj {
//public:
//    void operator()(int x)
//    {
//        for (int i = 0; i < x; i++)
//            cout << "Thread using function"
//                  " object as  callable\n";
//    }
//};

int main(int argc, char const *argv[])
{
	//calling the constractor for thread class
	std::thread* th1 = new thread(foo,3);
	std::thread::id id = th1->get_id();
	//std::thread th1(foo, 3);

	int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "200 OK Hello from server\r\n";

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
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(true){
		valread = read( new_socket , buffer, 1024);
		printf("%s\n",buffer );
		send(new_socket , hello , strlen(hello) , 0 );
		printf("Hello message sent\n");
		if (buffer[0] == 'Q' && buffer[1] == 'Q' && buffer[2] == 'Q') {
			printf("client sent disconnect signal. Existing\n");
			break;
		}
    }

    return 0;
}
