#include "../../src/formats/message.hpp"
#include "../../src/encoders/UDP_encoder.hpp"
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
		encoders::UDPEncoder encoder;
		uint8_t *data;
		uint16_t size;
		std::tie(data, size) = encoder.encodeBinary(message);
		for (int i = 0; i < size; i++)
		{
			std::cout << data[i];
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}