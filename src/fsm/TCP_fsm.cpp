#include <memory>
#include <chrono>
#include <thread>
#include <poll.h>
#include "TCP_fsm.hpp"
#include "../io_handler/io_handler.hpp"

TCPFSM::TCPFSM()
{
	auto nan_auth = [&](Messages &messages)
	{
		if (messages.output().getType() == formats::AUTH && messages.input().getType() == formats::NONE)
		{
			messages.clear();
			return true;
		}
		return false;
	};

	auto errbye_nan = [&](Messages &messages)
	{
		if (messages.output().getType() == formats::NONE && (messages.input().getType() == formats::ERR || messages.input().getType() == formats::BYE))
		{
			messages.clear();
			return true;
		}
		return false;
	};

	auto nan_bye = [&](Messages &messages)
	{
		if (messages.output().getType() == formats::BYE && messages.input().getType() == formats::NONE)
		{
			messages.clear();
			return true;
		}
		return false;
	};

	auto nreply_auth = [&](Messages &messages)
	{
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
		if (messages.input().getType() == formats::REPLY && messages.input().getStatus() == true)
		{
			messages.clear();
			return true;
		}
		return false;
	};

	auto msg_err = [&](Messages &messages)
	{
		if (messages.input().getType() == formats::MSG)
		{
			messages.output() = formats::Message(formats::ERR, "ERROR: Current state of this FSM is AUTH, receiving messages is not allowed!");
			this->send(this->encode(messages.output()));
			messages.clear();
			return true;
		}
		return false;
	};

	auto nan_join = [&](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE && messages.output().getType() == formats::JOIN)
		{
			messages.clear();
			return true;
		}
		return false;
	};

	auto nan_msg = [&](Messages &messages)
	{
		if (messages.input().getType() == formats::NONE && messages.output().getType() == formats::MSG)
		{
			messages.clear();
			return true;
		}
		return false;
	};

	auto msg_nan = [&](Messages &messages)
	{
		if (messages.output().getType() == formats::MSG && messages.input().getType() == formats::NONE)
		{
			messages.clear();
			return true;
		}
		return false;
	};

	auto anyreply_err = [&](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY)
		{
			messages.output() = formats::Message(formats::ERR, "ERROR: Client was not requesting anything, reply is unexpected!");
			this->send(this->encode(messages.output()));
			messages.clear();
			return true;
		}
		return false;
	};

	auto anyreply_nan = [&](Messages &messages)
	{
		if (messages.input().getType() == formats::REPLY && messages.output().getType() == formats::MessageType::NONE)
		{
			messages.clear();
			return true;
		}
		return false;
	};

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

void TCPFSM::run()
{
	bool running = true;
	struct pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[1].fd = TCPReceiver::getSocket();
	fds[1].events = POLLIN;

	TCPFSM::states curr = END;
	TCPTransceiver::init();
	while (running)
	{
		int ret = poll(fds, 2, 200);
		if (ret < 0)
		{
			std::cerr << "poll" << std::endl;
			break;
		}
		if (fds[0].revents & POLLIN)
		{
			std::string messageText;
			std::getline(std::cin, messageText);
			messages.output() = handler.readMessage(messageText);
			if (messages.output().getType() != formats::MessageType::NONE)
				this->send(this->encode(messages.output()));
		}
		if (fds[1].revents & POLLIN)
		{
			std::string messageText = TCPReceiver::receive();
			formats::Message line = TCPDecoder::decode(messageText);
			std::cout << messageText << std::endl;
			messages.input() = line;
			if (line.getType() != formats::MessageType::NONE)
				handler.printMessage(line);
		}

		if (this->state != curr)
		{
			std::cout << "State: " << this->state << std::endl;
			curr = this->state;
		}
		if (messages.input().getType() != formats::MessageType::NONE || messages.output().getType() != formats::MessageType::NONE)
		{
			std::shared_ptr<FSMNode> next = NodeStates[this->state]->next(messages);
			if(next != nullptr)
				this->state = next->state;
		}
		if (this->state == END)
			running = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
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