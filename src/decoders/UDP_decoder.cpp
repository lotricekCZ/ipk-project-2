#include <arpa/inet.h>
#include <iostream>
#include "UDP_decoder.hpp"

namespace decoders
{
	/**
	 * Constructor for UDPDecoder.
	 * Initializes the state machine used for decoding the UDP protocol.
	 */
	UDPDecoder::UDPDecoder()
	{

		/**
		 * Save a string to controls from given character pointer.
		 * @param s string to be saved
		 * @param c character pointer to the start of the string
		 */
		auto saveString = [&](std::string &s, char *c)
		{
			s = std::string(c);
		};

		/**
		 * Simple tautology, always returns true.
		 * @param c character pointer, not used
		 */
		auto tautology = [](char *c)
		{
			(void)c;
			return true;
		};

		/**
		 * Save a message ID from given character pointer.
		 * @param c character pointer to the start of the message ID
		 */
		auto saveID = [&](char *c)
		{
			this->controls.messageID = ntohs(*reinterpret_cast<uint16_t *>(c - 1));
			return true;
		};

		/**
		 * Save a reply ID from given character pointer.
		 * @param c character pointer to the start of the reply ID
		 */
		auto saveReplyID = [&](char *c)
		{
			this->controls.replyID = ntohs(*reinterpret_cast<uint16_t *>(c - 1));
			return true;
		};


		/**
		 * Scan a character pointer for a zero.
		 * @param c character pointer to be checked
		 */
		auto scanZero = [&](char *c)
		{
			return (*c == 0x00);
		};

		/**
		 * Save a content string from given character pointer.
		 * @param c character pointer to the start of the content string
		 */
		auto saveContent = [&](char *c)
		{
			if (isprint(*c)) // TODO: revise condition
			{
				saveString(this->controls.content, c);
				return true;
			}
			return false;
		};

		/**
		 * Scan a character pointer for a valid content string.
		 * @param c character pointer to be checked
		 */
		auto scanContent = [&](char *c)
		{
			return (isprint(*c)); // TODO: revise condition
		};

		/**
		 * Save a secret from given character pointer.
		 * @param c character pointer to the start of the secret
		 */
		auto saveSecret = [&](char *c)
		{
			if (isprint(*c))
			{
				saveString(this->controls.secret, c);
				return true;
			}
			return false;
		};

		/**
		 * Scan a character pointer for a valid secret.
		 * @param c character pointer to be checked
		 */
		auto scanSecret = [&](char *c)
		{
			return (isalnum(*c) || *c == '_' || *c == '-' || *c == '.');
		};

		/**
		 * Save a username from given character pointer.
		 * @param c character pointer to the start of the username
		 */
		auto saveUsername = [&](char *c)
		{
			if (isprint(*c))
			{
				saveString(this->controls.secret, c);
				return true;
			}
			return false;
		};

		/**
		 * Scan a character pointer for a valid username.
		 * @param c character pointer to be checked
		 */
		auto scanUsername = [&](char *c)
		{
			return (isalnum(*c) || *c == '_' || *c == '-' || *c == '.');
		};

		/**
		 * Save a display name from given character pointer.
		 * @param c character pointer to the start of the display name
		 */
		auto saveDispName = [&](char *c)
		{
			if (isprint(*c))
			{
				saveString(this->controls.dispName, c);
				return true;
			}
			return false;
		};

		/**
		 * Scan a character pointer for a valid display name.
		 * @param c character pointer to be checked
		 */
		auto scanDispName = [&](char *c)
		{
			return (isprint(*c));
		};

		/**
		 * Save a channel from given character pointer.
		 * @param c character pointer to the start of the channel
		 */
		auto saveChannel = [&](char *c)
		{
			if (isalnum(*c) || *c == '_' || *c == '-' || *c == '.')
			{
				saveString(this->controls.dispName, c);
				return true;
			}
			return false;
		};

		/**
		 * Scan a character pointer for a valid channel.
		 * @param c character pointer to be checked
		 */
		auto scanChannel = [&](char *c)
		{
			return (isalnum(*c) || *c == '_' || *c == '-' || *c == '.');
		};

		/**
		 * Save a status from given character pointer.
		 * @param c character pointer to the start of the status
		 */
		auto saveStatus = [&](char *c)
		{
			if (*c == 0x00 || *c == 0x01)
			{
				this->controls.status = *c;
				return true;
			}
			return false;
		};

		// assign edges to each node in the state machine
		for (auto state : {
				 INIT,
				 CONFIRM, CNF_ID_1, CNF_ID_2,
				 REPLY, RPL_ID_1, RPL_ID_2, RPL_RES, RPL_REF_ID_1, RPL_REF_ID_2, RPL_CONTENT, RPL_ZERO,
				 MESSAGE, MSG_ID_1, MSG_ID_2, MSG_DISPNAME, MSG_ZERO, MSG_CONTENT, MSG_ZERO_1,
				 PING, PNG_ID_1, PNG_ID_2,
				 ERROR, ERR_ID_1, ERR_ID_2, ERR_DISPNAME, ERR_ZERO, ERR_CONTENT, ERR_ZERO_1,
				 BYE, BYE_ID_1, BYE_ID_2, BYE_DISPNAME, BYE_ZERO,
				 JOIN, JOIN_ID_1, JOIN_ID_2, JOIN_CHANNELID, JOIN_ZERO, JOIN_DISPNAME, JOIN_ZERO_1,
				 AUTH, AUTH_ID_1, AUTH_ID_2, AUTH_USERNAME, AUTH_ZERO, AUTH_DISPNAME, AUTH_ZERO_1, AUTH_SECRET, AUTH_ZERO_2,
				 END})
		{
			NodeStates[state] = std::make_shared<FSMNode>(state);
		}

		// assign edges for INIT message
		{
			NodeStates[INIT]->assignEdges(std::make_shared<FSMEdge>(NodeStates[CONFIRM], [](char *c)
																	{ return (unsigned char)*c == formats::MessageType::CONFIRM; }),
										  std::make_shared<FSMEdge>(NodeStates[REPLY], [](char *c)
																	{ return (unsigned char)*c == formats::MessageType::REPLY; }),
										  std::make_shared<FSMEdge>(NodeStates[MESSAGE], [](char *c)
																	{ return (unsigned char)*c == formats::MessageType::MSG; }),
										  std::make_shared<FSMEdge>(NodeStates[PING], [](char *c)
																	{ return (unsigned char)*c == formats::MessageType::PING; }),
										  std::make_shared<FSMEdge>(NodeStates[ERROR], [](char *c)
																	{ return (unsigned char)*c == formats::MessageType::ERR; }),
										  std::make_shared<FSMEdge>(NodeStates[BYE], [](char *c)
																	{ return (unsigned char)*c == formats::MessageType::BYE; }),
										  std::make_shared<FSMEdge>(NodeStates[JOIN], [](char *c)
																	{ return (unsigned char)*c == formats::MessageType::JOIN; }),
										  std::make_shared<FSMEdge>(NodeStates[AUTH], [](char *c)
																	{ return (unsigned char)*c == formats::MessageType::AUTH; }));
		}
		// assign edges for CONFIRM message
		{
			NodeStates[CONFIRM]->assignEdges(std::make_shared<FSMEdge>(NodeStates[CNF_ID_1], tautology));
			NodeStates[CNF_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[CNF_ID_2], saveID));
		}

		// assign edges for REPLY message
		{
			NodeStates[REPLY]->assignEdges(std::make_shared<FSMEdge>(NodeStates[RPL_ID_1], tautology));
			NodeStates[RPL_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[RPL_ID_2], saveID));
			NodeStates[RPL_ID_2]->assignEdges(std::make_shared<FSMEdge>(NodeStates[RPL_RES], saveStatus));
			NodeStates[RPL_RES]->assignEdges(std::make_shared<FSMEdge>(NodeStates[RPL_REF_ID_1], tautology));
			NodeStates[RPL_REF_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[RPL_REF_ID_2], saveReplyID));
			NodeStates[RPL_REF_ID_2]->assignEdges(std::make_shared<FSMEdge>(NodeStates[RPL_CONTENT], saveContent));
			NodeStates[RPL_CONTENT]->assignEdges(std::make_shared<FSMEdge>(NodeStates[RPL_CONTENT], scanContent),
												 std::make_shared<FSMEdge>(NodeStates[RPL_ZERO], scanZero));
		}

		// assign edges for MESSAGE message
		{
			NodeStates[MESSAGE]->assignEdges(std::make_shared<FSMEdge>(NodeStates[MSG_ID_1], tautology));
			NodeStates[MSG_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[MSG_ID_2], saveID));
			NodeStates[MSG_ID_2]->assignEdges(std::make_shared<FSMEdge>(NodeStates[MSG_DISPNAME], saveDispName));
			NodeStates[MSG_DISPNAME]->assignEdges(std::make_shared<FSMEdge>(NodeStates[MSG_ZERO], scanZero),
												  std::make_shared<FSMEdge>(NodeStates[MSG_DISPNAME], scanDispName));
			NodeStates[MSG_ZERO]->assignEdges(std::make_shared<FSMEdge>(NodeStates[MSG_CONTENT], saveContent));
			NodeStates[MSG_CONTENT]->assignEdges(std::make_shared<FSMEdge>(NodeStates[MSG_CONTENT], scanContent),
												 std::make_shared<FSMEdge>(NodeStates[MSG_ZERO_1], scanZero));
		}

		// assign edges for ERROR message
		{
			NodeStates[ERROR]->assignEdges(std::make_shared<FSMEdge>(NodeStates[ERR_ID_1], tautology));
			NodeStates[ERR_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[ERR_ID_2], saveID));
			NodeStates[ERR_ID_2]->assignEdges(std::make_shared<FSMEdge>(NodeStates[ERR_DISPNAME], saveDispName));
			NodeStates[ERR_DISPNAME]->assignEdges(std::make_shared<FSMEdge>(NodeStates[ERR_ZERO], scanZero),
												  std::make_shared<FSMEdge>(NodeStates[ERR_DISPNAME], scanDispName));
			NodeStates[ERR_ZERO]->assignEdges(std::make_shared<FSMEdge>(NodeStates[ERR_CONTENT], saveContent));
			NodeStates[ERR_CONTENT]->assignEdges(std::make_shared<FSMEdge>(NodeStates[ERR_CONTENT], scanContent),
												 std::make_shared<FSMEdge>(NodeStates[ERR_ZERO_1], scanZero));
		}

		// assign edges for BYE message
		{
			NodeStates[BYE]->assignEdges(std::make_shared<FSMEdge>(NodeStates[BYE_ID_1], tautology));
			NodeStates[BYE_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[BYE_ID_2], saveID));
			NodeStates[BYE_ID_2]->assignEdges(std::make_shared<FSMEdge>(NodeStates[BYE_DISPNAME], saveDispName));
			NodeStates[BYE_DISPNAME]->assignEdges(std::make_shared<FSMEdge>(NodeStates[BYE_ZERO], scanZero),
												  std::make_shared<FSMEdge>(NodeStates[BYE_DISPNAME], scanDispName));
		}

		// assign edges for JOIN message
		{
			NodeStates[JOIN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN_ID_1], tautology));
			NodeStates[JOIN_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN_ID_2], saveID));
			NodeStates[JOIN_ID_2]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN_CHANNELID], saveChannel));
			NodeStates[JOIN_CHANNELID]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN_ZERO], scanZero),
													std::make_shared<FSMEdge>(NodeStates[JOIN_CHANNELID], scanChannel));
			NodeStates[JOIN_ZERO]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN_DISPNAME], saveDispName));
			NodeStates[JOIN_DISPNAME]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN_DISPNAME], scanDispName),
												   std::make_shared<FSMEdge>(NodeStates[JOIN_ZERO_1], scanZero));
		}

		// assign edges for AUTH message
		{
			NodeStates[AUTH]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH_ID_1], tautology));
			NodeStates[AUTH_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH_ID_2], saveID));
			NodeStates[AUTH_ID_2]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH_USERNAME], saveUsername));
			NodeStates[AUTH_USERNAME]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH_ZERO], scanZero),
												   std::make_shared<FSMEdge>(NodeStates[AUTH_USERNAME], scanUsername));
			NodeStates[AUTH_ZERO]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH_DISPNAME], saveDispName));
			NodeStates[AUTH_DISPNAME]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH_DISPNAME], scanDispName),
												   std::make_shared<FSMEdge>(NodeStates[AUTH_ZERO_1], scanZero));
			NodeStates[AUTH_ZERO_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH_SECRET], saveSecret));
			NodeStates[AUTH_SECRET]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH_SECRET], scanSecret),
												 std::make_shared<FSMEdge>(NodeStates[AUTH_ZERO_2], scanZero));
		}

		// assign edges for PING message
		{
			NodeStates[PING]->assignEdges(std::make_shared<FSMEdge>(NodeStates[PNG_ID_1], tautology));
			NodeStates[PNG_ID_1]->assignEdges(std::make_shared<FSMEdge>(NodeStates[PNG_ID_2], saveID));
		}
	}

	/**
	 * Decodes a UDP message and returns a formats::Message object.
	 *
	 * The function takes a pointer to a byte array and the size of the array as
	 * arguments. It runs the UDPDecoder finite state machine on the input data,
	 * and returns a formats::Message object that represents the decoded message.
	 *
	 * The function will stop running the finite state machine as soon as it
	 * encounters a state that does not have a transition, or if the end of the
	 * input data is reached.
	 *
	 * If the function encounters a state that does not have a transition, it will
	 * set the output state to the current state and stop running the finite state
	 * machine.
	 *
	 * @param data   The input data to decode.
	 * @param size   The size of the input data in bytes.
	 * @return       A formats::Message object that represents the decoded message.
	 */
	formats::Message UDPDecoder::decode(uint8_t *data, uint16_t size)
	{
		states curr = INIT;
		output = END;
		for (int i = 0; i < size; i++)
		{
			// Run the finite state machine
			std::shared_ptr<FSMNode> next = NodeStates[curr]->next((char *)&data[i]);
			if (next != nullptr)
				curr = next->state;
			else
			{
				output = curr;
				curr = END;
			}
			// Check if we should stop
			if (curr == END)
				break;
		}
		if (output == END)
			output = curr;

		return translate();
	}

	/**
	 * Translates the current state of the UDPDecoder finite state machine into a
	 * formats::Message object based on the output state. The function looks up the
	 * message type in the translationMap using the current output state and
	 * constructs a Message object accordingly. Each message type has specific fields
	 * set in the Message object, such as the author, text, ID, or status. If the
	 * output state does not correspond to a valid message type, a runtime error is
	 * thrown. After constructing the message, it resets the controls and sets the
	 * output state to END.
	 *
	 * @return A formats::Message object representing the decoded message.
	 * @throws std::runtime_error if the finite automata is in a non-terminal state.
	 */

	formats::Message UDPDecoder::translate()
	{
		formats::Message message;
		try
		{
			if (translationMap.find(output) == translationMap.end())
				throw std::runtime_error("finite automata in non-terminal state");
			formats::MessageType type = translationMap.at(output);
			message.setID(controls.messageID);
			message.setType(type);
			switch (type)
			{
			case formats::REPLY:
				message.setStatus(controls.status == 1);
				message.setRefID(controls.replyID);
				message.setText(controls.content);
				break;
			case formats::MSG:
				message.setAuthor(controls.dispName);
				message.setText(controls.content);
				break;
			case formats::ERR:
				message.setAuthor(controls.dispName);
				message.setText(controls.content);
				break;
			case formats::BYE:
				message.setAuthor(controls.dispName);
				break;
			case formats::JOIN:
				message.setText(controls.channelID);
				message.setAuthor(controls.dispName);
				break;
			case formats::AUTH:
				message.setAuthor(controls.username);
				message.setText(controls.secret);
				break;
			default:
				break;
			}
			controls = {};
			output = END;
		}
		catch (std::runtime_error &e)
		{
			message.setType(formats::ERR_INTERNAL);
			message.setText("Malformed message incoming from server!");
		}
		return message;
	}

	UDPDecoder::~UDPDecoder() {
		NodeStates.clear();
	}
}