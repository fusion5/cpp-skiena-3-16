#ifndef SKIENA_3_16_RB_TREE
#define SKIENA_3_16_RB_TREE

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <tuple>

/*
 * Think of an R-B tree as a 2-3 tree encoded in a binary
 * tree. This idea was seen in a paper by Robert Sedgewick, "Left-leaning
 * Red-Black Trees".
 * */

template <class K, class V>
class RBTree {
	public:
		virtual ~RBTree<K, V>               ()         = 0;
		virtual RBTree<K, V>  *find         (K k)      = 0;
		// virtual RBTree<K, V> *remove     (K k)      = 0;
		virtual RBTree<K, V>  *insert       (K k, V v) = 0;
		virtual bool          empty         ()         = 0;
		virtual void          replace_left  (RBTree<K, V> *lt) = 0;
		virtual void          replace_right (RBTree<K, V> *rt) = 0;
		virtual RBTree<K, V>  *release_left  () = 0;
		virtual RBTree<K, V>  *release_right () = 0;
		virtual RBTree<K, V>  *release_min   () = 0;
		virtual RBTree<K, V>  *left          () = 0;
		virtual RBTree<K, V>  *right         () = 0;
		virtual RBTree<K, V>  *move_red_left () = 0;
		virtual bool          red            () = 0;
		virtual void          flip_red       () = 0;
		virtual string        pp             () = 0;
		virtual RBTree<K, V>  *rotate_left   () = 0;
		virtual RBTree<K, V>  *rotate_right  () = 0;
		virtual void          flip_colors    () = 0;
};

template <class K, class V>
class RBNode: public RBTree<K, V> {
	public:
		RBNode (RBTree<K, V> *lt, RBTree<K, V> *rt, K x, V v);
		~RBNode ();
		RBTree<K, V>  *find          (K k);
		RBTree<K, V>  *insert        (K k, V v);
		bool          empty          ();
		void          replace_left   (RBTree<K, V> *lt);
		void          replace_right  (RBTree<K, V> *rt);
		RBTree<K, V>  *release_left  ();
		RBTree<K, V>  *release_right ();
		RBTree<K, V>  *release_min   ();
		RBTree<K, V>  *left          ();
		RBTree<K, V>  *right         ();
		RBTree<K, V>  *move_red_left ();
		bool          red            ();
		void          flip_red       ();
		string        pp             ();
		RBTree<K, V>  *rotate_left   ();
		RBTree<K, V>  *rotate_right  ();
		void          flip_colors   ();
	private:
		K key;
		V val;
		bool is_red = true;
		unique_ptr<RBTree<K, V> > lt;
		unique_ptr<RBTree<K, V> > rt;
};

template <class K, class V>
class RBEmpty: public RBTree<K, V> {
	public:
		RBEmpty  ();
		~RBEmpty ();
		RBTree<K, V>  *find          (K k);
		RBTree<K, V>  *insert        (K k, V v);
		bool          empty          ();
		void          replace_left   (RBTree<K, V> *lt);
		void          replace_right  (RBTree<K, V> *rt);
		RBTree<K, V>  *release_left  ();
		RBTree<K, V>  *release_right ();
		RBTree<K, V>  *release_min   ();
		RBTree<K, V>  *left          ();
		RBTree<K, V>  *right         ();
		RBTree<K, V>  *move_red_left ();
		bool          red            ();
		void          flip_red       ();
		string        pp             ();
		RBTree<K, V>  *rotate_left   ();
		RBTree<K, V>  *rotate_right  ();
		void          flip_colors    ();
	private:
		K key;
		V val;
};

/* ctors */
template <class K, class V>
RBNode<K, V>::RBNode (RBTree<K, V> *lt, RBTree<K, V> *rt, K k, V v) {
	this->key = k;
	this->val = v;
	this->lt = unique_ptr<RBTree<K, V> >(lt);
	this->rt = unique_ptr<RBTree<K, V> >(rt);
}
template <class K, class V>
RBEmpty<K, V>::RBEmpty () { }

/* dtors */
template <class K, class V>
RBTree<K, V> ::~RBTree  () { }
template <class K, class V>
RBNode<K, V> ::~RBNode  () { }
template <class K, class V>
RBEmpty<K, V>::~RBEmpty () { }

/* empty */
template <class K, class V>
bool RBEmpty<K, V>::empty() { return true; }
template <class K, class V>
bool RBNode<K, V>::empty()  { return false; }

/* red */
template <class K, class V>
bool RBEmpty<K, V>::red() { return false; }
template <class K, class V>
bool RBNode <K, V>::red() { return this->is_red; }

/* find */
template <class K, class V>
RBTree<K, V> *RBNode<K, V>::find(K key) {
	if (this->key == key) return this;
	if (key < this->key)  return this->lt->find(key);
	if (key > this->key)  return this->rt->find(key);
}
template <class K, class V>
RBTree<K, V> *RBEmpty<K, V>::find(K key) {
	return this;
}

/* insert 
 * returns nullptr if my parent link should not be changed; a new pointer 
 * otherwise
 * */
template <class K, class V>
RBTree<K, V>* RBEmpty<K, V>::insert (K k, V v) {
	return new RBNode<K, V> (new RBEmpty<K, V>(), new RBEmpty<K, V>(), k, v);
}
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::insert (K k, V v) {
	
	if (k == this->key) return nullptr;

	RBTree<K, V> *res;

	if (k < this->key) {
		res = this->lt->insert (k, v);
		if (res) this->replace_left (res);
	}
	if (k > this->key) {
		res = this->rt->insert (k, v);
		if (res) this->replace_right (res);
	}

	assert (this->rt);
	assert (this->lt);

	if (this->rt->red()  && !this->lt->red()) return this->rotate_left();
	if (!this->rt->red() && this->lt->red())  return this->rotate_right();

	// If, as a result of insert, we are a node with 2 red nodes, we split
	// them and, if this is a black node, it passes a red link upwards (it 
	// transitions to a red node).
	// If this was already a red node, then it becomes black. This reduces 
	// the size of the red-cluster of which it was part of.
	if (this->lt->red()  && this->rt->red())  this->flip_colors();

	return nullptr;
}

/* release_min */
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::release_min() {

	RBTree<K, V>* ret = this;
	
	// Found the node to be deleted...
	if (this->lt->empty()) return new RBEmpty<K, V>(); 
	
	if (!this->left()->red() && !this->left()->left()->red())
		ret = ret->move_red_left();

	ret->replace_left(ret->left()->release_min());
	
	assert (ret->left() && ret->right());

	if (ret->right()->red() && !ret->left()->red())
		ret = ret->rotate_left();
	if (!ret->right()->red() && ret->left()->red())
		ret = ret->rotate_right();
	if (ret->left()->red() && ret->right()->red())
		ret->flip_colors();

	return ret;
}
template <class K, class V>
RBTree<K, V>* RBEmpty<K, V>::release_min() {
	assert (false);
}

/* left */
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::left() {
	return this->lt.get();
}
template <class K, class V>
RBTree<K, V>* RBEmpty<K, V>::left() {
	assert (false);
}

/* right */
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::right() {
	return this->rt.get();
}
template <class K, class V>
RBTree<K, V>* RBEmpty<K, V>::right() {
	assert (false);
}

/* move_red_left 
 * This is the counterpart of split and merge from the 2-3 tree, which moves a
 * child from the node to its sibling...
 * */
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::move_red_left() {
	RBTree<K, V>* ret = this;
	this->flip_colors();
	if (this->right()->left()->red()) {
		this->replace_right(this->right()->rotate_right());
		ret = this->rotate_left();
		ret->flip_red();
	}
	return ret;
}
template <class K, class V>
RBTree<K, V>* RBEmpty<K, V>::move_red_left() {
	assert (false);
}

/* rotate_left */
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::rotate_left () {
	RBTree<K, V> *l = this->release_left();
	RBTree<K, V> *r = this->release_right();

	RBTree<K, V> *rl = r->release_left();
	r->replace_left (new RBNode<K, V>(l, rl, this->key, this->val));

	return r;
}
template <class K, class V>
RBTree<K, V>* RBEmpty<K, V>::rotate_left () { assert(false); }

/* rotate_right */
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::rotate_right() {
	RBTree<K, V> *l = this->release_left();
	RBTree<K, V> *r = this->release_right();

	RBTree<K, V> *lr = l->release_right();
	l->replace_right(new RBNode<K, V>(lr, r, this->key, this->val));

	return l;
}
template <class K, class V>
RBTree<K, V>* RBEmpty<K, V>::rotate_right() { assert(false); }

/* release_left  */
template <class K, class V>
RBTree<K, V> *RBNode<K, V>::release_left() {
	RBTree<K, V> *r = this->lt.release();
	this->lt.reset(new RBEmpty<K, V>());
	return r;
}
template <class K, class V>
RBTree<K, V> *RBEmpty<K, V>::release_left() {
	assert (false);
}

/* release_right */
template <class K, class V>
RBTree<K, V> *RBNode<K, V>::release_right() {
	RBTree<K, V> *r = this->rt.release();
	this->rt.reset(new RBEmpty<K, V>());
	return r;
}
template <class K, class V>
RBTree<K, V> *RBEmpty<K, V>::release_right() {
	assert (false);
}

/* replace_left, replace_right... */
template <class K, class V>
void RBNode<K, V>::replace_left(RBTree<K, V> *lt) {
	this->lt.reset(lt);
}
template <class K, class V>
void RBNode<K, V>::replace_right(RBTree<K, V> *rt) {
	this->rt.reset(rt);
}
template <class K, class V>
void RBEmpty<K, V>::replace_left(RBTree<K, V> *_)  { assert(false); }
template <class K, class V>
void RBEmpty<K, V>::replace_right(RBTree<K, V> *_) { assert(false); }

/* flip_colors */
template <class K, class V>
void RBNode<K, V>::flip_colors() {
	this->flip_red();
	this->lt->flip_red();
	this->rt->flip_red();
}
template <class K, class V>
void RBEmpty<K, V>::flip_colors() {
	assert(false);
}

/* flip_red */
template <class K, class V>
void RBNode<K, V>::flip_red() {
	this->is_red = !this->is_red;
}
template <class K, class V>
void RBEmpty<K, V>::flip_red() {
	assert (false);
}

template <class K, class V>
void rb_insert (RBTree<K, V> **t, K key, V val) {
	RBTree<K, V> *r;
	r = (*t)->insert (key, val);
	if (r) {
		if (r->red()) r->flip_red(); // Ensure that the root is black
		delete *t;
		*t = r;
	}
}

template <class K, class V>
void rb_release_min (RBTree<K, V> **t) {
	RBTree<K, V> *r;
	r = (*t)->release_min();
	if (r != *t) {
		if (r->red()) r->flip_red(); // Ensure that the root is black
		delete *t;
		*t = r;
	}
}

/* pp */
template <class K, class V>
string RBNode<K, V>::pp() {
	return "(Node "
		+ this->lt->pp() + ", "
		+ boost::lexical_cast<std::string>(this->key) + "="
		+ boost::lexical_cast<std::string>(this->val) + ", "
		+ this->rt->pp() + ")";
}
template <class K, class V>
string RBEmpty<K, V>::pp() {
	return "<>";
}

#endif
