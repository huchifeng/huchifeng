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
AVL: named by author: Adelson-Velskii & Landis
 self-adjusting
 heights of left subtree and right subtree differ at most 1
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
	int depth;
	T t;
	node(){
		left = NULL;
		right = NULL;
		depth = 0;
	}
	int left_depth(){
		return left ? 1+left->depth : 0;
	}
	int right_depth(){
		return right ? 1+right->depth : 0;
	}
	void calc_depth(){
		depth = max(left_depth(), right_depth());
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
	n->left = NULL;
	n->right = NULL;
	n->depth = 0;
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
	re_balance(root);
}
template<class T>
void re_balance(node<T>*& root){
	root->calc_depth();
	// check restore balance !
	if(root->left_depth() - root->right_depth() > 1){
		if(root->left->left_depth() >= root->left->right_depth()){
			cerr << "---1---" << endl;
			node<T>* l = root->left;
			node<T>* t = l->right;
			root->left = t;
			l->right = root;
			root = l;
			root->right->calc_depth();
			root->calc_depth();
		}
		else{
			cerr << "---2---" << endl;
			node<T>* l = root->left;
			node<T>* lr = l->right;
			root->left = lr->right;
			l->right = lr->left;
			lr->left = l;
			lr->right = root;
			root = lr;
			root->left->calc_depth();
			root->right->calc_depth();
			root->calc_depth();
		}
	}
	else if(root->left_depth() - root->right_depth() < -1){
		if(root->right->right_depth() >= root->right->left_depth()){
			cerr << "---3---" << endl;
			node<T>* r = root->right;
			node<T>* t = r->left;
			root->right = t;
			r->left = root;
			root = r;
			root->left->calc_depth();
			root->calc_depth();
		}
		else{
			cerr << "---4---" << endl;
			node<T>* r = root->right;
			node<T>* rl = r->left;
			root->right = rl->left;
			r->left = rl->right;
			rl->left = root;
			rl->right = r;
			root = rl;
			root->left->calc_depth();
			root->right->calc_depth();
			root->calc_depth();
		}
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
	cerr << root->t;
	if(root->depth > 0)
		;//cerr << ":" << root->depth;
	cerr << endl;

	is_left.push_back(false);
	print_mid(root->right, is_left);
	is_left.pop_back();
}
template<class T>
int depth(node<T>* root){
	return !root ? 0 : 1 + max(depth(root->left), depth(root->right));
}
template<class T>
void check_balance(node<T>* root){
	if(root==NULL)
		return;
	int old = root->depth;
	check_balance(root->left);
	check_balance(root->right);
	root->calc_depth();
	assert(old == root->depth);
	assert(abs(root->left_depth() - root->right_depth())<=1);
}
template<class T>
void remove(node<T>*& root, T t){
	if(!root)
		return;
	if(root->t==t){
		// move to leaf
		if(root->left){
			vector<node<T>*> vp;
			node<T>* n = root->left;
			while(n->right!=NULL){
				vp.push_back(n);
				n = n->right;
			}
			root->t = n->t;
			delete n;
			if(vp.size()>0)
				vp.back()->right = NULL;
			else
				root->left = NULL;
			for(int i=vp.size()-1; i>=0; i--)
				re_balance(vp[i]);
			re_balance(root);
			return;
		}
		else if(root->right){
			vector<node<T>*> vp;
			node<T>* n = root->right;
			while(n->left!=NULL){
				vp.push_back(n);
				n = n->left;
			}
			root->t = n->t;
			delete n;
			if(vp.size()>0)
				vp.back()->left = NULL;
			else
				root->right = NULL;
			for(int i=vp.size()-1; i>=0; i--)
				re_balance(vp[i]);
			re_balance(root);
			return;
		}
		delete root;
		root = NULL;
	}
	else if(t < root->t){
		remove(root->left, t);
		re_balance(root);
	}
	else{
		remove(root->right, t);
		re_balance(root);
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
		insert(root, a[i]);
		print_mid(root, has_brother);
	}
	cerr << "-----------------removing------------" << endl;
	rand_sort(&(a[0]), sizeof(a[0]), a.size());
	for(int i=0; i<a.size(); i++){
		remove(root, a[i]);
		print_mid(root, has_brother);
	}
	//print_mid(root, has_brother);
	cerr << "depth:" << depth(root) << endl;
	check_balance(root);
}

