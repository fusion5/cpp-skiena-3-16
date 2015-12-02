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
using insert_ret_type = tuple <Balanced23Tree<T>*, Balanced23Tree<T>*, T>;

template <class T>
using release_type = tuple <Balanced23Tree<T>*, Balanced23Tree<T>* >;
//                          what I become       what I release

template <class T>
class Balanced23Tree {
	public:
		virtual Balanced23Tree<T> *find (T x) = 0;
		/*
		 * When I get a new value through insert, I obtain 1 or 2 
		 * nodes and an updated maximum. If I get back just 1 node, I 
		 * only update the maximum, but if I receive 2, it means that 
		 * the child where I inserted a value has split itself, and I 
		 * must handle the split and accomodate the node. Potentially, 
		 * this means that I might myself also.
		 **/
		virtual insert_ret_type<T> insert       (T x) = 0;
		virtual T                  max          ()    = 0;
		virtual bool               empty        ()    = 0;
		virtual bool               leaf         ()    = 0;
		virtual string             pp           ()    = 0;
		virtual int                size         ()    = 0;
		virtual T                  value        ()    = 0;
		virtual bool               can_release  ()    = 0;
		virtual release_type<T>    release_min  ()    = 0;
		virtual release_type<T>    release_max  ()    = 0;
		virtual bool               remove       (T x) = 0;
		virtual Balanced23Tree<T>  *fill_gap    (Balanced23Tree<T> *t) 
		                                              = 0;
		virtual Balanced23Tree<T>  *release_lt  ()    = 0;
		virtual Balanced23Tree<T>  *release_mt  ()    = 0;
		virtual Balanced23Tree<T>  *release_rt  ()    = 0;
		static int steps;
};

template <class T>
int Balanced23Tree<T>::steps = 0;

template <class T>
class BalancedNode2: public Balanced23Tree<T> {
	public:
		BalancedNode2<T>   ( Balanced23Tree<T> *lt
		                   , Balanced23Tree<T> *mt );
		Balanced23Tree<T>  *find (T x);
		insert_ret_type<T> insert (T x);
		T                  max ();
		bool               empty ();
		bool               leaf ();
		string             pp ();
		int                size();
		T                  value       ();
		bool               can_release ();
		release_type<T>    release_min ();
		release_type<T>    release_max ();
		bool               remove      (T x);
		Balanced23Tree<T>  *fill_gap   (Balanced23Tree<T> *t);
		Balanced23Tree<T>  *release_lt ();
		Balanced23Tree<T>  *release_mt ();
		Balanced23Tree<T>  *release_rt ();
	private:
		T leftMax;
		T middleMax;
		Balanced23Tree<T>  *lt; // Left sub-tree
		Balanced23Tree<T>  *mt; // Middle sub-tree
};

template <class T>
class BalancedNode3: public Balanced23Tree<T> {
	public:
		BalancedNode3<T>   ( Balanced23Tree<T> *lt
		                   , Balanced23Tree<T> *mt
		                   , Balanced23Tree<T> *rt );
		Balanced23Tree<T>  *find (T x);
		insert_ret_type<T> insert (T x);
		T                  max ();
		bool               empty ();
		bool               leaf ();
		string             pp ();
	        int                size();
		T                  value       ();
		bool               can_release ();
		release_type<T>    release_min ();
		release_type<T>    release_max ();
		bool               remove      (T x);
		Balanced23Tree<T>  *fill_gap   (Balanced23Tree<T> *t);
		Balanced23Tree<T>  *release_lt ();
		Balanced23Tree<T>  *release_mt ();
		Balanced23Tree<T>  *release_rt ();
	private:
		T leftMax;
		T middleMax;
		Balanced23Tree<T>  *lt; // Left sub-tree
		Balanced23Tree<T>  *mt; // Middle sub-tree
		Balanced23Tree<T>  *rt; // Right sub-tree
};

template <class T>
class BalancedLeaf: public Balanced23Tree<T> {
	public:
		BalancedLeaf<T>    (T x);
		Balanced23Tree<T>  *find (T x);
		insert_ret_type<T> insert (T x);
		T                  max ();
		bool               empty ();
		bool               leaf ();
		string             pp ();
		int                size ();
		T                  value       ();
		bool               can_release ();
		release_type<T>    release_min ();
		release_type<T>    release_max ();
		bool               remove      (T x);
		Balanced23Tree<T>  *fill_gap   (Balanced23Tree<T> *t);
		Balanced23Tree<T>  *release_lt ();
		Balanced23Tree<T>  *release_mt ();
		Balanced23Tree<T>  *release_rt ();

	private:
		T x;
};

template <class T>
class BalancedEmpty: public Balanced23Tree<T> {
	public:
		BalancedEmpty<T>   ();
		Balanced23Tree<T>  *find (T x);
		insert_ret_type<T> insert (T x);
		T                  max ();
		bool               empty ();
		bool               leaf ();
		string             pp ();
		int                size ();
		T                  value       ();
		bool               can_release ();
		release_type<T>    release_min ();
		release_type<T>    release_max ();
		bool               remove      (T x);
		Balanced23Tree<T>  *fill_gap   (Balanced23Tree<T> *t);
		Balanced23Tree<T>  *release_lt ();
		Balanced23Tree<T>  *release_mt ();
		Balanced23Tree<T>  *release_rt ();
};

template <class T>
void borrow_from_small_to_large (Balanced23Tree<T> **small_sib, 
  Balanced23Tree<T> **large_sib) {
	assert ((*small_sib)->can_release());
	assert (!((*large_sib)->can_release()));
	Balanced23Tree<T> *borrowed;
	// Balanced23Tree<T> *new_small_sib;
	tie (*small_sib, borrowed) = (*small_sib)->release_max();
	if ((*large_sib)->leaf()) {
		*large_sib = new BalancedNode2<T>(borrowed, *large_sib);
	} else {
		Balanced23Tree<T> *lslt;
		Balanced23Tree<T> *lsrt;
		tie (lslt, lsrt) = (*large_sib)->release_max();
		*large_sib = new BalancedNode3<T>(borrowed, lslt, lsrt);
	}
	assert (!((*small_sib)->can_release()));
	assert (!((*small_sib)->leaf()));
	assert (!((*large_sib)->leaf()));
}
template <class T>
void borrow_from_large_to_small (Balanced23Tree<T> **large_sib,
  Balanced23Tree<T> **small_sib) {
	assert (!((*small_sib)->can_release()));
	assert ((*large_sib)->can_release());
	Balanced23Tree<T> *borrowed;
	// Balanced23Tree<T> *new_large_sib;
	tie (*large_sib, borrowed) = (*large_sib)->release_min();
	if ((*small_sib)->leaf()) {
		*small_sib = new BalancedNode2<T>(*small_sib, borrowed);
	} else {
		Balanced23Tree<T> *sslt;
		Balanced23Tree<T> *ssrt;
		tie (sslt, ssrt) = (*small_sib)->release_max();
		*small_sib = new BalancedNode3<T>(sslt, ssrt, borrowed);
	}
	assert (!((*large_sib)->can_release()));
	assert (!((*small_sib)->leaf()));
	assert (!((*large_sib)->leaf()));
}

template <class T>
Balanced23Tree<T> *reduce_3node (Balanced23Tree<T> *t) {
	Balanced23Tree<T> *new_t;
	Balanced23Tree<T> *released;
	tie (new_t, released) = t->release_min();
	assert (released->empty());
	return new_t;
}

template <class T>
Balanced23Tree<T> *merge_with_smaller (Balanced23Tree<T> *large, 
  Balanced23Tree<T> *small) {
	assert (!(small->can_release()));
	assert (!(large->can_release()));
	assert (!(small->leaf()));
	assert (!(large->leaf()));
	assert (!(small->empty()));
	assert (!(large->empty()));

	Balanced23Tree<T> *llt;
	Balanced23Tree<T> *lrt;
	tie (llt, lrt) = large->release_max();

	Balanced23Tree<T> *slt;
	Balanced23Tree<T> *srt;
	tie (slt, srt) = small->release_max();

	assert (llt->empty() || lrt->empty());

	if (llt->empty()) return new BalancedNode3<T> (slt, srt, lrt);
	else              return new BalancedNode3<T> (slt, srt, llt);
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
	assert (!(small->empty()));
	assert (!(large->empty()));

	Balanced23Tree<T> *slt;
	Balanced23Tree<T> *srt;
	tie (slt, srt) = small->release_max();

	Balanced23Tree<T> *llt;
	Balanced23Tree<T> *lrt;
	tie (llt, lrt) = large->release_max();

	assert (slt->empty() || srt->empty());

	if (slt->empty()) return new BalancedNode3<T> (srt, llt, lrt);
	else              return new BalancedNode3<T> (slt, llt, lrt);
}

// vanilla function that allows us to insert (it handles the tuple 
// return value properly).
template <class T>
Balanced23Tree<T> *balanced_23_tree_insert (Balanced23Tree<T> *t, T x) {
	
	Balanced23Tree<T> *li;
	Balanced23Tree<T> *ri;
	
	tie (li, ri, ignore) = t->insert(x);
	
	if (ri->empty()) return li;
	
	// li and ri are 2-nodes. Make a new 2-node that holds them, which makes 
	// the tree one level higher.
	return new BalancedNode2<T> (li, ri);
}

template <class T>
void balanced_23_tree_remove (Balanced23Tree<T> **t, T x) {
	if ((*t)->empty()) return;

	if ((*t)->leaf()) {
		if ((*t)->value() == x)
			*t = new BalancedEmpty<T>();
		return;
	}

	bool gap = (*t)->remove(x);
	
	if (!gap) return;
	
	if ((*t)->can_release()) {
		*t = reduce_3node (*t);
		return;
	}

	// t is a 2-node with a gap. we make the non-gap node the new root!
	Balanced23Tree<T> *lt;
	Balanced23Tree<T> *mt;
	tie (lt, mt) = (*t)->release_min();
	if (lt->empty()) *t = mt;
	else             *t = lt;
/*
	Balanced23Tree<T> *r = (*t)->remove(x, nullptr, nullptr);
	if (r != (*t)) {
		delete (*t);
		*t = r;
	}
*/
}

/* constructors */
template <class T>
BalancedLeaf<T>::BalancedLeaf (T x) { this->x = x; };
template <class T>
BalancedEmpty<T>::BalancedEmpty () {};
template <class T>
BalancedNode2<T>::BalancedNode2 (Balanced23Tree<T> *lt, Balanced23Tree<T> *mt) {
	this->leftMax   = lt->max();
	this->middleMax = mt->max();
	this->lt = lt;
	this->mt = mt;
}
template <class T>
BalancedNode3<T>::BalancedNode3 ( Balanced23Tree<T> *lt
	                        , Balanced23Tree<T> *mt
	                        , Balanced23Tree<T> *rt) {
	this->leftMax   = lt->max();
	this->middleMax = mt->max();
	this->lt = lt;
	this->mt = mt;
	this->rt = rt;
}

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
template <class T> T BalancedEmpty<T>::value() { assert (false); }
template <class T> T BalancedLeaf<T> ::value() { return this->x; }
template <class T> T BalancedNode2<T>::value() { assert (false); }
template <class T> T BalancedNode3<T>::value() { assert (false); }

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
	Balanced23Tree<T> *t = this->lt;
	this->lt = nullptr;
	return t;
}
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::release_lt () {
	Balanced23Tree<T> *t = this->lt;
	this->lt = nullptr;
	return t;
}

/* Boilerplate code: release_mt... */
template <class T>
Balanced23Tree<T> *BalancedEmpty<T>::release_mt () { assert (false); }
template <class T> 
Balanced23Tree<T> *BalancedLeaf<T>::release_mt ()  { assert (false); }
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::release_mt () {
	Balanced23Tree<T> *t = this->mt;
	this->mt = nullptr;
	return t;

}
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::release_mt () {
	Balanced23Tree<T> *t = this->mt;
	this->mt = nullptr;
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
	Balanced23Tree<T> *t = this->rt;
	this->rt = nullptr;
	return t;
}

/* release_min 
 *   returns: see release_type
 */
template <class T>
release_type<T> BalancedEmpty<T>::release_min () { assert(false); }
template <class T>
release_type<T> BalancedLeaf<T>::release_min ()  { assert(false); }
template <class T>
release_type<T> BalancedNode2<T>::release_min () { 
	Balanced23Tree<T> *lt = this->lt;
	Balanced23Tree<T> *mt = this->mt;
	this->lt = nullptr;
	this->mt = nullptr;
	return make_tuple (mt, lt);
}
template <class T>
release_type<T> BalancedNode3<T>::release_min () {
	Balanced23Tree<T> *lt = this->lt;
	Balanced23Tree<T> *mt = this->mt;
	Balanced23Tree<T> *rt = this->rt;

	this->lt = nullptr;
	this->mt = nullptr;
	this->rt = nullptr;

	return make_tuple (new BalancedNode2<T>(mt, rt), lt);
}

/*
 * fill_gap
 * */
template <class T>
Balanced23Tree<T> *BalancedEmpty<T>::fill_gap (Balanced23Tree<T> *t) {
	return t;
}
template <class T>
Balanced23Tree<T> *BalancedLeaf<T>::fill_gap (Balanced23Tree<T> *t) {
	assert(false);
}
template <class T>
Balanced23Tree<T> *BalancedNode2<T>::fill_gap (Balanced23Tree<T> *t) {
	assert (this->lt->empty() || this->mt->empty());
	assert (!(this->lt->empty() && this->mt->empty()));

	Balanced23Tree<T> *t2;

	if (this->lt->empty()) t2 = this->mt;
	else                   t2 = this->lt;

	if (t->max() < t2->max()) {
		this->lt = t; 
		this->mt = t2;
	} else {
		this->lt = t2;
		this->mt = t;
	}
	this->leftMax   = this->lt->max();
	this->middleMax = this->mt->max();
	return this;
}
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::fill_gap (Balanced23Tree<T> *t) {
	assert (false); return nullptr;
}

/* release_max 
 *   returns: see release_type
 * */
template <class T>
release_type<T> BalancedEmpty<T>::release_max() { assert(false); }
template <class T>
release_type<T> BalancedLeaf<T>::release_max()  { assert(false); }
template <class T>
release_type<T> BalancedNode2<T>::release_max() { 
	return make_tuple (this->lt, this->mt);
}
template <class T>
release_type<T> BalancedNode3<T>::release_max() {
	return make_tuple(new BalancedNode2<T>(this->lt, this->mt), this->rt);
}

/* can_release */
template <class T>
bool BalancedEmpty<T>::can_release () { return false; }
template <class T>
bool BalancedLeaf<T>::can_release() { return false; }
template <class T>
bool BalancedNode2<T>::can_release() { return false; }
template <class T>
bool BalancedNode3<T>::can_release() { return true; }

/* find */
template <class T>
Balanced23Tree<T> *BalancedEmpty<T>::find (T x) { 
	Balanced23Tree<T>::steps++;
	return this; 
}
template <class T>
Balanced23Tree<T> *BalancedLeaf<T>::find (T x) {
	Balanced23Tree<T>::steps++;
	if (this->x == x) return this;
	return new BalancedEmpty<T>();
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
T BalancedNode2<T>::max() { 
	Balanced23Tree<T>::steps++;
	// return this->mt->max(); 
	return this->middleMax;
}
template <class T>
T BalancedNode3<T>::max() { 
	Balanced23Tree<T>::steps++;
	return this->rt->max(); }
template <class T>
T BalancedLeaf<T>::max()  { 
	Balanced23Tree<T>::steps++;
	return this->x; 
}
template <class T>
T BalancedEmpty<T>::max() { assert(false); throw "Cannot call max here"; }

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

	assert (!(this->lt->empty()));
	assert (!(this->mt->empty()));

	if (this->lt->leaf() && this->mt->leaf()) {
		if (this->lt->value() == x) {
			delete this->lt;
			this->lt = new BalancedEmpty<T>();
			return true;
		}
		if (this->mt->value() == x) {
			delete this->mt;
			this->mt = this->release_lt();
			this->lt = new BalancedEmpty<T>();
			return true;
		}
		return false;
	}
	assert (!(this->lt->leaf()));
	assert (!(this->mt->leaf()));
	if (x <= this->leftMax) {
		if (!this->lt->remove(x)) return false;
		if (this->lt->can_release()) {
			tie (this->lt, ignore) = this->release_lt()->release_min();
			return false;
		}
		if (this->mt->can_release()) {
			// borrow_from_large_to_small (&(this->mt), gap);
			Balanced23Tree<T> *borrow;
			tie (this->mt, borrow) = this->release_mt()->release_min();

			this->lt->fill_gap (borrow);
			this->leftMax   = this->lt->max();
			this->middleMax = this->mt->max();
			return false;
		}
		// mt is a 2-node... and lt is a 2-node with a gap.
		// we transform these two into a 3-node and a gap
		this->mt        = merge_with_larger ( this->release_lt(), 
		                                      this->release_mt());
		this->lt        = new BalancedEmpty<T>();
		this->middleMax = this->mt->max();
		return true;
	} else if (x <= this->middleMax) {
		if (!this->mt->remove(x)) return false;
		if (this->mt->can_release()) {
			tie (this->mt, ignore) = this->release_mt()->release_min();
			return false;
		}
		if (this->lt->can_release()) {
			Balanced23Tree<T> *borrow;
			tie (this->lt, borrow) = this->release_lt()->release_max();
			this->mt->fill_gap (borrow);
			this->leftMax   = this->lt->max();
			this->middleMax = this->mt->max();
			return false;
		}
		// mt is a 2-node... and lt is a 2-node with a gap.
		// we transform these two into a 3-node and a gap
		this->mt        = merge_with_smaller ( this->release_mt(), 
		                                       this->release_lt());
		this->lt        = new BalancedEmpty<T>();
		this->middleMax = this->mt->max();
		return true;
	}
	return false;
}
template <class T>
bool BalancedNode3<T>::remove (T x) {

	if (this->lt->leaf() && this->mt->leaf() && this->rt->leaf()) {
		if (x == this->lt->value()) {
			delete this->lt;
			this->lt = new BalancedEmpty<T>();
			return true;
		}
		else if (x == this->mt->value()) {
			delete this->mt;
			this->mt = this->release_lt();
			this->lt = new BalancedEmpty<T>();
			return true;
		}
		else if (x == this->rt->value()) {
			delete this->rt;
			this->rt = this->release_mt();
			this->mt = this->release_lt();
			this->lt = new BalancedEmpty<T>();
			return true;
		}
		return false;
	}

	assert (!this->lt->leaf());
	assert (!this->mt->leaf());
	assert (!this->rt->leaf());

	if (x <= this->leftMax) {
		if (!this->lt->remove(x)) return false;

		if (this->lt->can_release()) {
			tie (this->lt, ignore) = this->release_lt()->release_min();
			return false;
		}
		/*
		if (this->rt->can_release() && !(this->mt->can_release()))
			borrow_from_large_to_small (&(this->rt), &(this->mt));
		*/
		if (this->mt->can_release()) {
			Balanced23Tree<T> *borrow;
			tie (this->mt, borrow) = this->release_mt()->release_min();
			this->lt->fill_gap (borrow);
			this->leftMax = this->lt->max();
			this->middleMax = this->mt->max();
			return false;
		}
		// mt and rt are 2-nodes... and lt is a 2-node with a gap.
		// Pass lt on to mt and leave a gap to the left...
		this->mt = merge_with_larger (this->release_lt(), 
		                              this->release_mt());
		this->lt = new BalancedEmpty<T>();
		this->middleMax = this->mt->max();
		return true;
	} else if (x <= this->middleMax) {
		if (!this->mt->remove(x)) return false;
		
		if (this->mt->can_release()) {
			tie (this->mt, ignore) = this->release_mt()->release_min();
			return false;
		}
		if (this->rt->can_release()) {
			Balanced23Tree<T> *borrow;
			tie (this->rt, borrow) = this->release_rt()->release_min();
			this->mt->fill_gap (borrow);
			this->middleMax = this->mt->max();
			return false;
		} 
		if (this->lt->can_release()) {
			Balanced23Tree<T> *borrow;
			tie (this->lt, borrow) = this->release_lt()->release_max();
			this->mt->fill_gap (borrow);
			this->middleMax = this->mt->max();
			this->leftMax   = this->lt->max();
			return false;
		}
		this->mt = merge_with_smaller (this->release_mt(), 
		                               this->release_lt());
		this->lt = new BalancedEmpty<T>();
		this->middleMax = this->mt->max();
		return true;
	} else {
		if (!this->rt->remove(x)) return false;

		if (this->rt->can_release()) {
			tie (this->rt, ignore) = this->release_rt()->release_min();
			return false;
		}
		/*
		if (this->lt->can_release() && !(this->mt->can_release()))
			borrow_from_small_to_large (&(this->lt), &(this->mt));
		*/
		if (this->mt->can_release()) {
			Balanced23Tree<T> *borrow;
			tie (this->mt, borrow) = this->release_mt()->release_max();
			this->rt->fill_gap (borrow);
			this->leftMax   = this->lt->max();
			this->middleMax = this->mt->max();
			return false;
		}
		this->rt = merge_with_smaller (this->release_rt(), 
		                               this->release_mt());
		this->mt = this->lt;
		this->lt = new BalancedEmpty<T>;
		return true;
	}
	return false;
}

/* insert */
template <class T>
insert_ret_type<T> BalancedEmpty<T>::insert (T x) {
	Balanced23Tree<T>::steps++;
	return make_tuple (new BalancedLeaf<T> (x), new BalancedEmpty<T>(), x);
}
template <class T>
insert_ret_type<T> BalancedLeaf<T>::insert (T x) {
	Balanced23Tree<T>::steps++;

	// Inserting an already existing value does nothing
	if (this->x == x) return make_tuple (this, new BalancedEmpty<T>, x); 

	BalancedLeaf<T> *new_leaf = new BalancedLeaf<T> (x);
	if (this->x < x)
		return make_tuple ( new BalancedNode2<T> (this, new_leaf)
				  , new BalancedEmpty<T> ()
				  , x );
	else // this->x > x
		return make_tuple ( new BalancedNode2<T> (new_leaf, this)
				  , new BalancedEmpty<T> ()
				  , this->x );
}
template <class T>
insert_ret_type<T> BalancedNode2<T>::insert (T x) {

	Balanced23Tree<T>::steps++;
	if (this->lt->leaf() && this->mt->leaf()) {
		// I become a 3-node, and add a new leaf where appropriate.
		Balanced23Tree<T> *leaf = new BalancedLeaf <T>(x);
		Balanced23Tree<T> *m    = this;
		T max = this->middleMax;

		if (x < this->leftMax)
			m = new BalancedNode3<T> (leaf, this->lt, this->mt);
		else if ((this->leftMax < x) && (x < this->middleMax))
			m = new BalancedNode3<T> (this->lt, leaf, this->mt);
		else if (this->middleMax < x) {
			m = new BalancedNode3<T> (this->lt, this->mt, leaf);
			max = x;
		} else {
			// Note: x == leftMax or x == middleMax
		}

		return make_tuple (m, new BalancedEmpty<T>(), max);
	}

	Balanced23Tree<T> *li; // Left insert result
	Balanced23Tree<T> *ri; // Right insert result (often a BalancedEmpty 
	// value)
	T                 max; // The maximum of both li and ri

	if (x <= this->leftMax) {
		tie (li, ri, max) = this->lt->insert (x);
		if (ri->empty()) {
			// I remain a 2-node
			this->lt = li;
			return make_tuple (
					this, new BalancedEmpty<T>, this->middleMax);
		} else {
			// I become a 3 node!
			return make_tuple (
					new BalancedNode3<T> (li, ri, this->mt)
					, new BalancedEmpty<T>
					, this->middleMax);
		}
	} else {
		tie (li, ri, max) = this->mt->insert (x);
		if (ri->empty()) {
			// I remain a 2-node
			this->mt = li;
			this->middleMax = max;
			return make_tuple (
					this
					, new BalancedEmpty<T>()
					, this->middleMax);
		} else {
			// I become a 3-node!
			return make_tuple (
					new BalancedNode3<T> (this->lt, li, ri)
					, new BalancedEmpty<T> ()
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
		Balanced23Tree<T> *m = this;
		Balanced23Tree<T> *n;

		T max = this->rt->max();
		Balanced23Tree<T> *leaf = new BalancedLeaf<T>(x);

		if (x < this->lt->max()) {
			m = new BalancedNode2<T> (leaf, this->lt);
			n = new BalancedNode2<T> (this->mt, this->rt);
		} else if ((this->lt->max() < x) && (x < this->mt->max())) {
			m = new BalancedNode2<T> (this->lt, leaf);
			n = new BalancedNode2<T> (this->mt, this->rt);
		} else if ((this->mt->max() < x) && (x < this->rt->max())) {
			m = new BalancedNode2<T> (this->lt, this->mt);
			n = new BalancedNode2<T> (leaf, this->rt);
		} else if (this->rt->max() < x) {
			m = new BalancedNode2<T> (this->lt, this->mt);
			n = new BalancedNode2<T> (this->rt, leaf);
			max = x;
		} else {
			// x is equal to one of our leaves; therefore, 
			// we do nothing.
			n = new BalancedEmpty<T>();
		}
		// Now we return m, n and the new maximum...
		return make_tuple (m, n, max);
	}

	Balanced23Tree<T> *li; // Left insert result
	Balanced23Tree<T> *ri; // Right insert result
	T                 max; // The maximum of both li and ri

	if (x <= this->leftMax) {
		tie (li, ri, max) = this->lt->insert(x);
		if (ri->empty()) {
			// I remain a 3-node
			this->lt = li;
			return 	make_tuple (
					this
					, new BalancedEmpty<T> ()
					, this->rt->max());
		} else {
			// I split into 2 2-nodes, because our lt has split in 2
			// 1st case
			return 	make_tuple (
					new BalancedNode2<T> (li, ri)
					, new BalancedNode2<T> (this->mt, this->rt)
					, this->rt->max());
		}
	} else if (x <= this->middleMax) {
		tie (li, ri, max) = this->mt->insert(x);
		if (ri->empty()) {
			this->mt = li;
			return  make_tuple (
					this
					, new BalancedEmpty<T>()
					, this->rt->max()); 
		} else {
			// I split into 2 2-nodes, 2nd case
			return  make_tuple (
					new BalancedNode2<T> (this->lt, li)
					, new BalancedNode2<T> (ri, this->rt)
					, this->rt->max());

		}
	} else {
		tie (li, ri, max) = this->rt->insert(x);
		if (ri->empty()) {
			this->rt = li;
			return make_tuple (
					this
					, new BalancedEmpty<T>()
					, max);
		} else {
			// I split into 2 2-nodes again, 3rd case
			return make_tuple(
					new BalancedNode2<T> (this->lt, this->mt), 
					new BalancedNode2<T> (li, ri), 
					max);
		}
	}
}
#endif
