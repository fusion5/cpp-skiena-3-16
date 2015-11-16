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


#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "linked_list.h"
#include "bin_tree.h"

using namespace std;

int not_isalnum (int x) {
	return !std::isalnum (x);
}

int main()
{

	// Parse words from a file...
	
	ifstream *f;
	f = new ifstream();
	f->open("pg1342.txt");

	string word;

	List<string> *l = new Empty<string>();
	List<string> *m;

	BinTree<string> *b = new Leaf<string>();
	BinTree<string> *node;

	int i = 0;

	while (f->good() && (i < 1000)) {
		
		*f >> word;
		boost::algorithm::to_lower(word);
		word.erase(
		  std::remove_if ( word.begin(), word.end()
                                 , (int(*)(int)) not_isalnum ), word.end());

		m = l->find(word);
		if (m->empty()) {
			l = l->cons(word);
		}

		node = b->find(word);
		if (m->empty()) {
			b = b->insert(word);
		}

		i++;
	}

	cout << b->pp() << endl;

	cout << "Total words: " << i << "; list size (unique words): " 
 	     << l->size() 
	     << "; tree size (unique words): "
	     << b->size()
	     << endl;
	cout << "List steps: " << List<string>::global_linked_list_stepcount << endl;
	cout << "BinTree steps: " << BinTree<string>::global_binary_tree_stepcount << endl;

	return 0;
}

