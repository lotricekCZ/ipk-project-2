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
	formats::Message auxiliar_auth;
	auto nan_auth = [this](Messages &messages)
	{
		// If the user is sending an AUTH message and the input message is of type NONE, then clear the input and output messages
		if (messages.output().getType() == formats::AUTH && messages.input().getType() == formats::NONE)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto errbye_nan = [this](Messages &messages)
	{
		// If the user is sending an ERR or BYE message and the input message is of type NONE, then clear the input and output messages
		if (messages.output().getType() == formats::NONE && (messages.input().getType() == formats::ERR || messages.input().getType() == formats::BYE))
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto nan_bye = [this](Messages &messages)
	{
		// If the user is sending a BYE message and the input message is of type NONE, then clear the input and output messages
		if (messages.output().getType() == formats::BYE && messages.input().getType() == formats::NONE)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto nreply_auth = [this](Messages &messages)
	{
		// If the user is sending a REPLY message and the input message is of type NONE, then clear the input and output messages
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
		// If the user is sending a REPLY message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::REPLY && messages.input().getStatus() == true)
		{
			this->awaitsResponse = false;
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto msg_err = [this](Messages &messages)
	{
		// If the user is sending a MSG message and the input message is of type NONE, then send an ERR message and clear the input and output messages
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
		// If the user is sending a JOIN message and the input message is of type NONE, then clear the input and output messages
		if (/*messages.input().getType() == formats::NONE &&*/ messages.output().getType() == formats::JOIN)
		{
			this->awaitsResponse = true;
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto nan_msg = [this](Messages &messages)
	{
		// If the user is sending a MSG message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::NONE && messages.output().getType() == formats::MSG)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto msg_nan = [this](Messages &messages)
	{
		// If the user is sending a MSG message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::MSG && messages.output().getType() == formats::NONE)
		{
			this->messages.clear();
			return true;
		}
		return false;
	};

	auto anyreply_err = [this](Messages &messages)
	{
		// If the user is sending a REPLY message and the input message is of type NONE, then send an ERR message and clear the input and output messages
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
		// If the user is sending a REPLY message and the input message is of type NONE, then clear the input and output messages
		if (messages.input().getType() == formats::REPLY)
		{
			this->awaitsResponse = false;
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
			break;
		}
		// Check if there is input from the user
		if (fds[0].revents & POLLIN)
		{
			try
			{
				std::string messageText;
				if (!std::getline(std::cin, messageText))
					throw std::runtime_error("End of file reached");
				messages.output() = handler.readMessage(messageText);
				if (messages.output().getType() != formats::MessageType::NONE)
					this->send(this->encode(messages.output()));
			}
			catch (std::exception &e)
			{
				formats::Message message(formats::ERR_INTERNAL, config::displayName);
				message.setText(e.what());
				if (!std::cin)
				{
					this->exit();
					continue;
				}
				handler.printMessage(message);
			}
		}
		// Check if there is input from the server
		if (fds[1].revents & POLLIN)
		{
			std::string messageText = TCPReceiver::receive();
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

		// check if the timeout has expired
		if (awaitsResponse && (std::chrono::system_clock::now() - timestamp) > std::chrono::seconds(5))
		{
			this->exit();
		}
		timestamp = std::chrono::system_clock::now();

		// Run the finite state machine
		if (messages.input().getType() != formats::MessageType::NONE || messages.output().getType() != formats::MessageType::NONE)
		{
			std::shared_ptr<FSMNode> next = NodeStates[this->state]->next(messages);
			if (next != nullptr)
				this->state = next->state;
			else
			{
				formats::Message message(formats::ERR_INTERNAL, config::displayName);
				message.setText("Unexpected message");
				handler.printMessage(message);
				this->messages.clear();
			}
			// std::cout << "nState: " << this->state << std::endl;
		}
		// Check if we should stop
		if (this->state == END)
			running = false;
		// Wait a bit before checking again
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
	::close(fds[1].fd);
	::close(fds[0].fd);
}

void TCPFSM::exit()
{
	this->messages.clear();
	this->messages.output() = formats::Message(formats::MessageType::BYE, config::displayName);
	this->send(this->encode(messages.output()));
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
