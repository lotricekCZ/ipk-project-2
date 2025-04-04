/**
 * @file TCP_decoder.cpp
 * @brief Implementation of TCPDecoder class
 *
 * This file contains the implementation of the TCPDecoder class, which is used
 * to decode TCP messages.
 */

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
	/**
	 * @brief Decodes a TCP message
	 * @param data The message to be decoded
	 * @return A Message object containing the decoded data
	 */
	formats::Message TCPDecoder::decode(const std::string &data)
	{
		formats::Message message;

		// Define extractors for each MessageType
		std::unordered_map<formats::MessageType, std::function<void(std::string)>> extractors = {
			{formats::ERR, [&](std::string data) -> void
			 {
				 std::regex r(tcpMessageFormats.at(formats::ERR));
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
				 std::regex r(tcpMessageFormats.at(formats::REPLY));
				 std::smatch m;
				 if (std::regex_match(data, m, r))
				 {
					 message.setStatus(m[0].str() == "OK");
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
				 std::regex r(tcpMessageFormats.at(formats::AUTH));
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
				 std::regex r(tcpMessageFormats.at(formats::JOIN));
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
				 std::regex r(tcpMessageFormats.at(formats::MSG));
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
				 std::regex r(tcpMessageFormats.at(formats::BYE));
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

		// Iterate over message formats to find a match and extract data
		for (auto [key, value] : tcpMessageFormats)
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
