#include <memory>
#include <map>
#include <functional>
#include <utility>
#include <atomic>

#include "fsm.hpp"
#include "node.hpp"
#include "edge.hpp"

#include "../io_handler/io_handler.hpp"
#include "../encoders/TCP_encoder.hpp"
#include "../decoders/TCP_decoder.hpp"
#include "../transceivers/TCP_transceiver.hpp"
#include "../formats/message.hpp"

#ifndef TCP_FSM_HPP
#define TCP_FSM_HPP

class TCPFSM : public FSM, public TCPTransceiver, public decoders::TCPDecoder, public encoders::TCPEncoder
{
	bool awaitsResponse = false;
	std::chrono::time_point<std::chrono::system_clock> timestamp;
	class Messages {
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
	using FSMNode = Node<states, Messages>;
	using FSMEdge = Edge<states, Messages>;
	using TCPTransceiver::send;
	using TCPTransceiver::receive;
	using TCPTransceiver::init;
	using TCPTransceiver::isOpen;
	using decoders::TCPDecoder::decode;
	using encoders::TCPEncoder::encode;

public:
	states state = START;
	TCPFSM();
	std::map<states, std::shared_ptr<FSMNode>> NodeStates;
	void run() override;
	void exit() override;
};
#endif