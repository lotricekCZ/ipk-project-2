#include <thread>
#include <memory>
#include <chrono>
#include <argumentum/argparse-h.h>
#include <numeric>
#include <vector>
#include <cstdint>
#include <memory>
#include <signal.h>
#include <climits>
#include "./config.hpp"
#include "./formats/message.hpp"
#include "./transceivers/transceiver.hpp"
#include "./fsm/fsm.hpp"
#include "./fsm/TCP_fsm.hpp"
#include "./fsm/UDP_fsm.hpp"


std::shared_ptr<FSM> fsm;
int main(int argc, char *argv[])
{
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = [](int sig) {
		if(fsm != nullptr)
			fsm->exit();
		};
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);
	std::string protocol = "";
	// -t 	User provided 	tcp or udp 				Transport protocol used for connection
	// -s 	User provided 	IP address or hostname 	Server IP or hostname
	// -p 	4567 			uint16 					Server port
	// -d 	250 			uint16 					UDP confirmation timeout (in milliseconds)
	// -r 	3 				uint8 					Maximum number of UDP retransmissions
	// -h 											Prints program help output and exits
	auto parser = argumentum::argument_parser{};
	auto params = parser.params();
	parser.config().program(argv[0]).description("IPK2025 chat client");
	params.add_parameter(protocol, "--protocol", "-t").metavar("tcp/udp").choices({"tcp", "udp"}).required().nargs(1).help("Transport protocol used for connection");
	params.add_parameter(config::hostname, "--hostname", "-s").metavar("").nargs(1).required().help("Server IP or hostname");
	params.add_parameter(config::port, "--port", "-p").metavar("uint16").absent(4567).nargs(1).required(false).help("Server port");
	params.add_parameter(config::retransmissions, "--retransmissions", "-r").absent(3).metavar("uint8").nargs(1).required(false).help("Maximum number of UDP retransmissions");
	params.add_parameter(config::timeout, "--timeout", "-d").absent(250).metavar("uint16").nargs(1).required(false).help("UDP confirmation timeout (in milliseconds)");

	if (!parser.parse_args(argc, argv, 1))
		return 1;

	if (protocol == "tcp")
	{
		fsm = std::make_shared<TCPFSM>();
	} else if(protocol == "udp") {
		fsm = std::make_shared<UDPFSM>();
	} else {
		throw std::runtime_error("Unsupported transport protocol");
	}
	fsm->run();
	return 0;
}