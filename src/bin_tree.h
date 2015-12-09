#ifndef SKIENA_3_16_BIN_TREE
#define SKIENA_3_16_BIN_TREE

#include <string.h>
#include <boost/lexical_cast.hpp>

using namespace std;

template <class T>
class BinTree {
	public:
		static int steps;
		virtual BinTree<T> *find   (T x) = 0;
		virtual BinTree<T> *insert (T x) = 0;
		virtual int      size   ()    	 = 0;
		virtual string   pp     ()    	 = 0;
		virtual bool 	 empty  ()       = 0;
};

template <class T>
int BinTree<T>::steps= 0;

template <class T>
class Node: public BinTree<T> {
	public:
		Node<T>    (T x, BinTree<T> *lt, BinTree<T> *rt);
		Node<T>    (T x);
		BinTree<T> *insert(T x);
		BinTree<T> *find(T x);
		int 	   size();
		string     pp();
		bool       empty();
	private:
		BinTree<T> *lt; // Left subtree
		BinTree<T> *rt; // Right subtree
		T x;            // Value
};

template <class T>
class Leaf: public BinTree<T> {
	public:
		Leaf<T>    ();
		BinTree<T> *insert(T x);
		BinTree<T> *find(T x);
		string     pp();
		int 	   size();
		bool       empty();
};

/* ctors */
template <class T>
Leaf<T>::Leaf() { }

template <class T>
Node<T>::Node(T x, BinTree<T> *lt, BinTree<T> *rt) {
	this->lt = lt;
	this->rt = rt;
	this->x  = x;
}

/* dtors */

/* empty */
template <class T>
bool Leaf<T>::empty() { return true; }
template <class T>
bool Node<T>::empty() { return false; }

template <class T>
int Leaf<T>::size() { return 0; }
template <class T>
int Node<T>::size() { 
	BinTree<T>::steps ++;
	return 1 + this->lt->size() + this->rt->size(); }

// Return a reference to the updated tree
template <class T>
BinTree<T> *Node<T>::insert (T x) {

	BinTree<T>::steps ++;
	BinTree<T> *old;

	if (x > this->x) {
		old = this->rt;
		this->rt = this->rt->insert(x);
	} else {
		old = this->lt;
		this->lt = this->lt->insert(x);
	}

	// De-allocate the old if it was a leaf, because we know it has been 
	// replaced...
	if (old->empty()) delete old;

	return this;
}
template <class T>
BinTree<T> *Leaf<T>::insert (T x) {
	BinTree<T>::steps ++;
	return new Node<T> (x, new Leaf<T>(), new Leaf<T>());
}

template <class T>
BinTree<T> *Node<T>::find (T x) {
	BinTree<T>::steps ++;
	if (this->x == x) return this;
	if (x > this->x)
		return this->rt->find (x);
	else
		return this->lt->find (x);
}

template <class T>
BinTree<T> *Leaf<T>::find (T x) {
	BinTree<T>::steps ++;
	// If we reached this, it means we haven't found it
	return this;
}

template <class T>
string Leaf<T>::pp() { return "<>"; }

template <class T>
string Node<T>::pp() { 
	return 
	"(" 
	    + this->lt->pp() + " "
	    + boost::lexical_cast<std::string>(this->x) + " "
	    + this->rt->pp() 
	    + 
	")"; 
}

#endif
