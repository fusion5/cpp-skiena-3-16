#ifndef SKIENA_3_16_TRIE
#define SKIENA_3_16_TRIE

#include <boost/lexical_cast.hpp>

template <class T>
class Trie {
	public:
		static int steps;
		Trie<T>         (T x);
		Trie<T>         ();
		~Trie<T>        ();
		Trie<T> *find   ();
		void    insert  (T x[], int nx);
		T       *value  ();
		T       *max    ();
		T x;
		Trie<T> *release_subtrees();
		string  pp      ();
		Trie<T>& operator=(Trie<T>& t) {
			this->xs.reset(t.release_subtrees());
			return *this;
		}
	private:
		void    expand  ();
		bool    present;
		unique_ptr <Trie<T>[]> xs;
		int n;
};

/* ctors */
template <class T>
Trie<T>::Trie (T x) {
	this->x = x;
	this->xs = unique_ptr<Trie<T>[]>();
}
template <class T>
Trie<T>::Trie () {
	this->present = false;
	this->xs = unique_ptr<Trie<T>[]>();
}
/* dtors */
template <class T>
Trie<T>::~Trie () {
}

/* value */
template <class T>
T *Trie<T>::value() {
	return &(this->x);
}

template <class T>
void Trie<T>::insert (T xs[], int nx) {
	T x = xs[0];

	if (nx == 1) {
		assert (x == this->x);
		this->present = true;
		return;
	}

	// Is x in our xs?
	for (int i = 0; i < this->n; i++)
		if (*(this->xs[i].value()) == x) {
			this->xs[i].insert (xs++, n-1);
			return;
		}

	// x isn't in our xs... insert x to xs. We allocate more memory for 
	// that...
	this->expand();
	this->xs[this->n-1].x = x;
}

template <class T>
Trie<T> *Trie<T>::release_subtrees() {
	return this->xs.release();
}

template <class T>
void Trie<T>::expand () {
	// FIXME: optimize this so as to not expand for each addition...
	this->n++;
	Trie<T> *newxs = new Trie<T>[this->n];
	Trie<T> *oldxs = this->xs.release();
	for (int i = 0; i < this->n - 1; i++)
		newxs[i] = oldxs[i];
	this->xs.reset(newxs);
}

/* pp - pretty printing */
template <class T>
string Trie<T>::pp () {
	string ret;
	ret = "(" + boost::lexical_cast<std::string>(this->x) + " ";
	for (int i = 0; i < this->n - 1; i++)
		ret += this->xs[i].pp();
	ret += ")";
	return ret;
}

#endif
