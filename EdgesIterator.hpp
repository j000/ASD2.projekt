#ifndef GRAPH_HPP
#error Include Graph.hpp!
#endif
#ifndef EDGESITERATOR_HPP
#define EDGESITERATOR_HPP

#include "Graph.hpp"

template <typename V, typename E>
class Graph<V, E>::EdgesIterator {
	friend class Graph<V, E>;

public:
	EdgesIterator(const EdgesIterator&) = default;
	EdgesIterator(EdgesIterator&&) = default;
	EdgesIterator& operator=(const EdgesIterator&) = default;
	EdgesIterator& operator=(EdgesIterator&&) = default;
	~EdgesIterator() = default;

	bool operator==(const EdgesIterator& ei) const;
	bool operator!=(const EdgesIterator& ei) const;

	EdgesIterator& operator++();
	EdgesIterator operator++(int);

	const E& operator*() const;
	const E* operator->() const;

	std::size_t v1id() const;
	std::size_t v2id() const;

private:
	EdgesIterator(const Graph& graph, std::size_t nm_row, std::size_t nm_col);
	EdgesIterator(const Graph& graph);

	const Graph& m_graph;
	std::size_t m_row{};
	std::size_t m_column{};
};

////////////////////////////////////////
// EdgesIterator implementation
////////////////////////////////////////

template <typename V, typename E>
bool Graph<V, E>::EdgesIterator::
operator==(const typename Graph<V, E>::EdgesIterator& ei) const
{
	return m_row == ei.m_row
		&& (m_row == m_graph.m_data.size() || m_column == ei.m_column);
}

template <typename V, typename E>
bool Graph<V, E>::EdgesIterator::operator!=(const EdgesIterator& ei) const
{
	return !(*this == ei);
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator& Graph<V, E>::EdgesIterator::operator++()
{
	const auto count = m_graph.m_data.size();
	// past-the-end
	if (m_row == count) {
		return *this;
	}

	do {
		++m_column;
		if (m_column >= count) {
			++m_row;
			if (m_row == count) {
				return *this;
			}
			m_column = 0;
		}
	} while (!m_graph.m_data[m_row].second[m_column].has_value());
	return *this;
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::EdgesIterator::operator++(int)
{
	auto tmp = *this;
	this->operator++();
	return tmp;
}

template <typename V, typename E>
const E& Graph<V, E>::EdgesIterator::operator*() const
{
	return *m_graph.m_data[m_row].second[m_column];
}

template <typename V, typename E>
const E* Graph<V, E>::EdgesIterator::operator->() const
{
	return *m_graph.m_data[m_row].second[m_column];
}

template <typename V, typename E>
Graph<V, E>::EdgesIterator::EdgesIterator(
	const Graph& graph,
	std::size_t nm_row,
	std::size_t nm_col)
	: m_graph{graph}, m_row{nm_row}, m_column{nm_col}
{
	const auto count = m_graph.m_data.size();
	if (m_row >= count || m_column >= count) {
		m_row = count;
		return;
	}

	if (!m_graph.m_data[m_row].second[m_column].has_value())
		++(*this);
}

template <typename V, typename E>
Graph<V, E>::EdgesIterator::EdgesIterator(const Graph& graph)
	: m_graph{graph}, m_row{m_graph.m_data.size()}
{
}

template <typename V, typename E>
std::size_t Graph<V, E>::EdgesIterator::v1id() const
{
	return m_row;
}

template <typename V, typename E>
std::size_t Graph<V, E>::EdgesIterator::v2id() const
{
	return m_column;
}

#endif /* EDGESITERATOR_HPP */
