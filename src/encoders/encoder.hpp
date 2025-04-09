#include "../formats/message.hpp"
#include <string>
#include <tuple>

#ifndef ENCODER_HPP
#define ENCODER_HPP

class Encoder
{
public:
	virtual std::string encode(formats::Message &message) = 0;
	virtual std::tuple<uint8_t *, uint16_t> encodeBinary(formats::Message &message) = 0;
};

#endif // ENCODER_HPP