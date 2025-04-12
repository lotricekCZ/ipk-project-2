#ifndef UDP_DECODER_HPP
#define UDP_DECODER_HPP

#include <unordered_map>
#include <string>
#include "decoder.hpp"
#include "../formats/message.hpp"
#include "../fsm/fsm.hpp"
#include "../fsm/node.hpp"
#include "../fsm/edge.hpp"

namespace decoders
{
	enum states
	{
		INIT,

		CONFIRM,
		CNF_ID_1,
		CNF_ID_2,

		REPLY,
		RPL_ID_1,
		RPL_ID_2,
		RPL_RES,
		RPL_REF_ID_1,
		RPL_REF_ID_2,
		RPL_CONTENT,
		RPL_ZERO,

		MESSAGE,
		MSG_ID_1,
		MSG_ID_2,
		MSG_DISPNAME,
		MSG_ZERO,
		MSG_CONTENT,
		MSG_ZERO_1,

		PING,
		PNG_ID_1,
		PNG_ID_2,

		ERROR,
		ERR_ID_1,
		ERR_ID_2,
		ERR_DISPNAME,
		ERR_ZERO,
		ERR_CONTENT,
		ERR_ZERO_1,

		BYE,
		BYE_ID_1,
		BYE_ID_2,
		BYE_ZERO,
		BYE_DISPNAME,
		BYE_ZERO_1,

		JOIN,
		JOIN_ID_1,
		JOIN_ID_2,
		JOIN_CHANNELID,
		JOIN_ZERO,
		JOIN_DISPNAME,
		JOIN_ZERO_1,

		AUTH,
		AUTH_ID_1,
		AUTH_ID_2,
		AUTH_USERNAME,
		AUTH_ZERO,
		AUTH_DISPNAME,
		AUTH_ZERO_1,
		AUTH_SECRET,
		AUTH_ZERO_2,

		END // if there is no transition
	};

	class UDPDecoder : public Decoder
	{
		struct {
			uint16_t messageID = 0;
			uint16_t replyID = 0;
			uint8_t status = 0;
			std::string dispName;
			std::string channelID;
			std::string content;
			std::string secret;
			std::string username;
		} controls;
		states output = END;
		using FSMNode = Node<states, char*>;
		using FSMEdge = Edge<states, char*>;
		std::unordered_map<states, std::shared_ptr<FSMNode>> NodeStates;
		std::unordered_map<states, formats::MessageType> translationMap = {
			{CNF_ID_2, formats::CONFIRM},
			{RPL_ZERO, formats::REPLY},
			{MSG_ZERO_1, formats::MSG},
			{PNG_ID_2, formats::PING},
			{ERR_ZERO_1, formats::ERR},
			{BYE_ZERO_1, formats::BYE},
			{JOIN_ZERO_1, formats::JOIN},
			{AUTH_ZERO_2, formats::AUTH}
		};
		formats::Message translate();
	public:
		UDPDecoder();
		formats::Message decode(uint8_t *data, uint16_t size) override;
		formats::Message decode(const std::string &data) override {return decode((uint8_t *)data.c_str(), data.size());};
	};
}

#endif