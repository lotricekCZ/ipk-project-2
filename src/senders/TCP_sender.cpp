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


TCPSender::TCPSender(): Sender(config::hostname, config::port, IPPROTO_TCP) {

}

void TCPSender::send(std::string message) {

	std::cout << ::send(this->socket, message.c_str(), message.length(), 0) << std::endl;
}

std::string TCPSender::receive() {
	char buffer[65536];
	memset(buffer, 0, 65536);
	::recv(this->socket, buffer, 65536, 0);
	std::cout << buffer << std::endl;
	return std::string(buffer);
}