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
   sub trees count : ceil(m/2) ~ m  // except root node and leaf node
m=3  1~2 value per node, 2~3 sub trees
m=4  // eqv. rbtree 2-3-4 sub

B+Tree: 
  put all value in leaf nodes, parent nodes are just left-half-closed sections
  parent nodes are like sparse index; 
  fit for file system

B*Tree:
  modify B+ tree;
  minimum 1/2 -> minimum 2/3
  when insert overflow, prefer to move nodes to sibling, if fail then 2 nodes -> 3 nodes (3/3+3/3 -> 2/3+2/3+2/3);

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
void split_node(node<T>* n1, node<T>*& n2, T& t){
	n2 = new node<T>(n1->max);
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

	t = n1->val[keep];

	n1->val.resize(keep);
	if(n1->has_child())
		n1->sub.resize(keep+1);
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
	node<T>* n2 = NULL;
	T t1;
	split_node(n1, n2, t1);	
	n->val.insert(n->val.begin()+i, t1);
	n->sub.insert(n->sub.begin()+i+1, n2);
}

template<class T>
void insert(node<T>*& root, T t){
	insert_in_tree(root, t);
	if(root->val.size() < root->max)
		return;
	assert(root->val.size() == root->max);
	// make new root
	node<T>* n1 = root;
	node<T>* n2 = NULL;
	T t1;
	split_node(n1, n2, t1);	

	node<T>* n = new node<T>(root->max);
	n->val.insert(n->val.begin(), t1);
	n->sub.insert(n->sub.begin(), n1);
	n->sub.insert(n->sub.begin()+1, n2);
	root = n;
}
template<typename T>
void remove_in_node(node<T>* n, T t){
	assert(! n->has_child());
	int i=0;
	while(i< n->val.size() && !(t <= n->val[i]))
		i ++;
	if(i < n->val.size() && t==n->val[i]){
		n->val.erase(n->val.begin()+i);
	}
}
template<typename T>
void merge(node<T>* n1, node<T>* n2){
	for(int i=0; i<n2->val.size(); i++)
		n1->val.push_back(n2->val[i]);
	if(n2->has_child()){
		assert(n1->has_child());
		for(int i=0; i<n2->sub.size(); i++)
			n1->sub.push_back(n2->sub[i]);
	}
}

template<typename T>
void check_merge(node<T>* n){
	assert(n->val.size()+1==n->sub.size() && n->sub.size()>=2);
	for(int i=0; i<n->sub.size(); i++){
		if(n->sub[i]->val.size() >= (n->max -1)/2)
			continue;
		int j = i<n->sub.size()-1 ? i+1 : i-1;
		if(n->sub[j]->val.size() > (n->max-1)/2){
			i = min(i, j);
			if(n->sub[i+1]->val.size() == 0){
				// possible, when it's leaf, has single value which just removed
				n->sub[i]->val.push_back(n->val[i]);
				if(n->sub[i+1]->has_child()){
					assert(n->sub[i+1]->sub.size()==1);
					n->sub[i]->sub.push_back(n->sub[i+1]->sub[0]);
				}
				n->val.pop_back(); // n may have no val now
				n->sub.pop_back();
				break;
			}
			n->sub[i]->val.push_back(n->val[i]);
			n->val[i] = n->sub[i+1]->val[0];
			n->sub[i+1]->val.erase(n->sub[i+1]->val.begin());
			if(n->sub[i+1]->has_child()){
				n->sub[i]->sub.push_back(n->sub[i+1]->sub[0]);
				n->sub[i+1]->sub.erase(n->sub[i+1]->sub.begin());
			}
			break;
		}
		cerr << "merge!" << endl;
		i = min(i, j);
		node<T>* n1 = n->sub[i].get();
		node<T>* n2 = n->sub[i+1].get();
		n1->val.push_back(n->val[i]);
		merge(n1, n2);
		n->val.erase(n->val.begin()+i);
		n->sub.erase(n->sub.begin()+i+1);
		break;
	}
}

template<typename T>
void remove_in_tree(node<T>* n, T t){
	if(! n->has_child()){
		remove_in_node(n, t);
		return;
	}
	int i=0;
	while(i< n->val.size() && !(t <= n->val[i]))
		i ++;
	if(i < n->val.size() && t==n->val[i]){
		vector<node<T>*> vp;
		node<T>* n1 = n->sub[i].get();
		vp.push_back(n);
		while(n1->has_child()){
			vp.push_back(n1);
			n1 = n1->sub.back().get();
		}
		n->val[i] = n1->val.back();
		n1->val.pop_back();
		for(auto it=vp.rbegin(); it!=vp.rend(); it++){
			check_merge(*it);
		}
		return;
	}
	remove_in_tree(n->sub[i].get(), t);
	check_merge(n);
}
template<typename T>
void remove(node<T>*& root, T t){
	remove_in_tree(root, t);
	node<T>* n = root;
	if(n->sub.size()==0)
		return;
	assert(n->val.size()+1 == n->sub.size());
	if(n->val.size()==0){
		node<T>* n1 = n->sub[0].get();
		n->val = n1->val;
		// n1 may has children
		vector<shared_ptr<node<T> > > sub = n1->sub;
		n->sub = sub;
	}
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
	while(1){ // for testing
	vector<int> a;
	a.resize(10);
	for(int i=0; i<a.size(); i++)
		a[i] = i;
	rand_sort(&(a[0]), sizeof(a[0]), a.size());

	node<int>* root = new node<int>(3); // minimum meaningful
	vector<int> path;
	for(int i=0; i<a.size(); i++){
		insert(root, a[i]);
		//print_mid(root, path);
	}
	print_mid(root, path);
	rand_sort(&(a[0]), sizeof(a[0]), a.size());
	vector<int> b = a;
	rand_sort(&(b[0]), sizeof(b[0]), b.size());
	for(int i=0; i<a.size(); i++){
		cerr << "remove--" << a[i] << endl;
		remove(root, a[i]);
		//insert(root, b[i] + (int)a.size()*10);
		print_mid(root, path);
	}
	cerr << "depth:" << depth(root) << endl;
	} // for testing
}


