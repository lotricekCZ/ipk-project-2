#include <string>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>

#ifndef SENDER_HPP
#define SENDER_HPP

class Sender
{
public:
	enum InitiationPolicy : bool
	{
		COMPLETE_INITIATION, // bind and listen
		CONNECT_ONLY,		 // connect without opening new socket
	};
	int socket = -1;
	int port = -1;
	int protocol = 0;
	struct sockaddr_in address = {.sin_family = AF_UNSPEC};
	std::string hostname = "";
	Sender(std::string hostname, int port, int protocol);
	Sender();
	~Sender();
	void deinit();
	virtual void send(std::string message) = 0;
	virtual void send(std::uint8_t *data, std::uint16_t size) = 0;
	void init(std::string hostname, int port, int protocol);
	void init(int socket, sockaddr_in address);
	void init(int socket);
	void init(InitiationPolicy policy = COMPLETE_INITIATION);
	void setProtocol(int protocol);
	void setPort(int port);
	void setHostname(std::string hostname);
	void setAddress(sockaddr_in address);
	void setSocket(int socket);
	int getProtocol();
	int getPort();
	int getSocket();
	std::string getHostname();
	sockaddr_in getAddress();
	virtual std::string receive() = 0;
};

#endif