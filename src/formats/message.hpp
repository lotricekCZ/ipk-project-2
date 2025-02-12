#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

namespace formats {

/**
 * A message containing a text and an author
 */
class Message {
	std::string text;
	std::string author;
public:
	/**
	 * Construct a new message with the given text and author
	 *
	 * @param text the text of the message
	 * @param author the author of the message
	 */
	Message(const std::string& text, const std::string& author) : text(text), author(author) {}

	/**
	 * Get the text of the message
	 *
	 * @return the text of the message
	 */
	std::string& getText();

	/**
	 * Get the author of the message
	 *
	 * @return the author of the message
	 */
	std::string& getAuthor();

	/**
	 * Set the text of the message
	 *
	 * @param text the new text of the message
	 */
	void setText(const std::string& text);

	/**
	 * Set the author of the message
	 *
	 * @param author the new author of the message
	 */
	void setAuthor(const std::string& author);
};

} // namespace formats

#endif