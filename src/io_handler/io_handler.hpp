#include <iostream>
#include <argumentum/argparse-h.h>
#include <string>
#include <map>
#include "../formats/message.hpp"

#ifndef IO_HANDLER_HPP
#define IO_HANDLER_HPP

class IOHandler
{
	std::map<formats::MessageType, std::string> printFormats;
	argumentum::argument_parser parser{};
public:

	void printMessage(formats::Message &message);
	formats::Message readMessage(std::string &data);
};

#endif