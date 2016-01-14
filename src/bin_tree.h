#ifndef SKIENA_3_16_BIN_TREE
#define SKIENA_3_16_BIN_TREE

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace std;

template <class K, class V> class BinEmpty;

template <class K, class V>
class BinTree {
	public:
		virtual ~BinTree<K, V>              ()    = 0;
		virtual BinTree<K, V> *find         (K x) = 0;
		virtual BinTree<K, V> *insert       (K k, V v) = 0;
		virtual BinTree<K, V> *remove       (K k) = 0;
		virtual tuple<BinTree<K, V>*, K, V>
		                      release_max   ()    = 0;
		virtual BinTree<K, V> *release_right()    = 0;
		virtual BinTree<K, V> *release_left ()    = 0;
		virtual V             value         ()    = 0;
		virtual K             key           ()    = 0;
		virtual int           size          ()    = 0;
		virtual string        pp            ()    = 0;
		virtual bool          empty         ()    = 0;
		static  int           steps;
		static  int           *p_steps; // Pointer to steps variable
};

template <class K, class V>
int BinTree<K, V>::steps = 0;

template <class K, class V>
int *BinTree<K, V>::p_steps = &(BinTree<K, V>::steps);

template <class K, class V>
class BinNode: public BinTree<K, V> {
	public:
		BinNode<K, V> (K k, V v, BinTree<K, V> *lt, BinTree<K, V> *rt);
		~BinNode<K, V>();
		BinTree<K, V> *insert(K k, V v);
		BinTree<K, V> *find  (K k);
		BinTree<K, V> *remove(K k);
		int        size();
		string     pp();
		bool       empty();
		tuple<BinTree<K, V>*, K, V> release_max();
		BinTree<K, V> *release_right();
		BinTree<K, V> *release_left();
		V          value();
		K          key();
	private:
		unique_ptr<BinTree<K, V> > lt; // Left subtree
		unique_ptr<BinTree<K, V> > rt; // Right subtree
		K x;            // Key
		V val;          // Value
};

template <class K, class V>
class BinEmpty: public BinTree<K, V> {
	public:
		BinEmpty<K, V>();
		~BinEmpty<K, V>();
		BinTree<K, V> *insert(K k, V v);
		BinTree<K, V> *find  (K k);
		BinTree<K, V> *remove(K k);
		string     pp();
		int        size();
		bool       empty();
		tuple<BinTree<K, V>*, K, V> release_max();
		BinTree<K, V> *release_right();
		BinTree<K, V> *release_left();
		V          value();
		K          key();
};

/* ctors */
template <class K, class V>
BinNode<K, V>::BinNode(K x, V v, BinTree<K, V> *lt, BinTree<K, V> *rt) {
	this->lt  = unique_ptr<BinTree<K, V> >(lt);
	this->rt  = unique_ptr<BinTree<K, V> >(rt);
	this->x   = x;
	this->val = v;
}
template <class K, class V>
BinEmpty<K, V>::BinEmpty() { }

/* dtors */
template <class K, class V>
BinEmpty<K, V>::~BinEmpty() { }
template <class K, class V>
BinNode<K, V> ::~BinNode()  { }
template <class K, class V>
BinTree<K, V> ::~BinTree()  { }

/* empty */
template <class K, class V>
bool BinEmpty<K, V>::empty() { return true; }
template <class K, class V>
bool BinNode<K, V>::empty() { return false; }

template <class K, class V>
int BinEmpty<K, V>::size() { return 0; }
template <class K, class V>
int BinNode<K, V>::size() { 
	// (*(BinTree<K, V>::p_steps))++;
	return 1 + this->lt->size() + this->rt->size(); }

/* value */
template <class K, class V>
V BinNode<K, V>::value() {
	return this->val;
}
template <class K, class V>
V BinEmpty<K, V>::value() {
	V x;
	assert (false);
	return x;
}

/* key */
template <class K, class V>
K BinNode<K, V>::key() {
	return this->x;
}
template <class K, class V>
K BinEmpty<K, V>::key() {
	K k;
	assert (false);
	return k;
}

/* release_left
 * */
template <class K, class V>
BinTree<K, V> *BinNode<K, V>::release_left() {
	assert (this->lt);
	BinTree<K, V> *r = this->lt.release();
	this->lt.reset(new BinEmpty<K, V>());
	return r;
}
template <class K, class V>
BinTree<K, V> *BinEmpty<K, V>::release_left() {
	assert (false);
}

/* release_right
 * */
template <class K, class V>
BinTree<K, V> *BinNode<K, V>::release_right() {
	assert (this->rt);
	BinTree<K, V> *r = this->rt.release();
	this->rt.reset(new BinEmpty<K, V>());
	return r;
}
template <class K, class V>
BinTree<K, V> *BinEmpty<K, V>::release_right() {
	assert (false);
}

/*
 * release_max
 * returns a tuple
 * - left member: new value that should replace me or nullptr to delete...
 * - right member: the maximum 
 * */
template <class K, class V>
tuple<BinTree<K, V>*, K, V> BinNode<K, V>::release_max() {
	K kmax;
	V vmax;

	if (this->rt->empty()) {
		// Our right node is a leaf... we have our maximum: ourselves
		//   this
		//   /  \
		//  t   empty 
		return make_tuple (this->lt.release(), this->x, this->val);
	}

	BinTree<K, V> *replacement;
	tie (replacement, kmax, vmax) = this->rt->release_max();
	if (replacement != this->rt.get()) this->rt.reset (replacement);
	return make_tuple (this, kmax, vmax);
}
template <class K, class V>
tuple<BinTree<K, V>*, K, V> BinEmpty<K, V>::release_max() {
	assert (false);
}

/* remove 
 * returns:
 * - 'this' if the node should stay the same
 * - a node pointer with which to replace myself otherwise
 * */
template <class K, class V>
BinTree<K, V> *BinNode<K, V>::remove (K x) {
	(*(BinTree<K, V>::p_steps))++;

	if (x == this->x) {
		if (this->lt->empty() && this->rt->empty())
			return new BinEmpty<K, V>(); 
		else if (this->lt->empty())
			return this->release_right();
		else if (this->rt->empty())
			return this->release_left();
		else {
			assert (!(this->lt->empty()));
			assert (!(this->rt->empty()));
			K kmax;
			V vmax;
			BinTree<K, V> *replacement;
			tie (replacement, kmax, vmax) = this->lt->release_max();
			if (replacement != this->lt.get()) 
				this->lt.reset(replacement);
			return new BinNode( kmax, vmax, this->release_left(), 
			                    this->release_right());
		}
	} if (x < this->x) {
		BinTree<K, V> *result;
		result = this->lt->remove (x);
		if (result != this->lt.get()) this->lt.reset(result);
	} if (x > this->x) {
		BinTree<K, V> *result;
		result = this->rt->remove (x);
		if (result != this->rt.get()) this->rt.reset(result);
	}
	return this;
}
template <class K, class V>
BinTree<K, V> *BinEmpty<K, V>::remove (K x) {
	assert (false);
	return this;
}


// Return a reference to the updated tree
/* insert */
template <class K, class V>
BinTree<K, V> *BinNode<K, V>::insert (K x, V v) {
	(*(BinTree<K, V>::p_steps))++;
	BinTree<K, V> *res;

	if (x > this->x) {
		res = this->rt->insert(x, v);
		if (res != this->rt.get()) this->rt.reset(res);
	} else {
		res = this->lt->insert(x, v);
		if (res != this->lt.get()) this->lt.reset(res);
	}

	// De-allocate the old if it was a leaf, because we know it has been 
	// replaced...
	// if (old->empty()) delete old;

	return this;
}
template <class K, class V>
BinTree<K, V> *BinEmpty<K, V>::insert (K x, V v) {
	(*(BinTree<K, V>::p_steps))++;
	return new BinNode<K, V> (x, v, new BinEmpty<K, V>(), 
	  new BinEmpty<K, V>());
}

template <class K, class V>
BinTree<K, V> *BinNode<K, V>::find (K x) {
	assert (this->lt);
	assert (this->rt);
	(*(BinTree<K, V>::p_steps))++;
	if (this->x == x) return this;
	if (x > this->x)
		return this->rt->find (x);
	else
		return this->lt->find (x);
}

template <class K, class V>
BinTree<K, V> *BinEmpty<K, V>::find (K x) {
	(*(BinTree<K, V>::p_steps))++;
	// If we reached this, it means we haven't found it
	return this;
}

template <class K, class V>
string BinEmpty<K, V>::pp() { return "<>"; }

template <class K, class V>
string BinNode<K, V>::pp() { 
	return 
	"(" 
	    + this->lt->pp() + " "
	    + boost::lexical_cast<std::string>(this->x) + " "
	    + this->rt->pp() 
	    + 
	")"; 
}

template <class K, class V>
void bintree_remove (BinTree<K, V> **t, K x) {
	if ((*t)->empty()) return;
	BinTree<K, V> *r = (*t)->remove (x);
	if (r != *t) {
		delete *t;
		*t = r;
	}
}

template <class K, class V>
void bintree_insert (BinTree<K, V> **t, K x, V v) {
	BinTree<K, V> *r = (*t)->insert (x, v);
	if (r != *t) {
		delete *t;
		*t = r;
	}
}

template <class K, class V>
tuple<K, V> bintree_release_max (BinTree<K, V> **t) {
	(*(BinTree<K, V>::p_steps))++;
	if ((*t)->empty()) throw "could not release max!";
	BinTree<K, V> *replacement;
	K kmax;
	V vmax;

	tie (replacement, kmax, vmax) = (*t)->release_max();
	if (replacement != *t) {
		delete *t;
		*t = replacement;
	}
	return make_tuple(kmax, vmax);
}

#endif
