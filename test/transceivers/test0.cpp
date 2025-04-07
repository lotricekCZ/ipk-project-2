#include "../../src/transceivers/transceiver.hpp"
#include "../../src/transceivers/TCP_transceiver.hpp"
#include <thread>
#include <memory>
#include <chrono>

int main()
{
	std::shared_ptr<Transceiver> transceiver;
	transceiver = std::make_shared<TCPTransceiver>();
	transceiver->init();
	transceiver->send("AUTH xramas01 AS lotricek USING 41d59e50-0f35-44c1-abbe-48134dbc317f\r\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	transceiver->receive();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	transceiver->send("MSG FROM lotricek IS Dlouho jsem nic nepsal, no zda se, ze jsem odhalil chybu - rad pouzivam separatni .hpp na config, coz se vymstilo kdyz jsem ho importoval ve vicero souborech a double free vznikal na parametrech, ktere jsou non-const. Takze i tak hloupa vec jako je config se ted musi prekladat jako sdilena knihovna, pitomost :sob:. Receno toto mohl jsem na to prijit behem par minut kdybych pouzil valgrind hned na zacatku.\r\n");
	// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	// Transceiver->receive();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	transceiver->send("BYE FROM lotricek\r\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	transceiver->receive();
	return 0;
}