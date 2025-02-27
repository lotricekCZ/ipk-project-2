#include <vector>
#include <memory>
// #include "edge.hpp"

#ifndef NODE_HPP
#define NODE_HPP

template <typename T> class Edge;

template <typename T>
class Node
{
public:
	T state;
	template <typename E>
	std::vector<std::shared_ptr<Edge>> edges;
	template <typename... Args>
	void assignEdges(Args &&...args);
	template <typename E>
	void addEdge(std::shared_ptr<Edge> edge);
};

#endif