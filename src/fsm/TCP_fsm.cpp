#include <memory>
#include "TCP_fsm.hpp"

TCPFSM::TCPFSM()
{
	for (auto &node : {START, AUTH, OPEN, JOIN, END})
		NodeStates[node] = std::make_shared<FSMNode>(node);
	NodeStates[START]->assignEdges(std::make_shared<FSMEdge>(NodeStates[AUTH]),
								   std::make_shared<FSMEdge>(NodeStates[END]));
	NodeStates[AUTH]->assignEdges(std::make_shared<FSMEdge>(NodeStates[OPEN]), std::make_shared<FSMEdge>(NodeStates[END]), std::make_shared<FSMEdge>(NodeStates[AUTH]));
	NodeStates[OPEN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN]), std::make_shared<FSMEdge>(NodeStates[OPEN]), std::make_shared<FSMEdge>(NodeStates[END]));
	NodeStates[JOIN]->assignEdges(std::make_shared<FSMEdge>(NodeStates[JOIN]), std::make_shared<FSMEdge>(NodeStates[OPEN]), std::make_shared<FSMEdge>(NodeStates[END]));
}

void TCPFSM::run()
{
	
	if (state != END)
	{
		state = (NodeStates[state]->next(messages))->state;
	}
	messages.clear();
}

void TCPFSM::Messages::clear()
{
	_input = formats::Message();
	_output = formats::Message();
}

formats::Message &TCPFSM::Messages::input()
{
	return _input;
}

formats::Message &TCPFSM::Messages::output()
{
	return _output;
}