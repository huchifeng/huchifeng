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
BST: binary search tree

*/

#include <time.h>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

#define XCHG(T, a, b) do{T t=(a); (a)=(b); (b)=t; }while(0)

template<class T>
struct node{
	struct node* left;
	struct node* right;
	T t;
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
node<T>* new_node(T &t){
	node<T>* n = new node<T>;
	n->left = NULL;
	n->right = NULL;
	n->t = t;
	return n;
}


template<class T>
void insert(node<T>*& root, T t){
	if( ! root){
		root = new_node(t);
		return;
	}
	if(t < root->t){
		insert(root->left, t);
	}
	else{
		insert(root->right, t);
	}
}
template<class T>
void print(node<T>* root, vector<bool>& has_brother){
	if(!root)
		return;
	for(int i=0; i<has_brother.size(); i++){
		if(i == has_brother.size()-1){
			cerr << (has_brother[i] ? "©À" : "©¸");
		}
		else{
			cerr << (has_brother[i] ? "©¦" : "  ");
		}
	}
	cerr << root->t << endl;
	has_brother.push_back(root->right ? true : false);
	print(root->left, has_brother);
	has_brother.back() = false;
	print(root->right, has_brother);
	has_brother.pop_back();
}
template<class T>
void print_mid(node<T>* root, vector<bool>& is_left){
	if(!root)
		return;
	is_left.push_back(true);
	print_mid(root->left, is_left);
	is_left.pop_back();

	for(int i=0; i<is_left.size(); i++){
		if(i == is_left.size()-1){
			cerr << (is_left[i] ? "©°" : "©¸");
		}
		else{
			cerr << (is_left[i]==is_left[i+1] ? "  " : "©¦");
		}
	}
	cerr << root->t << endl;

	is_left.push_back(false);
	print_mid(root->right, is_left);
	is_left.pop_back();
}
template<class T>
int depth(node<T>* root){
	return !root ? 0 : 1 + max(depth(root->left), depth(root->right));
}

void main()
{
	srand(time(NULL));
	vector<int> a;
	a.resize(30000);
	for(int i=0; i<a.size(); i++)
		a[i] = i;
	rand_sort(&(a[0]), sizeof(a[0]), a.size());

	node<int>* root = NULL;
	for(int i=0; i<a.size(); i++){
		//cerr << a[i] << endl;
	}
	for(int i=0; i<a.size(); i++){
		insert(root, a[i]);
	}
	cerr << "depth:" << depth(root) << endl;
	vector<bool> has_brother;
	//print(root, has_brother);
	//print_mid(root, has_brother);
}

