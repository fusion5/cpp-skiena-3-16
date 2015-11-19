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
		virtual AVLTree<T> *find (T x) = 0;
		virtual insert_avl_type<T> insert (T x) = 0;
		virtual int height () = 0;
		virtual T value () = 0;
		virtual bool empty() = 0;
		virtual AVLTree<T> *left() = 0;
		virtual AVLTree<T> *right() = 0;
		virtual string pp () = 0;
		virtual int size () = 0;

		static int steps;
};

template <class T>
int AVLTree<T>::steps = 0;

template <class T>
class AVLNode: public AVLTree<T> {
	public:
		AVLNode (AVLTree<T> *lt, AVLTree<T> *rt, T x);
		~AVLNode ();
		AVLTree<T> *find (T x);
		insert_avl_type<T> insert (T x);
		int height();
		T value  ();
		bool empty ();
		AVLTree<T> *left();
		AVLTree<T> *right();
		string pp ();
		int size ();
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
		~AVLEmpty ();
		AVLTree<T> *find (T x);
		insert_avl_type<T> insert (T x);
		int height ();
		T value();
		bool empty ();
		AVLTree<T> *left();
		AVLTree<T> *right();
		string pp ();
		int size ();
};

/* constructors */
template <class T>
AVLNode<T>::AVLNode (AVLTree<T> *lt, AVLTree<T> *rt, T x) {
	this->lt = lt;
	this->rt = rt;
	this->x  = x;
	this->h  = max (lt->height(), rt->height()) + 1;
}
template <class T>
AVLEmpty<T>::AVLEmpty() {
}

/* destructors */
template <class T>
AVLNode<T>::~AVLNode() {
}
template <class T>
AVLEmpty<T>::~AVLEmpty() {
}


/* left */
template <class T>
AVLTree<T> *AVLNode<T>::left() {
	return this->lt;
}
template <class T>
AVLTree<T> *AVLEmpty<T>::left() {
	cerr << "Calling left() on an empty node!" << endl;
	throw "error";
}

/* right */
template <class T>
AVLTree<T> *AVLNode<T>::right() {
	return this->rt;
}
template <class T>
AVLTree<T> *AVLEmpty<T>::right() {
	cerr << "Calling right() on an empty node!" << endl;
	throw "error";
}

/* pp, pretty print */
template <class T>
string AVLNode<T>::pp() {
	return "(Node " 
		+ this->left()->pp() + ", " 
		+ this->value() + ", "
		+ this->right()->pp() + ")";
}
template <class T>
string AVLEmpty<T>::pp() {
	return "Empty";
}

/* size */
template <class T>
int AVLNode<T>::size() {
	AVLTree<T>::steps++;
	return 1 + this->left()->size() + this->right()->size();
}
template <class T>
int AVLEmpty<T>::size() {
	return 0;
}

/* value */
template <class T>
T AVLNode<T>::value() {
	return this->x;
}
template <class T>
T AVLEmpty<T>::value() {
	cerr << "Calling value() on an empty node!" << endl;
	throw "error";
}

/* empty */
template <class T>
bool AVLNode<T>::empty() { return false; }
template <class T>
bool AVLEmpty<T>::empty() { return true; }

/* height */
template <class T>
int AVLNode<T>::height() {
	return this->h;
}
template <class T>
int AVLEmpty<T>::height() {
	return 0;
}

/* find */
template <class T>
AVLTree<T> *AVLNode<T>::find(T x) {
	AVLTree<T>::steps++;
	if (this->x == x) return this;
	if (x < this->x)  return this->left()->find(x);
	if (x > this->x)  return this->right()->find(x);
}
template <class T>
AVLTree<T> *AVLEmpty<T>::find(T x) {
	AVLTree<T>::steps++;
	return this;
}

/* insert */
template <class T>
insert_avl_type<T> AVLNode<T>::insert(T x) {
	AVLTree<T>::steps++;
	// Inserting to an already existing value!
	if (x == this->x) return this;

	if (x < this->x) {
		this->lt = this->left()->insert (x);
		// Are we heavy on the left side after the insertion?
		if (this->left()->height() > this->right()->height() + 1) {

			// Rotate by creating a copy...
			// I think it can work without a copy as well!
			// After the insert, lt is a balanced tree of height 2 or more.
			// Therefore, it must have both lt->lt and lt->rt, otherwise
			// it wouldn't be a balanced tree.
			this->h = max ( this->left()->right()->height()	
				      , this->rt->height()) + 1;
			AVLNode<T> *n = new AVLNode(
				  this->left()->left()
				, this
				, this->left()->value());
			// Deallocate the old lt, because n takes its place
			// delete this->lt;
			this->lt = this->left()->right();
			return n;
		}
	} else {
		// x > this->x
		this->rt = this->right()->insert (x);
		// Are we heavy on the right side after the insertion?
		if (this->right()->height() > this->left()->height() + 1) {
			// Rotate by creating a copy...
			this->h = max ( this->left()->height()
				      , this->right()->left()->height()) + 1;
			AVLNode<T> *n = new AVLNode(
				  this
				, this->right()->right()
				, this->right()->value());
			// Deallocate the old rt, because n takes its place 
			// delete this->rt;
			this->rt = this->right()->left();
			return n; 
		}
	}
	this->h = max (this->lt->height(), this->rt->height()) + 1;
	return this;
}

template <class T>
insert_avl_type<T> AVLEmpty<T>::insert (T x) {
	AVLTree<T>::steps++;
	AVLTree<T> *n = new AVLNode<T>(new AVLEmpty(), new AVLEmpty(), x);
	// FIXME: I have to dealocate myself!
	return n;
}

#endif
