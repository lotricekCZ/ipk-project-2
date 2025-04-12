/**
 * @file UDP_fsm.cpp
 * @brief Implementation of UDPFSM class
 *
 * This file contains the implementation of the UDPFSM class, which is a
 * finite state machine for managing UDP connections.
 */

#include <memory>
#include <chrono>
#include <thread>
#include <poll.h>
#include "UDP_fsm.hpp"
#include "../io_handler/io_handler.hpp"

/**
 * Constructor for UDPFSM class
 *
 * This constructor initializes the finite state machine and sets up the
 * edges between the states.
 */
UDPFSM::UDPFSM()
{
	formats::Message auxiliar_auth;
	auto empty_auth_nan_auth_no = [this](Messages &messages)
	{		
		if (stack.empty() && messages.output().getType() == formats::AUTH && messages.input().getType() == formats::NONE)
		{
			stack.emplace_front(messages.output());
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto none_none_errbye_nan_no = [this](Messages &messages)
	{
		if (messages.output().getType() == formats::NONE && (messages.input().getType() == formats::ERR || messages.input().getType() == formats::BYE))
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto none_none_nan_bye_no = [this](Messages &messages)
	{
		if (messages.output().getType() == formats::BYE && messages.input().getType() == formats::NONE)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto auth_none_nreply_auth_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY && messages.input().getStatus() == false)
		{
			messages.output() = formats::Message(formats::AUTH, config::displayName);
			this->send(this->encode(messages.output()));
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto reply_nan = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY && messages.input().getStatus() == true)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto msg_err = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::MSG)
		{
			messages.output() = formats::Message(formats::ERR, config::displayName);
			messages.output().setText("ERROR: Current state of this FSM is AUTH, receiving messages is not allowed!");
			this->send(this->encode(messages.output()));
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto nan_join = [this](Messages &messages)
	{
		if (/*messages.input().getType() == formats::NONE &&*/ messages.output().getType() == formats::JOIN)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto nan_msg = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE && messages.output().getType() == formats::MSG)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto msg_nan = [this](Messages &messages)
	{
		if (messages.output().getType() == formats::MSG && messages.input().getType() == formats::NONE)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto none_none_anyreply_err_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY)
		{
			this->messages.output() = formats::Message(formats::ERR, config::displayName);
			this->messages.output().setText("ERROR: Client was not requesting anything, reply is unexpected!");
			this->send(this->encode(messages.output()));
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto anyreply_nan = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY && messages.output().getType() == formats::MessageType::NONE)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	// Assigning edges
	// START -> AUTH (nan_auth), END (nan_bye, errbye_nan)
	// AUTH -> OPEN (reply_nan), END (errbye_nan, nan_bye, msg_err), AUTH (nreply_auth)
	// OPEN -> JOIN (nan_join), OPEN (msg_nan, nan_msg), END (errbye_nan, nan_bye, anyreply_err)
	// JOIN -> JOIN (msg_nan), OPEN (anyreply_nan), END (nan_bye, errbye_nan)
	for (auto &node : {UDPFSM::START, UDPFSM::AUTH, UDPFSM::OPEN, UDPFSM::JOIN, UDPFSM::END})
		NodeStates[node] = std::make_shared<FSMNode>(node);
	NodeStates[START]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH], empty_auth_nan_auth_no),
								   std::make_shared<FSMEdge>(NodeStates[END], none_none_nan_bye_no, none_none_errbye_nan_no));
	NodeStates[AUTH]->assignEdges(std::make_shared<FSMEdge>(NodeStates[OPEN], reply_nan),
								  std::make_shared<FSMEdge>(NodeStates[END], none_none_errbye_nan_no, none_none_nan_bye_no, msg_err),
								  std::make_shared<FSMEdge>(NodeStates[AUTH], nreply_auth));
	NodeStates[OPEN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN], nan_join),
								  std::make_shared<FSMEdge>(NodeStates[OPEN], msg_nan, nan_msg),
								  std::make_shared<FSMEdge>(NodeStates[END], none_none_errbye_nan_no, none_none_nan_bye_no, none_none_anyreply_err_no));
	NodeStates[JOIN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN], msg_nan),
								  std::make_shared<FSMEdge>(NodeStates[OPEN], anyreply_nan),
								  std::make_shared<FSMEdge>(NodeStates[END], none_none_nan_bye_no, none_none_errbye_nan_no));
}

/**
 * Run the finite state machine
 *
 * This function runs the finite state machine and handles the events
 * triggered by the user.
 */
void UDPFSM::run()
{
	UDPTransceiver::init();
	bool running = true;
	struct pollfd fds[2];

	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[1].fd = UDPReceiver::getSocket();
	fds[1].events = POLLIN;

	UDPFSM::states curr = START;
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
			std::string messageText = UDPReceiver::receive();
			formats::Message line = UDPDecoder::decode(messageText);
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
void UDPFSM::Messages::clear()
{
	input() = formats::Message(formats::MessageType::NONE);
	output() = formats::Message(formats::MessageType::NONE);
}

/**
 * Get the input message
 *
 * This function returns the input message.
 */
formats::Message &UDPFSM::Messages::input()
{
	return _input;
}

/**
 * Get the output message
 *
 * This function returns the output message.
 */
formats::Message &UDPFSM::Messages::output()
{
	return _output;
}
