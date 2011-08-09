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

class diagram_t{
public:
	diagram_t(int n): m_n(n)
	{
		k.resize(n * n);
		label.resize(n);
	}
	diagram_t& operator()(int i, int j){
		has_link(i, j) = true;
		return *this;
	}
	diagram_t& operator()(int i, string s){
		label[i] = s;
		return *this;
	}
	int& has_link(int i, int j){
		return k[i * m_n + j];
	}
	void print();
	void print_digraph();
	void print_diagram();
	int size(){
		return m_n;
	}
private:
	int m_n;
	vector<int> k; // not to use vector<bool>
	vector<string> label;
};

void diagram_t::print(){
	for(int i=0; i<m_n; i++){
		for(int j=0; j<m_n; j++){
			cerr << (has_link(j, i) ? "+" : " ");
		}
		cerr << i << ":";
		for(int j=0; j<m_n; j++){
			if(has_link(j, i))
				cerr << j << ",";
		}
		cerr << "  :";
		for(int j=0; j<m_n; j++){
			if(has_link(i, j))
				cerr << j << ",";
		}
		cerr << endl;
	}
}
void diagram_t::print_diagram(){
	vector<int> max_link(m_n);
	vector<int> min_link2(m_n);
	for(int i=0; i<m_n; i++){
		min_link2[i] = i;
		for(int j=i-1; j>=0; j--){
			if(has_link(j, i)){
				min_link2[i] = j;
			}
		}
		for(int j=i+1; j<m_n; j++){
			if(has_link(i, j)){
				max_link[i] = j;
			}
		}
	}
	for(int i=0; i<m_n; i++){
		for(int j=0; j<i; j++){
			string s = "  ";
			if(min_link2[i]<=j)
				s = dots; //"─"; //
			if(has_link(j,i))
				s = "→";//"↔"; // unicode, not gb2312
			if(!has_link(j, i)){
				if(max_link[j]>i){
					cerr << "│" << s;
					continue;
				}
				if(min_link2[i]<j){
					cerr << dots << s;
					continue;
				}
				cerr << "  " << s;
				continue;
			}
			if(j==min_link2[i]){
				if(max_link[j]==i){
					cerr << "└" << s;
					continue;
				}
				cerr << "├" << s;
				continue;
			}
			if(max_link[j]==i){
				cerr << "└" << s; // "┴"
				continue;
			}
			cerr << "├"  << s; // "┼"
		}
		if(label[i]=="")
			cerr << i;
		else
			cerr << label[i];
		cerr << endl;
	}
}
void diagram_t::print_digraph(){
	vector<int> min_link(m_n);
	vector<int> max_link(m_n);
	vector<int> min_link2(m_n);
	for(int i=0; i<m_n; i++){
		max_link[i] = i;
		min_link[i] = i;
		min_link2[i] = i;
		for(int j=i-1; j>=0; j--){
			if(has_link(j, i)){
				min_link2[i] = j;
			}
		}
		for(int j=i-1; j>=0; j--){
			if(has_link(i, j)){
				min_link[i] = j;
			}
		}
		for(int j=i+1; j<m_n; j++){
			if(has_link(i, j)){
				max_link[i] = j;
			}
		}
		//cerr << i << ":" << min_link[i] << "~" << max_link[i] << endl;
	}
	for(int i=0; i<m_n; i++){
		for(int j=0; j<m_n; j++){
			string s = "  ";
			if(min_link2[i]<=j)
				s = dots;
			if(has_link(j, i))
				s = "→"; //"─";
			//else if(i>= min_link[j] && i<=max_link[j])
			//	s = "  ";
			//if(j==m_n-1)
			//	s = "";
			if(j==i){
				cerr << "○" << s; // ○├
				continue;
			}
			if(!has_link(j, i)){
				if(max_link[j]>=i && i>=min_link[j]){
					cerr << "│" << s;
					continue;
				}
				if(min_link2[i]<j){
					//cerr << "  " << s;
					cerr << dots << s;
					continue;
				}
				cerr << "  " << s;
				continue;
			}
			if(min_link[j]==i){
				cerr << "┌" << s; // ┌┬┼
				continue;
			}
			if(max_link[j]==i){
				cerr << "└" << s; // └┴┼
				continue;
			}
			cerr << "├" << s; // ├┼
		}
		if(label[i]=="")
			cerr << i;
		else
			cerr << label[i];
		cerr << endl;
	}
}

void main()
{
	srand(time(NULL));
	{
		diagram_t d(5);
		//d(0, "Zero")(1, "First")(2, "Second")(3, "Third")(4, "Forth");
		d(0, 1)(0, 3)(1, 2)(1,3)(1,4)(2, 4);
		d.print();
		d.print_diagram();
	}
	{
		diagram_t d(19);
		for(int i=0; i<2*d.size(); i++){
			d(rand()% d.size(), rand()% d.size());
		}
		d.print();
		d.print_diagram();
		cerr << endl;
		d.print_digraph();
	}
}


