#include <vector>
#include <memory>
#include "edge.hpp"

#ifndef NODE_HPP
#define NODE_HPP

template <typename T>
class Node
{
public:
	T state;
	std::vector<std::smart_ptr<Edge>> children;
};

#endif