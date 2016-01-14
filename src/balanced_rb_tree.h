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
		// virtual RBTree<K, V> *remove       (K k)      = 0;
		virtual RBTree<K, V>  *insert       (K k, V v) = 0;
		virtual bool          empty         ()         = 0;
		virtual void          replace_left  (RBTree<K, V> *lt) = 0;
		virtual void          replace_right (RBTree<K, V> *rt) = 0;
		virtual RBTree<K, V>  *release_left  () = 0;
		virtual RBTree<K, V>  *release_right () = 0;
		virtual bool          red            () = 0;
		virtual void          flip_red       () = 0;
		// virtual string        pp            () = 0;
};

template <class K, class V>
class RBNode: public RBTree<K, V> {
	public:
		RBNode (RBTree<K, V> *lt, RBTree<K, V> *rt, K x, V v);
		~RBNode ();
		RBTree<K, V>  *find         (K k);
		RBTree<K, V>  *insert       (K k, V v);
		bool          empty         ();
		void          replace_left  (RBTree<K, V> *lt);
		void          replace_right (RBTree<K, V> *rt);
		RBTree<K, V>  *release_left  ();
		RBTree<K, V>  *release_right ();
		bool          red            ();
		void          flip_red       ();
	private:
		K key;
		V val;
		bool is_red = false;
		RBTree<K, V> *rotate_left  ();
		RBTree<K, V> *rotate_right ();
		void flip_colors           ();
		unique_ptr<RBTree<K, V> > lt;
		unique_ptr<RBTree<K, V> > rt;
};

template <class K, class V>
class RBEmpty: public RBTree<K, V> {
	public:
		RBEmpty  ();
		~RBEmpty ();
		RBTree<K, V>  *find         (K k);
		RBTree<K, V>  *insert       (K k, V v);
		bool          empty         ();
		void          replace_left  (RBTree<K, V> *lt);
		void          replace_right (RBTree<K, V> *rt);
		RBTree<K, V>  *release_left  ();
		RBTree<K, V>  *release_right ();
		bool          red            ();
		void          flip_red       ();
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

	this->is_red = true;
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

/* insert */
template <class K, class V>
RBTree<K, V>* RBEmpty<K, V>::insert (K k, V v) {
	return new RBNode<K, V> (new RBEmpty<K, V>(), new RBEmpty<K, V>(), k, v);
}
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::insert (K k, V v) {
	
	if (this->lt->red() && this->rt->red()) this->flip_colors();

	if (k == this->key) return this;
	if (k < this->key)
		this->replace_left(this->lt->insert(k, v));
	if (k > this->key)
		this->replace_right(this->rt->insert(k, v));

	// Is one of our children passing a red link upwards?
	if (this->rt->red()  && !this->lt->red()) return this->rotate_left();
	if (!this->rt->red() && this->lt->red())  return this->rotate_right();

	return this;
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

/* rotate_right */
template <class K, class V>
RBTree<K, V>* RBNode<K, V>::rotate_right() {
	RBTree<K, V> *l = this->release_left();
	RBTree<K, V> *r = this->release_right();

	RBTree<K, V> *lr = l->release_right();
	l->replace_right(new RBNode<K, V>(lr, r, this->key, this->val));

	return l;
}

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
	*t = (*t)->insert (key, val);
}

#endif
