#ifndef SKIENA_3_16_HASH_TABLE
#define SKIENA_3_16_HASH_TABLE

#include "linked_list.h"
#include <iostream>
#include <cmath>

/*
 * Our hash table has a key type K and a value type V.
 * The key must have a hash function from K to int...
 * */
template <class K, class V>
class HashTable {
	public:
		HashTable<K, V>  (unsigned int size);
		~HashTable<K, V> ();
		List<K>          *find   (K k);
		void             insert (K k, V v);
		void             remove (K k);
		bool             empty  ();
		int              size   ();
		K                max    ();
		string           pp     ();
		static int steps;
	private:
		unsigned int     hash   (K k, int *counter);
		List<K>          **xs; // Array of pointers to list objects
		unsigned int n;
};

template <class K, class V>
int HashTable<K, V>::steps = 0;

/* ctors */
template <class K, class V>
HashTable<K, V>::HashTable (unsigned int size) {
	this->n  = size;
	this->xs = new List<K>*[size];

	// List<K> *it = *(this->xs);
	for (int i = 0; i < size; i++) {
		// Initialize all lists with an empty list...
		this->xs[i] = new Empty<K>();
	}
}

/* dtors */
template <class K, class V>
HashTable<K, V>::~HashTable () {
	for (int i = 0; i < this->n; i++)
		delete this->xs[i];
	delete this->xs[this->n];
}

int modular_pow (unsigned int base, unsigned int exponent, unsigned int modulus,
  int *counter) {
        // https://en.wikipedia.org/wiki/Modular_exponentiation
	if (modulus == 1) return 0;
	int c = 1;
	base = base % modulus;
	while (exponent > 0) {
		(*counter)++;
		if (exponent % 2 == 1)
			c = (c * base) % modulus;
		exponent = exponent >> 1;
		base = (base * base) % modulus;
	}
	/*
	for (int e_prime = 1; e_prime < exponent; e_prime++) {
		(*counter)++;
		c = (c * base) % modulus;
	}
	*/
	return c;
}


int char_to_int (char c) {
	switch (c) {
		case '0': return 0; case '1': return 1; case '2': return 2; 
		case '3': return 3; case '4': return 4; case '5': return 5;
		case '6': return 6; case '7': return 7; case '8': return 8;
		case '9': return 9;
		case 'a': return 10; case 'b': return 11; case 'c': return 12; 
		case 'd': return 13; case 'e': return 14; case 'f': return 15;
		case 'g': return 16; case 'h': return 17; case 'i': return 18;
		case 'j': return 19; case 'k': return 20; case 'l': return 21;
		case 'm': return 22; case 'n': return 23; case 'o': return 24;
		case 'p': return 25; case 'q': return 26; case 'r': return 27;
		case 's': return 28; case 't': return 29; case 'u': return 30;
		case 'v': return 31; case 'w': return 32; case 'x': return 33;
		case 'y': return 34; case 'z': return 35;
	}
	assert (false);
}
template <>
unsigned int HashTable<string, bool>::hash (string s, int *counter) {

	if (s == "") return 0;

	// Our alphabet is alphanumeric lowercase characters, a-z, 0-9
	// We use n = 
	//   sum from i = 0 to i = s.size of 
	//     char_to_int(z) ^ (s.size - i + 1) * char_to_int (s[i])
	// return n % this->n
	//
	// But clearly this polynomial will exceed max int so we work with modular 
	// arithmetic using the operations below...
	//
	//   (a mod n)   + (b mod n)  = (a + b) mod n...
	//   (21 mod 24) + (9 mod 24) = 6 mod 24
	//   https://en.wikipedia.org/wiki/Modular_exponentiation

	int i                   = 0;
	unsigned long long n    = 0;
	unsigned long long oldn = 0;

	while (i < s.size()) {
		(*counter) ++;
		n = (n % this->n) +
			(modular_pow(char_to_int('z'), s.size() - i + 1, this->n
			, counter) 
			* char_to_int(s[i])) % this->n;
		n = n % this->n;
		i++;
	}

	// If a1 == b1 (mod n)
	//    a2 == b2 (mod n)
	// Then a1*a2 == b1*b2 (mod n)
	//
	// (a mod n)*(b mod n) == (a*b) mod n
	assert (n <= this->n);
	assert (n >= 0);
	
	return n;
}

template <class K, class V>
void HashTable<K, V>::insert (K k, V v) {
	int hash = this->hash (k, &HashTable<K, V>::steps);
	assert (hash >= 0);
	assert (hash < this->n);
	this->xs[hash] = this->xs[hash]->insert(k);
}

template <class K, class V>
List<K> *HashTable<K, V>::find (K k) {
	// TODO: Make it return the value...
	int hash = this->hash (k, &HashTable<K, V>::steps);
	return this->xs[hash]->find (k);
}

template <class K, class V>
void HashTable<K, V>::remove (K k) {
	int hash = this->hash (k, &HashTable<K, V>::steps);
	list_remove (&(this->xs[hash]), k);
}

#endif
