#ifndef FSM_HPP
#define FSM_HPP

#include "../formats/message.hpp"
#include "../config.hpp"

class FSM
{
public:
	virtual void run() = 0;
	virtual void exit() = 0;
};

#endif // FSM_HPP