#include <stdexcept>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "receiver.hpp"

#ifndef UDP_RECEIVER_HPP
#define UDP_RECEIVER_HPP

class UDPReceiver : public Receiver
{
public:
	UDPReceiver();
	~UDPReceiver();
	UDPReceiver(int socket);
	std::string receive() override;
};

#endif