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
}

Receiver::Receiver()
{
}

Receiver::Receiver(std::string hostname, int port, int protocol) : hostname(hostname), port(port), protocol(protocol)
{
}

Receiver::Receiver(int socket, sockaddr_in address) : socket(socket), address(address)
{
	this->protocol = address.sin_family == AF_INET ? IPPROTO_TCP : IPPROTO_UDP;
	this->port = ntohs(address.sin_port);
	this->hostname = inet_ntoa(address.sin_addr);
}

Receiver::Receiver(int socket) : socket(socket)
{
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	if (getpeername(socket, (struct sockaddr *)&addr, &addr_len) == 0)
	{
		this->protocol = addr.sin_family == AF_INET ? IPPROTO_TCP : IPPROTO_UDP;
		this->port = ntohs(addr.sin_port);
		this->hostname = inet_ntoa(addr.sin_addr);
		this->address = addr;
	}
	else
	{
		throw std::runtime_error("Failed to retrieve socket information");
	}
}

void Receiver::init(std::string hostname, int port, int protocol)
{
	this->hostname = hostname;
	this->port = port;
	this->protocol = protocol;
	init();
}

void Receiver::init(int socket, sockaddr_in address)
{
	this->socket = socket;
	this->address = address;
	init(InitiationPolicy::CONNECT_ONLY);
}

void Receiver::init(int socket)
{
	this->socket = socket;
	init(InitiationPolicy::CONNECT_ONLY);
}

void Receiver::init(InitiationPolicy policy)
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
		// if (connect(socket, (struct sockaddr *)&address, sizeof(address)) == -1)
		// {
		// 	close(socket);
		// 	throw std::runtime_error("Connection failed");
		// }
	}
}

/**
 * @brief Sets the protocol for the receiver
 * @param protocol The protocol
 */
void Receiver::setProtocol(int protocol)
{
	this->protocol = protocol;
}

/**
 * @brief Sets the port for the receiver
 * @param port The port
 */
void Receiver::setPort(int port)
{
	this->port = port;
}

/**
 * @brief Sets the hostname for the receiver
 * @param hostname The hostname
 */
void Receiver::setHostname(std::string hostname)
{
	this->hostname = hostname;
}

/**
 * @brief Sets the address for the receiver
 * @param address The address
 */
void Receiver::setAddress(sockaddr_in address)
{
	this->address = address;
}

/**
 * @brief Gets the protocol for the receiver
 * @return The protocol
 */
int Receiver::getProtocol()
{
	return protocol;
}

/**
 * @brief Gets the port for the receiver
 * @return The port
 */
int Receiver::getPort()
{
	return port;
}

/**
 * @brief Gets the hostname for the receiver
 * @return The hostname
 */
std::string Receiver::getHostname()
{
	return hostname;
}

/**
 * @brief Gets the address for the receiver
 * @return The address
 */
sockaddr_in Receiver::getAddress()
{
	return address;
}

int Receiver::getSocket()
{
	return socket;
}

void Receiver::setSocket(int socket)
{
	this->socket = socket;
}

void Receiver::deinit()
{
	close(socket);
}