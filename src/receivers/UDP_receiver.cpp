/**
 * @file UDP_receiver.cpp
 * @brief Implementation of UDPReceiver class
 *
 * This file contains the implementation of the UDPReceiver class, which is a
 * receiver implementation using UDP sockets.
 */

#include "UDP_receiver.hpp"
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

#define BUF_SIZE 65536

/**
 * @brief Constructor for UDPReceiver
 *
 * This constructor creates a new UDPReceiver object and initializes it with
 * the given hostname, port and protocol.
 *
 * @param hostname The hostname of the receiver.
 * @param port The port number of the receiver.
 * @param protocol The protocol of the receiver.
 */
UDPReceiver::UDPReceiver(): Receiver(config::hostname, config::port, IPPROTO_UDP) {

}

/**
 * @brief Constructor for UDPReceiver
 *
 * This constructor creates a new UDPReceiver object and initializes it with
 * the given socket.
 *
 * @param socket The socket of the receiver.
 */
UDPReceiver::UDPReceiver(int socket): Receiver(socket) {
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = INADDR_ANY;
	clientAddr.sin_port = 0;
	if (bind(socket, (const struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0) {
        perror("bind failed");
        close(socket);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Destructor for UDPReceiver
 */
UDPReceiver::~UDPReceiver() {

}

/**
 * @brief Receive data from the socket
 *
 * This method receives data from the socket and returns it as a string.
 *
 * @return The received data as a string.
 */
std::string UDPReceiver::receive() {
	char buffer[BUF_SIZE];
	memset(buffer, 0, BUF_SIZE);
	socklen_t addrLen = sizeof(this->address);
	int bytesReceived = ::recvfrom(this->socket, buffer, BUF_SIZE, 0, (struct sockaddr *)&this->address, &addrLen);
	if (bytesReceived == -1) {
		throw std::runtime_error(std::string("Error receiving data: ") + strerror(errno));
	}
	return std::string(buffer);
}
