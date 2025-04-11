#include <stdexcept>
#include "sender.hpp"
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef UDP_SENDER_HPP
#define UDP_SENDER_HPP

class UDPSender : public Sender
{
public:
	UDPSender();
	~UDPSender();
	void send(std::string message) override {throw std::runtime_error("Not implemented");};
	void send(std::uint8_t * data, std::uint16_t size) override;
	std::string receive() override;
};

#endif