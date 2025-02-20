#include <stdexcept>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "receiver.hpp"

#ifndef TCP_SENDER_HPP
#define TCP_SENDER_HPP

class TCPReceiver : public Receiver
{
public:
	TCPReceiver();
	~TCPReceiver() {close(socket);};
	TCPReceiver(int socket);
	std::string receive() override;
};

#endif