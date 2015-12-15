/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc
 * Copyright (C) 2015 pc <pc@localhost.localdomain>
 * 
 * skiena_3_16 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * skiena_3_16 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <fstream>
#include <boost/algorithm/string.hpp>
// #include <sys/time.h>
// #include <sys/resource.h>
#include <set>

#include "linked_list.h"
#include "bin_tree.h"
#include "balanced_23_tree.h"
#include "balanced_avl_tree.h"
#include "trie.h"

// #define TEST_LIST
#define TEST_BIN
#define TEST_23
#define TEST_AVL
#define TEST_TRIE

#define CLEAR_INTERVAL 5000

using namespace std;

int not_isalnum (int x) {
	return !std::isalnum (x);
}

bool clear (int i) {
	return (i+1)%CLEAR_INTERVAL == 0;
}

int main()
{

	// To implement also: 
	// - A trie.
	// - Red-black tree
	// - A hash table
	
	ifstream *f;
	f = new ifstream();
	f->open("pg1342.txt.full");

	string word;

	List<string>           *l = new Empty<string>();
	List<string>           *m = nullptr;

	BinTree<string>        *b = new BinEmpty<string>();
	BinTree<string>        *node = nullptr;

	Balanced23Tree<string> *bal_23_tree = new BalancedEmpty<string>();
	Balanced23Tree<string> *bal_23_node = nullptr;

	AVLTree<string, bool>  *avl_tree    = new AVLEmpty<string, bool>();
	AVLTree<string, bool>  *avl_node    = nullptr;

	Trie<char>             *trie        = new Trie<char>();
	Trie<char>             *trie_node   = nullptr;

	set<string> *std_set = new set<string>();
	set<string>::iterator it;

	int i = 0;

	assert (l->empty());

	while (f->good() && (i < 115500)) {
		
		*f >> word;
		boost::algorithm::to_lower(word);
		word.erase(
		  std::remove_if ( word.begin(), word.end()
                                 , (int(*)(int)) not_isalnum ), word.end());

		// cout << word << endl;

		// cout << avl_tree->pp() << endl;
		// cout << l->pp() << endl;

		it = std_set->find(word);
		if (it == std_set->end()) std_set->insert (word);
		else                      std_set->erase  (it);
		bool empty = (it == std_set->end());

		if (clear(i)) {
			cout << "Clearing std set..." << endl;
			std_set->clear();
		}

		#ifdef TEST_LIST
			m = l->find(word);
			if (m->empty()) list_insert (&l, word);
			else            list_remove (&l, word);

			if (clear(i)) {
				cout << "Clearing list..." << endl;
				// m = nullptr;
				// delete l;
				// l = new Empty<string>();
				while (!l->empty())
					list_release_head(&l);
			}
		#endif

		#ifdef TEST_BIN
			node = b->find(word);
			if (node->empty()) 
				b = b->insert(word);
		#endif

		#ifdef TEST_23
			if (bal_23_tree) bal_23_node = bal_23_tree->find(word);
			assert ((bal_23_node == nullptr) == empty);

			if (!bal_23_node)
				balanced_23_tree_insert (&bal_23_tree, word);
			else
				balanced_23_tree_remove (&bal_23_tree, word);

			if (clear(i)) {
				cout << "Clearing 2-3 tree..." << endl;
				// delete bal_23_tree;
				// bal_23_tree = new BalancedEmpty<string>();
				while (!bal_23_tree->empty()) {
					balanced_23_tree_release_max (&bal_23_tree);
					// balanced_23_tree_remove(
					//  &bal_23_tree, *(bal_23_tree->max()));
				}
			}
		#endif

		#ifdef TEST_AVL
			avl_node = avl_tree->find(word);
			assert (avl_node->empty() == empty);

			if (avl_node->empty()) 
				avl_insert (&avl_tree, word, true);
			else                   
				avl_remove (&avl_tree, word);

			if (clear(i)) {
				cout << "Clearing AVL tree..." << endl;
				while (!avl_tree->empty())
					delete avl_release_max (&avl_tree);
				// delete avl_tree;
				// avl_tree = new AVLEmpty<string>();
			}

		#endif

		#ifdef TEST_TRIE
			// Benchmark avl steps as trie steps because
			// the trie uses the avl tree...
			AVLTree<char, Trie<char>*>::p_steps = 
			  &(Trie<char>::steps);

			trie_node = trie->find(word.c_str(), word.size());
			// cout << trie->pp()     << endl;
			// cout << avl_tree->pp() << endl;
			
			assert ((trie_node == nullptr) == empty);
			if (trie_node == nullptr)
				trie->insert(word.c_str(), word.size());
			else
				trie->remove(word.c_str(), word.size());

			if (clear(i)) {
				cout << "Clearing Trie..." << endl;
				delete trie;
				trie = new Trie<char>();
			}

			/*
			AVLTree<char, Trie<char>*>::p_steps = 
			  &(AVLTree<char, Trie<char>*>::steps);
			*/
		#endif

		i++;
	}

//	assert (avl_tree->size() == l->size());

//	cout << l->pp()        << endl << endl;
//	cout << b->pp()        << endl << endl;
//	cout << bal_23_tree->pp() << endl << endl;
//	cout << avl_tree->pp() << endl << endl;

	// Report...
	cout << "*** Number of Steps" << endl;
	cout << "List: " << List<string>::steps << endl;
	cout << "BinTree: " << BinTree<string>::steps << endl;
	cout << "Balanced 2-3 Tree: " 
	     << Balanced23Tree<string>::steps << endl;
	cout << "AVL Tree: " << AVLTree<string, bool>::steps << endl;
	cout << "Trie: " << Trie<char>::steps << endl;
	
	cout << "*** Total words: " << i << endl;
	cout << "*** Sizes" << endl;
	cout << "STL set: ";
	cout << std_set->size() << endl;
	cout << "List: "; 
	cout << l->size() << endl; 
	cout << "Binary tree: ";
	cout << b->size() << endl;
	cout << "Balanced 2-3 tree: ";
	cout << bal_23_tree->size() << endl;
	cout << "AVL tree size: ";
	cout << avl_tree->size()
	     << ", height " << avl_tree->height() << endl;
	// cout << "Trie size: " << trie->size() << endl;

	// Cleanup
	if (l) delete l;

	if (b) delete b;

	delete std_set;

	if (bal_23_tree) delete bal_23_tree;
	if (avl_tree)    delete avl_tree;
	if (trie)        delete trie;

	f->close();
	delete f;

	return 0;
}

