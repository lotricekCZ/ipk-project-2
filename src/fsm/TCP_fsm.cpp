/**
 * @file TCP_fsm.cpp
 * @brief Implementation of TCPFSM class
 *
 * This file contains the implementation of the TCPFSM class, which is a
 * finite state machine for managing TCP connections.
 */

#include <memory>
#include <chrono>
#include <thread>
#include <poll.h>
#include "TCP_fsm.hpp"
#include "../io_handler/io_handler.hpp"

/**
 * Constructor for TCPFSM class
 *
 * This constructor initializes the finite state machine and sets up the
 * edges between the states.
 */
TCPFSM::TCPFSM()
{
	auto nan_auth = [&](Messages &messages)
	{
		// If the user is sending an AUTH message and the input message is of type NONE, then clear the input and output messages
		if (messages.output().getType() == formats::AUTH && messages.input().getType() == formats::NONE)
		{
			std::cout << "nan_auth" << std::endl;
			messages.clear();
			return true;
		}
		return false;
	};

	auto errbye_nan = [&](Messages &messages)
	{
		// If the user is sending an ERR or BYE message and the input message is of type NONE, then clear the input and output messages
		if (messages.output().getType() == formats::NONE && (messages.input().getType() == formats::ERR || messages.input().getType() == formats::BYE))
		{
			std::cout << "errbye_nan" << std::endl;
			messages.clear();
			return true;
		}
		return false;
	};

	auto nan_bye = [&](Messages &messages)
	{
		// If the user is sending a BYE message and the input message is of type NONE, then clear the input and output messages
		if (messages.output().getType() == formats::BYE && messages.input().getType() == formats::NONE)
		{
			std::cout << "nan_bye" << std::endl;
			messages.clear();
			return true;
		}
		return false;
	};

	auto nreply_auth = [&](Messages &messages)
	{
		// If the user is sending a REPLY message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::REPLY && messages.input().getStatus() == false)
		{
			messages.output() = formats::Message(formats::AUTH, config::displayName);
			this->send(this->encode(messages.output()));
			messages.clear();
			return true;
		}
		return false;
	};

	auto reply_nan = [&](Messages &messages)
	{
		// If the user is sending a REPLY message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::REPLY && messages.input().getStatus() == true)
		{
			std::cout << "reply_nan" << std::endl;
			messages.clear();
			return true;
		}
		return false;
	};

	auto msg_err = [&](Messages &messages)
	{
		// If the user is sending a MSG message and the input message is of type NONE, then send an ERR message and clear the input and output messages
		if (messages.input().getType() == formats::MSG)
		{
			messages.output() = formats::Message(formats::ERR, "ERROR: Current state of this FSM is AUTH, receiving messages is not allowed!");
			std::cout << "msg_err" << std::endl;
			this->send(this->encode(messages.output()));
			messages.clear();
			return true;
		}
		return false;
	};

	auto nan_join = [&](Messages &messages)
	{
		// If the user is sending a JOIN message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::NONE && messages.output().getType() == formats::JOIN)
		{
			std::cout << "nan_join" << std::endl;
			messages.clear();
			return true;
		}
		return false;
	};

	auto nan_msg = [&](Messages &messages)
	{
		// If the user is sending a MSG message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::NONE && messages.output().getType() == formats::MSG)
		{
			std::cout << "nan_msg" << std::endl;
			messages.clear();
			return true;
		}
		return false;
	};

	auto msg_nan = [&](Messages &messages)
	{
		// If the user is sending a MSG message and the input message is of type NONE, then clear the input and output messages
		if (messages.output().getType() == formats::MSG && messages.input().getType() == formats::NONE)
		{
			std::cout << "msg_nan" << std::endl;
			messages.clear();
			return true;
		}
		return false;
	};

	auto anyreply_err = [&](Messages &messages)
	{
		// If the user is sending a REPLY message and the input message is of type NONE, then send an ERR message and clear the input and output messages
		if (messages.input().getType() == formats::REPLY)
		{
			std::cout << "anyreply_err" << std::endl;
			messages.output() = formats::Message(formats::ERR, "ERROR: Client was not requesting anything, reply is unexpected!");
			this->send(this->encode(messages.output()));
			messages.clear();
			return true;
		}
		return false;
	};

	auto anyreply_nan = [&](Messages &messages)
	{
		// If the user is sending a REPLY message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::REPLY && messages.output().getType() == formats::MessageType::NONE)
		{
			std::cout << "anyreply_nan" << std::endl;
			messages.clear();
			return true;
		}
		return false;
	};

	// Assigning edges
	// START -> AUTH (nan_auth), END (nan_bye, errbye_nan)
	// AUTH -> OPEN (reply_nan), END (errbye_nan, nan_bye, msg_err), AUTH (nreply_auth)
	// OPEN -> JOIN (nan_join), OPEN (msg_nan, nan_msg), END (errbye_nan, nan_bye, anyreply_err)
	// JOIN -> JOIN (msg_nan), OPEN (anyreply_nan), END (nan_bye, errbye_nan)
	for (auto &node : {TCPFSM::START, TCPFSM::AUTH, TCPFSM::OPEN, TCPFSM::JOIN, TCPFSM::END})
		NodeStates[node] = std::make_shared<FSMNode>(node);
	NodeStates[START]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH], nan_auth),
								   std::make_shared<FSMEdge>(NodeStates[END], nan_bye, errbye_nan));
	NodeStates[AUTH]->assignEdges(std::make_shared<FSMEdge>(NodeStates[OPEN], reply_nan),
								  std::make_shared<FSMEdge>(NodeStates[END], errbye_nan, nan_bye, msg_err),
								  std::make_shared<FSMEdge>(NodeStates[AUTH], nreply_auth));
	NodeStates[OPEN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN], nan_join),
								  std::make_shared<FSMEdge>(NodeStates[OPEN], msg_nan, nan_msg),
								  std::make_shared<FSMEdge>(NodeStates[END], errbye_nan, nan_bye, anyreply_err));
	NodeStates[JOIN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN], msg_nan),
								  std::make_shared<FSMEdge>(NodeStates[OPEN], anyreply_nan),
								  std::make_shared<FSMEdge>(NodeStates[END], nan_bye, errbye_nan));
}

/**
 * Run the finite state machine
 *
 * This function runs the finite state machine and handles the events
 * triggered by the user.
 */
void TCPFSM::run()
{
	TCPTransceiver::init();
	bool running = true;
	struct pollfd fds[2];

	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[1].fd = TCPReceiver::getSocket();
	fds[1].events = POLLIN;

	TCPFSM::states curr = START;
	while (running)
	{
		int ret = poll(fds, 2, 200);
		if (ret < 0)
		{
			std::cerr << "poll" << std::endl;
			break;
		}
		// Check if there is input from the user
		if (fds[0].revents & POLLIN)
		{
			std::string messageText;
			std::getline(std::cin, messageText);
			messages.output() = handler.readMessage(messageText);
			if (messages.output().getType() != formats::MessageType::NONE)
				this->send(this->encode(messages.output()));
		}
		// Check if there is input from the server
		if (fds[1].revents & POLLIN)
		{
			std::string messageText = TCPReceiver::receive();
			std::cout << messageText << std::endl;
			formats::Message line = TCPDecoder::decode(messageText);
			messages.input() = line;
			if (line.getType() != formats::MessageType::NONE)
				handler.printMessage(line);
		}

		// Check if the state has changed
		if (this->state != curr)
		{
			// std::cout << "State: " << this->state << std::endl;
			curr = this->state;
		}
		// Run the finite state machine
		if (messages.input().getType() != formats::MessageType::NONE || messages.output().getType() != formats::MessageType::NONE)
		{
			std::shared_ptr<FSMNode> next = NodeStates[this->state]->next(messages);
			if (next != nullptr)
				this->state = next->state;
			// std::cout << "nState: " << this->state << std::endl;
		}
		// Check if we should stop
		if (this->state == END)
			running = false;
		// Wait a bit before checking again
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}

/**
 * Clear the messages
 *
 * This function clears the input and output messages.
 */
void TCPFSM::Messages::clear()
{
	input() = formats::Message(formats::MessageType::NONE);
	output() = formats::Message(formats::MessageType::NONE);
}

/**
 * Get the input message
 *
 * This function returns the input message.
 */
formats::Message &TCPFSM::Messages::input()
{
	return _input;
}

/**
 * Get the output message
 *
 * This function returns the output message.
 */
formats::Message &TCPFSM::Messages::output()
{
	return _output;
}
