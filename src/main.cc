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

#define TEST_STDSET
// #define TEST_LIST
// #define TEST_BIN
#define TEST_23
#define TEST_AVL

#define CLEAR_INTERVAL 5000

using namespace std;

int not_isalnum (int x) {
	return !std::isalnum (x);
}

int main()
{

	// Parse words from a file...
	// To implement also: 
	// - A trie.
	// - Red-black tree
	
	ifstream *f;
	f = new ifstream();
	f->open("pg1342.txt.full");

	string word;

	List<string> *l = new Empty<string>();
	List<string> *m;

	BinTree<string> *b = new Leaf<string>();
	BinTree<string> *node;

	Balanced23Tree<string> *bal_23_tree = new BalancedEmpty<string>();
	Balanced23Tree<string> *bal_23_node = nullptr;

	AVLTree<string>        *avl_tree = new AVLEmpty<string>();
	AVLTree<string>        *avl_node = nullptr;

	set<string> *std_set = new set<string>();
	set<string>::iterator it;

	int i = 0;

	assert (l->empty());

	while (f->good() && (i < 9500)) {
		
		*f >> word;
		boost::algorithm::to_lower(word);
		word.erase(
		  std::remove_if ( word.begin(), word.end()
                                 , (int(*)(int)) not_isalnum ), word.end());

		// cout << avl_tree->pp() << endl;
		// cout << l->pp() << endl;

		#ifdef TEST_STDSET
			it = std_set->find(word);
			if (it == std_set->end())
				std_set->insert (word);
			else
				std_set->erase (it);

			if ((i+1)%CLEAR_INTERVAL == 0) {
				cout << "Clearing std set..." << endl;
				std_set->clear();
			}
		#endif

		#ifdef TEST_LIST
			m = l->find(word);
			bool empty = m->empty();
			// cout << word << " result empty? " << empty << endl;
			if (m->empty()) {
				// delete m;
				// cout << "Inserting: " << word << endl;
				// int os = l->size();
				list_insert (&l, word);
				// l = l->insert(word);
				// assert(l->size() == (os + 1));
			} else {
				// cout << "Removing: " << word << endl;
				// int os = l->size();
				list_remove (&l, word);
				// assert(l->size() == (os - 1));
			}
			if ((i+1)%CLEAR_INTERVAL == 0) {
				cout << "Clearing list..." << endl;
				// m = nullptr;
				// delete l;
				// l = new Empty<string>();
				while (!l->empty()) {
					m = l;
					l = l->release_xs();
					delete m;
				}
			}
		#endif

		#ifdef TEST_BIN
			node = b->find(word);
			#ifdef TEST_LIST
			if (node->empty()) b = b->insert(word);
			#endif
		#endif

		#ifdef TEST_23
			bal_23_node = bal_23_tree->find(word);
			#ifdef TEST_LIST
			assert (bal_23_node->empty() == empty);
			#endif

			if (bal_23_node->empty()) {
				// cout << "Insert " << word << endl;
				delete bal_23_node;
				bal_23_node = nullptr;
				bal_23_tree = balanced_23_tree_insert<string> (
				  bal_23_tree, word);
			}
			else {
				// cout << "Remove " << word << endl;
				balanced_23_tree_remove<string> (&bal_23_tree, 
				  word);
			}
			// cout << bal_23_tree->pp() << endl;

			if ((i+1)%CLEAR_INTERVAL == 0) {
				cout << "Clearing 2-3 tree..." << endl;
				// Empty the 23-tree
				// delete bal_23_tree;
				// bal_23_tree = new BalancedEmpty<string>();
				while (!bal_23_tree->empty()) {
					balanced_23_tree_remove<string>(
					  &bal_23_tree, *(bal_23_tree->max()));
				}
			}
		#endif

		#ifdef TEST_AVL
			// cout << avl_tree->pp() << endl;
			avl_node = avl_tree->find(word);
			#ifdef TEST_LIST
				assert (avl_node->empty() == empty);
			#endif
			if (avl_node->empty()) {
				avl_insert (&avl_tree, word);
			} else {
				avl_remove (&avl_tree, word);
			}
			if ((i+1)%CLEAR_INTERVAL == 0) {
				cout << "Clearing AVL tree..." << endl;
				while (!avl_tree->empty())
					avl_release_max (&avl_tree);
				// delete avl_tree;
				// avl_tree = new AVLEmpty<string>();
			}

		#endif
		i++;
	}

//	assert (avl_tree->size() == l->size());

/*
	cout << l->pp()        << endl << endl;
	cout << b->pp()        << endl << endl;
	cout << bal_23_tree->pp() << endl << endl;
	cout << avl_tree->pp() << endl << endl;
*/

	// Report...
	cout << "List steps: " << List<string>::steps << endl;
	cout << "BinTree steps: " << BinTree<string>::steps << endl;
	cout << "Balanced 2-3 Tree steps: " 
	     << Balanced23Tree<string>::steps << endl;
	cout << "AVL Tree steps: " << AVLTree<string>::steps << endl;
	
	cout << "Total words: " << i 
	     << "; set size: " 
	     << std_set->size()
	     << "; list size: " 
	     << l->size() 
	     << "; tree size: "
	     << b->size()
	     << "; balanced 2-3 tree size: "
	     << bal_23_tree->size()
	     << "; AVL tree size: "
	     << avl_tree->size()
	     << ", height " << avl_tree->height()
	     << endl;

	// Cleanup
	std_set->clear();
	delete bal_23_tree;
	delete bal_23_node;
	delete avl_tree;

	f->close();
	delete f;

	return 0;
}

