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
	int socket;
	int port;
	int protocol = 0;
	struct sockaddr_in address;
	std::string hostname;
	Sender(std::string hostname, int port, int protocol);
	~Sender();
	virtual void send(std::string message) = 0;
	virtual void send(std::uint8_t *data, std::uint16_t size) = 0;
	virtual std::string receive() = 0;
};

#endif