#ifndef EDGE_HPP
#define EDGE_HPP

#include <memory>
#include <vector>
#include <string>
#include <functional>

template <typename T, typename E>
class Node;

template <typename T, typename E>
class Edge
{
	using EdgeNode = Node<T, E>;
	std::shared_ptr<EdgeNode> target;
	std::vector<std::function<bool(E &)>> functions;

public:
	template <typename... Args>
	Edge(std::shared_ptr<EdgeNode>, Args &&...);
	bool accept(E &input);
	std::function<void(E &)> perform;
	void setTarget(std::shared_ptr<EdgeNode> target);
	std::shared_ptr<EdgeNode> getTarget();
};
#endif