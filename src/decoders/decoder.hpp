#include "../formats/message.hpp"

#ifndef DECODER_HPP
#define DECODER_HPP

namespace decoders
{
	class Decoder
	{
	public:
		virtual formats::Message decode(const std::string &data) = 0;
	};
}
#endif