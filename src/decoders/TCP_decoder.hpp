#ifndef TCP_DECODER_HPP
#define TCP_DECODER_HPP

#include <unordered_map>
#include <string>
#include "decoder.hpp"
#include "../formats/message.hpp"

namespace decoders
{
	static std::unordered_map<formats::MessageType, std::string> messageFormats = {
	{formats::ERR, "ERR FROM (.+) IS (.+)\r\n"},
	{formats::REPLY, "REPLY (OK|NOK) IS (.+)\r\n"},
	{formats::AUTH, "AUTH (.+) AS (.+) USING (.+)\r\n"},
	{formats::JOIN, "JOIN (.+) AS (.+)\r\n"},
	{formats::MSG, "MSG FROM (.+) IS (.+)\r\n"},
	{formats::BYE, "BYE FROM (.+)\r\n"}};
	
	class TCPDecoder : public Decoder
	{

	public:
		formats::Message decode(const std::string &data) override;
	};
}

#endif