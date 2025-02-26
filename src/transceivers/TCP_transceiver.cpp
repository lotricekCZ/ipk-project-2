#include "TCP_transceiver.hpp"

TCPTransceiver::TCPTransceiver() : TCPReceiver(), TCPSender()
{
}

TCPTransceiver::~TCPTransceiver()
{
	TCPReceiver::deinit();
}

std::string TCPTransceiver::receive()
{
	return TCPReceiver::receive();
}

void TCPTransceiver::send(std::string message)
{
	TCPSender::send(message);
}

void TCPTransceiver::init()
{
	TCPReceiver::init();
	TCPSender::setAddress(TCPReceiver::getAddress());
	TCPSender::setSocket(TCPReceiver::getSocket());
	TCPSender::init(Sender::InitiationPolicy::CONNECT_ONLY);
}