/**
 * @file UDP_sender.cpp
 * @brief Implementation of UDPSender class
 *
 * This file contains the implementation of the UDPSender class, which is a
 * sender implementation using UDP sockets.
 */

#include "UDP_sender.hpp"
#include <string>
#include "../config.hpp"

#include <iostream>
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
 * @brief Construct a new UDPSender object
 *
 */
UDPSender::UDPSender() : Sender(config::hostname, config::port, IPPROTO_UDP)
{
}

/**
 * @brief Destroy the UDPSender object
 *
 */
UDPSender::~UDPSender()
{
}

/**
 * @brief Send a message over the UDP connection
 *
 * @param message The message to be sent
 */
void UDPSender::send(std::uint8_t *data, std::uint16_t size)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = this->port;
	struct hostent *host = gethostbyname(hostname.c_str());
	if (host == nullptr)
	{
		throw std::runtime_error("Failed to resolve hostname");
	}
	address.sin_addr.s_addr = *reinterpret_cast<unsigned long *>(host->h_addr);
	std::cout << ::sendto(this->socket, data, size, 0, (struct sockaddr *)&address, sizeof(address)) << std::endl; 
}

/**
 * @brief Receive data from the socket
 *
 * This method receives data from the socket and returns it as a string.
 *
 * @return The received data as a string.
 */
std::string UDPSender::receive()
{
	char buffer[65536];
	memset(buffer, 0, 65536);
	socklen_t addrLen = sizeof(this->address);
	int bytesReceived = ::recvfrom(this->socket, buffer, 65536, 0, (struct sockaddr *)&this->address, &addrLen);
	if (bytesReceived == -1)
	{
		throw std::runtime_error(std::string("Error receiving data: ") + strerror(errno));
	}
	return std::string(buffer);
}
