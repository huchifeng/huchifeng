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
B-tree
m=3  1~2 value per node, 2~3 sub trees
m=4  // eqv. rbtree 2-3-4 sub

*/

#include <time.h>
#include <iostream>
#include <vector>
#include <list>
#include <assert.h>
using namespace std;

#define XCHG(T, a, b) do{T t=(a); (a)=(b); (b)=t; }while(0)

template<class T>
struct node{
	vector<shared_ptr<node> > sub;
	vector<T> val;
	int max;
	node(int m){
		max = m;
	}
	bool has_child(){
		return sub.size()>0;
	}
};


void rand_sort(void* x, int m, int n)
{
	for(int i=0; i<n; i++){
		int k = i+rand()%(n-i);
		if(k!=i){
			for(int s=0; s<m; s++)
				XCHG(char, ((char*)x)[i*m+s], ((char*)x)[k*m+s] );
		}
	}
}

template<class T>
void insert_in_node(node<T>* n, T t){
	assert(! n->has_child());
	int i=0;
	while(i< n->val.size() && !(t < n->val[i]))
		i ++;
	n->val.insert(n->val.begin()+i, t);
}
template<class T>
void insert_in_tree(node<T>* n, T t){
	if(!n->has_child()){
		insert_in_node(n, t);
		return;
	}
	int i=0;
	while(i< n->val.size() && !(t < n->val[i]))
		i ++;
	node<T>* n1 = n->sub[i].get();
	insert_in_tree(n1, t);
	if(n1->val.size() < n1->max)
		return;
	// split n1 to 2 nodes;
	assert(n1->val.size() == n1->max);
	node<T>* n2 = new node<T>(n1->max);
	int keep = n1->max / 2;
	{
		if(n1->has_child())
			n2->sub.push_back(n1->sub[keep+1]);
		for(int i= keep+1; i<n1->val.size(); i++){
			n2->val.push_back(n1->val[i]);
			if(n1->has_child())
				n2->sub.push_back(n1->sub[i+1]);
		}
	}

	n->val.insert(n->val.begin()+i, n1->val[keep]);
	n->sub.insert(n->sub.begin()+i+1, n2);

	n1->val.resize(keep);
	if(n1->has_child())
		n1->sub.resize(keep+1);
}

template<class T>
void insert(node<T>*& root, T t){
	insert_in_tree(root, t);
	if(root->val.size() < root->max)
		return;
	assert(root->val.size() == root->max);
	// make new root
	node<T>* n1 = root;
	node<T>* n2 = new node<T>(root->max);
	int keep = n1->max / 2;
	{
		if(n1->has_child())
			n2->sub.push_back(n1->sub[keep+1]);
		for(int i= keep+1; i<n1->val.size(); i++){
			n2->val.push_back(n1->val[i]);
			if(n1->has_child())
				n2->sub.push_back(n1->sub[i+1]);
		}
	}

	node<T>* n = new node<T>(root->max);
	n->val.insert(n->val.begin(), n1->val[keep]);
	n->sub.insert(n->sub.begin(), n1);
	n->sub.insert(n->sub.begin()+1, n2);

	n1->val.resize(keep);
	if(n1->has_child())
		n1->sub.resize(keep+1);

	root = n;
}
template<class T>
void print_mid(node<T>* root, vector<int>& path){
	if(!root)
		return;
	if(root->has_child()){ // at least 2 child
		path.push_back(0);
		print_mid(root->sub[0].get(), path);
		path.pop_back();
	}
	for(int i=0; i<root->val.size(); i++){
		for(int k=0; k<(int)path.size()-1; k++) // size() is unsigned!
			cerr << (path[k]==0 && path[k+1]==0 || path[k]==2 && path[k+1]==2 ? "  " : "©¦");
		if(path.size() > 0){
			if(path.back() == 0 && i==0)
				cerr << "©°";
			else if(path.back() == 2 && i==root->val.size()-1)
				cerr << "©¸";
			else
				cerr << "©À";
		}
		cerr << root->val[i] << endl;
		if(root->has_child()){
			path.push_back(i==root->val.size()-1 ? 2 : 1);
			print_mid(root->sub[i+1].get(), path);
			path.pop_back();
		}
	}
}
template<class T>
int depth(node<T>* root){
	return !root ? 0 : 1 + (root->sub.size()==0 ? 0 : depth(root->sub.begin()->get()));
}

void main()
{
	srand(time(NULL));
	vector<int> a;
	a.resize(300);
	for(int i=0; i<a.size(); i++)
		a[i] = i;
	rand_sort(&(a[0]), sizeof(a[0]), a.size());

	node<int>* root = new node<int>(3); // minimum meaningful
	vector<int> path;
	for(int i=0; i<a.size(); i++){
		insert(root, a[i]);
		//cerr << "--" << endl;
		//print_mid(root, path);
	}
	print_mid(root, path);
	cerr << "depth:" << depth(root) << endl;
}


