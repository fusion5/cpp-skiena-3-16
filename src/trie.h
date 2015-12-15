#ifndef SKIENA_3_16_TRIE
#define SKIENA_3_16_TRIE

#include <boost/lexical_cast.hpp>
#include "balanced_avl_tree.h"

template <class T>
class Trie {
	public:
		static int steps;
		Trie<T>         (T x);
		Trie<T>         (const Trie<T>& t);
		Trie<T>         ();
		~Trie<T>        ();
		Trie<T> *find   (const T xs[], int nx);
		void    insert  (const T xs[], int nx);
		void    remove  (const T xs[], int nx);
		bool    empty   ();
		T       *value  ();
		T       *max    ();
		T       x;
		string  pp      ();
		int     size    ();
		bool    present;
	private:
		unique_ptr <AVLTree<T, Trie<T>*> > xs;
		int n;
};

template <class T>
int Trie<T>::steps = 0;

/* ctors */
template <class T>
Trie<T>::Trie (T x) {
	this->x = x;
	this->present = false;
	this->xs = unique_ptr<AVLTree<T, Trie<T>*> >(
	  new AVLEmpty<T, Trie<T>*>());
}
/*
template <class T> 
Trie<T>::Trie (const Trie<T>& t) {
	// We need to initialize ourselves to be a copy of t...
	this->x  = t.x;
	this->xs = unique_ptr<AVLTree<T, Trie<T>*> >(t.copy_subtrees());
}
*/
template <class T>
Trie<T>::Trie () {
	this->present = false;
	this->xs = unique_ptr<AVLTree<T, Trie<T>*> >(
	  new AVLEmpty<T, Trie<T>*>());
}

/* dtors */
template <class T>
Trie<T>::~Trie () {
	// cout << "Trie dtor" << endl;
	AVLTree<T, Trie<T>*> *xs = this->xs.release();
	// Iterate through the tries in the avl tree and call delete on them,
	// otherwise they are pointers and they don't get freed...
	AVLTree<T, Trie<T>*> *node = nullptr;

	while (!xs->empty()) {
		node = avl_release_max (&xs);
		delete *(node->value());
		delete node;
	}
	delete xs;
}

/* empty */
template <class T>
bool Trie<T>::empty() {
	return !this->present && this->xs->empty();
}

/* value */
template <class T>
T *Trie<T>::value() {
	return &(this->x);
}

/* size */
template <class T>
int Trie<T>::size () {
	// Here I need to iterate through xs... and call size recursively.
	int children_size = 0;
	
	// create a copy of xs' pointers avl tree...
	AVLTree<T, Trie<T>*> *t   = this->xs.release();
	AVLTree<T, Trie<T>*> *xs  = new AVLEmpty<T,Trie<T>*>();
	AVLTree<T, Trie<T>*> *max = nullptr;
	
	while (!t->empty()) {
		max = avl_release_max (&t);
		avl_insert (&xs, *(max->key()), *(max->value()));
		children_size += (*(max->value()))->size();
		delete max;
	}

	this->xs.reset(xs);

	if (this->present) children_size++;
	
	return children_size;
}

/* remove */
template <class T>
void Trie<T>::remove (const T xs[], int nx) {

	Trie<T>::steps++;

	if (nx == 0) {
		this->present = false;
		return;
	}

	T x = xs[0];
	AVLTree<T, Trie<T>*> *avl_node = nullptr;
	avl_node = this->xs->find (x);
	if (avl_node->empty()) {
		return; // Trying to remove a value that isn't there does nothing
	}

	assert (nx > 0);

	xs ++;
	nx --;
	(*(avl_node->value()))->remove (xs, nx);
	if ((*(avl_node->value()))->empty()) {
		// Delete the sub-trie if it's no longer present
		AVLTree<T, Trie<T>*> *xs = this->xs.release();
		avl_remove (&xs, *(avl_node->key()));
		this->xs.reset(xs);
	}
}

/* insert */
template <class T>
void Trie<T>::insert (const T xs[], int nx) {

	Trie<T>::steps++;

	if (nx == 0) {
		this->present = true;
		return;
	}

	T x = xs[0];

	AVLTree<T, Trie<T>*> *avl_node = nullptr;
	avl_node = this->xs->find (x);
	if (avl_node->empty()) {
		Trie<T> *new_trie = new Trie(x);
		AVLTree<T, Trie<T>*> *xs = this->xs.release();
		avl_insert (&xs, x, new_trie);
		this->xs.reset (xs);
		avl_node = this->xs->find (x);
	}

	assert (nx > 0);

	// Advance the pointer to the next element
	xs ++; 
	nx --;
	(*(avl_node->value()))->insert (xs, nx);
}

template <class T>
Trie<T> *Trie<T>::find (const T xs[], int nx) {

	Trie<T>::steps++;

	if (nx == 0) {
		if (this->present) return this;
		return nullptr;
	}

	T x = xs[0];
	
	AVLTree<T, Trie<T>*> *avl_node = nullptr;
	avl_node = this->xs->find (x);

	// Not found...
	if (avl_node->empty()) return nullptr;

	xs ++;
	nx --;
	return (*(avl_node->value()))->find (xs, nx);
}

/* pp - pretty printing */
template <class T>
string Trie<T>::pp () {
	string ret;
	ret = "(" + boost::lexical_cast<std::string>(this->x);
	if (this->present) ret += "* ";
	else               ret += " ";
	ret += this->xs->pp();
	ret += ")";
	return ret;
}

/* avl tree type specialization of pretty printing of char Tries */
template <>
string AVLNode<char, Trie<char>* >::pp () {
	return "(Node "
		+ this->lt->pp() + ", "
		+ *(this->key()) + "="
		+ (*(this->value()))->pp() + ", "
		+ this->rt->pp() + ")";
}
#endif
