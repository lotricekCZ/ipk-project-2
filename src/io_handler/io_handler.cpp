#include <regex>
#include <argumentum/argparse-h.h>

#include "io_handler.hpp"
#include "../config.hpp"

void IOHandler::printMessage(formats::Message &message)
{
	switch (message.getType())
	{
	case formats::MSG:
		std::cout << message.getAuthor() << ": " << message.getText() << std::endl;
		break;
	case formats::ERR:
		std::cout << "ERROR FROM" << message.getAuthor() << ": " << message.getText() << std::endl;
		break;
	case formats::REPLY:
		std::cout << "Action " << (message.getStatus() ? "Success" : "Failure") << ": " << message.getText() << std::endl;
		break;
	default:
		break;
	}
}

formats::Message IOHandler::readMessage(std::string &data)
{
	formats::Message message;
	std::regex r("^\\/(join|auth|rename|help)\\s*(.*)");
	std::smatch m;
	std::string helpMessage = "Available commands:\n\t/auth USERNAME SECRET DISPLAYNAME\tSends AUTH message with the data provided from the command to the server (and correctly handles the Reply message), locally sets the DisplayName value (same as the /rename command)\n\t/join CHANNEL_ID\t\t\t\t\tSends JOIN message with channel name from the command to the server (and correctly handles the Reply message)\n\t/rename DISPLAYNAME\t\t\t\tLocally changes the display name of the user to be sent with new messages/selected commands\n\t/help\t\t\t\t\t\t\t\tDisplay this help message and exit.";
	auto count = [](std::smatch m) -> int
	{
		int i = 0;
		for (auto o : m)
			if (o.length() != 0)
				i++;
		return i - 1;
	};
	if (std::regex_match(data, m, r))
	{
		std::string args(m[2].str());
		if (m[1].str() == "join")
		{
			std::regex r2("\\s*(\\S+)\\s*");
			std::smatch m2;
			if (std::regex_match(args, m2, r2))
			{
				if (count(m2) != 1)
					throw std::runtime_error("Not enough parameters given!");
				config::channel = m2[1].str();
				message.setType(formats::JOIN);
				message.setAuthor(config::displayName);
				return message;
			}
			return message;
		}
		else if (m[1].str() == "auth")
		{
			std::regex r2("\\s*(\\S+)\\s+(\\S+)\\s+(\\S+)\\s*");
			std::smatch m2;
			if (std::regex_match(args, m2, r2))
			{
				message.setType(formats::AUTH);
				if (count(m2) != 3)
					throw std::runtime_error("Not enough parameters given!");
				config::username = m2[1].str();
				config::secret = m2[2].str();
				config::displayName = m2[3].str();
				message.setAuthor(config::displayName);
				message.setType(formats::AUTH);
				return message;
			}
			return message;
		}
		else if (m[1].str() == "rename")
		{
			std::regex r2("\\s*(\\S+)\\s*");
			std::smatch m2;
			if (std::regex_match(args, m2, r2))
			{
				if (count(m2) != 1)
					throw std::runtime_error("Not enough parameters given!");
				config::displayName = m2[1].str();
			}
			return message;
		}
		else if (m[1].str() == "help")
		{
			std::cout << helpMessage << std::endl;
			return message;
		}
	}
	message.setAuthor(config::displayName);
	message.setType(formats::MSG);
	message.setText(data);
	return message;
}