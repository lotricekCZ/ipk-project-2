#include "receiver.hpp"

#include <string>
#include <stdexcept>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

Receiver::~Receiver()
{
	close(socket);
}

Receiver::Receiver(std::string hostname, int port, int protocol) : hostname(hostname), port(port), protocol(protocol)
{
	this->socket = ::socket(AF_INET, SOCK_STREAM, protocol);
	address.sin_family = AF_INET;
	struct hostent *host = gethostbyname(hostname.c_str());
	if (host == nullptr)
	{
		throw std::runtime_error("Failed to resolve hostname");
	}
	address.sin_addr.s_addr = *reinterpret_cast<unsigned long *>(host->h_addr);
	address.sin_port = htons(port);
	if (connect(socket, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		close(socket);
		throw std::runtime_error("Connection failed");
	}
}

Receiver::Receiver(int socket, sockaddr_in address) : socket(socket), address(address)
{
	this->protocol = address.sin_family == AF_INET ? IPPROTO_TCP : IPPROTO_UDP;
	this->port = ntohs(address.sin_port);
	this->hostname = inet_ntoa(address.sin_addr);
}

Receiver::Receiver(int socket) : socket(socket) {
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	if (getpeername(socket, (struct sockaddr*)&addr, &addr_len) == 0) {
		this->protocol = addr.sin_family == AF_INET ? IPPROTO_TCP : IPPROTO_UDP;
		this->port = ntohs(addr.sin_port);
		this->hostname = inet_ntoa(addr.sin_addr);
		this->address = addr;
	} else {
		throw std::runtime_error("Failed to retrieve socket information");
	}

}