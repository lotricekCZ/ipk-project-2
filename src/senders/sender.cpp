#include "sender.hpp"

#include <string>
#include <stdexcept>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

Sender::~Sender()
{
	close(socket);
}

Sender::Sender(std::string hostname, int port, int protocol) : hostname(hostname), port(port), protocol(protocol)
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