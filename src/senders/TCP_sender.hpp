#include <stdexcept>
#include "sender.hpp"
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef TCP_SENDER_HPP
#define TCP_SENDER_HPP

class TCPSender : public Sender
{
public:
	TCPSender();
	~TCPSender();
	void send(std::string message) override;
	void send(std::uint8_t *data, std::uint16_t size) override
	{
		(void)data;
		(void)size;
		throw std::runtime_error("Not implemented");
	};
	std::string receive() override;
};

#endif