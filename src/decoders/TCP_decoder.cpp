#include <unordered_map>
#include <functional>
#include <utility>
#include <string>
#include <regex>
#include <exception>
#include "TCP_decoder.hpp"
#include "../formats/message.hpp"

namespace decoders
{
	formats::Message TCPDecoder::decode(const std::string &data)
	{
		formats::Message message;

		std::unordered_map<formats::MessageType, std::function<void(std::string)>> extractors = {
			{formats::ERR, [&](std::string data) -> void
			 {
				 std::regex r(messageFormats.at(formats::ERR));
				 std::smatch m;
				 if (std::regex_match(data, m, r))
				 {
					 message.setAuthor(m[0].str());
					 message.setText(m[1].str());
					 message.setType(formats::ERR);
				 }
				 else
				 {
					 throw std::runtime_error("Not enough parameters given!");
				 }
			 }},
			{formats::REPLY, [&](std::string data) -> void
			 {
				 std::regex r(messageFormats.at(formats::REPLY));
				 std::smatch m;
				 if (std::regex_match(data, m, r))
				 {
					 message.setStatus(m[0].str() == "OK" ? true : false);
					 message.setType(formats::REPLY);
					 message.setText(m[1].str());
				 }
				 else
				 {
					 throw std::runtime_error("Not enough parameters given!");
				 }
			 }},
			{formats::AUTH, [&](std::string data) -> void
			 {
				 std::regex r(messageFormats.at(formats::AUTH));
				 std::smatch m;
				 if (std::regex_match(data, m, r))
				 {
					 message.setAuthor(m[1].str());
					 message.setType(formats::AUTH);
				 }
				 else
				 {
					 throw std::runtime_error("Not enough parameters given!");
				 }
			 }},
			{formats::JOIN, [&](std::string data) -> void
			 {
				 std::regex r(messageFormats.at(formats::JOIN));
				 std::smatch m;
				 if (std::regex_match(data, m, r))
				 {
					 message.setAuthor(m[1].str());
					 message.setType(formats::JOIN);
				 }
				 else
				 {
					 throw std::runtime_error("Not enough parameters given!");
				 }
			 }},
			{formats::MSG, [&](std::string data) -> void
			 {
				 std::regex r(messageFormats.at(formats::MSG));
				 std::smatch m;
				 if (std::regex_match(data, m, r))
				 {
					 message.setAuthor(m[0].str());
					 message.setText(m[1].str());
					 message.setType(formats::MSG);
				 }
				 else
				 {
					 throw std::runtime_error("Not enough parameters given!");
				 }
			 }},
			{formats::BYE, [&](std::string data) -> void
			 {
				 std::regex r(messageFormats.at(formats::BYE));
				 std::smatch m;
				 if (std::regex_match(data, m, r))
				 {
					 message.setAuthor(m[0].str());
					 message.setType(formats::BYE);
				 }
				 else
				 {
					 throw std::runtime_error("Not enough parameters given!");
				 }
			 }}};

		for (auto [key, value] : messageFormats)
		{
			std::regex regex(value);
			if (std::regex_match(data, regex))
			{
				extractors.at(key)(data);
				break;
			}
		}
		return message;
	}
}