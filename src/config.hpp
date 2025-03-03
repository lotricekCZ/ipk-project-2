#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>
#include <string>

namespace config
{
	extern const std::string secret;
	extern std::string hostname;
	extern std::uint16_t port;
	extern std::string username;
	extern std::string displayName;
	extern std::uint16_t timeout;
	extern std::uint8_t retransmissions;
}

#endif