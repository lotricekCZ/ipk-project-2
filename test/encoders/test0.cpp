#include "../../src/formats/message.hpp"
#include "../../src/encoders/TCP_encoder.hpp"
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
		message.setText(std::string(argv[3]));
		message.setAuthor(std::string(argv[2]));
		message.setType((formats::MessageType)std::stoi(argv[1]));
		encoders::TCPEncoder encoder;
		std::cout << encoder.encode(message) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}