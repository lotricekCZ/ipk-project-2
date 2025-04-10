#include "UDP_encoder.hpp"
#include <string>
#include <regex>
#include <iostream>
#include <variant>
#include <unordered_map>
#include "../formats/message.hpp"
#include "../config.hpp"


namespace encoders
{
	uint16_t UDPEncoder::messageID = 0;

	std::tuple<uint8_t *, uint16_t> UDPEncoder::encodeBinary(formats::Message &message)
	{
		// replace placeholders in the message format with actual values
		// from the message and configuration
		std::unordered_map<std::string, std::variant<std::string, uint16_t>> values = {
			{"DisplayName", message.getAuthor()},
			{"MessageContent", message.getText()},
			{"Username", config::username},
			{"Secret", config::secret},
			{"ChannelID", config::channel},
			{"messageID", messageID},
			{"rMessageID", message.getRefID()}
		};
		
		try
		{
			// get the message format string for the given message type
			std::tuple<uint8_t *, uint16_t> encodedMessage{nullptr, 0};
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
