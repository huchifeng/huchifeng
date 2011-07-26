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
Splay tree:
all op avg time: O(log(N)); not guarantee for worst case
   amortized cost is logarithmic // guaranted, worst ((m+n)*log(n) / m), when m is big enough, got log(N)
data structure same with standard BST(Binary search tree )
most recent node will be the root

splay operation move x to root, and decrease the depth by about half for the nodes on the path

splay tree without parent field is also possible.

to delete one node x, use splay op to make x->left->right == NULL, then let x->left->right = x->right;
to merge three (ALL A < ALL B), just lookup max(A), to make root(A)->right == NULL, then root(A)->right = B; 

access
insert
delete
join(tree_1, tree_2);
split

1. bottom up;
ZIG(x): y(x(A B) C) -> x(A y(B C)); //  <- ZAG(y)
ZAG(x): y(A x(B C)) -> x(y(A B) C)
ZIG-ZIG: z(y(x(A B) C) D) -> x(A y(B z(C D))); // == ZIG(y) ZIG(x)
ZIG-ZAG: z(A y(x(B C) D)) -> x(z(A B) y(C D)); //  == ZIG(x) ZAG(x)

2. top-down

when search x, put small_then(x) to left tree 'L', bigger_then(x) to right tree 'R';
 sub tree under x is (A, B)
 then new tree will be x(L(null A) R(B null)) so it's called splay!

for tree X, left(X) is left(smallest(X)), right(X) is right(biggest(X))
  then X can be drawn as a single node;


3.

semi-splay tree: 
  don't need to move x to root.
     access x again will make it closer to root.
  for ZIG-ZIG: z(y(x(A B) C) D) -> y(x(A B) z(C D)); do ZIG(y) only; then use y to rotate, not x;
  for ZIG-ZAG, same with splay tree.

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
	node(){
		left = NULL;
		right = NULL;
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
node<T>* new_node(T &t){
	node<T>* n = new node<T>;
	n->t = t;
	return n;
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
template<class T>
int count(node<T>* root){
	return !root ? 0 : 1 + count(root->left) + count(root->right);
}
template<class T>
node<T>* splay(node<T>* root, T t){
	if(!root)
		return NULL; // no insert
	node<T> N;
	node<T>* right_tree = &N; // trace min(bigger(x))
	node<T>* left_tree = &N; // trace max(less(x))
	while(1){
		if(t < root->t)
		{
			if(root->left == NULL)
				break;
			if(t < root->left->t){
				node<T> *y = root->left;
				root->left = y->right;
				y->right = root;
				root = y;
				if(root->left == NULL)
					break;
			}
			right_tree->left = root;
			right_tree = root;
			root = root->left; // no need to break right_tree->left, will be set after break the loop
			continue;
		}
		if(t > root->t){
			if(root->right == NULL)
				break;
			if(t > root->right->t){
				node<T> *y = root->right;
				root->right = y->left;
				y->left = root;
				root = y;
				if(root->right == NULL)
					break;
			}
			left_tree->right = root;
			left_tree = root;
			root = root->right;
			continue;
		}
		break;
	}
	left_tree->right = root->left;
	right_tree->left = root->right;
	root->left = N.right;
	root->right = N.left;
	return root;
}
template<class T>
void splay_insert(node<T>*& root, T k){
	root = splay(root, k);
	if(!root){
		root = new_node<T>(k);
		return;
	}
	if(k < root->t){
		node<T>* n = new_node<T>(k);
		n->left = root->left;
		n->right = root;
		root->left = NULL;
		root = n;
		return;
	}
	else if(k > root->t){
		node<T>* n = new_node<T>(k);
		n->right = root->right;
		n->left = root;
		root->right = NULL;
		root = n;
		return;
	}
}

void main()
{
	srand(time(NULL));
	vector<int> a;
	a.resize(30);
	for(int i=0; i<a.size(); i++)
		a[i] = i;
	rand_sort(&(a[0]), sizeof(a[0]), a.size());

	node<int>* root = NULL;
	vector<bool> has_brother;
	for(int i=0; i<a.size(); i++){
		//cerr << a[i] << endl;
	}
	for(int i=0; i<a.size(); i++){
		splay_insert(root, a[i]);
		print_mid(root, has_brother);
	}
	cerr << "depth:" << depth(root) << endl;
	cerr << "count:" << count(root) << endl;
}

