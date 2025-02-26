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
}

Sender::Sender()
{
}

Sender::Sender(std::string hostname, int port, int protocol) : hostname(hostname), port(port), protocol(protocol)
{

}

void Sender::init(std::string hostname, int port, int protocol)
{
	this->hostname = hostname;
	this->port = port;
	this->protocol = protocol;
	init();
}

void Sender::init(int socket, sockaddr_in address)
{
	this->socket = socket;
	this->address = address;
	init(InitiationPolicy::CONNECT_ONLY);
}

void Sender::init(int socket)
{
	this->socket = socket;
	init(InitiationPolicy::CONNECT_ONLY);
}

void Sender::init(InitiationPolicy policy)
{
	if (policy == COMPLETE_INITIATION)
	{
		if (protocol == -1)
			throw std::runtime_error("Protocol not initialized");
		if (port == -1)
			throw std::runtime_error("Port not initialized");
		if (hostname.empty())
			throw std::runtime_error("Hostname not initialized");

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
	else if (policy == CONNECT_ONLY)
	{
		if (socket == -1)
			throw std::runtime_error("Socket not initialized");
		// if (address.sin_family == AF_UNSPEC)
		// 	throw std::runtime_error("Address not initialized");
		port = htons(port);
		// if (connect(socket, (struct sockaddr *)&address, sizeof(address)) == -1)
		// {
		// 	close(socket);
		// 	throw std::runtime_error("Connection failed");
		// }
	}
}

void Sender::setProtocol(int protocol)
{
	this->protocol = protocol;
}

void Sender::setPort(int port)
{
	this->port = port;
	address.sin_port = htons(port);
}

void Sender::setHostname(std::string hostname)
{
	this->hostname = hostname;
}

void Sender::setAddress(sockaddr_in address)
{
	this->address = address;
}

int Sender::getProtocol()
{
	return protocol;
}

int Sender::getPort()
{
	return port;
}

std::string Sender::getHostname()
{
	return hostname;
}

sockaddr_in Sender::getAddress()
{
	return address;
}

int Sender::getSocket()
{
	return socket;
}

void Sender::setSocket(int socket)
{
	this->socket = socket;
}

void Sender::deinit(){close(socket);};