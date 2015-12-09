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

template <class T>
class List;

template <class T>
class List {
	public:
		static  int steps;
		virtual ~List<T>            ()    = 0;
		virtual List<T> *find       (T x) = 0; 
		virtual List<T> *insert     (T x) = 0;
		virtual List<T> *remove     (T x) = 0;
		virtual List<T> *release_xs ()    = 0;
		virtual bool    empty       ()    = 0;
		virtual int     size        ()    = 0;
		virtual string  pp          ()    = 0; // Pretty Print
		virtual T       value       ()    = 0;
};

template <class T>
int List<T>::steps = 0;

template <class T>
class Cons: public List<T> {
	public: 
		Cons<T>  (T x, List<T> *xs);
		Cons<T>  (T x);
		~Cons<T> ();
		List<T> *find (T x);
		List<T> *insert (T x);
		List<T> *remove (T x);
		List<T> *release_xs ();
		bool    empty();
		int     size();
		string  pp (); // Pretty Print
		T       value ();
	private:
		List<T> *xs; // the rest of the list, x sequence
		T x;         // current node, x
};

template <class T>
class Empty : public List<T> {
	public: 
		Empty <T> ();
		~Empty<T> ();
		List<T> *find (T x);
		List<T> *insert (T x);
		List<T> *remove (T x);
		List<T> *release_xs ();
		bool    empty();
		int     size();
		string  pp(); // Pretty Print
		T       value ();
};

// Implementation:

template <class T>
List<T>::~List() {
	// This destructor is called for both cons and empty instances...
}

template <class T>
Empty<T>::Empty() { }
template <class T>
Empty<T>::~Empty() {}

template <class T>
Cons<T>::Cons(T x, List<T> *xs) {
	List<T>::steps++;
	this->x  = x;
	this->xs = xs;
}
template <class T>
Cons<T>::~Cons() {
	delete this->xs;
}

/* insert */
template <class T>
List<T> *Empty<T>::insert (T x) {
	List<T>::steps++;
	return new Cons<T>(x, this);
}
template <class T>
List<T> *Cons<T>::insert (T x) {
	List<T>::steps++;
	return new Cons<T>(x, this);
}

/* release_xs */
template <class T>
List<T> *Empty<T>::release_xs () {
	cerr << "release cannot be called on an empty list!";
	throw "error";
}
template <class T>
List<T> *Cons<T>::release_xs () {
	List<T> *xs = this->xs;
	this->xs = new Empty<T>();
	return xs;
}

/*
 * Which version is more efficient?
 * */
/*
template <class T>
List<T> *Cons<T>::remove (T x) {

	if (this->xs->empty()) return nullptr;

	if (this->xs->value() == x) {
		List<T> *r = this->xs->release_xs();
		delete this->xs;
		this->xs = r;
		return nullptr;
	}
	
	this->xs->remove(x);
}
*/
template <class T>
List<T> *Cons<T>::remove (T x) {
	List<T>::steps++;
	if (this->x == x) return this->release_xs();

	List<T> *r;
	r = this->xs->remove (x);

	if (r != nullptr) {
		delete this->xs;
		this->xs = r;
	}

	return nullptr;
}
template <class T>
List<T> *Empty<T>::remove(T x) {
	return nullptr;
}

template <class T>
bool Cons<T>::empty () { return false; }
template <class T>
bool Empty<T>::empty() { return true; }

/* find */
template <class T>
List<T> *Empty<T>::find (T x) { return this; }
template <class T>
List<T> *Cons<T>:: find (T x) {
	List<T>::steps++;
	if (this->x == x) return this;
	return            this->xs->find(x);
}

/* size */
template <class T>
int Empty<T>::size () { return 0; }
template <class T>
int Cons<T> ::size () {
	List<T>::steps++;
	return 1 + this->xs->size(); 
}

/* pp - pretty printing */
template <class T>
string Cons<T>::pp() {
	return (boost::lexical_cast<std::string>(this->x)) + ":" 
	     + this->xs->pp();
}
template <class T>
string Empty<T>::pp() { return "[]"; }

template <class T>
T Cons<T>::value() {
	return this->x;
}
template <class T>
T Empty<T>::value() {
	cerr << "value() called on empty list" << endl;
	throw "error";
}

template <class T>
void list_remove (List<T> **xs, T x) {

	if ((*xs)->empty()) return;
	if ((*xs)->value() == x) {
		List<T> *r;
		r = (*xs)->release_xs ();
		delete (*xs);
		*xs = r;
		return;
	}
	(*xs)->remove (x);
}

template <class T>
void list_insert (List<T> **xs, T x) {
	*xs = (*xs)->insert(x);
}
#endif
