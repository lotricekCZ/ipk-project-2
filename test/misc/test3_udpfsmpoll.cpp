#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "../../src/fsm/UDP_fsm.hpp"

int main()
{
    UDPFSM fsm;
    fsm.run();
    return 0;
}
