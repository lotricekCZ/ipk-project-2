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

TCPReceiver::TCPReceiver(): Receiver(config::hostname, config::port, IPPROTO_TCP) {

}

TCPReceiver::TCPReceiver(int socket): Receiver(socket) {

}

TCPReceiver::~TCPReceiver() {

}

std::string TCPReceiver::receive() {
	char buffer[BUF_SIZE];
	memset(buffer, 0, BUF_SIZE);
	::recv(this->socket, buffer, BUF_SIZE, 0);
	return std::string(buffer);
}