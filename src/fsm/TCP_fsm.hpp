#include <memory>
#include <map>
#include <functional>
#include <utility>

#include "fsm.hpp"
#include "node.hpp"
#include "edge.hpp"

#include "../encoders/TCP_encoder.hpp"
#include "../decoders/TCP_decoder.hpp"
#include "../transceivers/TCP_transceiver.hpp"
#include "../formats/message.hpp"

#ifndef TCP_FSM_HPP
#define TCP_FSM_HPP

class TCPFSM : public FSM, public TCPTransceiver, public decoders::TCPDecoder, public encoders::TCPEncoder
{
	struct Messages {
		formats::Message input;
		formats::Message output;
	} messages;

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
	void run();
};
#endif