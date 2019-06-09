#include "map.hpp"
#include <iostream>
#include <string>

using namespace std;

template <typename Iter>
void printDebugInfo(const std::pair<Iter, bool>& insert_info)
{
	std::cout
		<< (insert_info.second ? "Dodano " + *insert_info.first
							   : "Nie dodano ;/")
		<< std::endl;
}

template <typename Iter>
void printDebugInfo(const Iter& find_info, const Iter& end)
{
	std::cout << ((find_info != end) ? "Znaleziono " + *find_info : "Nie znaleziono ;/")
			  << std::endl;
}

template <typename K, typename V>
void printSetSizeAndElems(Map<K, V>& m)
{
	std::cout << "Size: " << m.size() << std::endl;
	for (auto& e : m)
		std::cout << e << ", ";
	std::cout << std::endl;
}

template <typename K, typename V>
void printSetSizeAndElems(const Map<K, V>& m)
{
	std::cout << "Size: " << m.size() << std::endl;
	for (const auto& e : m)
		std::cout << e << ", ";
	std::cout << std::endl;
}

int main()
{
	Map<int, std::string> m;
	printDebugInfo(m.insert({8, "eight"}));
	printDebugInfo(m.insert({4, "four"}));
	printDebugInfo(m.insert({10, "ten"}));
	printDebugInfo(m.insert({10, "ten"}));
	printDebugInfo(m.insert({2, "two"}));
	printDebugInfo(m.insert({5, "five"}));
	printDebugInfo(m.insert({9, "nine"}));
	printDebugInfo(m.insert({12, "twelve"}));
	printDebugInfo(m.insert({12, "twelve"}));
	printDebugInfo(m.insert({1, "one"}));
	printDebugInfo(m.insert({14, "fourteen"}));
	printDebugInfo(m.insert({7, "seven"}));
	{
		auto m2 = m;
		printSetSizeAndElems(m2);
	}
	auto it = m.find(10);
	*it = "10 (poprawione)";
	printDebugInfo(m.find(5), m.end());
	m.remove(4);
	printDebugInfo(m.find(5), m.end());
	m.remove(12);
	m.remove(14);
	printDebugInfo(m.insert({0, "zero"}));
	printDebugInfo(m.insert({16, "sixteen"}));
	{
		Map<int, std::string> m3;
		m3 = m;
		printSetSizeAndElems(m3);
	}
	m.remove(8);
	printSetSizeAndElems(m);
	const auto m4 = m;
	printSetSizeAndElems(m4);
	auto it2 = m4.find(10);
	// *it2 = "nie powinno sie skompilowac";
	// m4[2] = "nie powinno sie skompilowac";
	m[2] = "two (poprawione)";
	m[20] = "twenty";
	printSetSizeAndElems(m);

	return 0;
}
