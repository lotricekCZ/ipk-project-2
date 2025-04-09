#include "../../src/formats/message.hpp"
#include "../../src/decoders/TCP_decoder.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <cstring>

std::string printMessage(formats::Message &message)
{
	return message.getAuthor() + "[" + std::to_string(message.getType()) + "]: " + message.getText();
}

int main(int argc, char *argv[])
{
	decoders::TCPDecoder decoder;
	formats::Message message;
	try
	{
		std::string data(argv[1]);
		message = decoder.decode(data + "\r\n");
		std::cout << printMessage(message) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}