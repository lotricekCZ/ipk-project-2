#ifndef UDPENCODER_HPP
#define UDPENCODER_HPP

#include "encoder.hpp"
#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <cstring>

namespace encoders
{
	class bytes : std::vector<char>
	{
	public:
		using std::vector<char>::vector;
		using std::vector<char>::operator=;
		using std::vector<char>::assign;
		using std::vector<char>::get_allocator;
		using std::vector<char>::at;
		using std::vector<char>::operator[];
		using std::vector<char>::front;
		using std::vector<char>::back;
		using std::vector<char>::data;
		using std::vector<char>::empty;
		using std::vector<char>::size;
		using std::vector<char>::max_size;
		using std::vector<char>::reserve;
		using std::vector<char>::capacity;
		using std::vector<char>::shrink_to_fit;
		using std::vector<char>::clear;
		using std::vector<char>::insert;
		using std::vector<char>::emplace;
		using std::vector<char>::erase;
		using std::vector<char>::push_back;
		using std::vector<char>::emplace_back;
		using std::vector<char>::pop_back;
		using std::vector<char>::resize;
		using std::vector<char>::swap;
		bytes(std::string str) : std::vector<char>(str.begin(), str.end()) {}
		bytes(const char *str) : std::vector<char>(str, str + strlen(str)) {}
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
	static std::map<formats::MessageType, bytes> udpMessageFormats = {
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

	public:
		std::string
		encode(formats::Message &message) override
		{
			std::runtime_error("Not implemented");
		};
		std::tuple<uint8_t *, uint16_t> encodeBinary(formats::Message &message) override;
	};
}

#endif