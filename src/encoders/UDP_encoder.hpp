#ifndef UDPENCODER_HPP
#define UDPENCODER_HPP

#include "encoder.hpp"
#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <cstring>

namespace encoders
{
	class UDPEncoder : public Encoder
	{
		const std::string delimiter = "\r\n";

	public:
		std::string encode(formats::Message &message) override {
			std::runtime_error("Not implemented");
		};
		std::tuple<uint8_t *, uint16_t> encodeBinary(formats::Message &message) override;
	};
}

#endif