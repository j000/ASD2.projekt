#ifndef GRAPH_HPP
#error Include Graph.hpp!
#endif
#ifndef VERTICESITERATOR_HPP
#define VERTICESITERATOR_HPP

#include "Graph.hpp"

template <typename V, typename E>
class Graph<V, E>::VerticesIterator {
	friend class Graph<V, E>;

public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = V;
	using difference_type = std::ptrdiff_t;
	using pointer = V*;
	using reference = V&;

	VerticesIterator(const VerticesIterator&) = default;
	VerticesIterator(VerticesIterator&&) = default;
	VerticesIterator& operator=(const VerticesIterator&) = default;
	VerticesIterator& operator=(VerticesIterator&&) = default;
	~VerticesIterator() = default;

	bool operator==(const VerticesIterator& vi) const;
	bool operator!=(const VerticesIterator& vi) const;

	VerticesIterator& operator++();
	VerticesIterator operator++(int);

	const V& operator*() const;
	V* operator->() const;

	std::size_t id() const;

private:
	VerticesIterator(const Graph& graph, std::size_t current_vertex_id = 0u);
	const Graph& m_graph;
	std::size_t m_id{};
};

////////////////////////////////////////
// VerticesIterator implementation
////////////////////////////////////////

template <typename V, typename E>
bool Graph<V, E>::VerticesIterator::
operator==(const typename Graph<V, E>::VerticesIterator& vi) const
{
	return m_id == vi.m_id;
}

template <typename V, typename E>
bool Graph<V, E>::VerticesIterator::
operator!=(const typename Graph<V, E>::VerticesIterator& vi) const
{
	return !(*this == vi);
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator& Graph<V, E>::VerticesIterator::
operator++()
{
	++m_id;
	return *this;
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::VerticesIterator::
operator++(int)
{
	auto tmp = *this;
	this->operator++();
	return tmp;
}

template <typename V, typename E>
V* Graph<V, E>::VerticesIterator::operator->() const
{
	return &m_graph.m_data[m_id].first;
}

template <typename V, typename E>
const V& Graph<V, E>::VerticesIterator::operator*() const
{
	return m_graph.m_data[m_id].first;
}

template <typename V, typename E>
std::size_t Graph<V, E>::VerticesIterator::id() const
{
	return m_id;
}

template <typename V, typename E>
Graph<V, E>::VerticesIterator::VerticesIterator(
	const Graph& graph,
	std::size_t current_vertex_id)
	: m_graph{graph}, m_id{current_vertex_id}
{
	// ??
	// decltype(std::declval<Graph>().m_data.begin()) tmp =
	// graph.m_data.begin();
}

#endif /* VERTICESITERATOR_HPP */
