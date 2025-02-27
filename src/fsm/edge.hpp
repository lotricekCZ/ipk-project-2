#ifndef EDGE_HPP
#define EDGE_HPP

#include <memory>
#include <vector>
#include <string>
#include <functional>

template <typename T>
class Node;

template <typename T>
class Edge
{
	template <typename E>
	std::shared_ptr<Node<E>> target;
	std::vector<std::function<bool(T&)>> functions;
public:
	bool accept(T &input);
	template <typename E>
	void setTarget(std::shared_ptr<Node<E>> target);
};
#endif