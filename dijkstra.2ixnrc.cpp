#include "dijkstra.hpp"
#include "graph.hpp"
#include <functional>
#include <iostream>

using namespace std;

int main()
{
	Graph<std::string, double> g;

	for (std::size_t i = 0u; i < 6u; ++i) {
		g.insertVertex("data " + std::to_string(i));
	}

	for (std::size_t i = 0u; i < g.nrOfVertices(); ++i) {
		for (std::size_t j = 0u; j < g.nrOfVertices(); ++j) {
			if ((i + j) & 1u || i & 1u) {
				g.insertEdge(i, j, ((i != j) ? (i + j) / 2. : 1.));
			}
		}
	}
	g.insertEdge(0, 2, 4.);
	std::cout << (g.removeVertex(1) ? "Udalo sie" : "Nie udalo sie")
			  << std::endl;
	std::cout << (g.removeEdge(2, 2) ? "Udalo sie" : "Nie udalo sie")
			  << std::endl;
	std::cout << (g.removeEdge(2, 3) ? "Udalo sie" : "Nie udalo sie")
			  << std::endl;
	std::cout << (g.removeEdge(4, 3) ? "Udalo sie" : "Nie udalo sie")
			  << std::endl;
	std::cout << "Nr of vertices: " << g.nrOfVertices() << std::endl;
	std::cout << "Nr of edges: " << g.nrOfEdges() << std::endl;
	std::cout << std::endl;
	g.printNeighborhoodMatrix();
	std::cout << std::endl;
	std::cout << "Vertices data:" << std::endl;
	for (auto v_it = g.beginVertices(); v_it != g.endVertices(); ++v_it) {
		std::cout << *v_it << ", ";
	}
	std::cout << std::endl << std::endl;
	for (auto& v : g) {
		std::cout << v << ", ";
	}
	std::cout << std::endl << std::endl;
	std::cout << "Edges data:" << std::endl;
	for (auto e_it = g.beginEdges(); e_it != g.endEdges(); ++e_it) {
		std::cout << *e_it << ", ";
	}

	std::cout << std::endl << std::endl;
	std::cout << "DFS vertices data (begin from 1):" << std::endl;
	for (auto dfs_it = g.beginDFS(1); dfs_it != g.endDFS(); ++dfs_it) {
		std::cout << *dfs_it << ", ";
	}
	std::cout << std::endl << std::endl;
	std::cout << "DFS vertices data (begin from 3):" << std::endl;
	for (auto dfs_it = g.beginDFS(3); dfs_it != g.endDFS(); ++dfs_it) {
		std::cout << *dfs_it << ", ";
	}
	std::cout << std::endl << std::endl;

	std::cout << std::endl << std::endl;
	std::cout << "BFS vertices data (begin from 1):" << std::endl;
	for (auto dfs_it = g.beginBFS(1); dfs_it != g.endBFS(); ++dfs_it) {
		std::cout << *dfs_it << ", ";
	}
	std::cout << std::endl << std::endl;
	std::cout << "BFS vertices data (begin from 3):" << std::endl;
	for (auto dfs_it = g.beginBFS(3); dfs_it != g.endBFS(); ++dfs_it) {
		std::cout << *dfs_it << ", ";
	}
	std::cout << std::endl << std::endl;
	auto [shortest_path_distance, shortest_path]
		= dijkstra<std::string, double>(
			g, 2u, 4u, [](const double& e) -> double { return e; });
	std::cout << "Distance from 2 to 4: " << shortest_path_distance
			  << std::endl;
	std::cout << "Path from 2 to 4:" << std::endl;
	for (auto& v_id : shortest_path) {
		std::cout << v_id << ", ";
	}
	std::cout << std::endl;

	std::tie(shortest_path_distance, shortest_path)
		= dijkstra<std::string, double>(
			g, 1u, 0u, [](const double& e) -> double { return e; });
	std::cout << "Distance from 1 to 0: " << shortest_path_distance
			  << std::endl;
	std::cout << "Path from 1 to 0:" << std::endl;
	for (auto& v_id : shortest_path) {
		std::cout << v_id << ", ";
	}
	std::cout << std::endl;

	std::tie(shortest_path_distance, shortest_path)
		= dijkstra<std::string, double>(
			g, 3u, 0u, [](const double& e) -> double { return e; });
	std::cout << "Distance from 3 to 0: " << shortest_path_distance
			  << std::endl;
	std::cout << "Path from 3 to 0:" << std::endl;
	for (auto& v_id : shortest_path) {
		std::cout << v_id << ", ";
	}
	std::cout << std::endl;

	std::tie(shortest_path_distance, shortest_path)
		= dijkstra<std::string, double>(
			g, 3u, 1u, [](const double& e) -> double { return e; });
	std::cout << "Distance from 3 to 1: " << shortest_path_distance
			  << std::endl;
	std::cout << "Path from 3 to 1:" << std::endl;
	for (auto& v_id : shortest_path) {
		std::cout << v_id << ", ";
	}
	std::cout << std::endl;

	return 0;
}
