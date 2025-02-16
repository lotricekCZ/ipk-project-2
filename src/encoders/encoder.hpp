#include "../formats/message.hpp"
#include <string>

#ifndef ENCODER_HPP
#define ENCODER_HPP

class Encoder
{
public:
	virtual std::string encode(formats::Message &message) = 0;
};

#endif // ENCODER_HPP