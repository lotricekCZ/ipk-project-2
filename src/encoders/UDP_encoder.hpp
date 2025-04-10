#ifndef UDPENCODER_HPP
#define UDPENCODER_HPP

#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <cstring>
#include <functional>

#include "encoder.hpp"
#include "../fsm/fsm.hpp"
#include "../fsm/node.hpp"
#include "../fsm/edge.hpp"

namespace encoders
{
	class bytes : std::vector<uint8_t>
	{
	public:
		using std::vector<uint8_t>::iterator;
		using std::vector<uint8_t>::begin;
		using std::vector<uint8_t>::end;
		using std::vector<uint8_t>::vector;
		using std::vector<uint8_t>::operator=;
		using std::vector<uint8_t>::assign;
		using std::vector<uint8_t>::get_allocator;
		using std::vector<uint8_t>::at;
		using std::vector<uint8_t>::operator[];
		using std::vector<uint8_t>::front;
		using std::vector<uint8_t>::back;
		using std::vector<uint8_t>::data;
		using std::vector<uint8_t>::empty;
		using std::vector<uint8_t>::size;
		using std::vector<uint8_t>::max_size;
		using std::vector<uint8_t>::reserve;
		using std::vector<uint8_t>::capacity;
		using std::vector<uint8_t>::shrink_to_fit;
		using std::vector<uint8_t>::clear;
		using std::vector<uint8_t>::insert;
		using std::vector<uint8_t>::emplace;
		using std::vector<uint8_t>::erase;
		using std::vector<uint8_t>::push_back;
		using std::vector<uint8_t>::emplace_back;
		using std::vector<uint8_t>::pop_back;
		using std::vector<uint8_t>::resize;
		using std::vector<uint8_t>::swap;
		bytes(std::string str) : std::vector<uint8_t>(str.begin(), str.end()) {}
		bytes(const uint8_t *str) : std::vector<uint8_t>(str, str + strlen((char *)str)) {}
		std::string str() const
		{
			return std::string(this->begin(), this->end());
		}
		std::tuple<std::string, uint16_t> data() const;
	};

	/**
	 * CONFIRM
	 * |  0x00  |  Ref_MessageID  |
	 * REPLY
	 * |  0x01  |    MessageID    | Result |  Ref_MessageID  |  MessageContents  | 0 |
	 * AUTH
	 * |  0x02  |    MessageID    |  Username  | 0 |  DisplayName  | 0 |  Secret  | 0 |
	 * JOIN
	 * |  0x03  |    MessageID    |  ChannelID | 0 |  DisplayName  | 0 |
	 * MSG
	 * |  0x04  |    MessageID    |  DisplayName  | 0 |  MessageContents  | 0 |
	 * ERR
	 * |  0xFE  |    MessageID    |  DisplayName  | 0 |  MessageContents  | 0 |
	 * BYE
	 * |  0xFF  |    MessageID    |  DisplayName  | 0 |
	 * PING
	 * |  0xFD  |    MessageID    |
	 */
	static std::map<formats::MessageType, std::string> udpMessageFormats = {
		{formats::CONFIRM, "{%b:Type}{%h:messageID}"},
		{formats::ERR, "{%b:Type}{%h:messageID}{%s:DisplayName}\\x00{%s:MessageContent}\\x00"},
		{formats::REPLY, "{%b:Type}{%h:messageID}{%b:Result}{%h:rMessageID}{%s:MessageContent}\\x00"},
		{formats::AUTH, "{%b:Type}{%h:messageID}{%s:Username}\\x00{%s:DisplayName}\\x00{%s:Secret}\\x00"},
		{formats::JOIN, "{%b:Type}{%h:messageID}{%s:Channel}\\x00{%s:DisplayName}\\x00"},
		{formats::MSG, "{%b:Type}{%h:messageID}{%s:DisplayName}\\x00{%s:MessageContent}\\x00"},
		{formats::BYE, "{%b:Type}{%h:messageID}}"},
		{formats::PING, "{%b:Type}{%h:messageID}"}};

	class UDPEncoder : public Encoder
	{
		static uint16_t messageID;

		enum states
		{
			INIT,
			// non-special characters
			ORDINARY,
			// substitutes
			SUB_START,
			SUB_TYPE,

			SUB_BYTE,
			SUB_B_COL,
			SUB_B_ID,
			SUB_B_EXEC,

			SUB_SHORT,
			SUB_S_COL,
			SUB_S_ID,
			SUB_S_EXEC,

			SUB_INT,
			SUB_I_COL,
			SUB_I_ID,
			SUB_I_EXEC,

			SUB_STRING,
			SUB_STR_COL,
			SUB_STR_ID,
			SUB_STR_EXEC,

			SUB_UNDEFINED,
			SUB_U_EXEC,
			// special characters
			SPEC_START,
			BACKSLASH,
			HEX,
			HEX_NIBBLE,
			HEX_BYTE
		};

		states output = INIT;
		using FSMNode = Node<states, bytes::iterator>;
		using FSMEdge = Edge<states, bytes::iterator>;
		std::unordered_map<states, std::shared_ptr<FSMNode>> NodeStates;

	public:
		UDPEncoder();
		std::string encode(formats::Message &message) override
		{
			throw std::runtime_error("Not implemented");
			return "";
		};
		std::tuple<uint8_t *, uint16_t> encodeBinary(formats::Message &message) override;
	};
}

#endif