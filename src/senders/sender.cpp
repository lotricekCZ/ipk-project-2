/**
 * @file sender.cpp
 * @brief Implementation of the Sender class
 *
 * This file contains the implementation of the Sender class, which is a
 * base class for all senders.
 */

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

/**
 * @brief Destructor for the Sender class
 *
 * This function does not do except for calling thedeinit() function.
 */
Sender::~Sender()
{
	deinit();
}

/**
 * @brief Default constructor for the Sender class
 *
 */
Sender::Sender()
{
}

/**
 * @brief Constructor for the Sender class
 *
 * This function initializes the socket with the given hostname, port, and
 * protocol.
 *
 * @param hostname The hostname to connect to
 * @param port The port number to connect to
 * @param protocol The protocol to use (e.g. IPPROTO_TCP)
 */
Sender::Sender(std::string hostname, int port, int protocol) : hostname(hostname), port(port), protocol(protocol)
{
}

/**
 * @brief Initializes the sender with the given hostname, port, and protocol
 *
 * This function initializes the socket with the given hostname, port, and
 * protocol.
 *
 * @param hostname The hostname to connect to
 * @param port The port number to connect to
 * @param protocol The protocol to use (e.g. IPPROTO_TCP)
 */
void Sender::init(std::string hostname, int port, int protocol)
{
	this->hostname = hostname;
	this->port = port;
	this->protocol = protocol;
	init();
}

/**
 * @brief Initializes the sender with the given socket and address
 *
 * This function initializes the sender with the given socket and address.
 *
 * @param socket The socket to use
 * @param address The address to connect to
 */
void Sender::init(int socket, sockaddr_in address)
{
	this->socket = socket;
	this->address = address;
	init(InitiationPolicy::CONNECT_ONLY);
}

/**
 * @brief Initializes the sender with the given socket
 *
 * This function initializes the sender with the given socket.
 *
 * @param socket The socket to use
 */
void Sender::init(int socket)
{
	this->socket = socket;
	init(InitiationPolicy::CONNECT_ONLY);
}

/**
 * @brief Initializes the sender with the given initiation policy
 *
 * This function initializes the sender with the given initiation policy.
 *
 * @param policy The initiation policy to use (e.g. COMPLETE_INITIATION or CONNECT_ONLY)
 */
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

		this->socket = ::socket(AF_INET, (protocol == IPPROTO_UDP) ? SOCK_DGRAM : SOCK_STREAM, protocol);
		address.sin_family = AF_INET;
		struct hostent *host = gethostbyname(hostname.c_str());
		if (host == nullptr)
		{
			throw std::runtime_error("Failed to resolve hostname");
		}
		address.sin_addr.s_addr = *reinterpret_cast<unsigned long *>(host->h_addr);
		address.sin_port = htons(port);
		if (protocol == IPPROTO_TCP && connect(socket, (struct sockaddr *)&address, sizeof(address)) == -1)
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

/**
 * @brief Sets the protocol for the sender
 *
 * This function sets the protocol for the sender.
 *
 * @param protocol The protocol to use (e.g. IPPROTO_TCP)
 */
void Sender::setProtocol(int protocol)
{
	this->protocol = protocol;
}

/**
 * @brief Sets the port for the sender
 *
 * This function sets the port for the sender.
 *
 * @param port The port number to use
 */
void Sender::setPort(int port)
{
	this->port = port;
	address.sin_port = htons(port);
}

/**
 * @brief Sets the hostname for the sender
 *
 * This function sets the hostname for the sender.
 *
 * @param hostname The hostname to use
 */
void Sender::setHostname(std::string hostname)
{
	this->hostname = hostname;
}

/**
 * @brief Sets the address for the sender
 *
 * This function sets the address for the sender.
 *
 * @param address The address to use
 */
void Sender::setAddress(sockaddr_in address)
{
	this->address = address;
}

/**
 * @brief Gets the protocol for the sender
 *
 * This function returns the protocol for the sender.
 *
 * @return The protocol for the sender
 */
int Sender::getProtocol()
{
	return protocol;
}

/**
 * @brief Gets the port for the sender
 *
 * This function returns the port for the sender.
 *
 * @return The port for the sender
 */
int Sender::getPort()
{
	return port;
}

/**
 * @brief Gets the hostname for the sender
 *
 * This function returns the hostname for the sender.
 *
 * @return The hostname for the sender
 */
std::string Sender::getHostname()
{
	return hostname;
}

/**
 * @brief Gets the address for the sender
 *
 * This function returns the address for the sender.
 *
 * @return The address for the sender
 */
sockaddr_in Sender::getAddress()
{
	return address;
}

/**
 * @brief Gets the socket for the sender
 *
 * This function returns the socket for the sender.
 *
 * @return The socket for the sender
 */
int Sender::getSocket()
{
	return socket;
}

/**
 * @brief Sets the socket for the sender
 *
 * This function sets the socket for the sender.
 *
 * @param socket The socket to use
 */
void Sender::setSocket(int socket)
{
	this->socket = socket;
}

/**
 * @brief Deinitializes the sender
 *
 * This function deinitializes the sender by closing the socket.
 */
void Sender::deinit() { close(socket); };
