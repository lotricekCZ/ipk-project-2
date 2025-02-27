#include "node.hpp"
#include "edge.hpp"

template <typename T>
template <typename... Args>
void Node<T>::assignEdges(Args &&...args)
{
	this->edges = {std::make_shared<Edge>(std::forward<Args>(args))...};
}

template <typename T>
template <typename E>
void Node<T>::addEdge(std::shared_ptr<Edge<E>> edge){
	this->edges.push_back(edge);
	}