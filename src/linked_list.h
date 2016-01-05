#ifndef SKIENA_3_16_LINKED_LIST
#define SKIENA_3_16_LINKED_LIST

/**
 * Linked list template, inspired a little from Haskell's list...
 * There is an empty list object and a Cons object which here is just called
 * List. We can determine at runtime whether the next element is empty or not.
 */

#include <string.h>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;

template <class K, class V>
class List;

template <class K, class V>
class List {
	public:
		static  int steps;
		virtual ~List<K, V>            ()         = 0;
		virtual List<K, V> *find       (K x)      = 0; 
		virtual List<K, V> *insert     (K x, V v) = 0;
		virtual List<K, V> *remove     (K x)      = 0;
		virtual List<K, V> *release_xs ()         = 0;
		virtual bool    empty          ()         = 0;
		virtual int     size           ()         = 0;
		virtual string  pp             ()         = 0; // Pretty Print
		virtual V       value          ()         = 0;
		virtual K       key            ()         = 0;
};

template <class K, class V>
int List<K, V>::steps = 0;

template <class K, class V>
class Cons: public List<K, V> {
	public: 
		Cons<K, V>  (K x, V v, List<K, V> *xs);
		Cons<K, V>  (K x, V v);
		~Cons<K, V> ();
		List<K, V> *find   (K x);
		List<K, V> *insert (K x, V v);
		List<K, V> *remove (K x);
		List<K, V> *release_xs ();
		bool    empty();
		int     size();
		string  pp (); // Pretty Print
		V       value ();
		K       key ();
	private:
		List<K, V> *xs; // the rest of the list, x sequence
		K x;
		V v;
};

template <class K, class V>
class Empty : public List<K, V> {
	public: 
		Empty <K, V> ();
		~Empty<K, V> ();
		List<K, V> *find (K x);
		List<K, V> *insert (K x, V v);
		List<K, V> *remove (K x);
		List<K, V> *release_xs ();
		bool    empty();
		int     size();
		string  pp(); // Pretty Print
		V       value ();
		K       key ();
};

// Implementation:

template <class K, class V>
List<K, V>::~List() {
	// This destructor is called for both cons and empty instances...
}

template <class K, class V>
Empty<K, V>::Empty() { }
template <class K, class V>
Empty<K, V>::~Empty() {}

template <class K, class V>
Cons<K, V>::Cons(K x, V v,  List<K, V> *xs) {
	List<K, V>::steps++;
	this->x  = x;
	this->v  = v;
	this->xs = xs;
}
template <class K, class V>
Cons<K, V>::~Cons() {
	delete this->xs;
}

/* insert */
template <class K, class V>
List<K, V> *Empty<K, V>::insert (K x, V v) {
	List<K, V>::steps++;
	return new Cons<K, V>(x, v, this);
}
template <class K, class V>
List<K, V> *Cons<K, V>::insert (K k, V v) {
	List<K, V>::steps++;
	return new Cons<K, V>(x, v, this);
}

/* release_xs */
template <class K, class V>
List<K, V> *Empty<K, V>::release_xs () {
	cerr << "release cannot be called on an empty list!";
	throw "error";
}
template <class K, class V>
List<K, V> *Cons<K, V>::release_xs () {
	List<K, V> *xs = this->xs;
	this->xs = new Empty<K, V>();
	return xs;
}

/*
 * Which version is more efficient?
 * */
/*
template <class K, class V>
List<K, V> *Cons<K, V>::remove (T x) {

	if (this->xs->empty()) return nullptr;

	if (this->xs->value() == x) {
		List<K, V> *r = this->xs->release_xs();
		delete this->xs;
		this->xs = r;
		return nullptr;
	}
	
	this->xs->remove(x);
}
*/
template <class K, class V>
List<K, V> *Cons<K, V>::remove (K x) {
	List<K, V>::steps++;
	if (this->x == x) return this->release_xs();

	List<K, V> *r;
	r = this->xs->remove (x);

	if (r != nullptr) {
		delete this->xs;
		this->xs = r;
	}

	return nullptr;
}
template <class K, class V>
List<K, V> *Empty<K, V>::remove(K x) {
	return nullptr;
}

template <class K, class V>
bool Cons<K, V>::empty () { return false; }
template <class K, class V>
bool Empty<K, V>::empty() { return true; }

/* find */
template <class K, class V>
List<K, V> *Empty<K, V>::find (K x) { return this; }
template <class K, class V>
List<K, V> *Cons<K, V>:: find (K x) {
	List<K, V>::steps++;
	if (this->x == x) return this;
	return            this->xs->find(x);
}

/* size */
template <class K, class V>
int Empty<K, V>::size () { return 0; }
template <class K, class V>
int Cons<K, V> ::size () {
	List<K, V>::steps++;
	return 1 + this->xs->size(); 
}

/* pp - pretty printing */
template <class K, class V>
string Cons<K, V>::pp() {
	return (boost::lexical_cast<std::string>(this->x)) + ":" 
	     + this->xs->pp();
}
template <class K, class V>
string Empty<K, V>::pp() { return "[]"; }

/* key */
template <class K, class V>
K Cons<K, V>::key() {
	return this->x;
}
template <class K, class V>
K Empty<K, V>::key() {
	cerr << "key() called on empty list" << endl;
	throw "error";
}

/* value */
template <class K, class V>
V Cons<K, V>::value() {
	return this->v;
}
template <class K, class V>
V Empty<K, V>::value() {
	cerr << "value() called on empty list" << endl;
	throw "error";
}

template <class K, class V>
void list_remove (List<K, V> **xs, K x) {

	if ((*xs)->empty()) return;
	if ((*xs)->key() == x) {
		List<K, V> *r;
		r = (*xs)->release_xs ();
		delete (*xs);
		*xs = r;
		return;
	}
	(*xs)->remove (x);
}

template <class K, class V>
void list_insert (List<K, V> **xs, K x, V v) {
	*xs = (*xs)->insert(x, v);
}

template <class K, class V>
K list_release_head (List<K, V> **xs) {
	K ret;
	List<K, V> *x;
	x = (*xs);
	(*xs) = (*xs)->release_xs();
	ret = x->key();
	delete x;
	return ret;
}

#endif
