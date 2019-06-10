#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "Graph.hpp"
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>

// funkcja realizujacą algorytm dijkstry i zwracająca najkrótszą scieżkę
// (długość scieżki oraz indeksy wierzchołków wchodzących w jej skład - z
// początkowym i końcowym włącznie) pomiędzy wierzchołkami o indeksach
// "start_idx" i "end_idx" w przypadku gdy ścieżka nie istanieje zwraca długość
// równą 0 i brak indeksów ostatni argument (getEdgeLength) służy do przekazania
// funkcji/funktora/... realizującej pobranie długosci krawędzi (w najprostszym
// przypadku - gdy etykieta jest długością - zwraca etykietę krawędzi)
template <typename V, typename E>
std::pair<double, std::vector<std::size_t>> dijkstra(
	Graph<V, E>& graph,
	std::size_t start_idx,
	std::size_t end_idx,
	std::function<double(const E&)> getEdgeLength)
{
	return graph.dijkstra(start_idx, end_idx, getEdgeLength);
}

#endif // DIJKSTRA_HPP
