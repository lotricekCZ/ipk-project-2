#include "../formats/message.hpp"
#ifndef ENCODER_HPP
#define ENCODER_HPP

class Encoder
{
public:
	virtual formats::Message decode(const std::string &message) = 0;
};

#endif // ENCODER_HPP