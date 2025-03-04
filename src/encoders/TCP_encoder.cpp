#include <string>
#include <regex>
#include <iostream>
#include <unordered_map>

#include "TCP_encoder.hpp"
#include "../formats/message.hpp"
#include "../config.hpp"

namespace encoders
{
	std::string TCPEncoder::encode(formats::Message &message)
	{

		std::unordered_map<std::string, std::string> values = {
			{"DisplayName", message.getAuthor()},
			{"MessageContent", message.getText()},
			{"dl", delimiter},
			{"Username", config::username},
			{"Secret", config::secret},
			{"ChannelID", config::channel}};
		try
		{
			std::string encodedMessage = messageFormats.at(message.getType());
			std::regex placeholderRegex(R"(\{(\w+)\})"); // search for {key}

			std::smatch match;
			while (std::regex_search(encodedMessage, match, placeholderRegex))
			{
				std::string key = match[1].str();
				std::string replacement = values.contains(key) ? values.at(key) : "[UNKNOWN]";
				encodedMessage.replace(match.position(0), match.length(0), replacement);
			}
			return encodedMessage;
		}
		catch (const std::out_of_range &e)
		{
			std::cerr << e.what() << std::endl;
		}
		return "";
	}
}