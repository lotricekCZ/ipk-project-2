#include "../../src/senders/TCP_sender.hpp"
#include <thread>
#include <chrono>

int main() {
	TCPSender sender;
	sender.init();
	sender.send("AUTH xramas01 AS pokus43 USING 41d59e50-0f35-44c1-abbe-48134dbc317f\r\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	sender.receive();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	// sender.receive();
	// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	sender.send("MSG FROM pokus43 IS Ahoj, toto je pokus lotricek, hadam. Aktualne jsem ve stavu, kdy zpravu pisu jako string i s tou msg from a tak dal, vpn nebyla potreba, OOP bude dobry tak na hodnoceni a konecny automat. Pro lidi co to resi pres hrubou silu, prosim, prijmete tu zpravu co posle server po tom, co poslete auth, zkuste nejdriv TCP, radeji to nedelejte v C++, jestli jste neslyseli o ICP, a mejte se famfarove.\r\n");
	// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	// sender.receive();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	sender.send("BYE FROM pokus43\r\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	sender.receive();
	return 0;
}