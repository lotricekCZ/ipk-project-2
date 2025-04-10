#ifndef UDPENCODER_HPP
#define UDPENCODER_HPP

#include "encoder.hpp"
#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <cstring>

namespace encoders
{
	class bytes : std::vector<char>
	{
	public:
		using std::vector<char>::vector;
		using std::vector<char>::operator=;
		using std::vector<char>::assign;
		using std::vector<char>::get_allocator;
		using std::vector<char>::at;
		using std::vector<char>::operator[];
		using std::vector<char>::front;
		using std::vector<char>::back;
		using std::vector<char>::data;
		using std::vector<char>::empty;
		using std::vector<char>::size;
		using std::vector<char>::max_size;
		using std::vector<char>::reserve;
		using std::vector<char>::capacity;
		using std::vector<char>::shrink_to_fit;
		using std::vector<char>::clear;
		using std::vector<char>::insert;
		using std::vector<char>::emplace;
		using std::vector<char>::erase;
		using std::vector<char>::push_back;
		using std::vector<char>::emplace_back;
		using std::vector<char>::pop_back;
		using std::vector<char>::resize;
		using std::vector<char>::swap;
		bytes(std::string str) : std::vector<char>(str.begin(), str.end()) {}
		bytes(const char *str) : std::vector<char>(str, str + strlen(str)) {}
		std::string str() const
		{
			return std::string(this->begin(), this->end());
		}
		std::tuple<std::string, uint16_t> data() const;
	};

	class UDPEncoder : public Encoder
	{
		const std::string delimiter = "\r\n";

	public:
		std::string encode(formats::Message &message) override {
		{
			std::runtime_error("Not implemented");
		};
		std::tuple<uint8_t *, uint16_t> encodeBinary(formats::Message &message) override;
	};
}

#endif