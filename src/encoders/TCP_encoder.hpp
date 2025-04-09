#ifndef TCPENCODER_HPP
#define TCPENCODER_HPP

#include "encoder.hpp"
#include <string>
#include <stdexcept>
#include <map>

namespace encoders
{

	static std::map<formats::MessageType, std::string> tcpMessageFormats = {
		{formats::ERR, "ERR FROM {DisplayName} IS {MessageContent}{dl}"},
		{formats::REPLY, "REPLY {Reply} IS {MessageContent}{dl}"},
		{formats::AUTH, "AUTH {Username} AS {DisplayName} USING {Secret}{dl}"},
		{formats::JOIN, "JOIN {ChannelID} AS {DisplayName}{dl}"},
		{formats::MSG, "MSG FROM {DisplayName} IS {MessageContent}{dl}"},
		{formats::BYE, "BYE FROM {DisplayName}{dl}"}};

	class TCPEncoder : public Encoder
	{
		const std::string delimiter = "\r\n";

	public:
		std::string encode(formats::Message &message) override;
		std::tuple<uint8_t *, uint16_t> encodeBinary(formats::Message &message) override
		{
			throw std::runtime_error("Not implemented");
		};
	};
}

#endif