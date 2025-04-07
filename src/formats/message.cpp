/**
 * @file message.cpp
 * @brief Implementation of Message class
 *
 * This file contains the implementation of the Message class, which is a base
 * class for all messages.
 */

#include <string>
#include <stdexcept>
#include <cstdint>
#include "message.hpp"

namespace formats
{
	/**
	 * @brief Get the text of the message
	 * @return The text of the message
	 */
	std::string &Message::getText()
	{
		return text;
	}

	/**
	 * @brief Get the author of the message
	 * @return The author of the message
	 */
	std::string &Message::getAuthor()
	{
		return author;
	}

	/**
	 * @brief Set the text of the message
	 * @param text The new text of the message
	 * @throws std::runtime_error If the text is too long
	 */
	void Message::setText(const std::string &text)
	{
		if (text.length() <= 60000)
			this->text = text;
		else
			throw std::runtime_error("Message too long");
	}

	/**
	 * @brief Set the author of the message
	 * @param author The new author of the message
	 * @throws std::runtime_error If the author is too long
	 */
	void Message::setAuthor(const std::string &author)
	{
		if (author.length() <= 20)
			this->author = author;
		else
			throw std::runtime_error("Author too long");
	}

	/**
	 * @brief Get the type of the message
	 * @return The type of the message
	 */
	MessageType Message::getType()
	{
		return type;
	}

	/**
	 * @brief Set the type of the message
	 * @param type The new type of the message
	 */
	void Message::setType(const MessageType &type)
	{
		this->type = type;
	}

}

/**
 * @brief Get the status of the message
 * @return The status of the message
 */
bool formats::Message::getStatus()
{
	return isOk;
}

/**
 * @brief Set the status of the message
 * @param status The new status of the message
 */
void formats::Message::setStatus(const bool &status)
{
	isOk = status;
}

/**
 * Get the ID of the message
 *
 * @return the ID of the message
 */
uint16_t formats::Message::getID()
{
	return ID;
}

/**
 * Set the ID of the message
 *
 * @param ID the new ID of the message
 */
void formats::Message::setID(uint16_t ID)
{
	this->ID = ID;
}

/**
 * Get the refID of the message
 *
 * @return the refID of the message
 */
uint16_t formats::Message::getRefID()
{
	return refID;
}

/**
 * Set the refID of the message
 *
 * @param ID the new refID of the message
 */
void formats::Message::setRefID(uint16_t ID)
{
	this->refID = ID;
}