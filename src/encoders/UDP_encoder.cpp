#include "UDP_encoder.hpp"
#include <string>
#include <regex>
#include <iostream>
#include <variant>
#include <unordered_map>
#include <arpa/inet.h>
#include <functional>

#include "../formats/message.hpp"
#include "../config.hpp"

namespace encoders
{
	uint16_t UDPEncoder::messageID = 0;

	UDPEncoder::UDPEncoder() : Encoder()
	{
		auto idStart = [](bytes::iterator it)
		{
			return isalpha((char)*it) || (char)*it == '_';
		};
		auto idContinue = [](bytes::iterator it)
		{
			return isalnum((char)*it) || (char)*it == '_' || (char)*it == '-';
		};

		auto subEnd = [](bytes::iterator it)
		{
			return (char)*it == '}';
		};

		auto subColon = [](bytes::iterator it)
		{
			return (char)*it == ':';
		};

		auto isHex = [](bytes::iterator it)
		{
			return isxdigit((char)*it);
		};
		// assign edges to each node in the state machine
		for (auto state : {
				 INIT,
				 ORDINARY,
				 SUB_START, SUB_TYPE,
				 SUB_BYTE, SUB_B_COL, SUB_B_ID, SUB_B_EXEC,
				 SUB_SHORT, SUB_S_COL, SUB_S_ID, SUB_S_EXEC,
				 SUB_INT, SUB_I_COL, SUB_I_ID, SUB_I_EXEC,
				 SUB_STRING, SUB_STR_COL, SUB_STR_ID, SUB_STR_EXEC,
				 SUB_UNDEFINED, SUB_U_EXEC,
				 SPEC_START, BACKSLASH, HEX, HEX_NIBBLE, HEX_BYTE})
		{
			NodeStates[state] = std::make_shared<FSMNode>(state);
		}

		// assign edges to each node in the state machine
		NodeStates[INIT]->assignEdges(std::make_shared<FSMEdge>(NodeStates[ORDINARY], [](bytes::iterator it)
																{ return (char)*it != '\\' && (char)*it != '{' && (char)*it != '}'; }),
									  std::make_shared<FSMEdge>(NodeStates[SUB_START], [](bytes::iterator it)
																{ return (char)*it == '{'; }),
									  std::make_shared<FSMEdge>(NodeStates[SPEC_START], [](bytes::iterator it)
																{ return (char)*it == '\\'; }));

		NodeStates[SUB_START]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_TYPE], [](bytes::iterator it)
																	 { return (char)*it == '%'; }),
										   std::make_shared<FSMEdge>(NodeStates[SUB_UNDEFINED], idStart));

		NodeStates[SUB_TYPE]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_BYTE], [](bytes::iterator it)
																	{ return (char)*it == 'b'; }),
										  std::make_shared<FSMEdge>(NodeStates[SUB_SHORT], [](bytes::iterator it)
																	{ return (char)*it == 'h'; }),
										  std::make_shared<FSMEdge>(NodeStates[SUB_INT], [](bytes::iterator it)
																	{ return (char)*it == 'i'; }),
										  std::make_shared<FSMEdge>(NodeStates[SUB_STRING], [](bytes::iterator it)
																	{ return (char)*it == 's'; }));
		NodeStates[SUB_UNDEFINED]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_UNDEFINED], idContinue),
											   std::make_shared<FSMEdge>(NodeStates[SUB_U_EXEC], subEnd));

		NodeStates[SUB_BYTE]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_B_COL], subColon));
		NodeStates[SUB_B_COL]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_B_ID], idStart));
		NodeStates[SUB_B_ID]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_B_EXEC], subEnd),
										  std::make_shared<FSMEdge>(NodeStates[SUB_B_ID], idContinue));

		NodeStates[SUB_SHORT]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_S_COL], subColon));
		NodeStates[SUB_S_COL]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_S_ID], idStart));
		NodeStates[SUB_S_ID]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_S_EXEC], subEnd),
										  std::make_shared<FSMEdge>(NodeStates[SUB_S_ID], idContinue));

		NodeStates[SUB_INT]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_I_COL], subColon));
		NodeStates[SUB_I_COL]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_I_ID], idStart));
		NodeStates[SUB_I_ID]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_I_EXEC], subEnd),
										  std::make_shared<FSMEdge>(NodeStates[SUB_I_ID], idContinue));

		NodeStates[SUB_STRING]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_STR_COL], subColon));
		NodeStates[SUB_STR_COL]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_STR_ID], idStart));
		NodeStates[SUB_STR_ID]->assignEdges(std::make_shared<FSMEdge>(NodeStates[SUB_STR_EXEC], subEnd),
											std::make_shared<FSMEdge>(NodeStates[SUB_STR_ID], idContinue));

		NodeStates[SPEC_START]->assignEdges(std::make_shared<FSMEdge>(NodeStates[HEX], [](bytes::iterator it)
																	  { return (char)*it == 'x'; }),
											std::make_shared<FSMEdge>(NodeStates[BACKSLASH], [](bytes::iterator it)
																	  { return (char)*it != '\\'; }));

		NodeStates[HEX]->assignEdges(std::make_shared<FSMEdge>(NodeStates[HEX_NIBBLE], isHex));
		NodeStates[HEX_NIBBLE]->assignEdges(std::make_shared<FSMEdge>(NodeStates[HEX_BYTE], isHex));
	}

	UDPEncoder::~UDPEncoder()
	{
		for (auto it = NodeStates.begin(); it != NodeStates.end();)
		{
			it->second.reset();
			it = NodeStates.erase(it);
		}
		NodeStates.clear();
	}

	/**
	 * @brief Encodes the given message as a binary UDP message.
	 * @details This function replaces placeholders in the message format string with actual values
	 * from the message and configuration. The message type is determined by the message's type.
	 * @param message The message to encode.
	 * @return A tuple containing a pointer to the encoded message data and the size of the data.
	 * If an error occurs, the function returns a tuple with nullptr as the first element and 0 as the second element.
	 */
	std::tuple<uint8_t *, uint16_t> UDPEncoder::encodeBinary(formats::Message &message)
	{
		// replace placeholders in the message format with actual values
		// from the message and configuration
		using iterator = bytes::iterator;
		std::unordered_map<std::string, std::variant<std::string, int>> values = {
			{"DisplayName", message.getAuthor()},
			{"MessageContent", message.getText()},
			{"Username", config::username},
			{"Secret", config::secret},
			{"Type", message.getType()},
			{"ChannelID", config::channel},
			{"messageID", htons((message.getID() != 65535) ? message.getID() : messageID++)},
			{"rMessageID", (message.getRefID())}};
		std::unordered_map<states, std::function<bytes(iterator, iterator)>> translationMap = {
			{ORDINARY, [&](iterator it, iterator end)
			 { return bytes(it, end); }},
			{BACKSLASH, [&](iterator it, iterator end)
			 { (void) it; (void) end; return bytes('\\'); }},
			{HEX_NIBBLE, [&](iterator it, iterator end)
			 {
				 uint8_t value = static_cast<uint8_t>(std::stoi(std::string(it + 2, end), nullptr, 16));
				 return bytes({value});
			 }},
			{HEX_BYTE, [&](iterator it, iterator end)
			 {
				 uint8_t value = static_cast<uint8_t>(std::stoi(std::string(it + 2, end), nullptr, 16));
				 return bytes({value});
			 }},
			{SUB_B_EXEC, [&](iterator it, iterator end)
			 {
				std::string index(it+4, end-1);
				uint8_t value = static_cast<uint8_t>(std::get<int>(values.at(std::string(it+4, end-1))) & 0xff);
				return bytes({value}); }},
			{SUB_S_EXEC, [&](iterator it, iterator end)
			 {
				 uint16_t value = std::get<int>(values.at(std::string(it + 4, end - 1)));
				 return bytes({static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value & 0xff)});
			 }},
			{SUB_I_EXEC, [&](iterator it, iterator end)
			 {
				 int value = std::get<int>(values.at(std::string(it + 4, end - 1)));
				 return bytes((char *)&value, (char *)&value + sizeof(int));
			 }},
			{SUB_STR_EXEC, [&](iterator it, iterator end)
			 {
				std::string value = std::get<std::string>(values.at(std::string(it+4, end-1)));
				return bytes(value.begin(), value.end()); }},
		};
		try
		{
			// get the message format string for the given message type
			std::tuple<uint8_t *, uint16_t> encodedMessage{nullptr, 0};
			bytes format = udpMessageFormats.at(message.getType());
			// loop until all placeholders have been replaced
			for (iterator it = format.begin(); it != format.end();)
			{
				auto state = NodeStates[INIT];
				iterator scan = it;
				do
				{
					auto next = state->next(scan);
					if (next == nullptr)
						break;
					state = next;
					scan++;
				} while (scan != format.end());
				// process the captured group
				try
				{
					bytes result = translationMap.at(state->state)(it, scan);
					// replace the captured group with the processed result and shrink the format string
					format.erase(it, scan);
					it = format.insert(it, result.begin(), result.end());
					it += result.size();
				}
				catch (const std::out_of_range &e)
				{
					std::cerr << e.what() << " in " << __FILE__ << ":" << __LINE__ << std::endl;
					return {nullptr, 0};
				}
			}
			uint8_t *encodedData = new uint8_t[format.size()];
			std::copy(format.begin(), format.end(), encodedData);
			return {encodedData, format.size()};
		}
		catch (const std::out_of_range &e)
		{
			// handle the case where the message type is unknown

			std::cerr << e.what() << " in " << __FILE__ << ":" << __LINE__ << std::endl;
		}
		return {nullptr, 0};
	}
}
