// dydaktykafais@outlook.com
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

#define A_STAR_SHOW_VISITS 0
#define USE_FASTER_REMOVAL 0

////////////////////////////////////////
// Graph
////////////////////////////////////////

template <typename V, typename E>
class Graph {
public:
	class VerticesIterator;
	class EdgesIterator;
	class BFSIterator;
	class DFSIterator;

public:
	Graph() = default;
	Graph(const Graph&) = default;
	Graph(Graph&&) = default;
	Graph& operator=(const Graph&) = default;
	Graph& operator=(Graph&&) = default;
	~Graph() = default;

	std::size_t nrOfVertices() const;
	VerticesIterator begin() const;
	VerticesIterator end() const;
	VerticesIterator beginVertices() const;
	VerticesIterator endVertices() const;
	VerticesIterator vertex(std::size_t) const;
	const V& vertexData(std::size_t) const;
	V& vertexData(std::size_t);
	VerticesIterator insertVertex(const V&);
	bool removeVertex(std::size_t);

	std::size_t nrOfEdges() const;
	EdgesIterator beginEdges() const;
	EdgesIterator endEdges() const;
	EdgesIterator edge(std::size_t, std::size_t) const;
	bool edgeExist(std::size_t, std::size_t) const;
	const E& edgeLabel(std::size_t, std::size_t) const;
	E& edgeLabel(std::size_t, std::size_t);
	std::pair<EdgesIterator, bool>
	insertEdge(std::size_t, std::size_t, const E& = E(), bool = true);
	bool removeEdge(std::size_t, std::size_t);

	void printNeighborhoodMatrix() const;

	void bfs(std::size_t) const;
	void dfs(std::size_t) const;

	BFSIterator beginBFS(std::size_t = 0) const;
	BFSIterator endBFS() const;

	DFSIterator beginDFS(std::size_t = 0) const;
	DFSIterator endDFS() const;

	std::pair<double, std::vector<std::size_t>> dijkstra(
		const std::size_t,
		const std::size_t,
		const std::function<double(const E&)>) const;
	std::pair<double, std::vector<std::size_t>> a_star(
		const std::size_t,
		const std::size_t,
		const std::function<double(const E&)>,
		const std::function<
			double(const Graph<V, E>&, const std::size_t, const std::size_t)>)
		const;

private:
	std::vector<std::pair<V, std::vector<std::optional<E>>>> m_data{};
};

////////////////////////////////////////
// Iterators
////////////////////////////////////////

#include "BFSIterator.hpp"
#include "DFSIterator.hpp"
#include "EdgesIterator.hpp"
#include "VerticesIterator.hpp"

////////////////////////////////////////
// Graph implementation
////////////////////////////////////////

template <typename V, typename E>
std::size_t Graph<V, E>::nrOfVertices() const
{
	return m_data.size();
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::begin() const
{
	return beginVertices();
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::end() const
{
	return endVertices();
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::beginVertices() const
{
	return VerticesIterator(*this);
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator Graph<V, E>::endVertices() const
{
	return ++VerticesIterator(*this, m_data.size() - 1);
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator
Graph<V, E>::vertex(std::size_t vertex_id) const
{
	if (vertex_id >= m_data.size())
		return VerticesIterator();
	return VerticesIterator(*this, vertex_id);
}

template <typename V, typename E>
const V& Graph<V, E>::vertexData(std::size_t vertex_id) const
{
	return m_data[vertex_id].first;
}

template <typename V, typename E>
V& Graph<V, E>::vertexData(std::size_t vertex_id)
{
	return m_data[vertex_id].first;
}

template <typename V, typename E>
typename Graph<V, E>::VerticesIterator
Graph<V, E>::insertVertex(const V& vertex_data)
{
	for (auto& vertex : m_data) {
		vertex.second.push_back({});
	}
	m_data.push_back(std::make_pair(
		vertex_data, std::vector<std::optional<E>>(m_data.size() + 1)));

	return VerticesIterator(*this, m_data.size() - 1);
}

template <typename V, typename E>
bool Graph<V, E>::removeVertex(std::size_t vertex_id)
{
	if (vertex_id < 0 || vertex_id >= m_data.size())
		// throw std::out_of_range{"Index out of range"};
		return false;

	using std::swap;
#if USE_FASTER_REMOVAL
	swap(m_data[vertex_id], m_data.back());
	m_data.pop_back();
	for (auto& list : m_data) {
		swap(list.second[vertex_id], list.second.back());
		list.second.pop_back();
	}
#else
	m_data.erase(m_data.begin() + vertex_id);
	for (auto& list : m_data) {
		list.second.erase(list.second.begin() + vertex_id);
	}
#endif
	return true;
}

template <typename V, typename E>
std::size_t Graph<V, E>::nrOfEdges() const
{
	std::size_t out{0};
	for (const auto& i : m_data)
		for (const auto& j : i.second)
			if (j.has_value())
				++out;
	return out;
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::beginEdges() const
{
	return EdgesIterator{*this, 0, 0};
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator Graph<V, E>::endEdges() const
{
	return EdgesIterator{*this};
}

template <typename V, typename E>
typename Graph<V, E>::EdgesIterator
Graph<V, E>::edge(std::size_t vertex1_id, std::size_t vertex2_id) const
{
	if (vertex1_id >= m_data.size() || vertex2_id >= m_data.size()
		|| !edgeExist(vertex1_id, vertex2_id))
		return EdgesIterator{};
	return EdgesIterator{*this, vertex1_id, vertex2_id};
}

template <typename V, typename E>
const E&
Graph<V, E>::edgeLabel(std::size_t vertex1_id, std::size_t vertex2_id) const
{
	if (!edgeExist(vertex1_id, vertex2_id))
		throw std::logic_error{"Podana krawędź nie istnieje"};
	return *m_data[vertex1_id].second[vertex2_id];
}

template <typename V, typename E>
E& Graph<V, E>::edgeLabel(std::size_t vertex1_id, std::size_t vertex2_id)
{
	if (!edgeExist(vertex1_id, vertex2_id))
		throw std::logic_error{"Podana krawędź nie istnieje"};
	return *m_data[vertex1_id].second[vertex2_id];
}

template <typename V, typename E>
bool Graph<V, E>::edgeExist(std::size_t vertex1_id, std::size_t vertex2_id)
	const
{
	return m_data[vertex1_id].second[vertex2_id].has_value();
}

template <typename V, typename E>
std::pair<typename Graph<V, E>::EdgesIterator, bool> Graph<V, E>::insertEdge(
	std::size_t vertex1_id,
	std::size_t vertex2_id,
	const E& label,
	bool replace)
{
	if (edgeExist(vertex1_id, vertex2_id) && !replace)
		return std::make_pair(EdgesIterator{*this, 0, 0}, false);
	m_data[vertex1_id].second[vertex2_id] = label;
	return std::make_pair(EdgesIterator{*this, vertex1_id, vertex2_id}, true);
}

template <typename V, typename E>
bool Graph<V, E>::removeEdge(std::size_t vertex1_id, std::size_t vertex2_id)
{
	if (vertex1_id < 0 || vertex2_id >= m_data.size() || vertex2_id < 0
		|| vertex2_id >= m_data.size())
		// throw std::out_of_range{"Index out of range"};
		return false;

	auto& tmp = m_data[vertex1_id].second[vertex2_id];
	if (!tmp.has_value())
		return false;
	tmp.reset();
	return true;
}

template <typename V, typename E>
void Graph<V, E>::printNeighborhoodMatrix() const
{
	constexpr int width{8};
	std::cout << std::setw(width) << " ";
	for (const auto& i : m_data)
		std::cout << std::setw(width) << i.first;
	std::cout << std::endl;

	for (const auto& i : m_data) {
		std::cout << std::setw(width) << i.first;
		for (const auto& j : i.second) {
			if (j)
				std::cout << std::setw(width) << *j;
			else
				std::cout << std::setw(width) << "-";
		}
		std::cout << std::endl;
	}
}

template <typename V, typename E>
void Graph<V, E>::bfs(std::size_t start) const
{
	std::vector<bool> visited(m_data.size());
	std::queue<std::size_t> queue{};
	queue.push(start);
	while (!queue.empty()) {
		auto tmp = queue.front();
		queue.pop();
		if (visited[tmp])
			continue;
		visited[tmp] = true;
		for (std::size_t i = 0; i < m_data[tmp].second.size(); ++i) {
			if (edgeExist(tmp, i)) {
				queue.push(i);
			}
		}
		std::cout << m_data[tmp].first << ", ";
	}
	std::cout << std::endl;
}

template <typename V, typename E>
void Graph<V, E>::dfs(std::size_t start) const
{
	std::vector<bool> visited(m_data.size());
	std::stack<std::size_t> stack{};
	stack.push(start);
	while (!stack.empty()) {
		auto tmp = stack.top();
		stack.pop();
		if (visited[tmp])
			continue;
		visited[tmp] = true;
		for (std::size_t i = m_data[tmp].second.size(); i > 0; --i) {
			if (edgeExist(tmp, i - 1)) {
				stack.push(i - 1);
			}
		}
		std::cout << m_data[tmp].first << ", ";
	}
	std::cout << std::endl;
}

template <typename V, typename E>
typename Graph<V, E>::BFSIterator Graph<V, E>::beginBFS(std::size_t node) const
{
	return BFSIterator{*this, node};
}

template <typename V, typename E>
typename Graph<V, E>::BFSIterator Graph<V, E>::endBFS() const
{
	return BFSIterator{*this};
}

template <typename V, typename E>
typename Graph<V, E>::DFSIterator Graph<V, E>::beginDFS(std::size_t node) const
{
	return DFSIterator{*this, node};
}

template <typename V, typename E>
typename Graph<V, E>::DFSIterator Graph<V, E>::endDFS() const
{
	return DFSIterator{*this};
}

template <typename V, typename E>
std::pair<double, std::vector<std::size_t>> Graph<V, E>::dijkstra(
	const std::size_t start,
	const std::size_t end,
	const std::function<double(const E&)> f) const
{
	const auto comp
		= [](const auto& lhs, const auto& rhs) { return lhs.cost > rhs.cost; };
	struct node_elem {
		std::size_t node{0};
		double cost{0};
		std::size_t previous{0};
	};
	const auto number_of_vertices = nrOfVertices();

	std::priority_queue<node_elem, std::vector<node_elem>, decltype(comp)>
		frontier(comp);
	frontier.push({start, 0, end}); // end is a magic number here
	std::unordered_map<std::size_t, std::size_t> previous;

	while (!frontier.empty()) {
		auto current = frontier.top();
		frontier.pop();
		// std::cout << " *** visiting " << current.node << " from " <<
		// current.previous << std::endl;

		if (current.node == end) {
			// retrun solution
			std::vector<std::size_t> out{end};
			auto tmp = current.previous;
			do {
				out.push_back(tmp);
				tmp = previous[tmp];
			} while (tmp != end); // end is a magic number here
			std::reverse(out.begin(), out.end());
			return std::make_pair(current.cost, out);
		}
		if (previous.count(current.node))
			continue;

		// mark as visited
		previous[current.node] = current.previous;
		// loop through all neighbors
		for (std::size_t i = 0; i < number_of_vertices; ++i) {
			// if edge does not exist
			if (!edgeExist(current.node, i))
				continue;
			// if visited
			if (previous.count(i))
				continue;

			const auto& edge = m_data[current.node].second[i];
			const double cost = f(*edge);
			node_elem new_value{i, current.cost + cost, current.node};
			frontier.push(new_value);
		}
	}
	throw std::runtime_error{"No valid path"};
}

template <typename V, typename E>
std::pair<double, std::vector<std::size_t>> Graph<V, E>::a_star(
	const std::size_t start,
	const std::size_t end,
	const std::function<double(const E&)> f,
	const std::function<
		double(const Graph<V, E>&, const std::size_t, const std::size_t)> h)
	const
{
	const auto comp = [](const auto& lhs, const auto& rhs) {
		return lhs.expected_cost > rhs.expected_cost;
	};
	struct node_elem {
		std::size_t node{0};
		double cost{0};
		double expected_cost{0};
		std::size_t previous{0};
	};
	const auto number_of_vertices = nrOfVertices();

	std::priority_queue<node_elem, std::vector<node_elem>, decltype(comp)>
		frontier(comp);
	frontier.push({start, 0, 0, end}); // end is a magic number here
	std::unordered_map<std::size_t, std::size_t> previous;

	while (!frontier.empty()) {
		auto current = frontier.top();
		frontier.pop();
#if A_STAR_SHOW_VISITS
		// debug print
		std::cout << "    visiting " << current.node << " from "
				  << current.previous << " with cost " << current.cost
				  << std::endl;
#endif

		if (current.node == end) {
			// retrun solution
			std::vector<std::size_t> out{end};
			auto tmp = current.previous;
			do {
				out.push_back(tmp);
				tmp = previous[tmp];
			} while (tmp != end); // end is a magic number here
			std::reverse(out.begin(), out.end());
			return std::make_pair(current.cost, out);
		}
		if (previous.find(current.node) != previous.end())
			continue;

		// mark as visited
		previous[current.node] = current.previous;
		// loop through all neighbors
		for (std::size_t i = 0; i < number_of_vertices; ++i) {
			// if edge does not exist
			if (!edgeExist(current.node, i))
				continue;
			// if visited
			// if (previous.count(i))
			if (previous.find(i) != previous.end())
				continue;

			const auto& edge = m_data[current.node].second[i];
			const double cost = f(*edge);
			node_elem new_value{i,
								current.cost + cost,
								current.cost + cost + h(*this, i, end),
								current.node};
			frontier.push(new_value);
		}
	}
	throw std::runtime_error{"No valid path"};
}

#endif /* GRAPH_HPP */
