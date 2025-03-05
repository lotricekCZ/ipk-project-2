#include <memory>
#include <chrono>
#include <thread>
#include <poll.h>
#include "TCP_fsm.hpp"
#include "../io_handler/io_handler.hpp"


TCPFSM::TCPFSM()
{
	for (auto &node : {START, AUTH, OPEN, JOIN, END})
		NodeStates[node] = std::make_shared<FSMNode>(node);
	NodeStates[START]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH]),
								   std::make_shared<FSMEdge>(NodeStates[END]));
	NodeStates[AUTH]->assignEdges(std::make_shared<FSMEdge>(NodeStates[OPEN]), std::make_shared<FSMEdge>(NodeStates[END]), std::make_shared<FSMEdge>(NodeStates[AUTH]));
	NodeStates[OPEN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN]), std::make_shared<FSMEdge>(NodeStates[OPEN]), std::make_shared<FSMEdge>(NodeStates[END]));
	NodeStates[JOIN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN]), std::make_shared<FSMEdge>(NodeStates[OPEN]), std::make_shared<FSMEdge>(NodeStates[END]));
}

void TCPFSM::run()
{
	bool running = true;
	struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = TCPReceiver::getSocket();
    fds[1].events = POLLIN;

	TCPTransceiver::init();
	while (running)
	{	
		int ret = poll(fds, 2, 200); 
        if (ret < 0) {
            perror("poll");
            break;
        }
		if(fds[0].revents & POLLIN)
		{
			std::string messageText;
			std::getline(std::cin, messageText);
			formats::Message line = handler.readMessage(messageText);
			if(line.getType() != formats::MessageType::NONE)
				this->send(this->encode(line));
		}
		if(fds[1].revents & POLLIN)
		{
			std::string messageText = TCPReceiver::receive();
			formats::Message line = TCPDecoder::decode(messageText);
			if(line.getType() == !formats::MessageType::NONE)
				handler.printMessage(line);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
}

void TCPFSM::Messages::clear()
{
	_input = formats::Message();
	_output = formats::Message();
}

formats::Message &TCPFSM::Messages::input()
{
	return _input;
}

formats::Message &TCPFSM::Messages::output()
{
	return _output;
}