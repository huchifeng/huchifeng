/*

Copyright (C) 2011 huchifeng@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

CONTACT: huchifeng@gmail.com
HOMEPAGE: https://github.com/huchifeng/huchifeng

*/
/*
UG: Undirected graph
UAG: Undirected acyclic graph // tree
DG: Digraph/digram 
DAG: Directed acyclic graph

when graph degenerate to tree, the drawing should be a tree ?

*/

#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <assert.h>
using namespace std;

const char* dots = "┄"; // "┄";

class graph_t{
public:
	graph_t& operator()(string n, double v){
		item i;
		i.n = n;
		i.v = v;
		items.push_back(i);
		return *this;
	}
	void print(double m);
private:
	struct item{
		string n;
		double v;
	};
	vector<item> items;
};

void graph_t::print(double m){
	cerr << "↑" << endl;
	for(int i=0; i<items.size(); i++){
		cerr << "├"; //"│";
		int k = (int)(items[i].v * 30/ m);
		for(int j=0; j<k; j++)
			cerr << "─";
		cerr << "*";
		cerr << " " << items[i].n;
		cerr << "(" << items[i].v << ")";
		cerr << endl;
	}
	cerr << "└";
	for(int i=0; i<38; i++)
		cerr << "─";
	cerr << "→" << endl;
}


void main()
{
	srand(time(NULL));
	{
		graph_t d;
		for(int i=0; i<10; i++){
			char s[10];
			sprintf(s, "X%d", i);
			d(s, rand() % 100);
		}
		d.print(100);
	}
}


