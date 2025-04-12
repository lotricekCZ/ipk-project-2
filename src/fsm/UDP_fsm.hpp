#include <memory>
#include <map>
#include <functional>
#include <utility>
#include <atomic>
#include <chrono>
#include <deque>

#include "fsm.hpp"
#include "node.hpp"
#include "edge.hpp"

#include "../config.hpp"
#include "../io_handler/io_handler.hpp"
#include "../encoders/UDP_encoder.hpp"
#include "../decoders/UDP_decoder.hpp"
#include "../transceivers/UDP_transceiver.hpp"
#include "../formats/message.hpp"

#ifndef UDP_FSM_HPP
#define UDP_FSM_HPP

class UDPFSM : public FSM, public UDPTransceiver, public decoders::UDPDecoder, public encoders::UDPEncoder
{
	class StackElement
	{
	public:
		std::chrono::time_point<std::chrono::system_clock> timestamp;
		uint8_t retransmissions;
		formats::Message message;
		StackElement(formats::Message &message) : message(message)
		{
			timestamp = std::chrono::system_clock::now();
			retransmissions = config::retransmissions;
		}
		void decrement()
		{
			timestamp = std::chrono::system_clock::now();
			if (retransmissions != 0)
				retransmissions--;
		}
		uint16_t getID()
		{
			return message.getID();
		}
		bool isExpired()
		{
			return std::chrono::system_clock::now() - timestamp > std::chrono::milliseconds(config::timeout);
		}
		bool hasRetransmissions()
		{
			return retransmissions != 0;
		}
		formats::Message getMessage()
		{
			return message;
		}
		bool operator==(uint16_t id)
		{
			return message.getID() == id;
		}
	};

	class Messages
	{
		formats::Message _input;
		formats::Message _output;

	public:
		Messages() = default;
		void clear();
		formats::Message &input();
		formats::Message &output();
	} messages;
	IOHandler handler;
	enum states
	{
		START,
		AUTH,
		OPEN,
		JOIN,
		END
	};
	using Stack = std::deque<StackElement>;
	using FSMNode = Node<states, Messages>;
	using FSMEdge = Edge<states, Messages>;
	using decoders::UDPDecoder::decode;
	using encoders::UDPEncoder::encode;
	using UDPTransceiver::init;
	using UDPTransceiver::isOpen;
	using UDPTransceiver::receive;
	using UDPTransceiver::send;

	Stack stack;

public:
	states state = START;
	UDPFSM();
	std::map<states, std::shared_ptr<FSMNode>> NodeStates;
	void run();
};
#endif