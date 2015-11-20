#ifndef SKIENA_3_16_AVL_TREE
#define SKIENA_3_16_AVL_TREE

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace std;

template <class T> class AVLTree;

template <class T>
using insert_avl_type = unique_ptr<AVLTree<T> >;

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
		virtual AVLTree<T> *release_right() = 0;
		virtual AVLTree<T> *release_left() = 0;
		virtual string pp () = 0;
		virtual int size () = 0;

		unique_ptr<AVLTree<T> > lt;
		unique_ptr<AVLTree<T> > rt;

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
		AVLTree<T> *release_left();
		AVLTree<T> *release_right();
		string pp ();
		int size ();

		unique_ptr<AVLTree<T> > lt;
		unique_ptr<AVLTree<T> > rt;
	protected:
		// AVLTree<T> *lt;
		// AVLTree<T> *rt;
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
		AVLTree<T> *release_left();
		AVLTree<T> *release_right();
		string pp ();
		int size ();

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
	return this->lt.release();
}
template <class T>
AVLTree<T> *AVLEmpty<T>::release_left() {
	cerr << "Calling release_left() on an empty node!" << endl;
	// return nullptr;
	throw "error";
}

/* release_right */
template <class T>
AVLTree<T> *AVLNode<T>::release_right(){
	return this->rt.release();
}
template <class T>
AVLTree<T> *AVLEmpty<T>::release_right() {
	cerr << "Calling release_right() on an empty node!" << endl;
	throw "error";
}

/* left */
template <class T>
AVLTree<T> *AVLNode<T>::left() {
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

/* find */
template <class T>
AVLTree<T> *AVLNode<T>::find(T x) {
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
template <class T>
void avl_tree_insert (unique_ptr<AVLTree<T> > t, T x) {
	insert_avl_type<T> r = t->insert (x);
	if (r) t.reset(r.release()); 
}
*/

 /* insert */
template <class T>
insert_avl_type<T> AVLNode<T>::insert(T x) {
	AVLTree<T>::steps++;
	// Inserting to an already existing value, do nothing...
	if (x == this->x) return nullptr;

	if (x < this->x) {
		insert_avl_type<T> r = this->lt->insert (x);
		if (r) 	{
			cout << this->x << " left child reassiged... from: " 
			     << this->lt->pp()
			     << endl;

			this->lt.reset (r.release());

			cout << "to: "
			     << this->lt->pp()
			     << endl;
		}
		// Are we heavy on the left side after the insertion?
		if (this->lt->height() > this->rt->height() + 1) {

			cout << this->x << " rotate left" << endl;
			// Rotate...
			// Here, lt is a balanced tree of height 2 or 
			// more.
			// Therefore, it must have both lt->lt and lt->rt, else 
			// it wouldn't be a balanced tree.
			cout << this->x << ": " << this->pp() << endl;
			cout << "height: " << this->lt->height() 
			     << " empty: " << this->lt->empty()
			     << endl;

			cout << this->x << " l: " << this->lt->pp() << endl;
			cout << (bool) (this->lt) << endl;

			cout << this->x << " ll: " << endl;
			cout << (bool) (this->left()->left()) << endl;
			cout << this->left()->left()->pp() << endl;

			cout << this->x << " lr: " << this->left()->right()->pp() 
			     << endl;
			cout << (bool) (this->left()->rt) << endl;

			this->h = max ( this->left()->right()->height()	
				      , this->right()->height()) + 1;

			// Essentially, l needs to be deallocated so we pick
			// it apart...
			// AVLTree<T> *ll = this->lt->lt.release();
			AVLTree<T> *lr = this->left()->release_right();

			// T v = this->lt->value();

			/*
			   this          l 
			   / \          / \
			  l   r  -->   ll this
			 / \              / \
			ll  lr           lr  r
			*/

			AVLTree<T> *l = this->lt.release();
			AVLTree<T> *r = this->rt.release();

			// this->left is now null...
			// l->lt = unique_ptr<AVLTree<T> >(ll);
			// The problem is that now, 'this' becomes referenced by 2
			// unique_ptr, the one from our parent and this one.
			l->rt = unique_ptr<AVLTree<T> >(
				new AVLNode<T>(lr, r, this->x));

			cout << this->x << " ok2" << endl;

			// this->lt = unique_ptr<AVLTree<T> >(lr);
			// return unique_ptr<AVLTree<T> >(new AVLNode (ll, this, v));
			// AVLTree<T> *l = this->lt.release();
			// Make l the top child instead of me...
			// I will be freed by the caller.
			return unique_ptr<AVLTree<T> >(l);
		}
	} else {
		// x > this->x
		insert_avl_type<T> r = this->rt->insert (x);
		if (r) {
			cout << this->x << " right child reassign..." 
			     << endl;
			this->rt.reset(r.release());
		}
		// Are we heavy on the right side after the insertion?
		if (this->rt->height() > this->lt->height() + 1) {
			cout << "Rotate right" << endl;
			// Rotate by creating a copy...
			this->h = max ( this->left()->height()
				      , this->right()->left()->height()) + 1;

			AVLTree<T> *rl = this->right()->release_left();

			/*
 				this             r
				/ \             / \
			       l   r      ->  this rr
			          / \         / \
			         rl  rr      l  rl

 			*/

			AVLTree<T> *l = this->lt.release();
			AVLTree<T> *r = this->rt.release();

			r->lt = unique_ptr<AVLTree<T> >(
				new AVLNode<T>(l, rl, this->x));
			
			return unique_ptr<AVLTree<T> >(r);
			/*
			AVLTree<T> *r  = this->rt.release();
			AVLTree<T> *rl = r->left_release();
			AVLTree<T> *rr = r->right_release();

			this->rt = unique_ptr<AVLTree<T> >(rl);

			return unique_ptr<AVLTree<T> >(
				new AVLNode (this, rr, r->value()));
			*/
		}
	}

	this->h = max (this->lt->height(), this->rt->height()) + 1;

	return nullptr;
}
template <class T>
insert_avl_type<T> AVLEmpty<T>::insert (T x) {
	AVLTree<T>::steps++;
	return unique_ptr<AVLTree<T> > (
		new AVLNode<T>(new AVLEmpty(), new AVLEmpty(), x)
	);
}

#endif
