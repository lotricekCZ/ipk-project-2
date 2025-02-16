#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

namespace formats
{
	/**
	 * @brief Enum representing different types of messages.
	 */
	enum MessageType : char
	{
		ERR,	 /**< Error message */
		REPLY,	 /**< Reply message */
		AUTH,	 /**< Authentication message */
		JOIN,	 /**< Join message */
		MSG,	 /**< General message */
		BYE,	 /**< Goodbye message */
		CONFIRM, /**< Confirmation message */
		PING	 /**< Ping message */
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
		Message(MessageType type, const std::string &author) : type(type), author(author) {}

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