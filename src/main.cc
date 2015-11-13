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

// A global variable (ack!) which we use for benchmarks..

#include "linked_list.h"

using namespace std;


int main()
{

	// Parse words from a file...
	
	ifstream *f;
	f = new ifstream();
	f->open("pg1342.txt");

	string word;

	List<string> *l = new Empty<string>();
	List<string> *m;

	int i = 0;

	while (f->good()) {
		
		*f >> word;
		boost::algorithm::to_lower(word);

		m = l->find(word);
		if (m->empty()) { 
			// cout << "Adding: " << word << endl;
			l = l->cons(word);
		}

		i++;
	}

	cout << "Total words: " << i << "; list size (unique words): " 
 	     << l->size() << endl;
	cout << "Steps: " << List<string>::global_linked_list_stepcount << endl;

	/*
	List<int> *l;
	l = new Cons<int> (4, new Cons<int>(5, new Cons<int>(6, new Empty<int>())));
	List<int> *k;
	k = l->cons(3)->cons(2)->cons(1);

	List <int> *m;
	m = k->find(9); 
	
	if (m->empty())
		cout << "Not found: " << m->pp() << endl;
	else
		cout << "Found: " << m->pp() << endl;
	*/

	return 0;
}

