#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <chrono>
#include <thread>

int main()
{
	struct pollfd pfd;
	pfd.fd = STDIN_FILENO;
	pfd.events = POLLIN;

	int counter = 0;
	bool running = true;

	while (running)
	{
		std::cout << "Počítám: " << counter++ << "\r" << std::flush;

		int ret = poll(&pfd, 1, 500);
		if (ret > 0 && (pfd.revents & POLLIN))
		{
			std::string line;
			std::getline(std::cin, line);
			if (line == "exit")
			{
				running = false;
			}
			else
			{
				std::cout << "\nZadal jsi: " << line << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	std::cout << "\nProgram ukončen.\n";
	return 0;
}
