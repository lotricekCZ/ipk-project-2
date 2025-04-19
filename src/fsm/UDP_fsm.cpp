/**
 * @file UDP_fsm.cpp
 * @brief Implementation of UDPFSM class
 *
 * This file contains the implementation of the UDPFSM class, which is a
 * finite state machine for managing UDP connections.
 */

#include <memory>
#include <chrono>
#include <algorithm>
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

	// auth-auth conditions
	// #1
	auto auth_authminus_nan_authminus_yes = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE)
		{
			Stack::iterator element = std::find_if(stack.begin(), stack.end(),
												   [](UDPFSM::StackElement &element)
												   {
													   return element.getMessage().getType() == formats::AUTH && element.isExpired();
												   });
			if (element != stack.end())
			{
				element->decrement();
				formats::Message message = element->getMessage();

				uint8_t *data;
				uint16_t size;
				std::tie(data, size) = this->encodeBinary(message);
				if (size > 0)
					this->send(data, size);
				this->messages.clear();
				return true;
			}
			this->messages.clear();
		}
		return false;
	};

	// #2
	auto auth_none_nreply_nan_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY && messages.input().getStatus() == false)
		{
			std::erase_if(stack, [&messages](UDPFSM::StackElement &element)
						  { return element == messages.input().getRefID(); });
			this->messages.clear();
			return true;
		}
		return false;
	};

	// #3
	auto authzero_none_nan_nan_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE)
		{
			uint16_t count = 0;
			std::erase_if(stack, [&count](UDPFSM::StackElement &element)
						  {count += !element.hasRetransmissions(); 
							return !element.hasRetransmissions(); });
			this->messages.clear();
			return count > 0;
		}
		return false;
	};
	// #4
	auto empty_auth_nan_empty_no = [this](Messages &messages)
	{
		if (stack.empty() && messages.output().getType() == formats::AUTH)
		{
			stack.emplace_front(messages.output());
			this->messages.clear();
			return true;
		}
		return false;
	};

	// auth-open condition
	auto auth_empty_reply_confirm_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY && messages.input().getStatus() == true)
		{
			Stack::iterator element = std::find_if(stack.begin(), stack.end(), [&messages](UDPFSM::StackElement &element)
												   { return element == messages.input().getRefID(); });
			if (element != stack.end())
			{
				this->messages.output() = formats::Message(formats::CONFIRM, config::displayName);
				this->messages.output().setID(messages.input().getID());
				this->messages.output().setRefID(messages.input().getID());
				uint8_t *data;
				uint16_t size;
				std::tie(data, size) = this->encodeBinary(this->messages.output());
				if (size > 0)
					this->send(data, size);
				stack.erase(element);
				this->messages.clear();
				return true;
			}
		}
		return false;
	};

	// auth-end condition
	auto none_none_msg_err_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::MSG)
		{
			this->messages.output() = formats::Message(formats::ERR, config::displayName);
			uint8_t *data;
			uint16_t size;
			std::tie(data, size) = this->encodeBinary(this->messages.output());
			if (size > 0)
				this->send(data, size);
			this->messages.clear();
			return true;
		}
		return false;
	};

	// open-open conditions
	// #1
	auto none_none_msg_confirm_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::MSG)
		{
			this->messages.output() = formats::Message(formats::CONFIRM, config::displayName);
			this->messages.output().setID(messages.input().getID());
			this->messages.output().setRefID(messages.input().getID());
			uint8_t *data;
			uint16_t size;
			std::tie(data, size) = this->encodeBinary(this->messages.output());
			if (size > 0)
				this->send(data, size);
			this->messages.clear();
			return true;
		}

		return false;
	};

	// #2
	auto none_msg_nan_msg_no = [this](Messages &messages)
	{
		if (messages.output().getType() == formats::MSG)
		{
			stack.emplace_back(messages.output());
			this->messages.clear();
			return true;
		}
		return false;
	};

	// #3
	auto msg_msgminus_nan_msgminus_yes = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE)
		{
			Stack::iterator element = std::find_if(stack.begin(), stack.end(),
												   [](UDPFSM::StackElement &element)
												   {
													   return element.getMessage().getType() == formats::MSG && element.isExpired();
												   });
			if (element != stack.end())
			{
				element->decrement();
				formats::Message message = element->getMessage();
				uint8_t *data;
				uint16_t size;
				std::tie(data, size) = this->encodeBinary(message);
				if (size > 0)
					this->send(data, size);
				this->messages.clear();
				return true;
			}
			this->messages.clear();
		}
		return false;
	};

	// #4
	auto msg_none_confirm_nan_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::CONFIRM)
		{
			uint16_t count = 0;
			std::erase_if(stack, [&messages, &count](UDPFSM::StackElement &element)
						  { 
							count += (element == ntohs(messages.input().getID()));
							return element == ntohs(messages.input().getID()); });
			this->messages.clear();
			return count > 0;
		}
		return false;
	};

	// #5
	auto none_none_ping_confirm_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::PING)
		{
			this->messages.output() = formats::Message(formats::CONFIRM, config::displayName);
			this->messages.output().setID(messages.input().getID());
			this->messages.output().setRefID(messages.input().getID());
			uint8_t *data;
			uint16_t size;

			std::tie(data, size) = this->encodeBinary(this->messages.output());
			if (size > 0)
				this->send(data, size);
			this->messages.clear();
			return true;
		}
		return false;
	};

	// #6 & #7
	auto msgjoinzero_none_nan_nan_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE)
		{
			uint16_t count = 0;
			std::erase_if(stack, [&count](UDPFSM::StackElement &element)
						  {count += !element.hasRetransmissions(); 
							return !element.hasRetransmissions(); });
			this->messages.clear();
			return count > 0;
		}
		return false;
	};

	// open-join conditions
	// #1
	auto empty_join_nan_join_no = [this](Messages &messages)
	{
		if (stack.empty() && messages.output().getType() == formats::JOIN)
		{
			stack.emplace_front(messages.output());
			this->messages.clear();
			return true;
		}
		return false;
	};
	// #2
	auto join_join_nan_join_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE)
		{
			Stack::iterator element = std::find_if(stack.begin(), stack.end(),
												   [](UDPFSM::StackElement &element)
												   {
													   return element.getMessage().getType() == formats::JOIN && element.hasRetransmissions();
												   });
			if (element != stack.end())
			{
				formats::Message message = element->getMessage();
				uint8_t *data;
				uint16_t size;
				std::tie(data, size) = this->encodeBinary(message);
				if (size > 0)
					this->send(data, size);
				this->messages.clear();
				return true;
			}
		}
		this->messages.clear();
		return false;
	};

	// join-join conditions
	// #1 & #2
	auto none_none_msgping_confirm_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::MSG || messages.input().getType() == formats::PING)
		{
			this->messages.output() = formats::Message(formats::CONFIRM, config::displayName);
			this->messages.output().setID(messages.input().getID());
			this->messages.output().setRefID(this->messages.output().getID());
			uint8_t *data;
			uint16_t size;
			std::tie(data, size) = this->encodeBinary(this->messages.output());
			if (size > 0)
				this->send(data, size);
			this->messages.clear();
			return true;
		}
		return false;
	};

	// #3
	auto join_join_confirm_nan_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::CONFIRM)
		{
			Stack::iterator element = std::find_if(stack.begin(), stack.end(), [&messages](UDPFSM::StackElement &element)
												   { return element == messages.input().getID(); });
			if (element != stack.end())
			{
				this->messages.clear();
				return true;
			}
		}
		return false;
	};

	// join-message conditions
	// #1
	auto join_joinminus_nan_nan_yes = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE)
		{
			Stack::iterator element = std::find_if(stack.begin(), stack.end(),
												   [](UDPFSM::StackElement &element)
												   {
													   return element.getMessage().getType() == formats::JOIN && element.isExpired();
												   });
			if (element != stack.end())
			{
				element->decrement();
				this->messages.clear();
				return true;
			}
			this->messages.clear();
		}
		return false;
	};

	// #2
	auto join_none_reply_confirm_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY)
		{
			// check if the message is exists
			Stack::iterator element = std::find_if(stack.begin(), stack.end(),
												   [](UDPFSM::StackElement &element)
												   {
													   return element.getMessage().getType() == formats::JOIN && element.isExpired();
												   });
			if (element != stack.end())
			{
				this->messages.output() = formats::Message(formats::CONFIRM, config::displayName);
				this->messages.output().setID(messages.input().getID());
				this->messages.output().setRefID(messages.input().getID());
				uint8_t *data;
				uint16_t size;
				std::tie(data, size) = this->encodeBinary(this->messages.output());
				this->send(data, size);
				stack.erase(element);
				this->messages.clear();
				return true;
			}
		}
		return false;
	};

	auto none_none_anyreply_err_no = [this](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY && messages.input().getRefID() >= UDPEncoder::getID())
		{
			this->messages.output() = formats::Message(formats::ERR, config::displayName);
			this->messages.output().setText("ERROR: Client was not requesting anything, reply is unexpected!");
			uint8_t *data;
			uint16_t size;
			std::tie(data, size) = this->encodeBinary(this->messages.output());
			if (size > 0)
				this->send(data, size);
			this->messages.clear();
			return true;
		}
		return false;
	};

	// START -> AUTH (empty_auth_nan_auth_no), END (none_none_nan_bye_no, none_none_errbye_nan_no)
	// AUTH -> OPEN (auth_empty_reply_confirm_no), END (none_none_errbye_nan_no, none_none_nan_bye_no, none_msg_err_no), AUTH (authzero_none_nan_nan_no, auth_authminus_nan_authminus_yes, auth_none_nreply_nan_no)
	// OPEN -> JOIN (none_none_nan_join_no), OPEN (none_msg_nan_no, none_nan_msg_no), END (none_none_errbye_nan_no, none_none_nan_bye_no, none_anyreply_err_no)
	// JOIN -> JOIN (none_msg_nan_no), OPEN (none_anyreply_nan_no), END (none_none_nan_bye_no, none_none_errbye_nan_no)
	for (auto &node : {UDPFSM::START, UDPFSM::AUTH, UDPFSM::OPEN, UDPFSM::JOIN, UDPFSM::END})
		NodeStates[node] = std::make_shared<FSMNode>(node);
	NodeStates[START]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH], empty_auth_nan_auth_no),
								   std::make_shared<FSMEdge>(NodeStates[END], none_none_nan_bye_no, none_none_errbye_nan_no));
	NodeStates[AUTH]->assignEdges(std::make_shared<FSMEdge>(NodeStates[OPEN], auth_empty_reply_confirm_no),
								  std::make_shared<FSMEdge>(NodeStates[END], none_none_errbye_nan_no, none_none_nan_bye_no, none_none_msg_err_no),
								  std::make_shared<FSMEdge>(NodeStates[AUTH], authzero_none_nan_nan_no, auth_authminus_nan_authminus_yes, auth_none_nreply_nan_no,
															empty_auth_nan_auth_no));
	NodeStates[OPEN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN], empty_join_nan_join_no, join_join_nan_join_no),
								  std::make_shared<FSMEdge>(NodeStates[OPEN], msgjoinzero_none_nan_nan_no, msg_msgminus_nan_msgminus_yes,
															none_none_msg_confirm_no, none_none_ping_confirm_no,
															none_msg_nan_msg_no, msg_none_confirm_nan_no),
								  std::make_shared<FSMEdge>(NodeStates[END], none_none_errbye_nan_no, none_none_nan_bye_no,
															none_none_anyreply_err_no));
	NodeStates[JOIN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN], none_none_msg_confirm_no, none_none_ping_confirm_no,
															join_join_confirm_nan_no),
								  std::make_shared<FSMEdge>(NodeStates[OPEN], join_joinminus_nan_nan_yes, join_none_reply_confirm_no),
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
		uint8_t *data;
		uint16_t size;
		int ret = poll(fds, 2, 100);
		if (ret < 0)
		{
			break;
		}
		// Check if there is input from the user
		if (fds[0].revents & POLLIN)
		{
			std::string messageText;
			std::getline(std::cin, messageText);
			messages.output() = handler.readMessage(messageText);
			if (messages.output().getType() != formats::MessageType::NONE)
			{
				std::tie(data, size) = this->encodeBinary(messages.output());
				messages.output().setID(UDPEncoder::getID() - 1);
				if (size > 0)
					this->send(data, size);
			}
		}
		// Check if there is input from the server
		if (fds[1].revents & POLLIN)
		{
			std::string messageText = UDPReceiver::receive();
			formats::Message line = UDPDecoder::decode(messageText);

			messages.input() = line;
			if (line.getType() != formats::MessageType::NONE)
			{
				UDPSender::setPort(htons(UDPReceiver::getPort()));
				handler.printMessage(line);
			}
		}

		// Check if the state has changed
		if (this->state != curr)
		{
			curr = this->state;
		}
		// Run the finite state machine
		if (!stack.empty() || messages.input().getType() != formats::MessageType::NONE || messages.output().getType() != formats::MessageType::NONE)
		{
			std::shared_ptr<FSMNode> next = NodeStates[this->state]->next(messages);
			if (next != nullptr)
				this->state = next->state;
		}
		// Check if we should stop
		if (this->state == END)
			running = false;
		// Wait a bit before checking again
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

void UDPFSM::exit()
{
	this->messages.clear();
	this->messages.output() = formats::Message(formats::MessageType::BYE, config::displayName);
	uint8_t *data;
	uint16_t size;
	std::tie(data, size) = this->encodeBinary(messages.output());
	if (size > 0)
		this->send(data, size);
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

UDPFSM::~UDPFSM()
{
	for (auto it = NodeStates.begin(); it != NodeStates.end();)
	{
		it->second.reset();
		it = NodeStates.erase(it);
	}
	NodeStates.clear();
}
