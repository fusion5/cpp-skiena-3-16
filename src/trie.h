#ifndef SKIENA_3_16_TRIE
#define SKIENA_3_16_TRIE

#include <boost/lexical_cast.hpp>
#include "bin_tree.h"

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
		string  pp      ();
		int     size    ();
		bool    present;
		T       x;
	private:
		unique_ptr <BinTree<T, Trie<T>*> > xs;
		int n;
};

template <class T>
int Trie<T>::steps = 0;

/* ctors */
template <class T>
Trie<T>::Trie (T x) {
	this->x = x;
	this->present = false;
	this->xs = unique_ptr<BinTree<T, Trie<T>*> >(
	  new BinEmpty<T, Trie<T>*>());
}
template <class T>
Trie<T>::Trie () {
	this->present = false;
	this->xs = unique_ptr<BinTree<T, Trie<T>*> >(
	  new BinEmpty<T, Trie<T>*>());
}

/* dtors */
template <class T>
Trie<T>::~Trie () {
	// cout << ".";

	if (this->xs->empty()) {
		this->xs.reset();
		return;
	}

	BinTree<T, Trie<T>*> *xs = this->xs.release();
	// Iterate through the tries in the avl tree and call delete on them,
	// otherwise they are pointers and they don't get freed...

	while (!xs->empty()) {
		Trie<T>* subtrie = nullptr;
		tie (ignore, subtrie) = bintree_release_max(&xs);
		delete subtrie;
	}
	delete xs;
}

/* empty */
template <class T>
bool Trie<T>::empty() {
	return !this->present && this->xs->empty();
	// TODO: or, if all sub-tries are empty too...
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
	// TODO: Find a nicer way of iterating without moving so much memory 
	// around
	int children_size = 0;
	
	// create a copy of xs' pointers avl tree...
	BinTree<T, Trie<T>*> *t   = this->xs.release();
	BinTree<T, Trie<T>*> *xs  = new BinEmpty<T,Trie<T>*>();
	T key;
	Trie<T>* val;
	
	while (!t->empty()) {
		tie (key, val) = bintree_release_max (&t);
		bintree_insert (&xs, key, val);
		children_size += val->size();
		// delete max;
	}
	delete t;

	this->xs.reset(xs);

	if (this->present) children_size++;
	
	return children_size;
}

/* remove */
template <class T>
void Trie<T>::remove (const T xs[], int nx) {

	if (nx < 0) return;

	Trie<T>::steps++;

	if (nx == 0) {
		this->present = false;
		return;
	}

	T x = xs[0];
	BinTree<T, Trie<T>*> *bin_node = nullptr;
	bin_node = this->xs->find (x);
	// Trying to remove a value that isn't there does nothing:
	if (bin_node->empty()) return; 

	xs ++;
	nx --;
	(bin_node->value())->remove (xs, nx);
	if ((bin_node->value())->empty()) {
		// Delete the sub-trie if it's no longer present
		BinTree<T, Trie<T>*> *xs = this->xs.release();
		bintree_remove (&xs, bin_node->key());
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

	BinTree<T, Trie<T>*> *bin_node = nullptr;
	bin_node = this->xs->find (x);
	if (bin_node->empty()) {
		BinTree<T, Trie<T>*> *xs = this->xs.release();
		bintree_insert (&xs, x, new Trie(x));
		this->xs.reset (xs);
		bin_node = this->xs->find (x);
	}

	assert (nx > 0);

	// Advance the pointer to the next element
	xs ++; 
	nx --;
	(bin_node->value())->insert (xs, nx);
}

template <class T>
Trie<T> *Trie<T>::find (const T xs[], int nx) {

	Trie<T>::steps++;

	if (nx == 0) {
		if (this->present) return this;
		return nullptr;
	}

	T x = xs[0];
	
	BinTree<T, Trie<T>*> *bin_node = nullptr;
	bin_node = this->xs->find (x);

	// Not found...
	if (bin_node->empty()) return nullptr;

	xs ++;
	nx --;
	return bin_node->value()->find (xs, nx);
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
string BinNode<char, Trie<char>* >::pp () {
	return "(Node "
		+ this->lt->pp() + ", "
		+ this->key() + "="
		+ this->value()->pp() + ", "
		+ this->rt->pp() + ")";
}
#endif
