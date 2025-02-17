#include "../../src/formats/message.hpp"
#include <iostream>
#include <string>

std::string printMessage(formats::Message &message)
{
	return message.getAuthor() + "[" + std::to_string(message.getType()) + "]: " + message.getText();
}

int main(int argc, char *argv[])
{
	formats::Message message;
	try
	{
		message.setText(std::string(argv[2]));
		message.setAuthor(std::string(argv[1]));
		message.setType(formats::MSG);
		std::cout << printMessage(message) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}