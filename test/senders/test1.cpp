#include "../../src/senders/UDP_sender.hpp"
#include "../../src/decoders/UDP_decoder.hpp"
#include <thread>
#include <iostream>
#include <chrono>

std::string printMessage(formats::Message &message)
{
	return message.getAuthor() + "[" + std::to_string(message.getType()) + " " + std::to_string(message.getID()) + "]: " + message.getText();
}

int main()
{
	UDPSender sender;
	decoders::UDPDecoder decoder;
	sender.init();
	uint8_t dataConfirm[] = {
		0x00, 0x00, 0x00};
	uint8_t data[] = {
		0x02, 0x00, 0x00, 'x', 'r', 'a', 'm', 'a', 's', '0', '1', 0x00,
		'l', 'o', 't', 'r', 'i', 'c', 'e', 'k', 0x00, '4', '1', 'd', '5', '9', 'e', '5', '0',
		'-', '0', 'f', '3', '5', '-', '4', '4', 'c', '1', '-', 'a', 'b', 'b', 'e', '-',
		'4', '8', '1', '3', '4', 'd', 'b', 'c', '3', '1', '7', 'f', 0x00};
	// for (int i = 0; i < sizeof(data); i++)
	// 	std::cout << data[i];
	sender.send(data, sizeof(data));
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	
	// receive confirm
	formats::Message message = decoder.decode(sender.receive());
	std::cout << printMessage(message) << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// receive reply
	message = decoder.decode(sender.receive());
	sender.send(dataConfirm, sizeof(dataConfirm));
	dataConfirm[1] += 1;
	std::cout << printMessage(message) << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	// receive msg
	message = decoder.decode(sender.receive());
	sender.send(dataConfirm, sizeof(dataConfirm));
	dataConfirm[1] += 1;
	std::cout << printMessage(message) << std::endl;
	
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	// // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	// // sender.receive();
	// // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	uint8_t data2[] = {
		0x02, 0x01, 0x00, 'l', 'o', 't', 'r', 'i', 'c', 'e', 'k', 0x00,
		'A', 'h', 'o', 'j', ',', ' ', 't', 'o', 'h', 'l', 'e', ' ', 'j', 'e', ' ', 't', 'e',
		's', 't', ' ', 'p', 'o', 's', 'i', 'l', 'a', 'n', 'i', ' ', 'u', 'd', 'p', ' ', 'p',
		'a', 'k', 'e', 't', 'u', ',', ' ', 'a', 'u', 'k', 't', 'u', 'a', 'l', 'n', 'i', ' ',
		'p', 'a', 'k', 'e', 't', ' ', 'j', 'e', ' ', 'p', 'r', 'o', 's', 't', 'e', ' ', 'h',
		'a', 'r', 'd', 'c', 'o', 'd', 'e', 'd', 0x00};

	sender.send(data2, sizeof(data2));
	
	// receive confirm
	message = decoder.decode(sender.receive());
	std::cout << printMessage(message) << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(400));
	// sender.send(dataConfirm, sizeof(dataConfirm));
	// dataConfirm[1] += 1;
	// std::cout << printMessage(message) << std::endl;
	uint8_t data3[] = {
		0xff, 0x03, 0x00, 'l', 'o', 't', 'r', 'i', 'c', 'e', 'k', 0x00};
	sender.send(data3, sizeof(data3));
	
	// receive confirm
	message = decoder.decode(sender.receive());
	std::cout << printMessage(message) << std::endl;
	// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	// sender.receive();
	return 0;
}