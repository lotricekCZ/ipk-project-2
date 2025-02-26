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
	enum InitiationPolicy : bool
	{
		COMPLETE_INITIATION, // bind and listen
		CONNECT_ONLY,		 // connect without opening new socket
	};
	int socket = -1;
	int port = -1;
	int protocol = -1;
	struct sockaddr_in address = {.sin_family = AF_UNSPEC};
	std::string hostname = "";
	Receiver(std::string hostname, int port, int protocol);
	Receiver(int socket, sockaddr_in address);
	Receiver(int socket);
	Receiver();
	~Receiver();
	void init(std::string hostname, int port, int protocol);
	void init(int socket, sockaddr_in address);
	void init(int socket);
	void deinit();
	void init(InitiationPolicy policy = COMPLETE_INITIATION);
	void setSocket(int socket);
	void setProtocol(int protocol);
	void setPort(int port);
	void setHostname(std::string hostname);
	void setAddress(sockaddr_in address);
	int getSocket();
	int getProtocol();
	int getPort();
	std::string getHostname();
	sockaddr_in getAddress();
	virtual std::string receive() = 0;
};

#endif