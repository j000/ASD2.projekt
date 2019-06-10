#ifndef GRAPH_HPP
#error Include Graph.hpp!
#endif
#ifndef DFSITERATOR_HPP
#define DFSITERATOR_HPP

#include "Graph.hpp"

template <typename V, typename E>
class Graph<V, E>::DFSIterator {
	friend class Graph<V, E>;

public:
	DFSIterator(const DFSIterator&) = default;
	DFSIterator(DFSIterator&&) = default;
	DFSIterator& operator=(const DFSIterator&) = default;
	DFSIterator& operator=(DFSIterator&&) = default;
	~DFSIterator() = default;

	bool operator==(const DFSIterator& rhs) const;
	bool operator!=(const DFSIterator& rhs) const;

	DFSIterator& operator++();
	DFSIterator operator++(int);

	const V& operator*() const;
	V* operator->() const;

private:
	DFSIterator(const Graph& graph, std::size_t node);
	DFSIterator(const Graph& graph);

	const Graph& m_graph;
	std::size_t m_current{0};
	std::stack<std::size_t> m_stack{};
	std::vector<bool> m_visited{};
};

////////////////////////////////////////
// DFSIterator implementation
////////////////////////////////////////

template <typename V, typename E>
bool Graph<V, E>::DFSIterator::operator==(const DFSIterator& rhs) const
{
	return m_current == rhs.m_current;
}

template <typename V, typename E>
bool Graph<V, E>::DFSIterator::operator!=(const DFSIterator& rhs) const
{
	return !(*this == rhs);
}

template <typename V, typename E>
typename Graph<V, E>::DFSIterator& Graph<V, E>::DFSIterator::operator++()
{
	std::size_t tmp;
	do {
		if (m_stack.empty()) {
			m_current = m_graph.nrOfVertices();
			return *this;
		}
		tmp = m_stack.top();
		m_stack.pop();
	} while (m_visited[tmp]);
	m_visited[tmp] = true;
	for (std::size_t i = m_graph.m_data[tmp].second.size(); i > 0; --i) {
		if (m_graph.edgeExist(tmp, i - 1)) {
			m_stack.push(i - 1);
		}
	}
	m_current = tmp;
	return *this;
}

template <typename V, typename E>
typename Graph<V, E>::DFSIterator Graph<V, E>::DFSIterator::operator++(int)
{
	auto tmp = *this;
	this->operator++();
	return tmp;
}

template <typename V, typename E>
const V& Graph<V, E>::DFSIterator::operator*() const
{
	return m_graph.m_data[m_current].first;
}

template <typename V, typename E>
V* Graph<V, E>::DFSIterator::operator->() const
{
	return m_graph.m_data[m_current].first;
}

template <typename V, typename E>
Graph<V, E>::DFSIterator::DFSIterator(const Graph& graph, std::size_t node)
	: m_graph{graph}, m_current{node}
{
	m_visited.resize(graph.nrOfVertices());
	m_stack.push(node);
	++*this;
}

template <typename V, typename E>
Graph<V, E>::DFSIterator::DFSIterator(const Graph& graph)
	: m_graph{graph}, m_current{graph.nrOfVertices()}
{
}

#endif /* DFSITERATOR_HPP */
