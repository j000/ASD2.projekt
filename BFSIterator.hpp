#ifndef GRAPH_HPP
#error Include Graph.hpp!
#endif
#ifndef BFSITERATOR_HPP
#define BFSITERATOR_HPP

#include "Graph.hpp"

template <typename V, typename E>
class Graph<V, E>::BFSIterator {
	friend class Graph<V, E>;

public:
	BFSIterator(const BFSIterator&) = default;
	BFSIterator(BFSIterator&&) = default;
	BFSIterator& operator=(const BFSIterator&) = default;
	BFSIterator& operator=(BFSIterator&&) = default;
	~BFSIterator() = default;

	bool operator==(const BFSIterator& rhs) const;
	bool operator!=(const BFSIterator& rhs) const;

	BFSIterator& operator++();
	BFSIterator operator++(int);

	const V& operator*() const;
	V* operator->() const;

private:
	BFSIterator(const Graph& graph, std::size_t node);
	BFSIterator(const Graph& graph);

	const Graph& m_graph;
	std::size_t m_current{0};
	std::queue<std::size_t> m_queue{};
	std::vector<bool> m_visited{};
};

////////////////////////////////////////
// BFSIterator implementation
////////////////////////////////////////

template <typename V, typename E>
bool Graph<V, E>::BFSIterator::operator==(const BFSIterator& rhs) const
{
	return m_current == rhs.m_current;
}

template <typename V, typename E>
bool Graph<V, E>::BFSIterator::operator!=(const BFSIterator& rhs) const
{
	return !(*this == rhs);
}

template <typename V, typename E>
typename Graph<V, E>::BFSIterator& Graph<V, E>::BFSIterator::operator++()
{
	if (m_current == m_graph.nrOfVertices())
		return *this;

	std::size_t tmp;
	do {
		if (m_queue.empty()) {
			m_current = m_graph.nrOfVertices();
			return *this;
		}
		tmp = m_queue.front();
		m_queue.pop();
	} while (m_visited[tmp]);
	m_visited[tmp] = true;
	for (std::size_t i = 0; i < m_graph.m_data[tmp].second.size(); ++i) {
		if (m_graph.edgeExist(tmp, i))
			m_queue.push(i);
	}
	m_current = tmp;
	return *this;
}

template <typename V, typename E>
typename Graph<V, E>::BFSIterator Graph<V, E>::BFSIterator::operator++(int)
{
	auto tmp = *this;
	this->operator++();
	return tmp;
}

template <typename V, typename E>
const V& Graph<V, E>::BFSIterator::operator*() const
{
	return m_graph.m_data[m_current].first;
}

template <typename V, typename E>
V* Graph<V, E>::BFSIterator::operator->() const
{
	return m_graph.m_data[m_current].first;
}

template <typename V, typename E>
Graph<V, E>::BFSIterator::BFSIterator(const Graph& graph, std::size_t node)
	: m_graph{graph}, m_current{node}
{
	m_visited.resize(graph.nrOfVertices());
	m_queue.push(node);
	++*this;
}

template <typename V, typename E>
Graph<V, E>::BFSIterator::BFSIterator(const Graph& graph)
	: m_graph{graph}, m_current{graph.nrOfVertices()}
{
}

#endif /* BFSITERATOR_HPP */
