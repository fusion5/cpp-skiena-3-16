#ifndef SKIENA_3_16_AVL_TREE
#define SKIENA_3_16_AVL_TREE

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace std;

template <class T> class AVLTree;

/*
 *  e.g. tree:
 *
 *      2
 *     / \
 *    1   3
 **/

template <class T>
class AVLTree {
	public: 
		virtual AVLTree<T> *find          (T x) = 0;
		virtual AVLTree<T> *release_max   (AVLTree<T> *parent) = 0;
		virtual AVLTree<T> *remove        (T x) = 0;
		virtual AVLTree<T> *insert        (T x) = 0;
		virtual int        height         ()    = 0;
		virtual T          value          ()    = 0;
		virtual bool       empty          ()    = 0;
		virtual AVLTree<T> *left          ()    = 0;
		virtual AVLTree<T> *right         ()    = 0;
		virtual AVLTree<T> *release_right ()    = 0;
		virtual AVLTree<T> *release_left  ()    = 0;
		virtual void       replace_left   (AVLTree<T> *lt) = 0;
		virtual void       replace_right  (AVLTree<T> *rt) = 0;
		virtual string     pp             ()    = 0;
		virtual int        size           ()    = 0;
		static  int        steps;
	protected:
	private:
		unique_ptr<AVLTree<T> > lt;
		unique_ptr<AVLTree<T> > rt;
};

template <class T>
int AVLTree<T>::steps = 0;

template <class T>
class AVLNode: public AVLTree<T> {
	public:
		AVLNode  (AVLTree<T> *lt, AVLTree<T> *rt, T x);
		~AVLNode ();
		AVLTree<T> *find          (T x);
		AVLTree<T> *release_max   (AVLTree<T> *parent);
		AVLTree<T> *remove        (T x);
		AVLTree<T> *insert        (T x);
		int        height         ();
		T          value          ();
		bool       empty          ();
		AVLTree<T> *left          ();
		AVLTree<T> *right();
		AVLTree<T> *release_left  ();
		AVLTree<T> *release_right ();
		void       replace_left   (AVLTree<T> *lt);
		void       replace_right  (AVLTree<T> *rt);
		string     pp             ();
		int        size           ();

	protected:
	private:
		T          x;
		int        h;
		AVLTree<T> *rotate_left   ();
		AVLTree<T> *rotate_right  ();
		unique_ptr<AVLTree<T> > lt;
		unique_ptr<AVLTree<T> > rt;
};

template <class T>
class AVLEmpty: public AVLTree<T> {
	public:
		AVLEmpty                  ();
		~AVLEmpty                 ();
		AVLTree<T> *find          (T x);
		AVLTree<T> *release_max   (AVLTree<T> *parent);
		AVLTree<T> *remove        (T x);
		AVLTree<T> *insert        (T x);
		int        height         ();
		T          value          ();
		bool       empty          ();
		AVLTree<T> *left          ();
		AVLTree<T> *right         ();
		AVLTree<T> *release_left  ();
		AVLTree<T> *release_right ();
		void       replace_left   (AVLTree<T> *lt);
		void       replace_right  (AVLTree<T> *rt);
		string     pp             ();
		int        size           ();
	private:
		unique_ptr<AVLTree<T> > lt;
		unique_ptr<AVLTree<T> > rt;
};

/* constructors */
template <class T>
AVLNode<T>::AVLNode (AVLTree<T> *lt, AVLTree<T> *rt, T x) {
	this->lt = unique_ptr<AVLTree<T> >(lt);
	this->rt = unique_ptr<AVLTree<T> >(rt);
	this->x  = x;
	this->h  = max (lt->height(), rt->height()) + 1;
}
template <class T>
AVLEmpty<T>::AVLEmpty() {
	this->lt = nullptr;
	this->rt = nullptr;
}

/* Rotate left, right... 
 * These methods leave the object unusable, they should be called only before 
 * it's de-allocated... */
template <class T>
AVLTree<T> *AVLNode<T>::rotate_right() {
	/*
	   this          l 
	   / \          / \
	  l   r  -->   ll this
	 / \              / \
	ll  lr           lr  r
	*/

	assert (!this->left()->empty());

	this->h = max (this->left()->right()->height(), 
	  this->right()->height()) + 1;


	// 'this' will be deallocated, so make sure it doesn't
	// hold anything...
	
	AVLTree<T> *l = this->release_left();
	AVLTree<T> *r = this->release_right();

	assert (this->lt->empty());
	assert (this->rt->empty());

	AVLTree<T> *lr = l->release_right();

	assert (this->left()->empty());
	assert (this->lt->empty());
	assert (l->right()->empty());

	l->replace_right (new AVLNode<T>(lr, r, this->x));

	assert (!l->right()->empty());

	return l;
}

template <class T>
AVLTree<T> *AVLNode<T>::rotate_left() {
	// cout << this->x << " rotate right" << endl;
	// Rotate by creating a copy...
	/*
		this             r
		/ \             / \
	       l   r     -->  this rr
		  / \         / \
		 rl  rr      l  rl
	*/
	this->h = max (this-> left()->height(),
	  this->right()->left()->height()) + 1;
	
	AVLTree<T> *l = this->release_left();
	AVLTree<T> *r = this->release_right();

	AVLTree<T> *rl = r->release_left();
	AVLTree<T> *newthis = new AVLNode<T>(l, rl, this->x);

	r->replace_left (newthis);

	assert (!r->left()->empty());

	assert (newthis == r->left());
	assert (newthis->value() == this->value());

	assert (this->lt->empty());
	assert (this->rt->empty());

	return r;
}


/* destructors */
template <class T>
AVLNode<T>::~AVLNode() {
}
template <class T>
AVLEmpty<T>::~AVLEmpty() {
}

/* release_left */
template <class T>
AVLTree<T> *AVLNode<T>::release_left(){
	assert(this->lt);
	AVLTree<T> *r = this->lt.release();
	this->lt = unique_ptr<AVLTree<T> > (new AVLEmpty<T>());
	this->h  = this->right()->height() + 1;
	return r;
}
template <class T>
AVLTree<T> *AVLEmpty<T>::release_left() {
	cerr << "Calling release_left() on an empty node!" << endl;
	throw "error";
}

/* release_right */
template <class T>
AVLTree<T> *AVLNode<T>::release_right(){
	assert(this->rt);
	AVLTree<T> *r = this->rt.release();
	this->rt = unique_ptr<AVLTree<T> > (new AVLEmpty<T>());
	this->h = this->left()->height() + 1;
	return r;
}
template <class T>
AVLTree<T> *AVLEmpty<T>::release_right() {
	cerr << "Calling release_right() on an empty node!" << endl;
	throw "error";
}

/* replace_left */
template <class T>
void AVLNode<T>::replace_left(AVLTree<T> *lt) {
	assert(this->lt);
	this->lt.reset(lt);
}
template <class T>
void AVLEmpty<T>::replace_left(AVLTree<T> *lt) {
	cerr << "Calling replace_left() on an empty node!" << endl;
	throw "error";
}

/* replace_right */
template <class T>
void AVLNode<T>::replace_right(AVLTree<T> *rt) {
	assert(this->rt);
	this->rt.reset(rt);
}
template <class T>
void AVLEmpty<T>::replace_right(AVLTree<T> *rt) {
	cerr << "Calling replace_right() on an empty node!" << endl;
	throw "error";
}

/* left */
template <class T>
AVLTree<T> *AVLNode<T>::left() {
	assert(this->lt);
	return this->lt.get();
}
template <class T>
AVLTree<T> *AVLEmpty<T>::left() {
	cerr << "Calling left() on an empty node!" << endl;
	// return nullptr;
	throw "error";
}

/* right */
template <class T>
AVLTree<T> *AVLNode<T>::right() {
	assert(this->rt);
	return this->rt.get();
}
template <class T>
AVLTree<T> *AVLEmpty<T>::right() {
	cerr << "Calling right() on an empty node!" << endl;
	throw "error";
}

/* pp, pretty print */
template <class T>
string AVLNode<T>::pp() {
	assert(this->lt);
	assert(this->rt);
	return "(Node " 
		+ this->lt->pp() + ", " 
		+ this->value() + ", "
		+ this->rt->pp() + ")";
}
template <class T>
string AVLEmpty<T>::pp() {
	return "Empty";
}

/* size */
template <class T>
int AVLNode<T>::size() {
	assert(this->lt);
	assert(this->rt);
	AVLTree<T>::steps++;
	return 1 + this->lt->size() + this->rt->size();
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

/* remove 
 * returns:
 * 	nullptr if there is nothing to do
 * 	a node pointer with which to replace myself (this)
 * */
template <class T>
AVLTree<T> *AVLNode<T>::remove(T x) {
	AVLTree<T>::steps++;
	assert (this->lt);
	assert (this->rt);
	
	if (x == this->x) {
		// Remove myself... we know that the balance will be checked up
		// the chain, and that heights will be adjusted as necessary.
		
		if (this->left()->empty() && this->right()->empty())
			return new AVLEmpty<T>(); // I'm a leaf, just remove me
		else if (this->left()->empty())
			return this->release_right(); // Only got a right subtree
		else if (this->right()->empty())
			return this->release_left(); // Only got a left subtree
		else {
			// Neither is empty...
			assert (!(this->left()->empty()));
			assert (!(this->right()->empty()));

			// Return the rightmost leaf in the left subtree, 
			// and release it.

			AVLTree<T> *lmax  = this->left()->release_max(this);

			// lmaxl needs to take lmax's place...
			AVLTree<T> *lmaxl = lmax->release_left();

			// assert (lmax->left()->empty());
			assert (lmax->right()->empty());
			// Then, release my own children and give them to lmax
			
			assert (lmax->value() < this->x);
			assert (this->x < this->right()->value());
			assert (this->left()->empty() 
			       || (lmax->value() > this->left()->value()));

			lmax->replace_right(this->release_right());
			lmax->replace_left(this->release_left());
			
			// I will be deallocated by my parent and replaced with
			// rmost... this ensures a correct ordering in the
			// resulting subtree.
			return lmax;
		}
	}

	if (x < this->x) {
		AVLTree<T> *result;
		result = this->left()->remove(x);
		if (result) this->replace_left(result);
		// We have potentially removed a node in our lt...
		// Check if a rebalancing is needed, adjust heights.
		// Are we heavy on the rhs?
		if (this->rt->height() > this->lt->height() + 1)
			return this->rotate_left(); // Heavy on the right side...

		this->h = max (this->rt->height(), this->lt->height()) + 1;
	}

	if (x > this->x) {
		AVLTree<T> *result;
		result = this->right()->remove(x);
		if (result) this->replace_right(result);
		// We have potentially removed a node in our rt,
		// Check if a rebalancing is needed, adjust heights.
		// If no rebalancing is needed here return 'this'
		// because a rebalancing might be needed upwards in the 
		// tree.
		if (this->lt->height() > this->rt->height() + 1)
			return this->rotate_right(); // Heavy on the left side...

		this->h = max (this->rt->height(), this->lt->height()) + 1;
	}
	
	// Nothing to do upwards in the tree...
	return nullptr;
}
template <class T> 
AVLTree<T> *AVLEmpty<T>::remove(T x) {
	// Trying to delete a value that hasn't been found in the tree, do nothing
	return nullptr;
}

/* release_max */
template <class T>
AVLTree<T> *AVLNode<T>::release_max(AVLTree<T> *my_parent) {
	AVLTree<T>::steps++;
	if (this->right()->empty()) {
		// I should be released!
		//return this;
		
		assert (!(my_parent->empty()));

		// we have descended only right-wise:
		assert (!(my_parent->right()->empty()));

		if (my_parent->right()->value() == this->value()) {
			AVLTree<T> *lt = this->release_left();
			AVLTree<T> *ret = my_parent->release_right();
			my_parent->replace_right(lt);
			return ret;
		}

		if (my_parent->left()->value() == this->value()) {
			AVLTree<T> *lt = this->release_left();
			AVLTree<T> *ret = my_parent->release_left();
			my_parent->replace_left(lt);
			return ret;
		}
	}

	return this->right()->release_max(this);
}
template <class T>
AVLTree<T> *AVLEmpty<T>::release_max(AVLTree<T> *my_parent) {
	cerr << "Calling release_max() on an empty node!" << endl;
	throw "error";
}

/* find */
template <class T>
AVLTree<T> *AVLNode<T>::find(T x) {
	assert(this->lt);
	assert(this->rt);
	AVLTree<T>::steps++;
	if (this->x == x) return this;
	if (x < this->x)  return this->lt->find(x);
	if (x > this->x)  return this->rt->find(x);
}
template <class T>
AVLTree<T> *AVLEmpty<T>::find(T x) {
	AVLTree<T>::steps++;
	return this;
}

/* 
 * insert 
 *   Returns a new node to replace the subtree, or nullptr if there is nothing
 *   to replace...
 */
template <class T>
AVLTree<T> *AVLNode<T>::insert(T x) {
	AVLTree<T>::steps++;
	// Inserting to an already existing value, do nothing...
	if (x == this->x) return nullptr;

	assert(this->lt);
	assert(this->rt);

	if (x < this->x) {
		AVLTree<T> *r = this->lt->insert (x);
		if (r != nullptr)
			this->lt.reset (r);

		// Are we heavy on the left side after the insertion?
		if (this->lt->height() > this->rt->height() + 1)
			return this->rotate_right();
	} else {
		assert (x > this->x);
		AVLTree<T> *r = this->rt->insert (x);
		if (r != nullptr)
			this->rt.reset(r);

		// Are we heavy on the right side after the insertion?
		if (this->rt->height() > this->lt->height() + 1)
			return this->rotate_left(); 
	}
	this->h = max (this->lt->height(), this->rt->height()) + 1;

	return nullptr;
}
template <class T>
AVLTree<T> *AVLEmpty<T>::insert (T x) {
	AVLTree<T>::steps++;
	return new AVLNode<T>(new AVLEmpty(), new AVLEmpty(), x);
}

template <class T>
void avl_insert (unique_ptr<AVLTree<T> > *t, T x) {
	AVLTree<T> *r = (*t)->insert (x);
	if (r != nullptr) (*t).reset (r);
}

template <class T>
void avl_remove (unique_ptr<AVLTree<T> > *t, T x) {
	AVLTree<T> *r = (*t)->remove (x);
	if (r != nullptr) (*t).reset (r);
}

#endif
