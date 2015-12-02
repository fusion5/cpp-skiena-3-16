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

#include "linked_list.h"
#include "bin_tree.h"
#include "balanced_23_tree.h"
#include "balanced_avl_tree.h"

#define TEST_LIST
// #define TEST_BIN
// #define TEST_23
// #define TEST_AVL

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
	Balanced23Tree<string> *bal_23_node;

	unique_ptr<AVLTree<string> > avl_tree (new AVLEmpty<string>());
	AVLTree<string> *avl_node;

	int i = 0;

	while (f->good() && (i < 500000)) {
		
		*f >> word;
		boost::algorithm::to_lower(word);
		word.erase(
		  std::remove_if ( word.begin(), word.end()
                                 , (int(*)(int)) not_isalnum ), word.end());

		// cout << avl_tree->pp() << endl;
		// cout << l->pp() << endl;

		#ifdef TEST_LIST
			m = l->find(word);
			bool empty = m->empty();

			// cout << word << " result empty? " << empty << endl;

			if (m->empty()) {
				// cout << word << " inserting! " << endl;
				// int os = l->size();
				list_insert (&l, word);
				// assert(l->size() == (os + 1));
			} else {
				// cout << word << " removing! " << endl;
				// int os = l->size();
				list_remove (&l, word);
				// assert(l->size() == (os - 1));
			}
		#endif

		#ifdef TEST_BIN
			node = b->find(word);
			assert (node->empty() == empty);
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
				// cout << "Insert: " << word << endl;
				bal_23_tree = balanced_23_tree_insert<string> (
				  bal_23_tree, word);
			} else {
				// cout << "Remove: " << word << endl;
				balanced_23_tree_remove<string> (&bal_23_tree, 
				  word);
			}
			// cout << bal_23_tree->pp() << endl << endl;
		#endif

		#ifdef TEST_AVL
			avl_node = avl_tree->find(word);
			#ifdef TEST_LIST
				assert (avl_node->empty() == empty);
			#endif
			if (avl_node->empty()) {
				// cout << "Insert: " << word << endl;
				avl_insert (&avl_tree, word);
				// cout << avl_tree->pp() << endl;
			} else {
				// int os = avl_tree->size();
				// cout << "Delete: " << word << endl;
				// cout << avl_tree->pp() << endl;
				avl_remove (&avl_tree, word);
				// cout << avl_tree->pp() << endl;
				// cout << avl_tree->size() << " " << os << endl;
				// assert(avl_tree->size() == (os - 1));
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

	cout << "List steps: " << List<string>::steps << endl;
	cout << "BinTree steps: " << BinTree<string>::steps << endl;
	cout << "Balanced 2-3 Tree steps: " 
	     << Balanced23Tree<string>::steps << endl;
	cout << "AVL Tree steps: " << AVLTree<string>::steps << endl;
	
	cout << "Total words: " << i << "; list size: " 
 	     << l->size() 
	     << "; tree size: "
	     << b->size()
	     << "; balanced 2-3 tree size: "
	     << bal_23_tree->size()
	     << "; AVL tree size: "
	     << avl_tree->size()
	     << ", height " << avl_tree->height()
	     << endl;

	return 0;
}

