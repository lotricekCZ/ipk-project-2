/**
 * @file node.hpp
 * @brief Node class
 *
 * This class represents a node in a finite state machine. It is a
 * template class that takes two template parameters: the type of the
 * state and the type of the event. A node has a state and a vector of
 * edges. The edges are used to transition from one node to another.
 *
 * @tparam T The type of the state
 * @tparam E The type of the event
 */
#include <vector>
#include <memory>

#ifndef NODE_HPP
#define NODE_HPP

template <typename T, typename E>
class Edge;

/**
 * @brief Node class
 *
 * This class represents a node in a finite state machine. It is a
 * template class that takes two template parameters: the type of the
 * state and the type of the event. A node has a state and a vector of
 * edges. The edges are used to transition from one node to another.
 */
template <typename T, typename E>
class Node
{
public:
	/**
	 * @brief The state of the node
	 */
	T state;

	/**
	 * @brief The edges of the node
	 */
	using NodeEdge = Edge<T, E>;

	/**
	 * @brief The vector of edges
	 */
	std::vector<std::shared_ptr<NodeEdge>> edges;

	/**
	 * @brief Constructor
	 *
	 * The constructor takes a single argument, the state of the node.
	 *
	 * @param[in] state The state of the node
	 */
	Node(T state) : state(state) {}

	~Node();

	/**
	 * @brief Assign edges to the node
	 *
	 * This function assigns edges to the node. It takes a variable
	 * number of arguments, each of which is a shared pointer to an
	 * Edge object.
	 *
	 * @param[in] ...args The edges to be assigned
	 */
	template <typename... Args>
	void assignEdges(Args &&...args);

	/**
	 * @brief Add an edge to the node
	 *
	 * This function adds an edge to the node. It takes a single
	 * argument, a shared pointer to an Edge object.
	 *
	 * @param[in] edge The edge to be added
	 */
	void addEdge(std::shared_ptr<NodeEdge> edge);

	/**
	 * @brief Get the next node
	 *
	 * This function returns the next node in the finite state machine.
	 * It takes a single argument, an event, and it uses the event to
	 * determine which edge to follow.
	 *
	 * @param[in] input The event
	 * @return The next node
	 */
	std::shared_ptr<Node<T, E>> next(E input);
};

/**
 * @brief Assign edges to the node
 *
 * This function assigns edges to the node. It takes a variable
 * number of arguments, each of which is a shared pointer to an
 * Edge object.
 *
 * @param[in] ...args The edges to be assigned
 */
template <typename T, typename E>
template <typename... Args>
void Node<T, E>::assignEdges(Args &&...args)
{
	this->edges = {std::forward<Args>(args)...};
}

/**
 * @brief Add an edge to the node
 *
 * This function adds an edge to the node. It takes a single
 * argument, a shared pointer to an Edge object.
 *
 * @param[in] edge The edge to be added
 */
template <typename T, typename E>
void Node<T, E>::addEdge(std::shared_ptr<NodeEdge> edge)
{
	this->edges.push_back(edge);
}

/**
 * @brief Get the next node
 *
 * This function returns the next node in the finite state machine.
 * It takes a single argument, an event, and it uses the event to
 * determine which edge to follow.
 *
 * @param[in] input The event
 * @return The next node
 */
template <typename T, typename E>
std::shared_ptr<Node<T, E>> Node<T, E>::next(E input)
{
	for (auto edge : edges)
	{
		if (edge->accept(input))
		{
			return edge->getTarget();
		}
	}
	return nullptr;
}

template <typename T, typename E>
Node<T, E>::~Node()
{
	for (auto &edge : edges)
	{
		if (edge != nullptr)
		{
			edge.reset();
			edge = nullptr;
		}
	}
	edges.clear();
}

#endif
