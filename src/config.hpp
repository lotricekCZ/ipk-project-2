#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>
#include <string>

namespace config
{
	extern const std::string secret;
	extern const std::string hostname;
	extern std::uint16_t port;
	extern std::string username;
}

#endif