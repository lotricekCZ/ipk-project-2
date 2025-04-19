/**
 * @file io_handler.cpp
 * @brief Input/Output handler for the client
 */

#include <regex>

#include "io_handler.hpp"
#include "../config.hpp"

/**
 * @brief Prints a message to the console
 * @param message The message to be printed
 */
void IOHandler::printMessage(formats::Message &message)
{
	switch (message.getType())
	{
	case formats::MSG:
		std::cout << message.getAuthor() << ": " << message.getText() << std::endl;
		break;
	case formats::ERR:
		std::cout << "ERROR FROM " << message.getAuthor() << ": " << message.getText() << std::endl;
		break;
	case formats::REPLY:
		std::cout << "Action " << (message.getStatus() ? "Success" : "Failure") << ": " << message.getText() << std::endl;
		break;
	case formats::ERR_INTERNAL:
		std::cout << "ERROR: " << message.getText() << std::endl;
	default:
		break;
	}
}

/**
 * @brief Parses a user input command and converts it into a Message object.
 * 
 * This function takes a string input from the user, identifies if it matches
 * any predefined command patterns, and constructs a corresponding Message 
 * object. If the command is not recognized, it defaults to a exception, which is then caught in fsm.
 *
 * @param data The user-provided input string.
 * @return A Message object constructed from the parsed command.
 */
formats::Message IOHandler::readMessage(std::string &data)
{
	formats::Message message;
	std::regex r("^\\/(join|auth|rename|help)\\s*(.*)");
	std::smatch m;
	// Available commands:
	// /auth USERNAME SECRET DISPLAYNAME
	// /join CHANNEL_ID
	// /rename DISPLAYNAME
	// /help
	std::string helpMessage = "Available commands:\n\t/auth USERNAME SECRET DISPLAYNAME\tSends AUTH message with the data provided from the command to the server (and correctly handles the Reply message), locally sets the DisplayName value (same as the /rename command)\n\t/join CHANNEL_ID\t\t\t\t\tSends JOIN message with channel name from the command to the server (and correctly handles the Reply message)\n\t/rename DISPLAYNAME\t\t\t\tLocally changes the display name of the user to be sent with new messages/selected commands\n\t/help\t\t\t\t\t\t\t\tDisplay this help message and exit.";
	auto count = [](std::smatch m) -> int
	{
		// Count the number of matches
		int i = 0;
		for (auto o : m)
			if (o.length() != 0)
				i++;
		return i - 1;
	};
	if (std::regex_match(data, m, r))
	{
		std::string args(m[2].str());
		// Check if the command is valid
		if (m[1].str() == "join")
		{
			std::regex r2("\\s*(\\S+)\\s*");
			std::smatch m2;
			// Check if the command has the correct number of arguments
			if (std::regex_match(args, m2, r2))
			{
				if (count(m2) != 1)
					throw std::runtime_error("Not enough parameters given!");
				// Set the channel name

				config::channel = m2[1].str();
				message.setType(formats::JOIN);
				message.setAuthor(config::displayName);
				return message;
			} else {
				throw std::runtime_error("Wrong number of parameters given!");
			}
			return message;
		}
		else if (m[1].str() == "auth")
		{
			std::regex r2("\\s*(\\S+)\\s+(\\S+)\\s+(\\S+)\\s*");
			std::smatch m2;
			// Check if the command has the correct number of arguments
			if (std::regex_match(args, m2, r2))
			{
				if (count(m2) != 3)
					throw std::runtime_error("Not enough parameters given!");
				// Set the username, secret and display name
				config::username = m2[1].str();
				config::secret = m2[2].str();
				config::displayName = m2[3].str();
				message.setType(formats::AUTH);
				message.setAuthor(config::displayName);
				return message;
			} else {
				throw std::runtime_error("Wrong number of parameters given!");
			}
			return message;
		}
		else if (m[1].str() == "rename")
		{
			std::regex r2("\\s*(\\S+)\\s*");
			std::smatch m2;
			// Check if the command has the correct number of arguments
			if (std::regex_match(args, m2, r2))
			{
				if (count(m2) != 1)
					throw std::runtime_error("Not enough parameters given!");
				// Set the display name
				config::displayName = m2[1].str();
			} else {
				throw std::runtime_error("Wrong number of parameters given!");
			}
			return message;
		}
		else if (m[1].str() == "help")
		{
			std::cout << helpMessage << std::endl;
			return message;
		}
		else 
		{
			throw std::runtime_error("Invalid command!");
		}
	}
	// If the command is invalid, set the message as a regular message
	message.setAuthor(config::displayName);
	message.setType(formats::MSG);
	message.setText(data);
	return message;
}
