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
class List;

template <class T>
class List {
	public:
		static  int steps;
	   	virtual List<T> *find   (T x) = 0; 
		virtual List<T> *insert (T x) = 0;
		virtual bool    empty   ()    = 0;
		virtual int     size    ()    = 0;
		virtual string  pp      ()    = 0; // Pretty Print
};

template <class T>
int List<T>::steps = 0;

template <class T>
class Cons: public List<T> {
	public: 
		Cons<T> (T x, List<T> *xs);
		Cons<T> (T x);
		List<T> *find (T x);
		List<T> *insert (T x);
		T value ();
		bool    empty();
		int     size();
		string  pp (); // Pretty Print
	private:
		List<T> *xs;
		T x;
};

template <class T>
class Empty : public List<T> {
	public: 
		Empty <T> ();
		List<T> *find (T x);
		List<T> *insert (T x);
		bool    empty();
		int     size();
		string  pp(); // Pretty Print
};

// Implementation:

template <class T>
Empty<T>::Empty() { }

template <class T>
List<T> *Empty<T>::insert (T x) {
	List<T>::steps++;
	return new Cons<T>(x, this);
}

template <class T>
Cons<T>::Cons(T x, List<T> *xs) {
	List<T>::steps++;
	this->x  = x;
	this->xs = xs;
}

template <class T>
List<T> *Cons<T>::insert (T x) {
	List<T>::steps++;
	return new Cons<T>(x, this);
}

template <class T>
bool Cons<T>::empty () { return false; }
template <class T>
bool Empty<T>::empty() { return true; }

// TODO: Consider adding a 'Maybe' type and make the return type Maybe T?

template <class T>
List<T> *Empty<T>::find (T x) { return this; }
template <class T>
List<T> *Cons<T>:: find (T x) {
	List<T>::steps++;
	if (this->x == x) return this;
	// We know that xs is a *List<T> and that this is safe.
	return this->xs->find (x);
}

template <class T>
int Empty<T>::size () { return 0; }
template <class T>
int Cons<T> ::size () {
	List<T>::steps++;
	return 1 + this->xs->size(); 
}

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
// No implementation for Empty::value()


#endif
