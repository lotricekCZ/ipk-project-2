#include "../../src/formats/message.hpp"
#include "../../src/decoders/UDP_decoder.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <cstring>

std::string printMessage(formats::Message &message)
{
	return message.getAuthor() + "[" + std::to_string(message.getType()) + " " + std::to_string(message.getID()) + "]: " + message.getText();
}

int main(int argc, char *argv[])
{
	decoders::UDPDecoder decoder;
	formats::Message message;
	uint8_t *data = static_cast<uint8_t*>(malloc(std::atoi(argv[1])));
	data[0] = std::atoi(argv[2]);
	memcpy(data + 1, argv[3], std::atoi(argv[1]) - 1);
	try
	{
		message = decoder.decode(data, (uint16_t)std::atoi(argv[1]));
		std::cout << printMessage(message) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}