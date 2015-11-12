#ifndef SKIENA_3_16_LINKED_LIST
#define SKIENA_3_16_LINKED_LIST

#include <string.h>
#include <typeinfo>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

template <class T>
class EmptyList {
	public: 
		EmptyList<T> ();
		string pp(); // Pretty Print
};

template <class T>
class List : EmptyList<T> {
	public: 
		List<T> (T a);
		List<T> (T a, EmptyList<T> *right);
		List<T> next ();
		bool has_next ();
		List<T> *cons (T next);
		T value ();
		string pp(); // Pretty Print
	private:
		EmptyList<T> *nx;
		T data;
};

// Implementation:

template <class T>
EmptyList<T>::EmptyList() { }

template <class T>
string EmptyList<T>::pp() { return "[]"; }


template <class T>
List<T>::List(T a) {
	this->data = a;

	// Initialize next with the empty list so that we avoid null values...
	EmptyList<T> *e = new EmptyList<T>();
	this->nx = e;
}

template <class T>
List<T>::List(T a, EmptyList<T> *nx) {
	this->data = a;
	this->nx = nx;
}

template <class T>
List<T> List<T>::next() {
	return *(this->nx);
}

template <class T>
bool List<T>::has_next() {
	// this->nx is not an EmptyList...
	return typeid(this->nx) != typeid(EmptyList<T>);
}


template <class T>
List<T> *List<T>::cons (T x) {
	this->nx = new List<T>(x, this->nx);
	return this;
}

template <class T>
T List<T>::value() {
	return this->data;
}

template <class T>
string List<T>::pp() {
	return (boost::lexical_cast<std::string>(this->data)) + ":" 
	     + this->nx->pp();
}

#endif
