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
  for z(y(x(A B) C) D) -> y(x(A B) z(C D)); then use y to rotate, not x;

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
	struct node* parent;
	T t;
	node(){
		left = NULL;
		right = NULL;
		parent = NULL;
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
node<T>* insert(node<T>*& root, T t){
	if( ! root){
		root = new_node(t);
		return root;
	}
	if(t < root->t){
		node<T>* ret = insert(root->left, t);
		root->left->parent = root;
		return ret;
	}
	else{
		node<T>* ret = insert(root->right, t);
		root->right->parent = root;
		return ret;
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
template<class T>
void check_valid(node<T>* root){
	if(!root)
		return;
	if(root->left)
		assert(root->left->parent == root);
	if(root->right)
		assert(root->right->parent == root);
	check_valid(root->left);
	check_valid(root->right);
}
template<class T>
void rotate_to_root(node<T>*& root, node<T>* n){
	// this method has no use to decrease the height of the whole tree
	if(root == n)
		return;
	if(n->t < root->t){
		rotate_to_root(root->left, n);
		assert(root->left == n);
		node<T>* n_right = n->right;
		n->right = root;
		root->parent = n;
		root->left = n_right;
		if(n_right)
			n_right->parent = root;
		root = n;
		return;
	}
	else{
		rotate_to_root(root->right, n);
		assert(root->right == n);
		node<T>* n_left = n->left;
		n->left = root;
		root->parent = n;
		root->right = n_left;
		if(n_left)
			n_left->parent = root;
		root = n;
		return;
	}
}
template<class T>
void rotate_left(node<T>* p){
	node<T>* n = p->left;
	node<T>* tmp = n->right;
	node<T>* pp = p->parent;
	n->right = p;
	p->parent = n;
	p->left = tmp;
	if(tmp)
		tmp->parent = p;
	n->parent = pp;
	if(pp){
		if(pp->left == p){
			pp->left = n;
		}
		else{
			assert(pp->right==p);
			pp->right = n;
		}
	}
}
template<class T>
void rotate_right(node<T>* p){
	node<T>* n = p->right;
	node<T>* tmp = n->left;
	node<T>* pp = p->parent;
	n->left = p;
	p->parent = n;
	p->right = tmp;
	if(tmp)
		tmp->parent = p;
	n->parent = pp;
	if(pp){
		if(pp->left == p){
			pp->left = n;
		}
		else{
			assert(pp->right==p);
			pp->right = n;
		}
	}
}
template<class T>
void rotate_to_root(node<T>* n){
	while(n->parent)
	{
		node<T>* p = n->parent;
		node<T>* pp = p->parent;
		if(!pp){
			if(p->left == n){
				rotate_left(p);
			}
			else{
				assert(p->right == n);
				rotate_right(p);
			}
			continue;
		}
		if(pp->left ==p){
			if(p->left==n){
				rotate_left(pp);
				rotate_left(p);
				continue;
			}
			assert(p->right==n);
			rotate_right(p);
			rotate_left(pp);
			continue;
		}
		assert(pp->right ==p);
		if(p->right==n){
			rotate_right(pp);
			rotate_right(p);
			continue;
		}
		assert(p->left==n);
		rotate_left(p);
		rotate_right(pp);
		continue;
	}
}
template<class T>
node<T>* find(node<T>* root, T t){
	if(!root)
		return NULL;
	if(t== root->t){
		return root;
	}
	if(t < root->t){
		return find(root->left, t);
	}
	else{
		return find(root->right, t);
	}
}
template<class T>
void access(node<T>*& root, T k){
	node<int>* n = find(root, k);
	rotate_to_root(n);
	assert(n->parent == NULL);
	root = n;
}

void make_mid_order(int* a, int n, int* b){
	if(n<=0)
		return;
	b[0] = a[n/2];
	make_mid_order(a, n/2, b+1);
	make_mid_order(a+n/2+1, n-(n/2+1), b+n/2+1);
}

void main()
{
	srand(time(NULL));
	vector<int> a;
	a.resize(30);
	for(int i=0; i<a.size(); i++)
		a[i] = i;
	vector<int> b = a;
	make_mid_order(&a[0], a.size(), &b[0]);
	//a = b;
	//rand_sort(&(a[0]), sizeof(a[0]), a.size());

	node<int>* root = NULL;
	for(int i=0; i<a.size(); i++){
		cerr << a[i] << endl;
	}
	for(int i=0; i<a.size(); i++){
		node<int>* n = insert(root, a[i]);
		//access(root, a[i]);
	}
	vector<bool> has_brother;
	print_mid(root, has_brother);
	for(int i=0; i<a.size(); i++){
		//access(root, (int)(a.size()/2));
		//access(root, (int)(rand()%a.size()));//
	}
	//for(int i=0; i<1; i++){
	//	access(root, 0);
	//	print_mid(root, has_brother);
	//	access(root, (int)a.size()-1);
	//	print_mid(root, has_brother);
	//}
	for(int i=0; i<b.size(); i++){
		access(root, b[b.size()-1-i]); // make it very close to balanced
		print_mid(root, has_brother);
	}
	cerr << "depth:" << depth(root) << endl;
	//print(root, has_brother);
	check_valid(root);
}

