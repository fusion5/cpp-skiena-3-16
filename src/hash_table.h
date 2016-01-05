#ifndef SKIENA_3_16_HASH_TABLE
#define SKIENA_3_16_HASH_TABLE

#include "linked_list.h"
#include <iostream>
#include <cmath>

#define MAX_KEY_LEN 50

/*
 * Our hash table has a key type string and a value type V.
 * The key must have a hash function from string to int...
 * TODO: Actually store values V because now it's just a set...
 * */
template <class V>
class HashTable {
	public:
		HashTable<V>  (unsigned int size);
		~HashTable<V> ();
		List<string, V>  *find  (string k);
		void             insert (string k, V v);
		void             remove (string k);
		bool             empty  ();
		int              size   ();
		string           max    ();
		string           pp     ();
		static int steps;
	private:
		unsigned int     hash   (string k, int *counter);
		List<string, V>  **xs;      // Array of pointers to list objects
		int              *mod_pows; // Modulo power cache table
		unsigned int n;             // Hash size
};

template <class V>
int HashTable<V>::steps = 0;

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

/* ctors */
template <class V>
HashTable<V>::HashTable (unsigned int size) {
	this->n  = size;
	this->xs = new List<string, V>*[size];
	this->mod_pows = new int[MAX_KEY_LEN];

	for (int i = 0; i < MAX_KEY_LEN; i++)
		this->mod_pows[i] = modular_pow (char_to_int('z'), i, this->n, 
		  &HashTable<V>::steps);

	for (int i = 0; i < size; i++) {
		// Initialize all lists with an empty list...
		this->xs[i] = new Empty<string, V>();
	}
}

/* dtors */
template <class V>
HashTable<V>::~HashTable () {
	for (int i = 0; i < this->n; i++)
		delete this->xs[i];
	delete[] this->xs;
	delete[] this->mod_pows;
}

template <>
unsigned int HashTable<bool>::hash (string s, int *counter) {

	assert (s.size() < MAX_KEY_LEN);

	if (s == "") return 0;

	// Our alphabet is alphanumeric lowercase characters, a-z, 0-9
	// We use n = 
	//   sum from i = 0 to i = s.size of 
	//     char_to_int(z) ^ (s.size - i + 1) * char_to_int (s[i])
	// return n % this->n
	//
	// But clearly this polynomial will exceed max int so we work with 
	// modular arithmetic using the operations below...
	//
	//   (a mod n)   + (b mod n)  = (a + b) mod n...
	//   (21 mod 24) + (9 mod 24) = 6 mod 24
	//   https://en.wikipedia.org/wiki/Modular_exponentiation

	int i                   = 0;
	unsigned long long n    = 0;

	while (i < s.size()) {
		(*counter) ++;
		n = n + ( this->mod_pows[s.size() - i + 1] * char_to_int(s[i])) 
		      % this->n;
		n = n % this->n;
		i++;
	}

	assert (n <= this->n);
	assert (n >= 0);
	
	return n;
}

template <class V>
void HashTable<V>::insert (string k, V v) {
	int hash = this->hash (k, &HashTable<V>::steps);
	assert (hash >= 0);
	assert (hash < this->n);
	this->xs[hash] = this->xs[hash]->insert(k, v);
}

template <class V>
List<string, V> *HashTable<V>::find (string k) {
	int hash = this->hash (k, &HashTable<V>::steps);
	return this->xs[hash]->find (k);
}

template <class V>
void HashTable<V>::remove (string k) {
	int hash = this->hash (k, &HashTable<V>::steps);
	list_remove (&(this->xs[hash]), k);
}

template <class V>
int HashTable<V>::size() {
	int r = 0;
	for (int i = 0; i < this->n; i++)
		r += this->xs[i]->size();
	return r;
}
#endif
