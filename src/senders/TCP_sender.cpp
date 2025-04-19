/**
 * @file TCP_sender.cpp
 * @brief Implementation of TCPSender class
 *
 * This file contains the implementation of the TCPSender class, which is a
 * sender implementation using TCP sockets.
 */

#include "TCP_sender.hpp"
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

/**
 * @brief Construct a new TCPSender object
 * 
 */
TCPSender::TCPSender() : Sender(config::hostname, config::port, IPPROTO_TCP)
{
}

/**
 * @brief Destroy the TCPSender object
 * 
 */
TCPSender::~TCPSender()
{
	deinit();
}

/**
 * @brief Send a message over the TCP connection
 * 
 * @param message The message to be sent
 */
void TCPSender::send(std::string message)
{
	::send(this->socket, message.c_str(), message.length(), 0);
}

/**
 * @brief Receive a message from the TCP connection
 * 
 * @return std::string The received message
 */
std::string TCPSender::receive()
{
	char buffer[65536];
	memset(buffer, 0, 65536);
	::recv(this->socket, buffer, 65536, 0);
	std::cout << buffer << std::endl;
	return std::string(buffer);
}
