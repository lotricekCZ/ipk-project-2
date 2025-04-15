#include <iostream>
#include <string>
#include <map>
#include "../formats/message.hpp"

#ifndef IO_HANDLER_HPP
#define IO_HANDLER_HPP

class IOHandler
{
	std::map<formats::MessageType, std::string> printFormats;
public:

	void printMessage(formats::Message &message);
	formats::Message readMessage(std::string &data);
};

#endif