#ifndef SKIENA_3_16_AVL_TREE
#define SKIENA_3_16_AVL_TREE

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace std;

template <class K, class V> class AVLTree;

template <class K, class V>
class AVLTree {
	public: 
		virtual ~AVLTree<K,V>               ()    = 0;
		virtual AVLTree<K,V> *find          (K x) = 0;
		virtual K            *max           () = 0;
		virtual tuple<AVLTree<K,V>*, K, V>
		                     release_max    () = 0;
		virtual AVLTree<K,V> *remove        (K x) = 0;
		virtual AVLTree<K,V> *insert        (K x, V val) = 0;
		virtual int          height         () = 0;
		virtual K            *key           () = 0;
		virtual V            *value         () = 0;
		virtual bool         empty          () = 0;
		virtual AVLTree<K,V> *left          () = 0;
		virtual AVLTree<K,V> *right         () = 0;
		virtual AVLTree<K,V> *release_right () = 0;
		virtual AVLTree<K,V> *release_left  () = 0;
		virtual void         replace_left   (AVLTree<K,V> *lt) = 0;
		virtual void         replace_right  (AVLTree<K,V> *rt) = 0;
		virtual string       pp             () = 0;
		virtual int          size           () = 0;
		static  int          steps;
		static  int          *p_steps; // Pointer to steps variable
	protected:
	private:
		unique_ptr<AVLTree<K,V> > lt;
		unique_ptr<AVLTree<K,V> > rt;
};

template <class K, class V>
int AVLTree<K, V>::steps = 0;

template <class K, class V>
int *AVLTree<K, V>::p_steps = &(AVLTree<K, V>::steps);

template <class K, class V>
class AVLNode: public AVLTree<K, V> {
	public:
		AVLNode                   ( AVLTree<K, V> *lt, AVLTree<K, V> *rt
		                          , K x, V val);
		~AVLNode                  ();
		AVLTree<K, V> *find       (K x);
		K          *max           ();
		tuple<AVLTree<K, V>*, K, V>
		           release_max    ();
		AVLTree<K, V> *remove     (K x);
		AVLTree<K, V> *insert     (K x, V val);
		int        height         ();
		K          *key           ();
		V          *value         ();
		bool       empty          ();
		AVLTree<K, V> *left       ();
		AVLTree<K, V> *right      ();
		AVLTree<K, V> *release_left  ();
		AVLTree<K, V> *release_right ();
		void       replace_left   (AVLTree<K, V> *lt);
		void       replace_right  (AVLTree<K, V> *rt);
		string     pp             ();
		int        size           ();

	protected:
	private:
		K          x;
		V          val;
		int        h;
		AVLTree<K, V> *rotate_left   ();
		AVLTree<K, V> *rotate_right  ();
		unique_ptr<AVLTree<K, V> > lt;
		unique_ptr<AVLTree<K, V> > rt;
};

template <class K, class V>
class AVLEmpty: public AVLTree<K, V> {
	public:
		AVLEmpty                  ();
		~AVLEmpty                 ();
		AVLTree<K, V> *find       (K x);
		K          *max           ();
		tuple<AVLTree<K, V>*, K, V>
		           release_max    ();
		AVLTree<K, V> *remove     (K x);
		AVLTree<K, V> *insert     (K x, V val);
		int        height         ();
		K          *key           ();
		V          *value         ();
		bool       empty          ();
		AVLTree<K, V> *left       ();
		AVLTree<K, V> *right      ();
		AVLTree<K, V> *release_left  ();
		AVLTree<K, V> *release_right ();
		void       replace_left   (AVLTree<K, V> *lt);
		void       replace_right  (AVLTree<K, V> *rt);
		string     pp             ();
		int        size           ();
	private:
		unique_ptr<AVLTree<K, V> > lt;
		unique_ptr<AVLTree<K, V> > rt;
};

/* ctors */
template <class K, class V>
AVLNode<K, V>::AVLNode (AVLTree<K, V> *lt, AVLTree<K, V> *rt, K x, V val) {
	this->lt  = unique_ptr<AVLTree<K, V> >(lt);
	this->rt  = unique_ptr<AVLTree<K, V> >(rt);
	this->x   = x;
	this->val = val;
	this->h   = std::max (lt->height(), rt->height()) + 1;
}
template <class K, class V>
AVLEmpty<K, V>::AVLEmpty() {
	this->lt = nullptr;
	this->rt = nullptr;
}

/* dtors */
template <class K, class V>
AVLTree<K, V> ::~AVLTree()  { }
template <class K, class V>
AVLNode<K, V> ::~AVLNode()  { }
template <class K, class V>
AVLEmpty<K, V>::~AVLEmpty() { }

/* Rotate left, right... 
 * These methods leave the object unusable, they should be called only before 
 * it's de-allocated... */
template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::rotate_right() {
	(*(AVLTree<K, V>::p_steps))++;
	/*
	   this          l 
	   / \          / \
	  l   r  -->   ll this
	 / \              / \
	ll  lr           lr  r
	*/

	assert (!this->left()->empty());

	this->h = std::max (this->left()->right()->height(), 
	  this->right()->height()) + 1;

	// 'this' will be deallocated, so make sure it doesn't
	// hold anything...
	
	AVLTree<K, V> *l = this->release_left();
	AVLTree<K, V> *r = this->release_right();

	assert (this->lt->empty());
	assert (this->rt->empty());

	AVLTree<K, V> *lr = l->release_right();

	assert (this->left()->empty());
	assert (this->lt->empty());
	assert (l->right()->empty());

	l->replace_right (new AVLNode<K, V>(lr, r, this->x, this->val));

	assert (!l->right()->empty());

	return l;
}

template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::rotate_left() {
	(*(AVLTree<K, V>::p_steps))++;
	// cout << this->x << " rotate right" << endl;
	// Rotate by creating a copy...
	/*
		this             r
		/ \             / \
	       l   r     -->  this rr
		  / \         / \
		 rl  rr      l  rl
	*/
	this->h = std::max (this-> left()->height(),
	  this->right()->left()->height()) + 1;
	
	AVLTree<K, V> *l = this->release_left();
	AVLTree<K, V> *r = this->release_right();

	AVLTree<K, V> *rl = r->release_left();
	AVLTree<K, V> *newthis = new AVLNode<K, V>(l, rl, this->x, this->val);

	r->replace_left (newthis);

	assert (!r->left()->empty());

	assert (newthis == r->left());
	assert (*(newthis->key()) == *(this->key()));

	assert (this->lt->empty());
	assert (this->rt->empty());

	return r;
}



template <class K, class V>
K *AVLNode<K, V>::max() {
	if (this->rt->empty())
		return &(this->x);
	return this->rt->max();
}
template <class K, class V>
K *AVLEmpty<K, V>::max() {
	assert(false);
}
/* release_left */
template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::release_left() {
	assert(this->lt);
	AVLTree<K, V> *r = this->lt.release();
	this->lt.reset(new AVLEmpty<K, V>());
	this->h  = this->right()->height() + 1;
	return r;
}
template <class K, class V>
AVLTree<K, V> *AVLEmpty<K, V>::release_left() {
	assert (false);
}

/* release_right */
template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::release_right(){
	assert(this->rt);
	AVLTree<K, V> *r = this->rt.release();
	this->rt.reset(new AVLEmpty<K, V>());
	this->h = this->left()->height() + 1;
	return r;
}
template <class K, class V>
AVLTree<K, V> *AVLEmpty<K, V>::release_right() {
	cerr << "Calling release_right() on an empty node!" << endl;
	throw "error";
}

/* replace_left */
template <class K, class V>
void AVLNode<K, V>::replace_left(AVLTree<K, V> *lt) {
	assert(this->lt);
	this->lt.reset(lt);
}
template <class K, class V>
void AVLEmpty<K, V>::replace_left(AVLTree<K, V> *lt) {
	cerr << "Calling replace_left() on an empty node!" << endl;
	throw "error";
}

/* replace_right */
template <class K, class V>
void AVLNode<K, V>::replace_right(AVLTree<K, V> *rt) {
	assert(this->rt);
	this->rt.reset(rt);
}
template <class K, class V>
void AVLEmpty<K, V>::replace_right(AVLTree<K, V> *rt) {
	cerr << "Calling replace_right() on an empty node!" << endl;
	throw "error";
}

/* left */
template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::left() {
	assert(this->lt);
	return this->lt.get();
}
template <class K, class V>
AVLTree<K, V> *AVLEmpty<K, V>::left() {
	cerr << "Calling left() on an empty node!" << endl;
	// return nullptr;
	throw "error";
}

/* right */
template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::right() {
	assert(this->rt);
	return this->rt.get();
}
template <class K, class V>
AVLTree<K, V> *AVLEmpty<K, V>::right() {
	cerr << "Calling right() on an empty node!" << endl;
	throw "error";
}

/* pp, pretty print */
template <class K, class V>
string AVLNode<K, V>::pp() {
	assert(this->lt);
	assert(this->rt);
	return "(Node " 
		+ this->lt->pp() + ", " 
		+ *(this->key()) + "="
		+ boost::lexical_cast<std::string>(*(this->value())) + ", "
		+ this->rt->pp() + ")";
}
template <class K, class V>
string AVLEmpty<K, V>::pp() {
	return "Empty";
}

/* size */
template <class K, class V>
int AVLNode<K, V>::size() {
	assert(this->lt);
	assert(this->rt);
	// (*(AVLTree<K, V>::p_steps))++;
	return 1 + this->lt->size() + this->rt->size();
}
template <class K, class V>
int AVLEmpty<K, V>::size() {
	return 0;
}

/* value */
template <class K, class V>
V *AVLNode<K, V>::value() {
	return &(this->val);
}
template <class K, class V>
V *AVLEmpty<K, V>::value() {
	assert(false);
	throw "error";
}
/* key */
template <class K, class V>
K *AVLNode<K, V>::key() {
	return &(this->x);
}
template <class K, class V>
K *AVLEmpty<K, V>::key() {
	assert(false);
	throw "error";
}

/* empty */
template <class K, class V>
bool AVLNode<K, V>::empty() { return false; }
template <class K, class V>
bool AVLEmpty<K, V>::empty() { return true; }

/* height */
template <class K, class V>
int AVLNode<K, V>::height() {
	return this->h;
}
template <class K, class V>
int AVLEmpty<K, V>::height() {
	return 0;
}

/* remove 
 * returns:
 * - 'this' if there is nothing to do...
 * - a node pointer with which to replace myself (this)
 * */
template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::remove(K x) {
	(*(AVLTree<K, V>::p_steps))++;
	assert (this->lt);
	assert (this->rt);
	
	if (x == this->x) {
		// Remove myself... the balance will be checked up
		// the chain; heights will be adjusted as necessary.
		
		if (this->left()->empty() && this->right()->empty())
			return new AVLEmpty<K, V>(); // I'm a leaf, remove me
		else if (this->left()->empty())
			return this->release_right(); // Only got a right subtree
		else if (this->right()->empty()) {
			return this->release_left();
		} else {
			// Neither is empty...
			assert (!(this->left()->empty()));
			assert (!(this->right()->empty()));

			// Return the rightmost leaf in the left subtree, 
			// and release it.

			K kmax;
			V vmax;
			AVLTree<K, V> *replacement;

			tie (replacement, kmax, vmax) = this->left()->release_max();
			if (replacement != this->left())
				this->replace_left(replacement);
			// Then, release my own children and give them to lmax
			
			assert (kmax < this->x);
			assert (this->x < *(this->right()->key()));
			assert (this->left()->empty() 
			       || kmax > *(this->left()->key()));

			return new AVLNode(this->release_left(), 
			  this->release_right(), kmax, vmax);
		}
	}

	if (x < this->x) {
		AVLTree<K, V> *result;
		result = this->left()->remove(x);
		if (result != this->left()) this->replace_left(result);
		// We have potentially removed a node in our lt...
		// Check if a rebalancing is needed, adjust heights.
		// Are we heavy on the rhs?
		if (this->rt->height() > this->lt->height() + 1)
			return this->rotate_left(); // Heavy on the right side...
	}

	if (x > this->x) {
		AVLTree<K, V> *result;
		result = this->right()->remove(x);
		if (result != this->right()) this->replace_right (result);
		// We have potentially removed a node in our rt,
		// Check if a rebalancing is needed, adjust heights.
		// If no rebalancing is needed here return 'this'
		// because a rebalancing might be needed upwards in the 
		// tree.
		if (this->lt->height() > this->rt->height() + 1)
			return this->rotate_right(); // Heavy on the left side...
	}
	this->h = std::max (this->rt->height(), this->lt->height()) + 1;
	
	// Nothing to do upwards in the tree...
	return this;
}
template <class K, class V> 
AVLTree<K, V> *AVLEmpty<K, V>::remove(K x) {
	// Trying to delete a value that hasn't been found in the tree, do nothing
	assert (false);
	return this;
}

/* release_max 
 * - returns a tuple of (node*, node*)
 *     - The left member is the new value that should replace me, or nullptr; 
 *     - The right member is the maximum node that was found and removed.
 * */
template <class K, class V>
tuple<AVLTree<K, V>*, K, V> AVLNode<K, V>::release_max() {
	(*(AVLTree<K, V>::p_steps))++;
	if (this->right()->empty()) {
		// delete this->rt.release();
		return make_tuple (this->release_left(), *(this->key()), 
		  *(this->value()));
	}

	AVLTree<K, V> *replacement;
	K k; V v;
	tie (replacement, k, v) = this->right()->release_max();
	if (replacement != this->right())
		this->replace_right (replacement);
	return make_tuple (this, k, v);
}
template <class K, class V>
tuple<AVLTree<K, V>*, K, V> AVLEmpty<K, V>::release_max() {
	cerr << "Calling release_max() on an empty node!" << endl;
	assert (false);
}

/* find */
template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::find(K x) {
	assert(this->lt);
	assert(this->rt);
	(*(AVLTree<K, V>::p_steps))++;
	if (this->x == x) return this;
	if (x < this->x)  return this->lt->find(x);
	if (x > this->x)  return this->rt->find(x);
}
template <class K, class V>
AVLTree<K, V> *AVLEmpty<K, V>::find(K x) {
	(*(AVLTree<K, V>::p_steps))++;
	return this;
}

/* 
 * insert 
 *   Returns 
 *   - a new node to replace the subtree, or 
 *   - nullptr if nothing should be replaced...
 */
template <class K, class V>
AVLTree<K, V> *AVLNode<K, V>::insert(K x, V val) {
	(*(AVLTree<K, V>::p_steps))++;
	// Inserting to an already existing value, do nothing...
	if (x == this->x) return nullptr;

	assert(this->lt);
	assert(this->rt);

	if (x < this->x) {
		AVLTree<K, V> *result = this->lt->insert (x, val);
		if (result) this->replace_left(result);

		// Are we heavy on the left side after the insertion?
		if (this->lt->height() > this->rt->height() + 1)
			return this->rotate_right();
	} else {
		assert (x > this->x);
		AVLTree<K, V> *result = this->rt->insert (x, val);
		if (result) this->replace_right(result);

		// Are we heavy on the right side after the insertion?
		if (this->rt->height() > this->lt->height() + 1)
			return this->rotate_left(); 
	}
	this->h = std::max (this->lt->height(), this->rt->height()) + 1;

	return nullptr;
}
template <class K, class V>
AVLTree<K, V> *AVLEmpty<K, V>::insert (K x, V val) {
	(*(AVLTree<K, V>::p_steps))++;
	return new AVLNode<K, V>(new AVLEmpty(), new AVLEmpty(), x, val);
}

template <class K, class V>
void avl_insert (AVLTree<K, V> **t, K x, V val) {
	AVLTree<K, V> *r = (*t)->insert (x, val);
	if (r != nullptr) {
		delete *t;
		*t = r;
	}
}

template <class K, class V>
void avl_remove (AVLTree<K, V> **t, K x) {
	if ((*t)->empty()) return;
	AVLTree<K, V> *r = (*t)->remove (x);
	if (r != *t) {
		delete *t;
		*t = r;
	}
}

template <class K, class V>
tuple<K, V> avl_release_max (AVLTree<K, V> **t) {
	assert (!(*t)->empty());
	AVLTree<K, V> *replacement;
	K k;
	V v;

	tie (replacement, k, v) = (*t)->release_max();
	if (replacement != *t) {
		delete *t;
		*t = replacement;
	}
	return make_tuple (k, v);
}

#endif
