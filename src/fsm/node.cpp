#include "node.hpp"

template <typename T, typename E>
template <typename... Args>
void Node<T, E>::assignEdges(Args &&...args)
{
	this->edges = {std::make_shared<NodeEdge>(std::forward<Args>(args))...};
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
}