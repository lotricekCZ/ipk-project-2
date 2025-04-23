#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <cstdint>

namespace formats
{
	/**
	 * @brief Enum representing different types of messages.
	 */
	enum MessageType : unsigned char
	{
		NONE = 0x2A,		 /**< No message, internal use only */
		CONFIRM = 0x00,		 /**< Confirmation message */
		REPLY = 0x01,		 /**< Reply message */
		AUTH = 0x02,		 /**< Authentication message */
		JOIN = 0x03,		 /**< Join message */
		MSG = 0x04,			 /**< General message */
		ERR_INTERNAL = 0x45, /**< internal error, parsing failed */
		ERR = 0xFE,			 /**< Error message */
		BYE = 0xFF,			 /**< Goodbye message */
		PING = 0xFD			 /**< Ping message */
	};
	/**
	 * A message containing a text and an author
	 */
	class Message
	{
		MessageType type;
		std::string text;
		std::string author;
		bool isOk = false;
		uint16_t ID = 0;
		uint16_t refID = 0;
	public:
		/**
		 * Construct a new message with the given text and author
		 *
		 * @param text the text of the message
		 * @param author the author of the message
		 */
		Message(const std::string &text, const std::string &author) : text(text), author(author), ID(65535) {}

		/**
		 * Construct a new message with the given type and author
		 *
		 * @param type the type of the message
		 * @param author the author of the message
		 */
		Message(MessageType type = MessageType::NONE, const std::string &author = "") : type(type), author(author), ID(65535) {}

		/**
		 * Get the text of the message
		 *
		 * @return the text of the message
		 */
		std::string &getText();

		/**
		 * Get the author of the message
		 *
		 * @return the author of the message
		 */
		std::string &getAuthor();

		/**
		 * Get the type of the message
		 *
		 * @return the type of the message
		 */
		MessageType getType();

		/**
		 * Set the text of the message
		 *
		 * @param text the new text of the message
		 */
		void setText(const std::string &text);

		/**
		 * Set the author of the message
		 *
		 * @param author the new author of the message
		 */
		void setAuthor(const std::string &author);

		/**
		 * Set the type of the message
		 *
		 * @param type the new type of the message
		 */
		void setType(const MessageType &type);

		/**
		 * Get the status of the message
		 *
		 * @return the status of the message
		 */
		bool getStatus();

		/**
		 * Set the status of the message
		 *
		 * @param status the new status of the message
		 */
		void setStatus(const bool &status);

		/**
		 * Get the ID of the message
		 *
		 * @return the ID of the message
		 */
		uint16_t getID();

		/**
		 * Set the ID of the message
		 *
		 * @param ID the new ID of the message
		 */
		void setID(uint16_t ID);

		/**
		 * Get the refID of the message
		 *
		 * @return the refID of the message
		 */
		uint16_t getRefID();

		/**
		 * Set the refID of the message
		 *
		 * @param ID the new refID of the message
		 */
		void setRefID(uint16_t ID);
	};

} // namespace formats

#endif