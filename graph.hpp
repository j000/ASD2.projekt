#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstdint>
#include <iostream>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

// Uwaga! Kod powinien być odporny na błędy i każda z metod jeżeli zachodzi
// niebezpieczeństwo wywołania z niepoprawnymi parametrami powinna zgłaszac
// odpowiednie wyjątki!

// klasa reprezentująca graf skierowany oparty na MACIERZY SĄSIEDZTWA
// V - dane przechowywane przez wierzcholki
// E - dane przechowywane przez krawedzie (etykiety)
template <typename V, typename E>
class Graph {
public:
	// iterator po wierzchołkach (rosnąco po id wierzchołków)
	class VerticesIterator {
	public:
		bool operator==(const VerticesIterator& vi2) const;
		bool operator!=(const VerticesIterator& vi2) const;
		VerticesIterator& operator++();
		VerticesIterator operator++(int);
		V& operator*() const;
		V* operator->() const;
		// zwraca id aktualnego wierzchołka
		std::size_t id() const;
	};

	// iterator po istniejących krawędziach
	class EdgesIterator {
	public:
		bool operator==(const EdgesIterator& ei) const;
		bool operator!=(const EdgesIterator& ei) const;
		EdgesIterator& operator++();
		EdgesIterator operator++(int);
		E& operator*() const;
		E* operator->() const;
		// zwraca id wierzchołka startowego
		std::size_t v1id() const;
		// zwraca id wierzchołka koncowego
		std::size_t v2id() const;
	};

	// iterator po wierzchołkach w kolejności DFS
	class DFSIterator {
	public:
		bool operator==(const DFSIterator& dfsi) const;
		bool operator!=(const DFSIterator& dfsi) const;
		DFSIterator& operator++();
		DFSIterator operator++(int);
		V& operator*() const;
		V* operator->() const;
	};

	// iterator po wierzchołkach w kolejności BFS
	class BFSIterator {
	public:
		bool operator==(const BFSIterator& dfsi) const;
		bool operator!=(const BFSIterator& dfsi) const;
		BFSIterator& operator++();
		BFSIterator operator++(int);
		V& operator*() const;
		V* operator->() const;
	};

public:
	Graph();
	Graph(const Graph<V, E>& source);
	Graph(Graph<V, E>&& source);
	Graph& operator=(const Graph<V, E>& source);
	Graph& operator=(Graph<V, E>&& source);
	~Graph();

	// dodaje nowy wierzchołek z danymi przyjętymi w argumencie (wierzchołek
	// powinien posiadać kopie danych) i zwraca "VerticesIterator" na nowo
	// utworzony wierzchołek
	VerticesIterator insertVertex(const V& vertex_data);
	// dodaje nową krawędź między wierzchołkami o id "vertex1_id" i "vertex2_id"
	// i zwraca "EdgesIterator" na nowo dodaną krawędź jeśli krawędź między
	// podanymi wierzchołkami już istnieje to działanie funkcji zależy od
	// ostatniego argumentu jeśli ostatni argument przyjmuje wartość "true" to
	// krawędź zostaje zastąpiona, jeśli "false" to nie
	std::pair<EdgesIterator, bool> insertEdge(
		std::size_t vertex1_id,
		std::size_t vertex2_id,
		const E& label = E(),
		bool replace = true);
	// usuwa wierzchołek o podanym id i zwraca "VerticesIterator" na kolejny
	// wierzchołek, lub to samo co "endVertices()" w przypadku usunięcia
	// ostatniego wierzchołka, lub braku wierzchołka o podanym id
	VerticesIterator removeVertex(std::size_t vertex_id);
	// usuwa krawedź między dwoma wierzchołkami o podanych id i zwraca
	// "EdgesIterator" na kolejną krawędź, lub to samo co "endEdges()" w
	// przypadku usunięcia ostatniej krawedzi, lub braku krawędzi między
	// wierzchołkami o podanych id
	EdgesIterator removeEdge(std::size_t vertex1_id, std::size_t vertex2_id);
	// zwraca true jeśli istnieje krawędź między wierzchołkami o podanych id,
	// false w przeciwnym razie O(1)
	bool edgeExist(std::size_t vertex1_id, std::size_t vertex2_id) const;
	// zwraca ilość wierzchołków w grafie
	// O(1)
	std::size_t nrOfVertices() const;
	// zwraca ilość krawędzi w grafie
	// O(1)
	std::size_t nrOfEdges() const;
	// drukuje macierz sąsiedztwa na konsoli (debug)
	//    void printNeighborhoodMatrix() const;
	// zwraca "VerticesIterator" do wierzchołka o podanym id, lub to samo co
	// "endVertices()" w przypadku braku wierzchołka o podanym id
	VerticesIterator vertex(std::size_t vertex_id);
	// zwraca referencję do danych wierzchołka o podanym id
	const V& vertexData(std::size_t vertex_id) const;
	// zwraca referencję do danych wierzchołka o podanym id
	V& vertexData(std::size_t vertex_id);
	// zwraca "EdgesIterator" do krawędzi pomiędzy wierzchołkami o podanych id,
	// lub to samo co "endEdges()" w przypadku braku krawędzi między
	// wierzchołkami o podanych id
	EdgesIterator edge(std::size_t vertex1_id, std::size_t vertex2_id);
	// zwraca referencję do danych (etykiety) krawędzi pomiędzy wierzchołkami o
	// podanych id
	const E& edgeLabel(std::size_t vertex1_id, std::size_t vertex2_id) const;
	// zwraca referencję do danych (etykiety) krawędzi pomiędzy wierzchołkami o
	// podanych id
	E& edgeLabel(std::size_t vertex1_id, std::size_t vertex2_id);
	VerticesIterator begin()
	{
		return beginVertices();
	}
	VerticesIterator end()
	{
		return endVertices();
	}
	// zwraca "VerticesIterator" na pierwszy wierzchołek (o najmniejszym id)
	VerticesIterator beginVertices();
	// zwraca "VerticesIterator" "za ostatni" wierzchołek
	VerticesIterator endVertices();
	// zwraca "EdgesIterator" na pierwszą krawędz
	EdgesIterator beginEdges();
	// zwraca "EdgesIterator" "za ostatnią" krawędz
	EdgesIterator endEdges();
	// zwraca "DFSIterator" na wierzcholek o podanym id
	DFSIterator beginDFS(std::size_t vertex_id);
	// zwraca "DFSIterator" "za ostatni" wierzcholek
	DFSIterator endDFS();
	// zwraca "BFSIterator" na wierzcholek o podanym id
	BFSIterator beginBFS(std::size_t vertex_id);
	// zwraca "BFSIterator" "za ostatni" wierzcholek
	BFSIterator endBFS();
};

#endif // GRAPH_HPP
