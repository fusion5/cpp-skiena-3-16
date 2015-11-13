#ifndef SKIENA_3_16_LINKED_LIST
#define SKIENA_3_16_LINKED_LIST

/**
 * Linked list template, inspired a little from Haskell's list...
 * There is an empty list object and a Cons object which here is just called
 * List. We can determine at runtime whether the next element is empty or not.
 */

#include <string.h>
#include <boost/lexical_cast.hpp>

using namespace std;


template <class T>
class List {
	public:
		static  int global_linked_list_stepcount;
		virtual string  pp    ()    = 0; // Pretty Print
		virtual List<T> *find (T x) = 0; 
		virtual List<T> *cons (T x) = 0;
		virtual bool    empty ()    = 0;
		virtual int	size  ()    = 0;
};

template <class T>
int List<T>::global_linked_list_stepcount = 0;

template <class T>
class Cons: public List<T> {
	public: 
		Cons<T> (T x, List<T> *xs);
		Cons<T> (T x);
		List<T> *find (T x);
		List<T> *cons (T x);
		// List<T> *concat (Cons<T> xs);
		T value ();
		string pp (); // Pretty Print
		bool empty();
		int  size();
	private:
		List<T> *xs;
		T x;
};

template <class T>
class Empty : public List<T> {
	public: 
		Empty <T> ();
		string pp(); // Pretty Print
		List<T> *find (T x);
		List<T> *cons (T x);
		bool empty();
		int  size();
};

// Implementation:

template <class T>
Empty<T>::Empty() { }

template <class T>
string Empty<T>::pp() { return "[]"; }


template <class T>
bool Empty<T>::empty() { return true; }

template <class T>
List<T> *Empty<T>::cons (T x) {
	List<T>::global_linked_list_stepcount++;
	return new Cons<T>(x, this);
}

template <class T>
Cons<T>::Cons(T x, List<T> *xs) {
	List<T>::global_linked_list_stepcount++;
	this->x  = x;
	this->xs = xs;
}

template <class T>
List<T> *Cons<T>::cons (T x) {
	List<T>::global_linked_list_stepcount++;
	return new Cons<T>(x, this);
}

template <class T>
bool Cons<T>::empty () {
	List<T>::global_linked_list_stepcount++;
	return false;
}

// TODO: Consider adding a 'Maybe' type?

template <class T>
List<T> *Empty<T>::find (T x) { return this; }
template <class T>
List<T> *Cons<T>:: find (T x) {
	List<T>::global_linked_list_stepcount++;
	if (this->x == x) return this;
	// We know that xs is a *List<T> and that this is safe.
	return this->xs->find (x);
}

template <class T>
int Empty<T>::size () { return 0; }
template <class T>
int Cons<T> ::size () {
	List<T>::global_linked_list_stepcount++;
	return 1 + this->xs->size(); 
}

template <class T>
T Cons<T>::value() {
	return this->x;
}

template <class T>
string Cons<T>::pp() {
	return (boost::lexical_cast<std::string>(this->x)) + ":" 
	     + this->xs->pp();
}

#endif
