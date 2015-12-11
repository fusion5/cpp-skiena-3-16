#ifndef SKIENA_3_16_23_TREE
#define SKIENA_3_16_23_TREE

/**
 * 2-3 trees are trees in which all leaves are at the same (bottom) level;
 * Internal nodes do not hold data, only leaves do;
 * Internal nodes have either 2 or 3 children;
 * When we insert into a 2 node it becomes a 3 node
 * When we insert into a 3 node it splits into 2 2-nodes, which we 
 * re-insert into the parent
 *
 * I think this needs memory management to deallocate nodes when it
 * splits a node into 2...
 *
 */

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace std;

template <class T> class Balanced23Tree;

template <class T>
using insert_ret_type = tuple <Balanced23Tree<T>*, Balanced23Tree<T>*,      T*>;
//                             what I become       extra node to be fitted  max

template <class T>
using release_type = tuple <Balanced23Tree<T>*, Balanced23Tree<T>* >;
//                          what I become       what I release

template <class T>
class Balanced23Tree {
	public:
		virtual ~Balanced23Tree<T>              ()    = 0;
		virtual Balanced23Tree<T> *find         (T x) = 0;
		/*
		 * When I get a new value through insert, I obtain 1 or 2 
		 * nodes and an updated maximum. If I get back just 1 node, I 
		 * only update the maximum, but if I receive 2, it means that 
		 * the child where I inserted a value has split itself, and I 
		 * must handle the split and accomodate the new node. Potentially, 
		 * this means that I might also split myself.
		 **/
		virtual insert_ret_type<T> insert       (T x) = 0;
		virtual T                  *max         ()    = 0;
		virtual bool               empty        ()    = 0;
		virtual bool               leaf         ()    = 0;
		virtual string             pp           ()    = 0;
		virtual int                size         ()    = 0;
		virtual T                  *value       ()    = 0;
		virtual bool               can_release  ()    = 0;
		virtual release_type<T>    split_left   ()    = 0;
		virtual release_type<T>    split_right  ()    = 0;
		virtual Balanced23Tree<T>  *merge_left  (Balanced23Tree<T> *t)
		                                              = 0;
		virtual Balanced23Tree<T>  *merge_right (Balanced23Tree<T> *t)
		                                              = 0;
		virtual bool               remove       (T x) = 0;
		virtual Balanced23Tree<T>  *fill_gap    (Balanced23Tree<T> *t,
		                                         bool rhs) 
		                                              = 0;
		virtual Balanced23Tree<T>  *release_lt  ()    = 0;
		virtual Balanced23Tree<T>  *release_mt  ()    = 0;
		virtual Balanced23Tree<T>  *release_rt  ()    = 0;
		virtual release_type<T>    release_max  ()    = 0;
		static int steps;
};

template <class T>
int Balanced23Tree<T>::steps = 0;

template <class T>
class BalancedNode2: public Balanced23Tree<T> {
	public:
		BalancedNode2<T>   ( Balanced23Tree<T> *lt
		                   , Balanced23Tree<T> *mt );
		~BalancedNode2<T>  ();
		Balanced23Tree<T>  *find       (T x);
		insert_ret_type<T> insert      (T x);
		T                  *max        ();
		bool               empty       ();
		bool               leaf        ();
		string             pp          ();
		int                size        ();
		T                  *value      ();
		bool               can_release ();
		release_type<T>    split_left  ();
		release_type<T>    split_right ();
		Balanced23Tree<T>  *merge_left (Balanced23Tree<T> *t);
		Balanced23Tree<T>  *merge_right(Balanced23Tree<T> *t);
		bool               remove      (T x);
		Balanced23Tree<T>  *fill_gap   (Balanced23Tree<T> *t, bool rhs);
		Balanced23Tree<T>  *release_lt ();
		Balanced23Tree<T>  *release_mt ();
		Balanced23Tree<T>  *release_rt ();
		release_type<T>    release_max ();
	private:
		T leftMax;
		T middleMax;
		unique_ptr<Balanced23Tree<T> > lt; // Left sub-tree
		unique_ptr<Balanced23Tree<T> > mt; // Middle sub-tree
};

template <class T>
class BalancedNode3: public Balanced23Tree<T> {
	public:
		BalancedNode3<T>   ( Balanced23Tree<T> *lt
		                   , Balanced23Tree<T> *mt
		                   , Balanced23Tree<T> *rt );
		~BalancedNode3<T>  ();
		Balanced23Tree<T>  *find       (T x);
		insert_ret_type<T> insert      (T x);
		T                  *max        ();
		bool               empty       ();
		bool               leaf        ();
		string             pp          ();
	        int                size        ();
		T                  *value      ();
		bool               can_release ();
		release_type<T>    split_left  ();
		release_type<T>    split_right ();
		Balanced23Tree<T>  *merge_left (Balanced23Tree<T> *t);
		Balanced23Tree<T>  *merge_right(Balanced23Tree<T> *t);
		bool               remove      (T x);
		Balanced23Tree<T>  *fill_gap   (Balanced23Tree<T> *t, bool rhs);
		Balanced23Tree<T>  *release_lt ();
		Balanced23Tree<T>  *release_mt ();
		Balanced23Tree<T>  *release_rt ();
		release_type<T>    release_max ();
	private:
		T leftMax;
		T middleMax;
		unique_ptr<Balanced23Tree<T> > lt; // Left sub-tree
		unique_ptr<Balanced23Tree<T> > mt; // Middle sub-tree
		unique_ptr<Balanced23Tree<T> > rt; // Right sub-tree
};

template <class T>
class BalancedLeaf: public Balanced23Tree<T> {
	public:
		BalancedLeaf<T>    (T x);
		~BalancedLeaf<T>   ();
		Balanced23Tree<T>  *find       (T x);
		insert_ret_type<T> insert      (T x);
		T                  *max        ();
		bool               empty       ();
		bool               leaf        ();
		string             pp          ();
		int                size        ();
		T                  *value      ();
		bool               can_release ();
		release_type<T>    split_left  ();
		release_type<T>    split_right ();
		Balanced23Tree<T>  *merge_left  (Balanced23Tree<T> *t);
		Balanced23Tree<T>  *merge_right (Balanced23Tree<T> *t);
		bool               remove      (T x);
		Balanced23Tree<T>  *fill_gap   (Balanced23Tree<T> *t, bool rhs);
		Balanced23Tree<T>  *release_lt ();
		Balanced23Tree<T>  *release_mt ();
		Balanced23Tree<T>  *release_rt ();
		release_type<T>    release_max ();
	private:
		T x;
};

template <class T>
class BalancedEmpty: public Balanced23Tree<T> {
	public:
		BalancedEmpty<T>   ();
		~BalancedEmpty<T>  ();
		Balanced23Tree<T>  *find       (T x);
		insert_ret_type<T> insert      (T x);
		T                  *max        ();
		bool               empty       ();
		bool               leaf        ();
		string             pp          ();
		int                size        ();
		T                  *value      ();
		bool               can_release ();
		release_type<T>    split_left  ();
		release_type<T>    split_right ();
		Balanced23Tree<T>  *merge_left (Balanced23Tree<T> *t);
		Balanced23Tree<T>  *merge_right(Balanced23Tree<T> *t);
		bool               remove      (T x);
		Balanced23Tree<T>  *fill_gap   (Balanced23Tree<T> *t, bool rhs);
		Balanced23Tree<T>  *release_lt ();
		Balanced23Tree<T>  *release_mt ();
		Balanced23Tree<T>  *release_rt ();
		release_type<T>    release_max ();
};

/* ctors */
template <class T>
BalancedLeaf<T>::BalancedLeaf (T x) { this->x = x; };
template <class T>
BalancedEmpty<T>::BalancedEmpty () {};
template <class T>
BalancedNode2<T>::BalancedNode2 (Balanced23Tree<T> *lt, Balanced23Tree<T> *mt) {
	assert (lt);
	assert (mt);
	this->leftMax   = *(lt->max());
	this->middleMax = *(mt->max());
	this->lt = unique_ptr<Balanced23Tree<T> >(lt);
	this->mt = unique_ptr<Balanced23Tree<T> >(mt);
}
template <class T>
BalancedNode3<T>::BalancedNode3 ( Balanced23Tree<T> *lt
	                        , Balanced23Tree<T> *mt
	                        , Balanced23Tree<T> *rt) {
	assert (lt); assert (mt); assert (rt);
	this->leftMax   = *(lt->max());
	this->middleMax = *(mt->max());
	this->lt = unique_ptr<Balanced23Tree<T> >(lt);
	this->mt = unique_ptr<Balanced23Tree<T> >(mt);
	this->rt = unique_ptr<Balanced23Tree<T> >(rt);
}
/* dtors */
template <class T>
Balanced23Tree<T>::~Balanced23Tree () { }
template <class T>
BalancedEmpty<T>::~BalancedEmpty ()   { }
template <class T>
BalancedLeaf<T>::~BalancedLeaf ()     { }
template <class T>
BalancedNode2<T>::~BalancedNode2 ()   { }
template <class T>
BalancedNode3<T>::~BalancedNode3 ()   { }

/* pp */
template <class T> string BalancedEmpty<T>::pp() { return "Empty"; }
template <class T> string BalancedLeaf <T>::pp() { 
	return "Leaf " + boost::lexical_cast<string>(this->x); }
template <class T> string BalancedNode2<T>::pp() {
	return "(Node2 " + this->lt->pp() + ", " 
	                 + this->mt->pp() + ")";  }
template <class T> string BalancedNode3<T>::pp() {
	return "(Node3 " + this->lt->pp() + ", " 
	                 + this->mt->pp() + ", " 
	                 + this->rt->pp() + ")";  }

/* size */
template <class T> int BalancedEmpty<T>::size() { return 0; }
template <class T> int BalancedLeaf <T>::size() { return 1; }
template <class T> int BalancedNode2<T>::size() {
	return this->lt->size() + this->mt->size(); }
template <class T> int BalancedNode3<T>::size() {
	return this->lt->size() + this->mt->size() + this->rt->size(); }

/* value */
template <class T> T *BalancedEmpty<T>::value() { assert (false); }
template <class T> T *BalancedLeaf<T> ::value() { return &(this->x); }
template <class T> T *BalancedNode2<T>::value() { assert (false); }
template <class T> T *BalancedNode3<T>::value() { assert (false); }

/* empty */
template <class T> bool BalancedEmpty<T>::empty() { return true;  }
template <class T> bool BalancedLeaf <T>::empty() { return false; }
template <class T> bool BalancedNode2<T>::empty() { return false; }
template <class T> bool BalancedNode3<T>::empty() { return false; }

/* leaf */
template <class T> bool BalancedEmpty<T>::leaf() { return false;  }
template <class T> bool BalancedLeaf <T>::leaf() { return true; }
template <class T> bool BalancedNode2<T>::leaf() { return false; }
template <class T> bool BalancedNode3<T>::leaf() { return false; }


template <class T> 
release_type<T> *release_2node (Balanced23Tree<T> *t) {
	assert (!(t->can_release()));
	assert (!(t->is_leaf()));
}

/* Boilerplate code: release_lt... */
template <class T>
Balanced23Tree<T> *BalancedEmpty<T>::release_lt () { assert (false); }
template <class T>
Balanced23Tree<T> *BalancedLeaf<T>::release_lt  () { assert (false); }
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::release_lt () {
	// assert(this->lt);
	if (!this->lt) return nullptr;
	Balanced23Tree<T> *t = this->lt.release();
	// this->lt.reset(new BalancedEmpty<T>());
	return t;
}
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::release_lt () {
	// assert(this->lt);
	if (!this->lt) return nullptr;
	Balanced23Tree<T> *t = this->lt.release();
	// this->lt.reset(new BalancedEmpty<T>());
	return t;
}

/* Boilerplate code: release_mt... */
template <class T>
Balanced23Tree<T> *BalancedEmpty<T>::release_mt () { assert (false); }
template <class T> 
Balanced23Tree<T> *BalancedLeaf<T>::release_mt ()  { assert (false); }
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::release_mt () {
	if (!this->mt) return nullptr;
	assert(this->mt);
	Balanced23Tree<T> *t = this->mt.release();
//	this->mt.reset(new BalancedEmpty<T>());
	return t;
}
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::release_mt () {
	if (!this->mt) return nullptr;
	assert(this->mt);
	Balanced23Tree<T> *t = this->mt.release();
//	this->mt.reset(new BalancedEmpty<T>());
	return t;
}

/* Boilerplate code: release_rt... */
template <class T>
Balanced23Tree<T> *BalancedEmpty<T>::release_rt () { assert (false); }
template <class T> 
Balanced23Tree<T> *BalancedLeaf<T>::release_rt ()  { assert (false); }
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::release_rt () { assert (false); }
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::release_rt () {
	if (!this->rt) return nullptr;
	assert (this->rt);
	Balanced23Tree<T> *t = this->rt.release();
//	this->rt.reset(new BalancedEmpty<T>());
	return t;
}

/* release_max
 *   returns: see release_type
 */
template <class T>
release_type<T> BalancedEmpty<T>::release_max () { assert(false); }
template <class T>
release_type<T> BalancedLeaf<T> ::release_max () { assert(false); }
template <class T>
release_type<T> BalancedNode2<T>::release_max () {
	assert (this->lt);
	assert (this->mt);
	if (this->lt->leaf() && this->mt->leaf()) {
		Balanced23Tree<T> *max = this->release_mt();
		this->mt.reset(this->release_lt());
		return make_tuple (this, max); 
	}

	Balanced23Tree<T> *new_mt;
	Balanced23Tree<T> *max;
	tie (new_mt, max) = this->mt->release_max();
	if (!new_mt) {
		// We have found the maximum and there is nothing to 
		// do
		if (*(max->value()) == this->middleMax)
			this->middleMax = *(this->mt->max());
		return make_tuple (nullptr, max);
	}
	if (this->mt->can_release()) {
		this->mt.reset(reduce_3node(this->release_mt()));
		return make_tuple (nullptr, max);
	}
	if (this->lt->can_release()) {
		Balanced23Tree<T> *borrow;
		Balanced23Tree<T> *new_lt;
		tie (new_lt, borrow) = this->lt->split_left();
		this->lt.reset     (new_lt);
		this->mt->fill_gap (borrow, false);
		this->leftMax   = *(this->lt->max());
		this->middleMax = *(this->mt->max());
		return make_tuple  (nullptr, max);
	}
	// mt is a 2-node... we make it a 3-node and we leave a gap on the left.
	// lt is also a 2-node...
	assert (this->lt);
	this->mt.reset(this->mt->merge_left(this->release_lt()));
	// assert (this->lt);
	return make_tuple (this, max); // 'this' as the left means we have a gap...
}
template <class T>
release_type<T> BalancedNode3<T>::release_max () {
	assert (this->lt);
	assert (this->mt);
	assert (this->rt);
	if (this->lt->leaf() && this->mt->leaf() && this->rt->leaf()) {
		Balanced23Tree<T> *max;
		max = this->release_rt();
		this->rt.reset (this->release_mt());
		this->mt.reset (this->release_lt());
		return make_tuple (this, max);
	}

	Balanced23Tree<T> *new_rt;
	Balanced23Tree<T> *max;
	tie (new_rt, max) = this->rt->release_max();
	if (!new_rt)
		return make_tuple (nullptr, max);
	
	if (this->rt->can_release()) {
		this->rt.reset(reduce_3node(this->release_rt()));
		return make_tuple (nullptr, max);
	}
	if (this->mt->can_release()) {
		Balanced23Tree<T> *borrow;
		Balanced23Tree<T> *new_mt;
		tie (new_mt, borrow) = this->mt->split_right();
		this->rt->fill_gap (borrow, false);
		this->mt.reset(new_mt);
		this->middleMax = *(this->mt->max());
		return make_tuple (nullptr, max);
	}
	// I have a gap...
	this->rt.reset(this->rt->merge_left(this->release_mt()));
	this->mt.reset(this->release_lt());
	return make_tuple (this, max);
}

/* split_right 
 *   returns: see release_type
 * */
template <class T>
release_type<T> BalancedEmpty<T>::split_right () { assert(false); }
template <class T>
release_type<T> BalancedLeaf<T>::split_right  () { assert(false); }
template <class T>
release_type<T> BalancedNode2<T>::split_right () { 
	Balanced23Tree<T> *lt = this->release_lt();
	Balanced23Tree<T> *mt = this->release_mt();
	return make_tuple (lt, mt);
}
template <class T>
release_type<T> BalancedNode3<T>::split_right () {
	Balanced23Tree<T> *lt = this->release_lt();
	Balanced23Tree<T> *mt = this->release_mt();
	Balanced23Tree<T> *rt = this->release_rt();
	return make_tuple (new BalancedNode2<T>(lt, mt), rt);
}
/* split_left
 *   returns: see release_type
 */
template <class T>
release_type<T> BalancedEmpty<T>::split_left () { assert(false); }
template <class T>
release_type<T> BalancedLeaf<T>::split_left ()  { assert(false); }
template <class T>
release_type<T> BalancedNode2<T>::split_left () { 
	Balanced23Tree<T> *lt = this->release_lt();
	Balanced23Tree<T> *mt = this->release_mt();
	return make_tuple (mt, lt);
}
template <class T>
release_type<T> BalancedNode3<T>::split_left () {
	Balanced23Tree<T> *lt = this->release_lt();
	Balanced23Tree<T> *mt = this->release_mt();
	Balanced23Tree<T> *rt = this->release_rt();
	return make_tuple (new BalancedNode2<T>(mt, rt), lt);
}

/*
 * fill_gap
 * */
template <class T>
Balanced23Tree<T> *BalancedEmpty<T>::fill_gap (Balanced23Tree<T> *t, bool rhs) {
	return t;
}
template <class T>
Balanced23Tree<T> *BalancedLeaf<T>::fill_gap (Balanced23Tree<T> *t, bool rhs) {
	assert(false); return t;
}
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::fill_gap (Balanced23Tree<T> *t, bool rhs) {
	assert (!this->lt);
	assert (this->mt);
	assert (t);
	if (rhs) {
		this->lt.reset(this->release_mt()); 
		this->mt.reset(t);
	} else {
		this->lt.reset(t);
	}
	this->leftMax   = *(this->lt->max());
	this->middleMax = *(this->mt->max());
	return this;
}
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::fill_gap (Balanced23Tree<T> *t, bool right) {
	assert (false); return t;
}

/* can_release */
template <class T>
bool BalancedEmpty<T>::can_release () { return false; }
template <class T>
bool BalancedLeaf<T>::can_release()   { return false; }
template <class T>
bool BalancedNode2<T>::can_release()  { return false; }
template <class T>
bool BalancedNode3<T>::can_release()  { return true; }

/* find */
template <class T>
Balanced23Tree<T> *BalancedEmpty<T>::find (T x) { 
	Balanced23Tree<T>::steps++;
	return nullptr; 
}
template <class T>
Balanced23Tree<T> *BalancedLeaf<T>::find (T x) {
	Balanced23Tree<T>::steps++;
	if (this->x == x) return this;
	return nullptr;
}
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::find (T x) {
	Balanced23Tree<T>::steps++;
	if (x <= this->leftMax) return this->lt->find (x);
	else                    return this->mt->find (x);
}
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::find (T x) {
	Balanced23Tree<T>::steps++;
	if (x <= this->leftMax)   return this->lt->find (x);
	if (x <= this->middleMax) return this->mt->find (x);
	else                      return this->rt->find (x);
}

/* max */
template <class T>
T *BalancedEmpty<T>::max() { assert(false); throw "Cannot call max here"; }
template <class T>
T *BalancedNode2<T>::max() {
	Balanced23Tree<T>::steps++;
	// return this->mt->max(); 
	return &(this->middleMax);
}
template <class T>
T *BalancedNode3<T>::max() { 
	Balanced23Tree<T>::steps++;
	return this->rt->max(); }
template <class T>
T *BalancedLeaf<T>::max()  { 
	Balanced23Tree<T>::steps++;
	return this->value(); 
}

/* remove 
 *   returns - true if a gap was left after removal
 *           - false otherwise
 * */
template <class T>
bool BalancedEmpty<T>::remove (T x) {
	assert (false);
	Balanced23Tree<T>::steps++;
	return false;
}
template <class T>
bool BalancedLeaf<T>::remove (T x) {
	assert (false);
	return false; 
}
template <class T>
bool BalancedNode2<T>::remove (T x) {

	assert (this->lt);
	assert (this->mt);

	if (this->lt->leaf() && this->mt->leaf()) {
		if (*(this->lt->value()) == x) {
			delete this->release_lt();
			return true;
		}
		if (*(this->mt->value()) == x) {
			this->mt.reset(this->release_lt());
			return true;
		}
		return false;
	}
	assert (!(this->lt->leaf()));
	assert (!(this->mt->leaf()));
	if (x <= this->leftMax) {
		if (!this->lt->remove(x)) {
			if (x == this->leftMax) 
			  this->leftMax = *(this->lt->max());
			return false;
		}
		if (this->lt->can_release()) {
			this->lt.reset (reduce_3node(this->release_lt()));
			this->leftMax = *(this->lt->max());
			return false;
		}
		if (this->mt->can_release()) {
			Balanced23Tree<T> *borrow;
			Balanced23Tree<T> *new_mt;
			tie (new_mt, borrow) = this->mt->split_left();
			this->lt->fill_gap (borrow, true);
			assert (new_mt);
			this->mt.reset (new_mt);
			this->leftMax   = *(this->lt->max());
			this->middleMax = *(this->mt->max());
			return false;
		}
		// mt is a 2-node... and lt is a 2-node with a gap.
		// we transform these two into a 3-node and a gap
		this->lt.reset(this->lt->merge_right(this->release_mt()));
		this->mt.reset(this->release_lt());
		return true;
	} else if (x <= this->middleMax) {
		if (!this->mt->remove(x)) {
			if (x == this->middleMax) 
			  this->middleMax = *(this->mt->max());
			return false;
		}
		if (this->mt->can_release()) {
			this->mt.reset(reduce_3node(this->release_mt()));
			return false;
		}
		if (this->lt->can_release()) {
			Balanced23Tree<T> *borrow;
			Balanced23Tree<T> *new_lt;
			tie (new_lt, borrow) = this->lt->split_right();
			this->lt.reset(new_lt);
			this->mt->fill_gap (borrow, false);
			this->leftMax   = *(this->lt->max());
			this->middleMax = *(this->mt->max());
			return false;
		}
		// mt is a 2-node... and lt is a 2-node with a gap.
		// we transform these two into a 3-node and a gap
		this->mt.reset (this->mt->merge_left(this->release_lt()));
		assert (!this->lt);
		return true;
	}
	assert (false); // should have returned by now.
}
template <class T>
bool BalancedNode3<T>::remove (T x) {

	assert (this->lt);
	assert (this->mt);
	assert (this->rt);

	if (this->lt->leaf() && this->mt->leaf() && this->rt->leaf()) {
		if (x == (*this->lt->value())) {
			delete this->release_lt();
			return true;
		}
		else if (x == (*this->mt->value())) {
			this->mt.reset (this->release_lt());
			return true;
		}
		else if (x == (*this->rt->value())) {
			this->rt.reset (this->release_mt());
			this->mt.reset (this->release_lt());
			return true;
		}
		return false;
	}

	assert (!this->lt->leaf());
	assert (!this->mt->leaf());
	assert (!this->rt->leaf());

	if (x <= this->leftMax) {
		if (!this->lt->remove(x)) {
			if (x == this->leftMax) this->leftMax = 
			  *(this->lt->max());
			return false;
		}
		if (this->lt->can_release()) {
			this->lt.reset(reduce_3node(this->release_lt()));
			this->leftMax = *(this->lt->max());
			return false;
		}
		if (this->mt->can_release()) {
			Balanced23Tree<T> *new_mt;
			Balanced23Tree<T> *borrow;
			tie (new_mt, borrow) = this->mt->split_left ();
			this->mt.reset(new_mt);
			this->lt->fill_gap (borrow, true);
			this->leftMax   = *(this->lt->max());
			this->middleMax = *(this->mt->max());
			return false;
		}
		// mt and rt are 2-nodes... and lt is a 2-node with a gap.
		// Pass lt on to mt and leave a gap to the left...
		this->lt.reset(this->lt->merge_right(this->release_mt()));
		this->mt.reset(this->release_lt());
		assert (!this->lt);
		// this->middleMax = this->mt->max();
		return true;
	} else if (x <= this->middleMax) {
		if (!this->mt->remove(x)) {
			if (x == this->middleMax) this->middleMax = 
			  *(this->mt->max());
			return false;
		}
		if (this->mt->can_release()) {
			/*
			Balanced23Tree<T> *borrow;
			Balanced23Tree<T> *new_mt;
			tie (new_mt, borrow) = this->mt->split_left ();
			this->mt.reset(new_mt);
			assert (borrow->empty());
			delete borrow;
			*/
			this->mt.reset(reduce_3node(this->release_mt()));
			this->middleMax = *(this->mt->max());
			return false;
		}
		if (this->rt->can_release()) {
			Balanced23Tree<T> *borrow;
			Balanced23Tree<T> *new_rt;
			tie (new_rt, borrow) = this->rt->split_left();
			this->rt.reset(new_rt);
			this->mt->fill_gap (borrow, true);
			this->middleMax = *(this->mt->max());
			return false;
		} 
		if (this->lt->can_release()) {
			Balanced23Tree<T> *borrow;
			Balanced23Tree<T> *new_lt;
			tie (new_lt, borrow) = this->lt->split_right();
			this->lt.reset(new_lt);
			this->mt->fill_gap (borrow, false);
			this->middleMax = *(this->mt->max());
			this->leftMax   = *(this->lt->max());
			return false;
		}
		this->mt.reset (this->mt->merge_left(this->release_lt()));
		assert (!this->lt);
		return true;
	} else {
		if (!this->rt->remove(x)) return false;

		if (this->rt->can_release()) {
			/*
			Balanced23Tree<T> *borrow;
			Balanced23Tree<T> *new_rt;
			tie (new_rt, borrow) = this->rt->split_left();
			assert (borrow->empty());
			delete borrow;
			this->rt.reset(new_rt);
			*/
			this->rt.reset(reduce_3node(this->release_rt()));
			return false;
		}
		/*
		if (this->lt->can_release() && !(this->mt->can_release()))
			borrow_from_small_to_large (&(this->lt), &(this->mt));
		*/
		if (this->mt->can_release()) {
			Balanced23Tree<T> *borrow;
			Balanced23Tree<T> *new_mt;
			tie (new_mt, borrow) = this->mt->split_right();
			this->rt->fill_gap (borrow, false);
			this->mt.reset(new_mt);
			this->middleMax = *(this->mt->max());
			return false;
		}
		this->rt.reset(this->rt->merge_left(this->release_mt()));
		this->mt.reset(this->release_lt());
		assert (!this->lt);
		return true;
	}
	assert (false); // should have returned by now.
}

/* insert 
 *   leaves the object in a state ready for deletion
 *   returns: see insert_ret_type
 * */
template <class T>
insert_ret_type<T> BalancedEmpty<T>::insert (T x) {
	Balanced23Tree<T>::steps++;
	Balanced23Tree<T> *l = new BalancedLeaf<T> (x);
	return make_tuple (l, new BalancedEmpty<T>(), l->value());
}
template <class T>
insert_ret_type<T> BalancedLeaf<T>::insert (T x) {
	Balanced23Tree<T>::steps++;

	// Inserting an already existing value does nothing
	if (this->x == x) 
		return make_tuple (this, nullptr, this->value()); 

	// I get deallocated so I create a copy of myself in the me pointer
	BalancedLeaf<T> *me       = new BalancedLeaf<T> (this->x);
	BalancedLeaf<T> *new_leaf = new BalancedLeaf<T> (x);
	if (this->x < x)
		return make_tuple ( new BalancedNode2<T> (me, new_leaf)
				  , nullptr
				  , new_leaf->value() );
	else // this->x > x
		return make_tuple ( new BalancedNode2<T> (new_leaf, me)
				  , nullptr
				  , this->value() );
}
template <class T>
insert_ret_type<T> BalancedNode2<T>::insert (T x) {

	assert (this->lt != nullptr); 
	assert (this->mt != nullptr);

	Balanced23Tree<T>::steps++;
	if (this->lt->leaf() && this->mt->leaf()) {
		// I become a 3-node, and add a new leaf where appropriate.
		Balanced23Tree<T> *lt   = this->release_lt();
		Balanced23Tree<T> *mt   = this->release_mt();
		Balanced23Tree<T> *leaf = new BalancedLeaf<T>(x);
		Balanced23Tree<T> *m;
		T *max = &(this->middleMax);

		if (x < this->leftMax)
			m = new BalancedNode3<T> (leaf, lt, mt);
		else if ((this->leftMax < x) && (x < this->middleMax))
			m = new BalancedNode3<T> (lt, leaf, mt);
		else if (this->middleMax < x) {
			m = new BalancedNode3<T> (lt, mt, leaf);
			max = leaf->value();
		} else {
			// Copy of myslef
			// Note: x == leftMax or x == middleMax
			m = new BalancedNode2<T> (lt, mt);
		}
		return make_tuple (m, nullptr, max);
	}

	Balanced23Tree<T> *li;  // Left insert result
	Balanced23Tree<T> *ri;  // Right insert res. (often a BalancedEmpty 
	                        // value)
	T                 *max; // The maximum of both li and ri

	if (x <= this->leftMax) {
		tie (li, ri, ignore) = this->lt->insert (x);
		if (!ri) {
			// delete ri;
			// I remain a 2-node
			this->lt.reset(li);
			return make_tuple (
			  new BalancedNode2<T> (this->release_lt(), 
			    this->release_mt())
			, nullptr
			, &(this->middleMax));
		} else {
			// I become a 3 node!
			if (this->lt) delete this->release_lt();
			return make_tuple (
			  new BalancedNode3<T> (li, ri, this->release_mt())
			, nullptr
			, &(this->middleMax));
		}
	} else {
		// x > leftMax
		tie (li, ri, max) = this->mt->insert (x);
		if (!ri) {
			// delete ri;
			// I remain a 2-node
			// Note: max gets destroyed after the reset so 
			// it is important for this statement to occur before
			// mt.reset()
			if (*max > middleMax) this->middleMax = *max;
			this->mt.reset(li);
			return make_tuple (
			  new BalancedNode2<T> (this->release_lt(), 
			    this->release_mt())
			, nullptr
			, &(this->middleMax));
		} else {
			if (this->mt) delete this->release_mt();
			// I become a 3-node!
			return make_tuple (
			  new BalancedNode3<T> (this->release_lt(), li, ri)
			, nullptr
			, max);
		}
	}
}
template <class T>
insert_ret_type<T> BalancedNode3<T>::insert(T x) {

	Balanced23Tree<T>::steps++;
	if (this->lt->leaf() && this->mt->leaf() && this->rt->leaf()) {
		// We are the parent of a new node!
		// But we cannot have 4 children so we have to split
		// recursively upwards...
		Balanced23Tree<T> *lt   = this->release_lt();
		Balanced23Tree<T> *mt   = this->release_mt();
		Balanced23Tree<T> *rt   = this->release_rt();
		Balanced23Tree<T> *m;
		Balanced23Tree<T> *n;

		T *max = rt->max();
		T lmax = *(lt->max());
		T mmax = *(mt->max());
		T rmax = *(rt->max());
		Balanced23Tree<T> *leaf = new BalancedLeaf<T>(x);

		if (x < lmax) {
			m = new BalancedNode2<T>(leaf, lt);
			n = new BalancedNode2<T>(mt, rt);
		} else if ((lmax < x) && (x < mmax)) {
			m = new BalancedNode2<T>(lt, leaf);
			n = new BalancedNode2<T>(mt, rt);
		} else if ((mmax < x) && (x < rmax)) {
			m = new BalancedNode2<T>(lt, mt);
			n = new BalancedNode2<T>(leaf, rt);
		} else if (rmax < x) {
			m = new BalancedNode2<T>(lt, mt);
			n = new BalancedNode2<T>(rt, leaf);
			max = leaf->value();
		} else {
			// x is equal to one of our leaves; therefore, 
			// we do nothing.
			m = new BalancedNode3<T>(lt, mt, rt);
			n = nullptr;
		}
		// Now we return m, n and the new maximum...
		return make_tuple (m, n, max);
	}

	Balanced23Tree<T> *li;  // Left insert result
	Balanced23Tree<T> *ri;  // Right insert result
	T                 *max; // The maximum of both li and ri

	if (x <= this->leftMax) {
		tie (li, ri, ignore) = this->lt->insert(x);
		if (!ri) {
			// delete ri;
			// I remain a 3-node
			this->lt.reset(li);
			return make_tuple (
			  new BalancedNode3<T> ( this->release_lt()
					       , this->release_mt()
					       , this->release_rt())
			, nullptr
			, this->rt->max());
		} else {
			// I split into 2 2-nodes, because our lt has split in 2
			// 1st case
			if (this->lt) delete this->release_lt();
			return make_tuple (
			  new BalancedNode2<T> (li, ri)
			, new BalancedNode2<T> ( this->release_mt()
					       , this->release_rt())
			, this->rt->max());
		}
	} else if (x <= this->middleMax) {
		tie (li, ri, ignore) = this->mt->insert(x);
		if (!ri) {
			// delete ri;
			assert (this->rt);
			this->mt.reset(li);
			return make_tuple (
			  new BalancedNode3<T> ( this->release_lt()
					       , this->release_mt()
					       , this->release_rt())
			, nullptr
			, this->rt->max()); 
		} else {
			// I split into 2 2-nodes, 2nd case
			if (this->mt) delete this->release_mt();
			return make_tuple (
			  new BalancedNode2<T> (this->release_lt(), li)
			, new BalancedNode2<T> (ri, this->release_rt())
			, this->rt->max());

		}
	} else {
		tie (li, ri, max) = this->rt->insert(x);
		if (!ri) {
			// delete ri;
			this->rt.reset(li);
			return make_tuple (
			  new BalancedNode3<T> ( this->release_lt()
					       , this->release_mt()
					       , this->release_rt())
			, nullptr
			, this->rt->max());
		} else {
			// I split into 2 2-nodes again, 3rd case
			if (this->rt) delete this->release_rt();
			return make_tuple(
			  new BalancedNode2<T> (this->release_lt(), 
			                        this->release_mt())
			, new BalancedNode2<T> (li, ri)
			, ri->max());
		}
	}
}

/* Frees t's memory and returns a pointer to a modified memory zone with 
 * a reduced node. Reduces a 3-node into a 2-node.
 * */
template <class T>
Balanced23Tree<T> *reduce_3node (Balanced23Tree<T> *t) {
	Balanced23Tree<T> *new_t;
	Balanced23Tree<T> *released;
	tie (new_t, released) = t->split_left();
	// assert (released);
	if (released) delete released;
	delete t;
	return new_t;
}

/* merge_left
 * Returns: a pointer to a node that should replace myself.
 * This is the counterpart of splitting. If I am a 2-node with a gap, eliminate
 * the gap and put t's children to my left
 */
template <class T> 
Balanced23Tree<T> *BalancedEmpty<T>::merge_left (Balanced23Tree<T> *t) { 
	assert (false); }
template <class T> 
Balanced23Tree<T> *BalancedLeaf<T>:: merge_left (Balanced23Tree<T> *t) {
	assert (false); }
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::merge_left (Balanced23Tree<T> *t) {
	assert (!(t->can_release()));
	Balanced23Tree<T> *lt = this->release_lt();
	Balanced23Tree<T> *mt = this->release_mt();
	Balanced23Tree<T> *tlt;
	Balanced23Tree<T> *tmt;
	assert (lt || mt);

	tie (tlt, tmt) = t->split_right();
	delete t;
	if (lt) return new BalancedNode3<T>(tlt, tmt, lt);
	else    return new BalancedNode3<T>(tlt, tmt, mt);

}
template <class T> 
Balanced23Tree<T> *BalancedNode3<T>::merge_left (Balanced23Tree<T> *t) {
	assert (false); }

/* merge_right
 * Returns: a pointer to a node that should replace myself.
 * This is the counterpart of splitting. If I am a 2-node with a gap, eliminate
 * the gap and put t's children to my right.
 * */
template <class T> 
Balanced23Tree<T> *BalancedEmpty<T>::merge_right (Balanced23Tree<T> *t) { 
	assert (false); }
template <class T> 
Balanced23Tree<T> *BalancedLeaf<T>:: merge_right (Balanced23Tree<T> *t) {
	assert (false); }
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::merge_right (Balanced23Tree<T> *t) {
	Balanced23Tree<T> *lt = this->release_lt();
	Balanced23Tree<T> *mt = this->release_mt();
	Balanced23Tree<T> *tlt;
	Balanced23Tree<T> *tmt;
	assert (!(t->can_release()));
	assert (lt || mt);
	
	tie (tlt, tmt) = t->split_right();
	delete t;

	if (lt) return new BalancedNode3<T>(lt, tlt, tmt);
	else    return new BalancedNode3<T>(mt, tlt, tmt);
}
template <class T> 
Balanced23Tree<T> *BalancedNode3<T>::merge_right (Balanced23Tree<T> *t) {
	assert (false); }

template <class T>
Balanced23Tree<T> *merge_with_smaller (Balanced23Tree<T> *large, 
  Balanced23Tree<T> *small) {
	assert (!(small->can_release()));
	assert (!(large->can_release()));
	assert (!(small->leaf()));
	assert (!(large->leaf()));
	assert (small);
	assert (large);

	Balanced23Tree<T> *llt;
	Balanced23Tree<T> *lrt;
	tie (llt, lrt) = large->split_right();

	Balanced23Tree<T> *slt;
	Balanced23Tree<T> *srt;
	tie (slt, srt) = small->split_right();

	assert (llt || lrt);

	delete large;
	delete small;

	if (!llt) {
		return new BalancedNode3<T> (slt, srt, lrt);
	} else  {
		return new BalancedNode3<T> (slt, srt, llt);
	}
}

/*
 * The two parameters are 2-nodes.
 * The small tree must have a gap. We put the 2 large nodes in the gap...
 * */
template <class T>
Balanced23Tree<T> *merge_with_larger (Balanced23Tree<T> *small, 
  Balanced23Tree<T> *large) {
	assert (!(small->can_release()));
	assert (!(large->can_release()));
	assert (!(small->leaf()));
	assert (!(large->leaf()));
	assert (small);
	assert (large);

	Balanced23Tree<T> *slt;
	Balanced23Tree<T> *srt;
	tie (slt, srt) = small->split_right();

	Balanced23Tree<T> *llt;
	Balanced23Tree<T> *lrt;
	tie (llt, lrt) = large->split_right();

	assert (!slt || !srt);

	delete large;
	delete small;

	if (!slt) {
		return new BalancedNode3<T> (srt, llt, lrt);
	} else {
		return new BalancedNode3<T> (slt, llt, lrt);
	}
}

// vanilla function that allows us to insert (it handles the tuple 
// return value properly).
template <class T>
void balanced_23_tree_insert (Balanced23Tree<T> **t, T x) {
	
	Balanced23Tree<T> *li;
	Balanced23Tree<T> *ri;
	
	if ((*t)->empty()) {
		delete *t;
		*t = new BalancedLeaf<T>(x);
		return;
	}

	tie (li, ri, ignore) = (*t)->insert(x);
	
	if (!ri) {
		delete ri;
		delete *t;
		*t = li;
		return;
	}
	
	// li and ri are 2-nodes. Make a new 2-node that holds them, which 
	// makes the tree one level higher.
	delete *t;
	*t = new BalancedNode2<T> (li, ri);
}

template <class T>
void balanced_23_tree_remove (Balanced23Tree<T> **t, T x) {
	if ((*t)->empty()) return;

	if ((*t)->leaf()) {
		if (*((*t)->value()) == x) {
			delete *t;
			*t = new BalancedEmpty<T>();
		}
		return;
	}

	if (!(*t)->remove(x)) return;
	
	// t has a gap and is a 3-node
	if ((*t)->can_release()) {
		// t is a 3-node: we transform it into a 2-node.
		// Balanced23Tree<T> *q = reduce_3node (*t);
		// delete *t;
		// *t = q;
		*t = reduce_3node (*t);
		return;
		// 54660
	}
	// t is a 2-node with a gap. we make the non-gap node the new root!
	Balanced23Tree<T> *lt;
	Balanced23Tree<T> *mt;
	tie (lt, mt) = (*t)->split_left();
	delete *t;
	assert (!mt);
	// delete mt;
	*t = lt;
}

template <class T>
T balanced_23_tree_release_max (Balanced23Tree<T> **t) {

	if ((*t)->empty()) {
		throw "called release max on an empty tree";
		assert(false);
	}

	if ((*t)->leaf()) {
		T ret = *((*t)->value());
		delete *t;
		*t = new BalancedEmpty<T>();
		return ret;
	}

	Balanced23Tree<T> *newt;
	Balanced23Tree<T> *max;
	tie (newt, max) = (*t)->release_max();

	if (newt == nullptr) {
		T ret = *(max->value());
		delete max;
		return ret;
	}

	if ((*t)->can_release()) {
		T ret = *(max->value());
		// Balanced23Tree<T> *q = reduce_3node (*t);
		// delete *t;
		// *t = q;
		*t = reduce_3node (*t);
		delete max;
		return ret;
	}

	Balanced23Tree<T> *lt;
	Balanced23Tree<T> *mt;
	tie (lt, mt) = (*t)->split_left();
	delete *t;
	assert (!mt);
	*t = lt;

	T ret = *(max->value());
	delete max;
	return ret;
}

#endif
