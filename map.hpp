#ifndef MAP_HPP
#define MAP_HPP

#include "binaryTree.hpp"

#include <cstdint>
#include <utility>

// Uwaga! Kod powinien być odporny na błędy i każda z metod jeżeli zachodzi
// niebezpieczeństwo wywołania z niepoprawnymi parametrami powinna zgłaszać
// odpowiednie wyjątki!

// klasa reprezentująca słownik (mnap) oparty na drzewie BST/AVL
// K - typ kluczy
// V - typ wartosci
template <typename K, typename V>
class Map {
public:
	class Iterator {
	public:
		bool operator==(const Iterator& it) const;
		bool operator!=(const Iterator& it) const;
		Iterator& operator++();
		Iterator operator++(int);
		V& operator*() const;
		V* operator->() const;
	};

	class ConstIterator {
	public:
		bool operator==(const ConstIterator& it) const;
		bool operator!=(const ConstIterator& it) const;
		ConstIterator& operator++();
		ConstIterator operator++(int);
		const V& operator*() const;
		const V* operator->() const;
	};

public:
	Map();
	Map(const Map<K, V>& source);
	Map(Map<K, V>&& source);
	Map<K, V>& operator=(const Map<K, V>& source);
	Map<K, V>& operator=(Map<K, V>&& source);
	~Map();

	// zwraca ilość elementów
	// O(1)
	std::size_t size() const;
	// dodaje klucz i wartość - zwraca "Iterator" do dodanej wartości i true,
	// lub "Iterator" do istniejącej wartości i false
	std::pair<Iterator, bool> insert(const std::pair<K, V>& key_value);
	// wyszukuje element o podanym kluczu - jeżeli element został znaleziony to
	// zwraca "ConstIterator" do znalezionej wartości, jeżeli nie to zwraca to
	// samo co end()
	ConstIterator find(const K& key) const;
	// wyszukuje element o podanym kluczu - jeżeli element został znaleziony to
	// zwraca "Iterator" do znalezionej wartości, jeżeli nie to zwraca to samo
	// co end()
	Iterator find(const K& key);
	// wyszukuje element o podanym kluczu - jeżeli element został znaleziony to
	// zwraca referencję do znalezionej wartości, jeżeli nie to dodaje nowy
	// element o podanym kluczu i domyślnej wartości V() i zwraca referencję do
	// wartości
	V& operator[](const K& key);
	// usuwa element o podanej wartości - jeżeli element został usunięty to
	// zwraca "Iterator" na kolejny element, jeżeli elementu o podanej wartości
	// nie udało się odnaleźć to zwraca to samo co "end()"
	Iterator remove(const K& key);
	// usuwa wszystkie elementy
	void clear();

	// zwraca "ConstIterator" na pierwszy element
	ConstIterator begin() const;
	// zwraca "Iterator" na pierwszy element
	Iterator begin();
	// zwraca "ConstIterator" "za ostatni" element
	ConstIterator end() const;
	// zwraca "Iterator" "za ostatni" element
	Iterator end();
};

#endif // MAP_HPP
