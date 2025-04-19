/**
 * @file edge.hpp
 * @brief Edge class
 *
 * This class represents an edge in a finite state machine. It is a
 * template class that takes two template parameters: the type of the
 * state and the type of the event. An edge is a connection between two
 * states and is triggered when an event occurs. The edge can be set to
 * call a function when it is triggered.
 */
#ifndef EDGE_HPP
#define EDGE_HPP

#include <memory>
#include <vector>
#include <string>
#include <functional>

template <typename T, typename E>
class Node;

/**
 * @brief Edge class
 *
 * This class represents an edge in a finite state machine. It is a
 * template class that takes two template parameters: the type of the
 * state and the type of the event. An edge is a connection between two
 * states and is triggered when an event occurs. The edge can be set to
 * call a function when it is triggered.
 */
template <typename T, typename E>
class Edge
{
	using EdgeNode = Node<T, E>;
	std::shared_ptr<EdgeNode> target;
	std::vector<std::function<bool(E &)>> functions;

public:
	/**
	 * @brief Constructor
	 *
	 * The constructor takes a pointer to the target state and a
	 * variable number of functions. The functions are stored in a
	 * vector and are called when the edge is triggered.
	 *
	 * @param[in] target The target state
	 * @param[in] ...args Functions to be called when the edge is
	 * triggered
	 */
	template <typename... Args>
	Edge(std::shared_ptr<EdgeNode> target, Args &&...args);

	~Edge();
	/**
	 * @brief Check if the edge is triggered
	 *
	 * This function checks if the edge is triggered by calling each of
	 * the functions in the vector. If any of the functions return true,
	 * the function returns true. If none of the functions return true,
	 * the function returns false.
	 *
	 * @param[in] input The event that triggered the edge
	 * @return True if the edge is triggered, false otherwise
	 */
	bool accept(E &input);
	/**
	 * @brief Set the target state
	 *
	 * This function sets the target state of the edge.
	 *
	 * @param[in] target The target state
	 */
	void setTarget(std::shared_ptr<EdgeNode> target);
	/**
	 * @brief Get the target state
	 *
	 * This function returns the target state of the edge.
	 *
	 * @return The target state
	 */
	std::shared_ptr<EdgeNode> getTarget();
};

/**
 * @brief Constructor
 *
 * The constructor takes a pointer to the target state and a variable
 * number of functions. The functions are stored in a vector and are
 * called when the edge is triggered.
 *
 * @param[in] target The target state
 * @param[in] ...args Functions to be called when the edge is
 * triggered
 */
template <typename T, typename E>
template <typename ... Args>
Edge<T, E>::Edge(std::shared_ptr<EdgeNode> target, Args &&...args){
	this->target = target;
	this->functions = {std::forward<Args>(args)...};
}

/**
 * @brief Destructor
 *
 * This destructor clears the vector of functions.
 */
template <typename T, typename E>
Edge<T, E>::~Edge(){
	functions.clear();
}

/**
 * @brief Check if the edge is triggered
 *
 * This function checks if the edge is triggered by calling each of
 * the functions in the vector. If any of the functions return true,
 * the function returns true. If none of the functions return true,
 * the function returns false.
 *
 * @param[in] input The event that triggered the edge
 * @return True if the edge is triggered, false otherwise
 */
template <typename T, typename E>
bool Edge<T, E>::accept(E &input){
	if(functions.empty()) return true;
	for(auto f : functions){
		if(f(input)) return true;
	}
	return false;
}

/**
 * @brief Set the target state
 *
 * This function sets the target state of the edge.
 *
 * @param[in] target The target state
 */
template <typename T, typename E>
void Edge<T, E>::setTarget(std::shared_ptr<EdgeNode> target){
	this->target = target;
}

/**
 * @brief Get the target state
 *
 * This function returns the target state of the edge.
 *
 * @return The target state
 */
template <typename T, typename E>
std::shared_ptr<Node<T, E>> Edge<T, E>::getTarget(){
	if(!target.expired()) return target.lock();
	throw std::runtime_error("Edge target is expired");
}

#endif
