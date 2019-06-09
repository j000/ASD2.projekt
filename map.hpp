#ifndef MAP_HPP
#define MAP_HPP

#include "BinaryTree.hpp"

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
	class MapElem;
	class Iterator;
	class ConstIterator;

	Map() = default;
	Map(const Map<K, V>&) = default;
	Map(Map<K, V>&&) = default;
	Map<K, V>& operator=(const Map<K, V>&) = default;
	Map<K, V>& operator=(Map<K, V>&&) = default;
	~Map() = default;

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

private:
	BinaryTree<MapElem> m_tree{};
};

////////////////////////////////////////

template <typename K, typename V>
class Map<K, V>::MapElem {
public:
	MapElem() = default;
	MapElem(const MapElem&) = default;
	MapElem(MapElem&&) = default;
	MapElem(const K& key, const V& value) : m_key{key}, m_value{value}
	{
	}
	MapElem(const K& key) : m_key{key}
	{
	}

	MapElem& operator=(const MapElem&) = default;
	MapElem& operator=(MapElem&&) = default;
	MapElem& operator=(const V& value);

	bool operator==(const MapElem& other) const;
	bool operator<(const MapElem& other) const;
	bool operator>(const MapElem& other) const;

	friend std::ostream& operator<<(std::ostream& os, const MapElem& me)
	{
		os << "(" << me.m_key << ", " << me.m_value << ")";
		return os;
	}

	K m_key{};
	V m_value{};
};

////////////////////////////////////////

template <typename K, typename V>
class Map<K, V>::Iterator {
public:
	Iterator(const Iterator&) = default;
	Iterator(const typename BinaryTree<MapElem>::Iterator& copy)
		: m_iterator{copy}
	{
	}
	Iterator& operator=(const Iterator&) = default;
	Iterator(Iterator&&) = default;
	Iterator& operator=(Iterator&&) = default;
	~Iterator() = default;

	bool operator==(const Iterator& it) const;
	bool operator!=(const Iterator& it) const;
	Iterator& operator++();
	Iterator operator++(int);
	V& operator*() const;
	V* operator->() const;

private:
	typename BinaryTree<MapElem>::Iterator m_iterator{};
};

////////////////////////////////////////

template <typename K, typename V>
class Map<K, V>::ConstIterator {
public:
	ConstIterator(const ConstIterator&) = default;
	ConstIterator(const typename BinaryTree<MapElem>::Iterator& copy)
		: m_iterator{copy}
	{
	}
	ConstIterator& operator=(const ConstIterator&) = default;
	ConstIterator(ConstIterator&&) = default;
	ConstIterator& operator=(ConstIterator&&) = default;
	~ConstIterator() = default;

	bool operator==(const ConstIterator& it) const;
	bool operator!=(const ConstIterator& it) const;
	ConstIterator& operator++();
	ConstIterator operator++(int);
	V& operator*() const;
	V* operator->() const;

private:
	typename BinaryTree<MapElem>::Iterator m_iterator{};
};

////////////////////////////////////////
// Map::MapElem implementation
////////////////////////////////////////

template <typename K, typename V>
typename Map<K, V>::MapElem& Map<K, V>::MapElem::operator=(const V& value)
{
	m_value = value;
	return *this;
}

template <typename K, typename V>
bool Map<K, V>::MapElem::operator==(const MapElem& other) const
{
	return m_key == other.m_key;
}

template <typename K, typename V>
bool Map<K, V>::MapElem::operator<(const MapElem& other) const
{
	return m_key < other.m_key;
}

template <typename K, typename V>
bool Map<K, V>::MapElem::operator>(const MapElem& other) const
{
	return m_key > other.m_key;
}

////////////////////////////////////////
// Map::Iterator implementation
////////////////////////////////////////

template <typename K, typename V>
bool Map<K, V>::Iterator::operator==(const Iterator& it) const
{
	return m_iterator == it.m_iterator;
}

template <typename K, typename V>
bool Map<K, V>::Iterator::operator!=(const Iterator& it) const
{
	return m_iterator != it.m_iterator;
}

template <typename K, typename V>
typename Map<K, V>::Iterator& Map<K, V>::Iterator::operator++()
{
	++m_iterator;
	return *this;
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::Iterator::operator++(int)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template <typename K, typename V>
V& Map<K, V>::Iterator::operator*() const
{
	return const_cast<V&>((*m_iterator).m_value);
}

template <typename K, typename V>
V* Map<K, V>::Iterator::operator->() const
{
	return &((*m_iterator).m_value);
}

////////////////////////////////////////
// Map::ConstIterator implementation
////////////////////////////////////////

template <typename K, typename V>
bool Map<K, V>::ConstIterator::operator==(const ConstIterator& it) const
{
	return m_iterator == it.m_iterator;
}

template <typename K, typename V>
bool Map<K, V>::ConstIterator::operator!=(const ConstIterator& it) const
{
	return m_iterator != it.m_iterator;
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator& Map<K, V>::ConstIterator::operator++()
{
	++m_iterator;
	return *this;
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::ConstIterator::operator++(int)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template <typename K, typename V>
V& Map<K, V>::ConstIterator::operator*() const
{
	return const_cast<V&>(m_iterator->m_value);
}

template <typename K, typename V>
V* Map<K, V>::ConstIterator::operator->() const
{
	return &(m_iterator->value);
}

////////////////////////////////////////
// Map implementation
////////////////////////////////////////

template <typename K, typename V>
std::size_t Map<K, V>::size() const
{
	return m_tree.size();
}

template <typename K, typename V>
std::pair<typename Map<K, V>::Iterator, bool>
Map<K, V>::insert(const std::pair<K, V>& key_value)
{
	MapElem x{key_value.first, key_value.second};
	auto tmp = m_tree.find(x);
	if (tmp != m_tree.end())
		return std::make_pair(tmp, false);
	return std::make_pair(m_tree.insert(x), true);
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::find(const K& key) const
{
	return m_tree.find(key);
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key)
{
	return m_tree.find(key);
}

template <typename K, typename V>
V& Map<K, V>::operator[](const K& key)
{
	auto tmp = m_tree.find(key);
	if (tmp != m_tree.end())
		return const_cast<V&>((*tmp).m_value);
	return const_cast<V&>((*m_tree.insert(key)).m_value);
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::remove(const K& key)
{
	return m_tree.remove(key);
}

template <typename K, typename V>
void Map<K, V>::clear()
{
	decltype(m_tree) tmp{};
	swap(tmp, m_tree);
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::begin() const
{
	return ConstIterator{m_tree.begin()};
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin()
{
	return Iterator{m_tree.begin()};
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::end() const
{
	return ConstIterator{m_tree.end()};
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end()
{
	return Iterator{m_tree.end()};
}

#endif // MAP_HPP
