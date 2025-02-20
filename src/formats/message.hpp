#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

namespace formats
{
	/**
	 * @brief Enum representing different types of messages.
	 */
	enum MessageType : unsigned char
	{
		NONE = 0x42,    /**< No message, internal use only */
		ERR = 0x00,		/**< Error message */
		REPLY = 0x01,	/**< Reply message */
		AUTH = 0x02,	/**< Authentication message */
		JOIN = 0x03,	/**< Join message */
		MSG = 0x04,		/**< General message */
		BYE = 0xFD,		/**< Goodbye message */
		CONFIRM = 0xFE, /**< Confirmation message */
		PING = 0xFF		/**< Ping message */
	};
	/**
	 * A message containing a text and an author
	 */
	class Message
	{
		std::string text;
		std::string author;
		MessageType type;

	public:
		/**
		 * Construct a new message with the given text and author
		 *
		 * @param text the text of the message
		 * @param author the author of the message
		 */
		Message(const std::string &text, const std::string &author) : text(text), author(author) {}

		/**
		 * Construct a new message with the given type and author
		 *
		 * @param type the type of the message
		 * @param author the author of the message
		 */
		Message(MessageType type = MessageType::NONE, const std::string &author = "") : type(type), author(author) {}

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
	};

} // namespace formats

#endif