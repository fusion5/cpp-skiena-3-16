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
 * I think it needs better memory management to deallocate nodes when it
 * splits a node into 2...
 */

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace std;

template <class T> class Balanced23Tree;

template <class T>
using insert_ret_type = tuple <Balanced23Tree<T>*, Balanced23Tree<T>*, T>;

template <class T>
class Balanced23Tree {
	public:
		virtual Balanced23Tree *find (T x) = 0;
		/*
		 * When I get a new value through insert, I obtain 1 or 2 
		 * nodes and an updated maximum. If I get back just 1 node, I 
		 * only update the maximum, but if I receive 2, it means that 
		 * the child where I inserted a value has split itself, and I 
		 * must handle the split and accomodate the node. Potentially, 
		 * this means that I might myself also.
		 **/
		virtual insert_ret_type<T> insert (T x) = 0;
		virtual T                  max    ()    = 0;
		virtual bool		   empty  ()    = 0;
		virtual bool		   leaf   ()    = 0;
		virtual string 		   pp     ()    = 0;
		virtual int 		   size   ()    = 0;

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
		bool		   empty ();
		bool		   leaf ();
		string		   pp ();
		int		   size();
	private:
		T leftMax;
		T middleMax;
		Balanced23Tree<T> *lt; // Left sub-tree
		Balanced23Tree<T> *mt; // Middle sub-tree
};

template <class T>
class BalancedNode3: public Balanced23Tree<T> {
	public:
		BalancedNode3<T>  ( Balanced23Tree<T> *lt
				  , Balanced23Tree<T> *mt
				  , Balanced23Tree<T> *rt );
		Balanced23Tree<T> *find (T x);
		insert_ret_type<T> insert (T x);
		T                  max ();
		bool		   empty ();
		bool 		   leaf ();
		string		   pp ();
	        int 		   size();
	private:
		T leftMax;
		T middleMax;
		Balanced23Tree<T> *lt; // Left sub-tree
		Balanced23Tree<T> *mt; // Middle sub-tree
		Balanced23Tree<T> *rt; // Right sub-tree
};

template <class T>
class BalancedLeaf: public Balanced23Tree<T> {
	public:
		BalancedLeaf<T>    (T x);
		Balanced23Tree<T>  *find (T x);
		insert_ret_type<T> insert (T x);
		T                  max ();
		bool		   empty ();
		bool		   leaf ();
		string		   pp ();
		int		   size ();
	private:
		T x;
};

template <class T>
class BalancedEmpty: public Balanced23Tree<T> {
	public:
		BalancedEmpty<T>  ();
		Balanced23Tree<T> *find (T x);
		insert_ret_type<T> insert (T x);
		T                  max ();
		bool		   empty ();
		bool		   leaf ();
		string		   pp ();
		int		   size ();
};

// vanilla function that allows us to insert properly (it handles the tuple 
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
	else 			return this->mt->find (x);
}
template <class T>
Balanced23Tree<T> *BalancedNode3<T>::find (T x) {
	Balanced23Tree<T>::steps++;
	if (x <= this->leftMax)   return this->lt->find (x);
	if (x <= this->middleMax) return this->mt->find (x);
	else			  return this->rt->find (x);
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
	return this->x; }
template <class T>
T BalancedEmpty<T>::max() { throw "Cannot call max here"; }

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
		}

		return make_tuple (m, new BalancedEmpty<T>(), max);
	}

	Balanced23Tree<T> *li; // Left insert result
	Balanced23Tree<T> *ri; // Right insert result (often a BalancedEmpty 
			       // value)
	T		  max; // The maximum of both li and ri

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
	T		  max; // The maximum of both li and ri

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
			return make_tuple (
				  this
				, new BalancedEmpty<T>()
				, this->rt->max()); 
		} else {
			// I split into 2 2-nodes, 2nd case
			return make_tuple (
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
			return make_tuple (
				  new BalancedNode2<T> (this->lt, this->mt)
				, new BalancedNode2<T> (li, ri)
				, max);

		}
	}
}

#endif
