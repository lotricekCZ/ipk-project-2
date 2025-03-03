#include "../../src/io_handler/io_handler.hpp"
#include "../../src/formats/message.hpp"
#include "../../src/encoders/TCP_encoder.hpp"

#include <string>
#include <iostream>

std::string printMessage(formats::Message &message)
{
	return message.getAuthor() + "[" + std::to_string(message.getType()) + "]: " + message.getText();
}


int main(int argc, char *argv[])
{
	IOHandler ioHandler;
	encoders::TCPEncoder encoder;
	formats::Message message;
	std::string messageText(argv[1]);
	while (messageText != "/exit")
	{
		message = ioHandler.readMessage(messageText);
		ioHandler.printMessage(message);
		std::cout << printMessage(message) << std::endl;
		std::cout << encoder.encode(message) << std::endl;
		std::getline(std::cin, messageText);
	}
	
}