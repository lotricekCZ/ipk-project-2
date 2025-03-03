#include <string>
#include <stdexcept>
#include "message.hpp"

namespace formats
{
	std::string &Message::getText()
	{
		return text;
	}

	std::string &Message::getAuthor()
	{
		return author;
	}

	void Message::setText(const std::string &text)
	{
		if (text.length() <= 60000)
			this->text = text;
		else
			throw std::runtime_error("Message too long");
	}

	void Message::setAuthor(const std::string &author)
	{
		if (author.length() <= 20)
			this->author = author;
		else
			throw std::runtime_error("Author too long");
	}

	MessageType Message::getType()
	{
		return type;
	}

	void Message::setType(const MessageType &type)
	{
		this->type = type;
	}

}

bool formats::Message::getStatus()
{
	return isOk;
}

void formats::Message::setStatus(const bool &status)
{
	isOk = status;
}