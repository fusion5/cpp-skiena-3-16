#ifndef SKIENA_3_16_BIN_TREE
#define SKIENA_3_16_BIN_TREE

#include <string.h>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

template <class T> class BinEmpty;

template <class T>
class BinTree {
	public:
		virtual ~BinTree<T>              ()    = 0;
		static int steps;
		virtual BinTree<T> *find         (T x) = 0;
		virtual BinTree<T> *insert       (T x) = 0;
		virtual BinTree<T> *remove       (T x) = 0;
		virtual tuple<BinTree<T>*, T>
		                   release_max   ()    = 0;
		virtual BinTree<T> *release_right()    = 0;
		virtual BinTree<T> *release_left ()    = 0;
		virtual T          value         ()    = 0;
		virtual int        size          ()    = 0;
		virtual string     pp            ()    = 0;
		virtual bool       empty         ()    = 0;
};

template <class T>
int BinTree<T>::steps= 0;

template <class T>
class BinNode: public BinTree<T> {
	public:
		BinNode<T> (T x, BinTree<T> *lt, BinTree<T> *rt);
		~BinNode<T>();
		BinTree<T> *insert(T x);
		BinTree<T> *find  (T x);
		BinTree<T> *remove(T x);
		int        size();
		string     pp();
		bool       empty();
		tuple<BinTree<T>*, T> release_max();
		BinTree<T> *release_right();
		BinTree<T> *release_left();
		T          value();
	private:
		unique_ptr<BinTree<T> > lt; // Left subtree
		unique_ptr<BinTree<T> > rt; // Right subtree
		T x;            // Value
};

template <class T>
class BinEmpty: public BinTree<T> {
	public:
		BinEmpty<T>();
		~BinEmpty<T>();
		BinTree<T> *insert(T x);
		BinTree<T> *find(T x);
		BinTree<T> *remove(T x);
		string     pp();
		int        size();
		bool       empty();
		tuple<BinTree<T>*, T> release_max();
		BinTree<T> *release_right();
		BinTree<T> *release_left();
		T          value();
};

/* ctors */
template <class T>
BinNode<T>::BinNode(T x, BinTree<T> *lt, BinTree<T> *rt) {
	this->lt = unique_ptr<BinTree<T> >(lt);
	this->rt = unique_ptr<BinTree<T> >(rt);
	this->x  = x;
}
template <class T>
BinEmpty<T>::BinEmpty() { }

/* dtors */
template <class T>
BinEmpty<T>::~BinEmpty() { }
template <class T>
BinNode<T> ::~BinNode()  { }
template <class T>
BinTree<T> ::~BinTree()  { }

/* empty */
template <class T>
bool BinEmpty<T>::empty() { return true; }
template <class T>
bool BinNode<T>::empty() { return false; }

template <class T>
int BinEmpty<T>::size() { return 0; }
template <class T>
int BinNode<T>::size() { 
	BinTree<T>::steps ++;
	return 1 + this->lt->size() + this->rt->size(); }

/* value */
template <class T>
T BinNode<T>::value() {
	return this->x;
}
template <class T>
T BinEmpty<T>::value() {
	T x;
	assert (false);
	return x;
}

/* release_left
 * */
template <class T>
BinTree<T> *BinNode<T>::release_left() {
	assert (this->lt);
	BinTree<T> *r = this->lt.release();
	this->lt.reset(new BinEmpty<T>());
	return r;
}
template <class T>
BinTree<T> *BinEmpty<T>::release_left() {
	assert (false);
}
/*
 * release_right
 * */

template <class T>
BinTree<T> *BinNode<T>::release_right() {
	assert (this->rt);
	BinTree<T> *r = this->rt.release();
	this->rt.reset(new BinEmpty<T>());
	return r;
}
template <class T>
BinTree<T> *BinEmpty<T>::release_right() {
	assert (false);
}

/*
 * release_max
 * returns a tuple
 * - left member: new value that should replace me or nullptr to delete...
 * - right member: the maximum 
 * */
template <class T>
tuple<BinTree<T>*, T> BinNode<T>::release_max() {
	T max;

	if (this->rt->empty()) {
		// Our right node is a leaf... we have our maximum: ourselves
		//   this
		//   /  \
		//  t   empty (will need to cast it)
		max = this->x;
		return make_tuple (this->lt.release(), max);
	}

	BinTree<T> *replacement;
	tie (replacement, max) = this->rt->release_max();
	if (replacement != this->rt.get()) {
		// max = this->rt.release()->value();
		this->rt.reset (replacement);
	}
	return make_tuple (this, max);
}
template <class T>
tuple<BinTree<T>*, T> BinEmpty<T>::release_max() {
	assert (false);
}

/* remove 
 * returns:
 * - 'this' if the node should stay the same
 * - a node pointer with which to replace myself otherwise
 * */
template <class T>
BinTree<T> *BinNode<T>::remove (T x) {
	BinTree<T>::steps ++;

	if (x == this->x) {
		if (this->lt->empty() && this->rt->empty())
			return new BinEmpty<T>(); 
		else if (this->lt->empty())
			return this->release_right();
		else if (this->rt->empty())
			return this->release_left();
		else {
			assert (!(this->lt->empty()));
			assert (!(this->rt->empty()));
			T max;
			BinTree<T> *replacement;
			tie (replacement, max) = this->lt->release_max();
			if (replacement != this->lt.get()) 
				this->lt.reset(replacement);
			return new BinNode( max, this->release_left(), 
			                         this->release_right());
		}
	}
	if (x < this->x) {
		BinTree<T> *result;
		result = this->lt->remove(x);
		if (result != this->lt.get()) this->lt.reset(result);
	}
	if (x > this->x) {
		BinTree<T> *result;
		result = this->rt->remove (x);
		if (result != this->rt.get()) this->rt.reset(result);
	}
	return this;
}
template <class T>
BinTree<T> *BinEmpty<T>::remove (T x) {
	assert (false);
	return this;
}


// Return a reference to the updated tree
/* insert */
template <class T>
BinTree<T> *BinNode<T>::insert (T x) {

	BinTree<T>::steps ++;
	BinTree<T> *res;

	if (x > this->x) {
		res = this->rt->insert(x);
		if (res != this->rt.get())
			this->rt.reset(res);
		// this->rt.reset(this->release_right()->insert(x));
	}
	else {
		res = this->lt->insert(x);
		if (res != this->lt.get())
			this->lt.reset(res);
		// this->lt.reset(this->release_left()->insert(x));
	}

	// De-allocate the old if it was a leaf, because we know it has been 
	// replaced...
	// if (old->empty()) delete old;

	return this;
}
template <class T>
BinTree<T> *BinEmpty<T>::insert (T x) {
	BinTree<T>::steps ++;
	return new BinNode<T> (x, new BinEmpty<T>(), new BinEmpty<T>());
}

template <class T>
BinTree<T> *BinNode<T>::find (T x) {
	assert (this->lt);
	assert (this->rt);
	BinTree<T>::steps ++;
	if (this->x == x) return this;
	if (x > this->x)
		return this->rt->find (x);
	else
		return this->lt->find (x);
}

template <class T>
BinTree<T> *BinEmpty<T>::find (T x) {
	BinTree<T>::steps ++;
	// If we reached this, it means we haven't found it
	return this;
}

template <class T>
string BinEmpty<T>::pp() { return "<>"; }

template <class T>
string BinNode<T>::pp() { 
	return 
	"(" 
	    + this->lt->pp() + " "
	    + boost::lexical_cast<std::string>(this->x) + " "
	    + this->rt->pp() 
	    + 
	")"; 
}

template <class T>
void bintree_remove (BinTree<T> **t, T x) {
	// cout << "Bintree remove..." << endl;
	if ((*t)->empty()) return;
	BinTree<T> *r = (*t)->remove (x);
	if (r != *t) {
		delete *t;
		*t = r;
	}
}

template <class T>
void bintree_insert (BinTree<T> **t, T x) {
	BinTree<T> *r = (*t)->insert (x);
	if (r != *t) {
		delete *t;
		*t = r;
	}
}

template <class T>
T bintree_release_max (BinTree<T> **t) {
	if ((*t)->empty()) throw "could not release max!";
	BinTree<T> *replacement;
	T max;

	tie (replacement, max) = (*t)->release_max();
	if (replacement != *t) {
		delete *t;
		*t = replacement;
	}
	return max;
}

#endif
