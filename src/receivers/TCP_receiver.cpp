/**
 * @file TCP_receiver.cpp
 * @brief Implementation of TCPReceiver class
 *
 * This file contains the implementation of the TCPReceiver class, which is a
 * receiver implementation using TCP sockets.
 */

#include "TCP_receiver.hpp"
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
 * @brief Constructor for TCPReceiver
 *
 * This constructor creates a new TCPReceiver object and initializes it with
 * the given hostname, port and protocol.
 *
 * @param hostname The hostname of the receiver.
 * @param port The port number of the receiver.
 * @param protocol The protocol of the receiver.
 */
TCPReceiver::TCPReceiver(): Receiver(config::hostname, config::port, IPPROTO_TCP) {

}

/**
 * @brief Constructor for TCPReceiver
 *
 * This constructor creates a new TCPReceiver object and initializes it with
 * the given socket.
 *
 * @param socket The socket of the receiver.
 */
TCPReceiver::TCPReceiver(int socket): Receiver(socket) {

}

/**
 * @brief Destructor for TCPReceiver
 */
TCPReceiver::~TCPReceiver() {

}

/**
 * @brief Receive data from the socket
 *
 * This method receives data from the socket and returns it as a string.
 *
 * @return The received data as a string.
 */
std::string TCPReceiver::receive() {
	char buffer[BUF_SIZE];
	memset(buffer, 0, BUF_SIZE);
	::recv(this->socket, buffer, BUF_SIZE, 0);
	return std::string(buffer);
}
