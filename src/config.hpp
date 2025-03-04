#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>
#include <string>

namespace config
{
	extern std::string secret;
	extern std::string hostname;
	extern std::uint16_t port;
	extern std::string username;
	extern std::string displayName;
	extern std::string channel;
	extern std::uint16_t timeout;
	extern std::uint8_t retransmissions;
}

#endif