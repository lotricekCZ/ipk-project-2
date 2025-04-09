#include "UDP_encoder.hpp"
#include <string>
#include <regex>
#include <iostream>
#include <unordered_map>
#include "../formats/message.hpp"
#include "../config.hpp"

namespace encoders
{
	std::tuple<uint8_t *, uint16_t> UDPEncoder::encodeBinary(formats::Message &message)
	{
		// replace placeholders in the message format with actual values
		// from the message and configuration
		std::unordered_map<std::string, std::string> values = {
			{"DisplayName", message.getAuthor()},
			{"MessageContent", message.getText()},
			{"Username", config::username},
			{"Secret", config::secret},
			{"ChannelID", config::channel}};
		
		std::map<formats::MessageType, std::function<std::tuple<uint8_t *, uint16_t>(formats::Message &)>> encoders = {
			{formats::ERR, [](formats::Message &message) {return std::tuple<uint8_t *, uint16_t>{nullptr, 0};}},
			{formats::REPLY, [](formats::Message &message) {return std::tuple<uint8_t *, uint16_t>{nullptr, 0};}},
			{formats::AUTH, [](formats::Message &message) {return std::tuple<uint8_t *, uint16_t>{nullptr, 0};}},
			{formats::JOIN, [](formats::Message &message) {return std::tuple<uint8_t *, uint16_t>{nullptr, 0};}},
			{formats::BYE, [](formats::Message &message) {return std::tuple<uint8_t *, uint16_t>{nullptr, 0};}},
			{formats::MSG, [](formats::Message &message) {return std::tuple<uint8_t *, uint16_t>{nullptr, 0};}},
			{formats::PING, [](formats::Message &message) {return std::tuple<uint8_t *, uint16_t>{nullptr, 0};}},
		};
		try
		{
			// get the message format string for the given message type
			
			std::tuple<uint8_t *, uint16_t> encodedMessage = encoders.at(message.getType())(message);
			return encodedMessage;
		}
		catch (const std::out_of_range &e)
		{
			// handle the case where the message type is unknown
			std::cerr << e.what() << std::endl;
		}
		return {nullptr, 0};
	}
}
