#include "edge.hpp"

template <typename T>
bool Edge<T>::accept(T &input){
	for(auto f : functions){
		if(f(input)) return true;
	}
	return false;
}

template <typename T>
template <typename E>
void Edge<T>::setTarget(std::shared_ptr<Node<E>> target){
	this->target = target;
}