#include <vector>
#include <memory>

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


template <typename T, typename E>
template <typename... Args>
void Node<T, E>::assignEdges(Args &&...args)
{
	this->edges = {std::forward<Args>(args)...};
}

template <typename T, typename E>
void Node<T, E>::addEdge(std::shared_ptr<NodeEdge> edge){
	this->edges.push_back(edge);
}

template <typename T, typename E>
std::shared_ptr<Node<T, E>> Node<T, E>::next(E input){
	for(auto edge : edges){
		if(edge->accept(input)){
			return edge->getTarget();
		}
	}
	return nullptr;
}

#endif