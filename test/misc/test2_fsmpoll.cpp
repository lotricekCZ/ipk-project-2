#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "../../src/fsm/TCP_fsm.hpp"

int main()
{
    TCPFSM fsm;
    fsm.run();
    return 0;
}
