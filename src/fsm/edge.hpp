#ifndef EDGE_HPP
#define EDGE_HPP

#include <memory>
#include <vector>
#include <string>

class Edge
{
	std::shared_ptr<Node> target;

public:
	virtual bool accept(std::string &input) = 0;
};
#endif