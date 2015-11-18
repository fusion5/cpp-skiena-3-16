#ifndef SKIENA_3_16_AVL_TREE
#define SKIENA_3_16_AVL_TREE

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace std;

template <class T> class AVLTree;

template <class T>
using insert_avl_type = AVLTree<T>*;

template <class T>
class AVLTree {
	public: 
		// virtual AVLTree *find (T x) = 0;
		virtual insert_avl_type<T> insert (T x) = 0;
		virtual int height () = 0;
		virtual T value () = 0;
		// virtual string pp () = 0;
		// virtual int size () = 0;

		static int steps;
};

template <class T>
int AVLTree<T>::steps = 0;

template <class T>
class AVLNode: public AVLTree<T> {
	public:
		AVLNode (AVLTree<T> *lt, AVLTree<T> *rt, T x);
		insert_avl_type<T> insert (T x);
		bool empty  ();
		T value  ();
		int height();
	protected:
		AVLTree<T> *lt;
		AVLTree<T> *rt;
	private:
		T x;
		int h;
};

template <class T>
class AVLEmpty: public AVLTree<T> {
	public:
		AVLEmpty ();
		insert_avl_type<T> insert (T x);
		int height ();
		T value();
};

/* constructors */
template <class T>
AVLNode<T>::AVLNode (AVLTree<T> *lt, AVLTree<T> *rt, T x) {
	this->lt = lt;
	this->rt = rt;
	this->x  = x;
}
template <class T>
AVLEmpty<T>::AVLEmpty() {
}

/* insert */
template <class T>
insert_avl_type<T> AVLNode<T>::insert (T x) {
	
	if (x == this->x) return this;

	if (x < this->x) {
		this->lt->insert (x);
		// Are we heavy on the left side?
		if (this->lt->height() > this->rt->height() + 1) {

			// Rotate, we create a copy...
			// I think we can get by without a copy as well!
			// After the insert, lt is a balanced tree of height 2 or more.
			// Therefore, it must have both lt->lt and lt->rt, otherwise
			// it wouldn't be a balanced tree.
			AVLNode<T> *n  = new AVLNode(
				  this->lt->lt
				, this
				, this->lt->value());

			this->lt = this->lt->rt;

			return n;
		}
		this->h = max (this->lt->height(), this->rt->height());
	}
}
template <class T>
insert_avl_type<T> AVLEmpty<T>::insert (T x) {
	AVLTree<T> *n = new AVLNode<T>(new AVLEmpty(), new AVLEmpty(), x);
	// FIXME: I have to dealocate myself!
	return n;
}

#endif
