#ifndef UDP_DECODER_HPP
#define UDP_DECODER_HPP

#include <unordered_map>
#include <string>
#include "decoder.hpp"
#include "../formats/message.hpp"

namespace decoders
{
	static std::unordered_map<formats::MessageType, std::string> udpMessageFormats = {};

	class UDPDecoder : public Decoder
	{

	public:
		formats::Message decode(const std::string &data) override;
	};
}

#endif