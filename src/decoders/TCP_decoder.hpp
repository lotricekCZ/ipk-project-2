#ifndef TCP_DECODER_HPP
#define TCP_DECODER_HPP

#include <unordered_map>
#include <string>
#include <stdexcept>
#include "decoder.hpp"
#include "../formats/message.hpp"

namespace decoders
{
	static std::unordered_map<formats::MessageType, std::string> tcpMessageFormats = {
		{formats::ERR, "[E|e][R|r][R|r] [F|f][R|r][O|o][M|m] (.+) [I|i][S|s] (.+)\r\n.*"},
		{formats::REPLY, "[R|r][E|e][P|p][L|l][Y|y] ([O|o][K|k]|[N|n][O|o][K|k]) [I|i][S|s] (.+)\r\n.*"},
		{formats::AUTH, "[A|a][U|u][T|t][H|h] (.+) [A|a][S|s] (.+) [U|u][S|s][I|i][N|n][G|g] (.+)\r\n.*"},
		{formats::JOIN, "[J|j][O|o][I|i][N|n] (.+) [A|a][S|s] (.+)\r\n.*"},
		{formats::MSG, "[M|m][S|s][G|g] [F|f][R|r][O|o][M|m] (.+) [I|i][S|s] (.+)\r\n.*"},
		{formats::BYE, "[B|b][Y|y][E|e] [F|f][R|r][O|o][M|m] (.+)\r\n.*"}};

	class TCPDecoder : public Decoder
	{

	public:
		formats::Message decode(const std::string &data) override;
		formats::Message decode(uint8_t *data, uint16_t size) override {
			(void) data;
			(void) size;
			throw std::runtime_error("Not implemented");
			return formats::Message();
		};
	};
}

#endif