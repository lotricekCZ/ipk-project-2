#include <string>
#include <regex>
#include <iostream>
#include <unordered_map>

#include "TCP_encoder.hpp"
#include "../formats/message.hpp"
#include "../config.hpp"

namespace encoders
{
	/**
	 * @brief Encodes a message into a string format based on its type.
	 * 
	 * This function replaces placeholders in the message format with actual
	 * values from the message and configuration.
	 * 
	 * @param message The message to encode.
	 * @return The encoded message as a string.
	 */
	std::string TCPEncoder::encode(formats::Message &message)
	{
		// replace placeholders in the message format with actual values
		// from the message and configuration
		std::unordered_map<std::string, std::string> values = {
			{"DisplayName", message.getAuthor()},
			{"MessageContent", message.getText()},
			{"dl", delimiter},
			{"Username", config::username},
			{"Secret", config::secret},
			{"ChannelID", config::channel}};

		try
		{
			// get the message format string for the given message type
			std::string encodedMessage = tcpMessageFormats.at(message.getType());

			// search for {key} in the message format
			std::regex placeholderRegex(R"(\{(\w+)\})");

			// loop until all placeholders have been replaced
			std::smatch match;
			while (std::regex_search(encodedMessage, match, placeholderRegex))
			{
				// get the name of the placeholder
				std::string key = match[1].str();

				// get the replacement string for the placeholder
				std::string replacement = values.contains(key) ? values.at(key) : "[UNKNOWN]";

				// replace the placeholder with the replacement string
				encodedMessage.replace(match.position(0), match.length(0), replacement);
			}
			return encodedMessage;
		}
		catch (const std::out_of_range &e)
		{
			// handle the case where the message type is unknown
			std::cerr << e.what() << std::endl;
		}
		return "";
	}
}
