#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

// LLVM
#include "PointerIntPair.h"

template <typename T>
class BinaryTree {
	enum : bool { THREAD = 0, NO_THREAD };
public:
	class Node;
	class Iterator;

	using NodePtr = llvm::PointerIntPair<Node*, 1, bool>;

	static bool isThread(const NodePtr&) noexcept;

	BinaryTree() = default;
	~BinaryTree();
	BinaryTree(BinaryTree&&) noexcept = default;
	BinaryTree& operator=(BinaryTree&&) noexcept = default;
	BinaryTree(const BinaryTree&);
	BinaryTree& operator=(const BinaryTree&);

	void swap(BinaryTree& other);

	Iterator insert(const T&);
	Iterator insert(T&&);

	Iterator remove(const T&);

	Iterator find(const T&) const noexcept;

	void prettyPrint() const noexcept;
	void inorder(const std::function<void(const T&)>&) const noexcept;
	void preorder(const std::function<void(const T&)>&) const noexcept;
	void postorder(const std::function<void(const T&)>&) const noexcept;

	Iterator begin() const;
	Iterator end() const;
	Iterator rbegin() const;
	Iterator rend() const;

	const T& minimum() const;
	const T& maximum() const;
	size_t size() const noexcept;
	size_t depth() const noexcept;

private:
	void copyHelper(Node* const);

	Iterator insertLeft(NodePtr&, Node*, Node*, size_t = 1) noexcept;
	Iterator insertRight(NodePtr&, Node*, Node*, size_t = 1) noexcept;

	void inorder(const Node*, const std::function<void(const T&)>&) const
		noexcept;
	void preorder(const Node*, const std::function<void(const T&)>&) const
		noexcept;
	void postorder(const Node*, const std::function<void(const T&)>&) const
		noexcept;
	void prettyPrint(const Node*, const std::string&, const std::string&) const
		noexcept;

	NodePtr m_root{nullptr};
	size_t m_size{0};
	size_t m_depth{0};
};

template <typename T>
void swap(BinaryTree<T>& lhs, BinaryTree<T>& rhs)
{
	lhs.swap(rhs);
}

////////////////////////////////////////

template <typename T>
class BinaryTree<T>::Node {
	friend BinaryTree<T>;

public:
	Node(const T value) : m_value{value}
	{
	}
	~Node() = default;
	// Node(const Node&) = default;
	Node& operator=(const Node&) = default;
	Node(Node&&) = default;
	Node& operator=(Node&&) = default;

	const T& value() const noexcept;
	Node* leftChild() const noexcept;
	Node* rightChild() const noexcept;
	Node* previous() const noexcept;
	Node* next() const noexcept;

private:
	NodePtr& leftPointer() noexcept;
	NodePtr& rightPointer() noexcept;
	bool isLeftThread() const noexcept;
	bool isRightThread() const noexcept;

	T m_value{};
	NodePtr m_leftChild{nullptr};
	NodePtr m_rightChild{nullptr};
};

////////////////////////////////////////

template <typename T>
class BinaryTree<T>::Iterator {
	friend BinaryTree<T>;

public:
	Iterator() = default;
	Iterator(BinaryTree<T>::Node* const& copy) : m_ptr{copy}
	{
	}
	Iterator(BinaryTree<T>::Node*&& move) : m_ptr{std::move(move)}
	{
	}
	Iterator(Iterator&&) = default;
	Iterator& operator=(Iterator&&) = default;
	Iterator(const Iterator&) = default;
	Iterator& operator=(const Iterator&) = default;

	bool operator==(const Iterator& it) const
	{
		return m_ptr == it.m_ptr;
	};
	bool operator!=(const Iterator& it) const
	{
		return m_ptr != it.m_ptr;
	}
	Iterator& operator++()
	{
		if (m_ptr)
			m_ptr = m_ptr->next();
		return *this;
	};
	Iterator operator++(int) const
	{
		auto tmp = *this;
		++(*this);
		return tmp;
	};
	Iterator& operator--()
	{
		if (m_ptr)
			m_ptr = m_ptr->previous();
		return *this;
	};
	Iterator operator--(int) const
	{
		auto tmp = *this;
		--(*this);
		return tmp;
	};
	const T& operator*() const
	{
		return m_ptr->value();
	}
	const T* operator->() const
	{
		return &(m_ptr->value());
	}

private:
	BinaryTree<T>::Node* m_ptr{nullptr};
};

////////////////////////////////////////
// Node implementation

template <typename T>
inline const T& BinaryTree<T>::Node::value() const noexcept
{
	return m_value;
}

template <typename T>
inline typename BinaryTree<T>::Node* BinaryTree<T>::Node::leftChild() const
	noexcept
{
	if (isThread(m_leftChild))
		return nullptr;
	return m_leftChild.getPointer();
}

template <typename T>
inline typename BinaryTree<T>::Node* BinaryTree<T>::Node::rightChild() const
	noexcept
{
	if (isThread(m_rightChild))
		return nullptr;
	return m_rightChild.getPointer();
}

template <typename T>
inline typename BinaryTree<T>::Node* BinaryTree<T>::Node::previous() const
	noexcept
{
	if (BinaryTree::isThread(m_leftChild))
		return m_leftChild.getPointer();

	auto current{m_leftChild.getPointer()};
	while (current->rightChild()) {
		current = current->rightChild();
	}
	return current;
}

template <typename T>
inline typename BinaryTree<T>::Node* BinaryTree<T>::Node::next() const noexcept
{
	if (BinaryTree::isThread(m_rightChild))
		return m_rightChild.getPointer();

	auto current{m_rightChild.getPointer()};
	while (current->leftChild()) {
		current = current->leftChild();
	}
	return current;
}

template <typename T>
inline typename BinaryTree<T>::NodePtr&
BinaryTree<T>::Node::leftPointer() noexcept
{
	return m_leftChild;
}

template <typename T>
inline typename BinaryTree<T>::NodePtr&
BinaryTree<T>::Node::rightPointer() noexcept
{
	return m_rightChild;
}

////////////////////////////////////////
// BinaryTree implementation

template <typename T>
bool BinaryTree<T>::isThread(
	const typename BinaryTree<T>::NodePtr& ptr) noexcept
{
	return ptr.getInt() == BinaryTree::THREAD;
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
	if (!m_root.getPointer())
		return;
	std::vector<Node*> queue{m_root.getPointer()};
	while (!queue.empty()) {
		auto current = queue.back();
		queue.pop_back();
		if (auto tmp = current->leftChild())
			queue.push_back(tmp);
		if (auto tmp = current->rightChild())
			queue.push_back(tmp);
		delete current;
	}
}

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& copy)
{
	if (copy.m_size == 0)
		return;
	insert(copy.m_root.getPointer()->value());
	copyHelper(copy.m_root.getPointer());
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& copy)
{
	using std::swap;
	auto tmp(copy);
	swap(tmp, *this);
	return *this;
}

template <typename T>
void BinaryTree<T>::copyHelper(Node* const copy)
{
	auto left = copy->leftChild();
	if (left) {
		insert(left->value());
		copyHelper(left);
	}
	auto right = copy->rightChild();
	if (right) {
		insert(right->value());
		copyHelper(right);
	}
}

template <typename T>
void BinaryTree<T>::swap(BinaryTree& other)
{
	using std::swap; // not needed now, just in case
	swap(m_root, other.m_root);
	swap(m_size, other.m_size);
	swap(m_depth, other.m_depth);
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::insert(const T& x)
{
	auto tmp{new Node(x)};
	return insertLeft(m_root, tmp, nullptr);
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::insert(T&& x)
{
	auto tmp{new Node(std::forward<T>(x))};
	return insertLeft(m_root, tmp, nullptr);
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::remove(const T& x)
{
	auto tmp = find(x).m_ptr;
	if (!tmp)
		return end();

	if (tmp->rightChild()) {
		// we have both children or only right
		auto val = tmp->next()->value();
		remove(val);
		tmp->m_value = val;
		return tmp;
	} else if (tmp->leftChild()) {
		// has only left child
		auto val = tmp->previous()->value();
		remove(val);
		tmp->m_value = val;
		return tmp;
	} else {
		// deleting leaf node
		if (tmp->previous() && tmp->previous()->rightChild() == tmp) {
			// we are right child
			auto parent = tmp->previous();
			parent->m_rightChild = tmp->m_rightChild;
			delete tmp;
			return parent;
		} else {
			// we are left child
			auto parent = tmp->next();
			parent->m_leftChild = tmp->m_leftChild;
			delete tmp;
			return parent;
		}
	}
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::find(const T& x) const noexcept
{
	auto tmp{m_root.getPointer()};
	while (tmp != nullptr) {
		if (tmp->value() == x)
			return tmp;
		if (tmp->value() > x)
			tmp = tmp->leftChild();
		else
			tmp = tmp->rightChild();
	}

	return end();
}

template <typename T>
void BinaryTree<T>::prettyPrint() const noexcept
{
	std::string tmp{""};
	prettyPrint(m_root.getPointer(), std::string{""}, tmp);
}

template <typename T>
void BinaryTree<T>::prettyPrint(
	const Node* const node,
	const std::string& sp,
	const std::string& sn) const noexcept
{
#if 1
	static const std::string cc{"    "};
	static const std::string cr{"┌───"};
	static const std::string cp{"│   "};
	static const std::string cl{"└───"};
#else
	static const std::string cc{"   "};
	static const std::string cr{",--"};
	static const std::string cp{"|  "};
	static const std::string cl{"`--"};
#endif

	if (!node)
		return;

	std::string s;

	s = sp;
	if (&sn == &cr) {
		s = s.substr(0, s.length() - cp.length());
		s += cc;
	}
	prettyPrint(node->rightChild(), s + cp, cr);

	s = sp.substr(0, sp.length() - cp.length());
	std::cout << "\033[2;34m" << s << sn << "\033[22;39m" << node->value()
			  << std::endl;

	s = sp;
	if (&sn == &cl) {
		s = s.substr(0, s.length() - cp.length());
		s += cc;
	}
	prettyPrint(node->leftChild(), s + cp, cl);
}

template <typename T>
inline void BinaryTree<T>::inorder(const std::function<void(const T&)>& f) const
	noexcept
{
	inorder(m_root.getPointer(), f);
}

template <typename T>
void BinaryTree<T>::inorder(
	const Node* const node,
	const std::function<void(const T&)>& f) const noexcept
{
	if (!node)
		return;
	inorder(node->leftChild(), f);
	f(node->value());
	inorder(node->rightChild(), f);
}

template <typename T>
inline void
BinaryTree<T>::preorder(const std::function<void(const T&)>& f) const noexcept
{
	preorder(m_root, f);
}

template <typename T>
void BinaryTree<T>::preorder(
	const Node* const node,
	const std::function<void(const T&)>& f) const noexcept
{
	if (!node)
		return;
	f(node->value());
	preorder(node->leftChild(), f);
	preorder(node->rightChild(), f);
}

template <typename T>
inline void
BinaryTree<T>::postorder(const std::function<void(const T&)>& f) const noexcept
{
	postorder(m_root, f);
}

template <typename T>
void BinaryTree<T>::postorder(
	const Node* const node,
	const std::function<void(const T&)>& f) const noexcept
{
	if (!node)
		return;
	postorder(node->leftChild(), f);
	postorder(node->rightChild(), f);
	f(node->value());
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::begin() const
{
	if (m_size == 0)
		return Iterator();
	auto tmp = m_root.getPointer();
	while (tmp->leftChild()) {
		tmp = tmp->leftChild();
	}
	return Iterator{tmp};
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::end() const
{
	return Iterator(nullptr);
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::rbegin() const
{
	if (m_size == 0)
		return Iterator();
	auto tmp = m_root.getPointer();
	while (tmp->rightChild()) {
		tmp = tmp->rightChild();
	}
	return Iterator{tmp};
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::rend() const
{
	return Iterator(nullptr);
}

template <typename T>
const T& BinaryTree<T>::minimum() const
{
	if (!m_root.getPointer())
		throw std::out_of_range("Drzewo puste");

	auto tmp{m_root.getPointer()};

	while (tmp->leftChild())
		tmp = tmp->leftChild();

	return tmp->value();
}

template <typename T>
const T& BinaryTree<T>::maximum() const
{
	if (!m_root.getPointer())
		throw std::out_of_range("Drzewo puste");

	auto tmp{m_root.getPointer()};

	while (tmp->rightChild())
		tmp = tmp->rightChild();

	return tmp->value();
}

template <typename T>
inline size_t BinaryTree<T>::size() const noexcept
{
	return m_size;
}

template <typename T>
inline size_t BinaryTree<T>::depth() const noexcept
{
	return m_depth;
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::insertLeft(
	NodePtr& parent,
	Node* const newNode,
	Node* const previous,
	size_t level) noexcept
{
	if (isThread(parent)) {
		++m_size;
		if (level > m_depth)
			m_depth = level;
		newNode->m_leftChild = parent;
		newNode->m_rightChild.setPointerAndInt(previous, BinaryTree::THREAD);
		parent.setPointerAndInt(newNode, BinaryTree::NO_THREAD);
		return Iterator{newNode};
	}
	if (parent.getPointer()->value() > newNode->value()) {
		return insertLeft(
			parent.getPointer()->leftPointer(),
			newNode,
			parent.getPointer(),
			level + 1);
	}
	return insertRight(
		parent.getPointer()->rightPointer(),
		newNode,
		parent.getPointer(),
		level + 1);
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::insertRight(
	NodePtr& parent,
	Node* const newNode,
	Node* const next,
	size_t level) noexcept
{
	if (isThread(parent)) {
		++m_size;
		if (level > m_depth)
			m_depth = level;
		newNode->m_rightChild = parent;
		newNode->m_leftChild.setPointerAndInt(next, BinaryTree::THREAD);
		parent.setPointerAndInt(newNode, BinaryTree::NO_THREAD);
		return Iterator{newNode};
	}
	if (parent.getPointer()->value() > newNode->value()) {
		return insertLeft(
			parent.getPointer()->leftPointer(),
			newNode,
			parent.getPointer(),
			level + 1);
	}
	return insertRight(
		parent.getPointer()->rightPointer(),
		newNode,
		parent.getPointer(),
		level + 1);
}

////////////////////////////////////////

#endif /* BINARYTREE_HPP */
