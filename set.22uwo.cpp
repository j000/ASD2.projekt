#include "set.hpp"
#include <iostream>
#include <string>

using namespace std;

template <typename Iter>
void printDebugInfo(const std::pair<Iter, bool>& insert_info)
{
	std::cout
		<< (insert_info.second ? "Dodano " + std::to_string(*insert_info.first)
							   : "Nie dodano ;/")
		<< std::endl;
}

template <typename Iter>
void printDebugInfo(const Iter& find_info)
{
	std::cout
		<< (find_info ? "Znaleziono " + std::to_string(*find_info)
					  : "Nie znaleziono ;/")
		<< std::endl;
}

template <typename T>
void printSetSizeAndElems(Set<T>& s)
{
	std::cout << "Size: " << s.size() << std::endl;
	for (auto& e : s)
		std::cout << e << ", ";
	std::cout << std::endl;
}

template <typename T>
void printSetSizeAndElems(const Set<T>& s)
{
	std::cout << "Size: " << s.size() << std::endl;
	for (const auto& e : s)
		std::cout << e << ", ";
	std::cout << std::endl;
}

int main()
{
	Set<int> s;
	printDebugInfo(s.insert(8));
	printDebugInfo(s.insert(4));
	printDebugInfo(s.insert(10));
	printDebugInfo(s.insert(10));
	printDebugInfo(s.insert(2));
	printDebugInfo(s.insert(5));
	printDebugInfo(s.insert(9));
	printDebugInfo(s.insert(12));
	printDebugInfo(s.insert(12));
	printDebugInfo(s.insert(1));
	printDebugInfo(s.insert(14));
	printDebugInfo(s.insert(7));
	{
		auto s2 = s;
		printSetSizeAndElems(s2);
	}
	auto it = s.find(10);
	// *it = 2;
	printDebugInfo(s.find(5));
	s.remove(4);
	printDebugInfo(s.find(5));
	s.remove(12);
	s.remove(14);
	printDebugInfo(s.insert(0));
	printDebugInfo(s.insert(16));
	{
		Set<int> s3;
		s3 = s;
		printSetSizeAndElems(s3);
	}
	s.remove(8);
	printSetSizeAndElems(s);
	const auto s4 = s;
	printSetSizeAndElems(s4);

	return 0;
}
