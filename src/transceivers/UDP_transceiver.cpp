/**
 * @file UDP_transceiver.cpp
 * @brief Implementation of UDPTransceiver class
 */
#include "UDP_transceiver.hpp"

/**
 * @brief Construct a new UDPTransceiver object
 * 
 */
UDPTransceiver::UDPTransceiver() : UDPReceiver(), UDPSender()
{
}

/**
 * @brief Destroy the UDPTransceiver object
 * 
 */
UDPTransceiver::~UDPTransceiver()
{
	UDPReceiver::deinit();
}

/**
 * @brief Receive a message from the server
 * 
 * @return std::string The received message
 */
std::string UDPTransceiver::receive()
{
	return UDPReceiver::receive();
}

/**
 * @brief Send a message to the server
 * 
 * @param message The message to send
 */
void UDPTransceiver::send(uint8_t *data, uint16_t size)
{
	UDPSender::send(data, size);
}

/**
 * @brief Initialize the UDPTransceiver object
 * 
 */
void UDPTransceiver::init()
{
	UDPReceiver::init();
	UDPSender::setAddress(UDPReceiver::getAddress());
	UDPSender::setSocket(UDPReceiver::getSocket());
	UDPSender::init(Sender::InitiationPolicy::CONNECT_ONLY);
}
