#include "edge.hpp"

template <typename T, typename E>
template <typename ... Args>
Edge<T, E>::Edge(std::shared_ptr<EdgeNode> target, Args &&...args){
	this->target = target;
	this->functions = {std::forward<Args>(args)...};
	this->perform = [](T &){};
}

template <typename T, typename E>
bool Edge<T, E>::accept(E &input){
	if(functions.empty()) return true;
	for(auto f : functions){
		if(f(input)) return true;
	}
	return false;
}

template <typename T, typename E>
void Edge<T, E>::setTarget(std::shared_ptr<EdgeNode> target){
	this->target = target;
}

template <typename T, typename E>
std::shared_ptr<Node<T, E>> Edge<T, E>::getTarget(){
	return this->target;
}