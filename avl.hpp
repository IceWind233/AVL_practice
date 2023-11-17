#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>
#include <memory>
#include <iterator>
#include <stack>
#include <initializer_list>

template <typename Ty_>
class AVL {
	struct Node;

	using pNode_ = std::shared_ptr<Node>;

	struct Node {
		Ty_ val;
		pNode_ left;
		pNode_ right;
		pNode_ parent;
		size_t depth;

		Node(const Ty_& val) :
			val(val),
			left(nullptr),
			right(nullptr),
			parent(nullptr),
			depth(0) {}

		~Node() = default;
	};

private:

	/*
	class Iterator {

		using iterator_category = std::forward_iterator_tag;
		using value_type = pNode_;
		using difference_type = std::ptrdiff_t;
		using pointer = pNode_*;
		using reference = pNode_&;

	public:
		Iterator() : ptr_(nullptr) {
			init();
		}

		Iterator(const pNode_& ptr) : ptr_(ptr) {
			init();
		}

		Iterator(const Iterator& ref) : stack_(ref.stack_), ptr_(ref.ptr_) {}

		Iterator& operator=(const Iterator& ref) {
			stack_ = ref.stack_;
			ptr_ = ref.ptr_;

			return *this;
		}

		Iterator(Iterator&& other) noexcept : ptr_(other.ptr_) {
			other.ptr_ = nullptr;

			init();
		}

		Iterator& operator=(Iterator&& other) noexcept {
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;

			return *this;
		}

		~Iterator() = default;

		Iterator& operator++() {
			auto ret = stack_.top();

			if (!stack_.empty()) {
				stack_.pop();
			}

			return *this;
		}

		Iterator operator++(int) = delete;

		bool operator==(const Iterator& ref) const {
			return ptr_ == ref.ptr_;
		}

		bool operator!=(const Iterator& ref) const {
			return ptr_ != ref.ptr_;
		}

		bool operator*() const {
			return ptr_->val;
		}
	private:

		void init() {
			do_init(ptr_);
		}

		void do_init(const pNode_& node) {
			if (node == nullptr) {
				return;
			}

			do_init(node->right);
			stack_.push(node);
			do_init(node->left);
		}

	private:

		std::stack<pNode_> stack_;
		pNode_ ptr_;
	};
	*/

public:
	AVL();

	AVL(Ty_&& val);

	AVL(const std::initializer_list<Ty_>& list);

	AVL(const AVL& ref);

	AVL& operator=(const AVL& ref);

	AVL(AVL&& other) noexcept;

	AVL& operator=(AVL&& other) noexcept;

	~AVL();

public:
	void insert(const Ty_& val);

	void erase(const Ty_& val);

	bool find(const Ty_& val) const;

	void clear();

	bool empty() const;

	size_t size() const;

	size_t height() const;

	/*
	Iterator begin() const;

	Iterator end() const;

	*/

	void print() const;

	friend void swap(AVL& lhs, AVL& rhs) noexcept {
		using std::swap;
		swap(lhs.root, rhs.root);
	}

private:

	pNode_ do_insert(const Ty_& val, pNode_& node);

	pNode_ rotate_l(pNode_& node);

	pNode_ rotate_r(pNode_& node);

	void do_copy(const AVL& src, AVL& tar);

	pNode_ update(Ty_ val, pNode_& node);

	pNode_ contain(const Ty_& val, const pNode_& node) const;

	void do_print(const pNode_& node) const;

	int get_balance(const pNode_& node);

	size_t get_size(const pNode_& node);

	size_t get_height(const pNode_ node);

	void erase_child(const Ty_& val, const pNode_& node);

	static bool is_less(const Ty_& lhs, const Ty_& rhs);

private:

	pNode_ root;

};

template <typename Ty_>
AVL<Ty_>::AVL() : root(nullptr) {}

template <typename Ty_>
AVL<Ty_>::AVL(Ty_&& val) : AVL() {
	root = std::make_shared<Node>(val);
}

template <typename Ty_>
AVL<Ty_>::AVL(const std::initializer_list<Ty_>& list) : AVL() {
	for (auto idx : list) {
		insert(idx);
	}
}

template <typename Ty_>
AVL<Ty_>::AVL(const AVL& ref) : AVL() {
	do_copy(ref, *this);
}

template <typename Ty_>
AVL<Ty_>& AVL<Ty_>::operator=(const AVL& ref) {
	if (this != ref) {
		do_copy(ref, *this);
	}

	return *this;
}

template <typename Ty_>
AVL<Ty_>::AVL(AVL&& other) noexcept : AVL() {
	if (this != other) {
		root = other.root;
		other.root = nullptr;
	}
}

template <typename Ty_>
AVL<Ty_>& AVL<Ty_>::operator=(AVL&& other) noexcept {
	if (this != other) {
		root = other.root;
		other = nullptr;
	}

	return *this;
}

template <typename Ty_>
AVL<Ty_>::~AVL() {
	clear();
}

template <typename Ty_>
void AVL<Ty_>::insert(const Ty_& val) {
	do_insert(val, root);
}

template <typename Ty_>
void AVL<Ty_>::erase(const Ty_& val) {
	auto node = contain(val, root);
	if (node == nullptr) {
		return;
	}

	erase_child(val, node);
}

template <typename Ty_>
bool AVL<Ty_>::find(const Ty_& val) const {
	return contain(val, root) != nullptr ;
}

template <typename Ty_>
void AVL<Ty_>::clear() {
	root.reset();
}

template <typename Ty_>
bool AVL<Ty_>::empty() const {
	return root == nullptr;
}

template <typename Ty_>
size_t AVL<Ty_>::size() const {
	return get_size(root);
}

template <typename Ty_>
size_t AVL<Ty_>::height() const {
	return get_height(root);
}

/*

template <typename Ty_>
typename AVL<Ty_>::Iterator AVL<Ty_>::begin() const {
	if (root) {
		pNode_ tmp = root;
		while (tmp->left) {
			tmp = tmp->left;
		}

		return Iterator(tmp);
	}

	return Iterator();
}

template <typename Ty_>
typename AVL<Ty_>::Iterator AVL<Ty_>::end() const {
	return Iterator();
}

*/

template <typename Ty_>
void AVL<Ty_>::print() const {
	do_print(root);
	std::cout << std::endl;
}

template <typename Ty_>
typename AVL<Ty_>::pNode_ AVL<Ty_>::do_insert(const Ty_& val, pNode_& node) {
	if (node == nullptr) {
		node = std::make_shared<Node>(val);
		return node;
	}

	if (is_less(val, node->val)) {
		node->left = do_insert(val, node->left);
		node->left->parent = node;
	} else if (is_less(node->val, val)) {
		node->right = do_insert(val, node->right);
		node->right->parent = node;
	} else {
		return node;
	}

	node->depth = std::max(get_height(node->left), get_height(node->right)) + 1;

	return update(val, node);
}

template <typename Ty_>
int AVL<Ty_>::get_balance(const pNode_& node) {
	if (node == nullptr) {
		return 0;
	}

	return get_height(node->left) - get_height(node->right);
}

template <typename Ty_>
typename AVL<Ty_>::pNode_ AVL<Ty_>::rotate_l(pNode_& node) {
	auto right = node->right;
	node->right = right->left;
	right->left = node;

	node->depth = std::max(get_height(node->left), get_height(node->right)) + 1;
	right->depth = std::max(get_height(right->left), get_height(right->right)) + 1;

	if (node == root) {
		root = right;
	}

	return right;
}

template <typename Ty_>
typename AVL<Ty_>::pNode_ AVL<Ty_>::rotate_r(pNode_& node) {
	auto left = node->left;
	node->left = left->right;
	left->right= node;

	node->depth = std::max(get_height(node->left), get_height(node->right)) + 1;
	left->depth = std::max(get_height(left->left), get_height(left->right)) + 1;

	if (node == root) {
		root = left;
	}

	return left;
}

template <typename Ty_>
void AVL<Ty_>::do_copy(const AVL& src, AVL& tar) {
	if (src.root) {
		tar.root = std::make_shared<Node>(src.root->val);
		do_copy(src.root->left, tar.root->left);
		do_copy(src.root->right, tar.root->right);
	}
}

template <typename Ty_>
typename AVL<Ty_>::pNode_ AVL<Ty_>::update(Ty_ val, pNode_& node) {
	const int balance = get_balance(node);
	if (balance > 1) {
		if (val < node->left->val) {
			return rotate_r(node);
		}
		if (val > node->left->val){
			node->left = rotate_l(node->left);
			return rotate_r(node);
		}
	}
	if (balance < -1) {
		if (val > node->right->val) {
			return rotate_l(node);
		}
		if (val < node->right->val) {
			node->left = rotate_r(node->left);
			return rotate_l(node);
		} 
	}

	return node;
}

template <typename Ty_>
typename AVL<Ty_>::pNode_ AVL<Ty_>::contain(const Ty_& val, const pNode_& node) const{
	if (node == nullptr) {
		return nullptr;
	}

	if (is_less(val, node->val)) {
		return contain(val, node->left);
	}
	if (is_less(node->val, val)) {
		return contain(val, node->right);
	}
	return node;
}

template <typename Ty_>
void AVL<Ty_>::do_print(const pNode_& node) const {
	if (node == nullptr) {
		return;
	}

	do_print(node->left);
	std::cout << node->val << " ";
	do_print(node->right);
}

template <typename Ty_>
size_t AVL<Ty_>::get_size(const pNode_& node) {
	if (node == nullptr) {
		return 0;
	}

	return get_size(node->left) + get_size(node->right) + 1;
}

template <typename Ty_>
size_t AVL<Ty_>::get_height(const pNode_ node) {
	if(node == nullptr) {
		return 0;
	}

	return std::max(get_height(node->left), get_height(node->right)) + 1;
}

template <typename Ty_>
void AVL<Ty_>::erase_child(const Ty_& val, const pNode_& node) {
	// node without children
	if (node->left == nullptr && node->right == nullptr) {
		if (node == root) {
			root = nullptr;
		} else {
			if (node->parent->left == node) {
				node->parent->left = nullptr;
			} else {
				node->parent->right = nullptr;
			}
		}
		//node with only right child
	} else if (node->left == nullptr) {
		if (node == root) {
			root = node->right;
		} else {
			if (node->parent->left == node) {
				node->parent->left = node->right;
			} else {
				node->parent->right = node->right;
			}
		}
		// node with only left child
	} else if (node->right == nullptr) {
		if (node == root) {
			root = node->left;
		} else {
			if (node->parent->left == node) {
				node->parent->left = node->left;
			} else {
				node->parent->right = node->left;
			}
		}
		// node with two children
	} else {
		// find the smallest node in the right subtree
		auto tmp = node->right;
		while (tmp->left) {
			tmp = tmp->left;
		}
		// replace the node with the smallest node
		node->val = tmp->val;
		erase_child(tmp->val, tmp);
	}
	// if not root, update node and tree
	if (node != root) {
		auto tmp = node->parent;
		while (tmp) {
			tmp->depth = std::max(get_height(tmp->left), get_height(tmp->right)) + 1;
			update(node->val, tmp);
			tmp = tmp->parent;
		}
	}
}

template <typename Ty_>
bool AVL<Ty_>::is_less(const Ty_& lhs, const Ty_& rhs) {
	return lhs < rhs;
}

#endif
