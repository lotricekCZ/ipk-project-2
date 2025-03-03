/**
 * @file TCP_transceiver.cpp
 * @brief Implementation of TCPTransceiver class
 */
#include "TCP_transceiver.hpp"

/**
 * @brief Construct a new TCPTransceiver object
 * 
 */
TCPTransceiver::TCPTransceiver() : TCPReceiver(), TCPSender()
{
}

/**
 * @brief Destroy the TCPTransceiver object
 * 
 */
TCPTransceiver::~TCPTransceiver()
{
	TCPReceiver::deinit();
}

/**
 * @brief Receive a message from the server
 * 
 * @return std::string The received message
 */
std::string TCPTransceiver::receive()
{
	return TCPReceiver::receive();
}

/**
 * @brief Send a message to the server
 * 
 * @param message The message to send
 */
void TCPTransceiver::send(std::string message)
{
	TCPSender::send(message);
}

/**
 * @brief Initialize the TCPTransceiver object
 * 
 */
void TCPTransceiver::init()
{
	TCPReceiver::init();
	TCPSender::setAddress(TCPReceiver::getAddress());
	TCPSender::setSocket(TCPReceiver::getSocket());
	TCPSender::init(Sender::InitiationPolicy::CONNECT_ONLY);
}
