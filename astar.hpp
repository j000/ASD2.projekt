#ifndef ASTAR_HPP
#define ASTAR_HPP

#include "graph.hpp"
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>

// funkcja realizująca algorytm A* i zwracająca najkrótszą scieżkę (długość
// scieżki oraz indeksy wierzchołków wchodzących w jej skład - z początkowym i
// końcowym włącznie) pomiędzy wierzchołkami o indeksach "start_idx" i "end_idx"
// w przypadku gdy ścieżka nie istanieje zwraca długość równą 0 i brak indeksów
// argument o nazwie heuristic służy do przekazania funkcji/funktora/...
// realizującej heurystykę argument o nazwie getEdgeLength służy do przekazania
// funkcji/funktora/... realizującej pobranie długosci krawędzi (w najprostszym
// przypadku - gdy etykieta jest długością - zwraca etykietę krawędzi)
template <typename V, typename E>
std::pair<double, std::vector<std::size_t>> astar(
	Graph<V, E>& graph,
	std::size_t start_idx,
	std::size_t end_idx,
	std::function<double(
		const Graph<V, E>&,
		std::size_t actual_vertex_id,
		std::size_t end_vertex_id)> heuristics,
	std::function<double(const E&)> getEdgeLength)
{
	// ...
}

#endif // ASTAR_HPP
