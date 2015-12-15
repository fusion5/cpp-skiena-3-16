#ifndef SKIENA_3_16_BIN_TREE
#define SKIENA_3_16_BIN_TREE

#include <string.h>
#include <boost/lexical_cast.hpp>

using namespace std;

template <class T> class BinEmpty;

template <class T>
class BinTree {
	public:
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
BinEmpty<T>::BinEmpty() { }

template <class T>
BinNode<T>::BinNode(T x, BinTree<T> *lt, BinTree<T> *rt) {
	this->lt = unique_ptr<BinTree<T> >(lt);
	this->rt = unique_ptr<BinTree<T> >(rt);
	this->x  = x;
}

/* dtors */

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
	BinTree<T> *r = this->lt.release();
	assert (this->lt);
}
template <class T>
BinTree<T> *BinEmpty<T>::release_left() {
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
		// Our right node is a leaf... we have our maximum.
		//   this
		//   /  \
		//  t   empty (will need to cast it)
		
		return make_tuple (this->lt.release(), max);
	}

	BinTree<T> *replacement;
	tie (replacement, max) = this->rt->release_max();
	if (replacement) {
		max = this->rt.release()->value();
		this->rt.reset (replacement);
	}
	return make_tuple (nullptr, max);
}
template <class T>
tuple<BinTree<T>*, T> BinEmpty<T>::release_max() {
	assert (false);
}

/* remove 
 * returns:
 * - nullptr if the node should be removed
 * - 'this' if the node should stay the same
 * - a node pointer with which to replace myself otherwise
 * */
template <class T>
BinTree<T> *BinNode<T>::remove (T x) {
	BinTree<T>::steps ++;

	if (x == this->x) {
		if (this->lt->empty() && this->rt->empty())
			return new BinEmpty<T>();
		else if (this->left()->empty())
			return this->release_right();
	}
	if (x < this->x) {
		BinTree<T> *result;
		result = this->lt->remove(x);
		/*
		if (result == this->lt.get())
			return;
		*/
	}
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
	// BinTree<T> *old;

	if (x > this->x) {
		// old = this->rt.release();
		this->rt.reset(this->rt->insert(x));
	} else {
		// old = this->lt.release();
		// this->lt = this->lt->insert(x);
		this->lt.reset(this->lt->insert(x));
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

#endif
