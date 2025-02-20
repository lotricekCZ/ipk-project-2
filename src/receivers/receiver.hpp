#include <string>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>

#ifndef RECEIVER_HPP
#define RECEIVER_HPP

class Receiver
{
public:
	int socket;
	int port;
	int protocol = 0;
	struct sockaddr_in address;
	std::string hostname;
	Receiver(std::string hostname, int port, int protocol);
	Receiver(int socket, sockaddr_in address);
	Receiver(int socket);
	~Receiver();
	virtual std::string receive() = 0;
};

#endif