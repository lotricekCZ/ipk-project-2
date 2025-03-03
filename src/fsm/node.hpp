#include <vector>
#include <memory>
// #include "edge.hpp"

#ifndef NODE_HPP
#define NODE_HPP

template <typename T, typename E> class Edge;

template <typename T, typename E>
class Node
{
public:
	T state;
	using NodeEdge = Edge<T, E>;

	std::vector<std::shared_ptr<NodeEdge>> edges;
	Node(T state) : state(state) {}
	template <typename... Args>
	void assignEdges(Args &&...args);
	void addEdge(std::shared_ptr<NodeEdge> edge);
	std::shared_ptr<Node<T, E>> next(E input);
};

#endif