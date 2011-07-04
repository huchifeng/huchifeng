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

#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

#define XCHG(T, a, b) do{T t=(a); (a)=(b); (b)=t; }while(0)
#define LESS(a,b) ((int)(a) < (int)(b))
//#define LESS(a,b) ((a) < (b))

void quick_sort(double* a, int n){
	// it's un stable
BEGIN:
	if(n<=1)
		return;
	double x = a[0];
	int k = 0;
	for(int i=1; i<n; i++){
		if(LESS(a[i], x)){
			k ++;
			if(k<i)
				XCHG(double, a[i], a[k]);
		}
	}
	XCHG(double, a[0], a[k]);
	if(k > 1)
		quick_sort(a, k);
	if(k < n-2){
		a = a + k + 1;
		n = n-1-k;
		goto BEGIN; // Tail Recursion  ?
		//quick_sort(a+k+1, n-1-k);
	}
}

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

void main()
{
	// --- init ---
	vector<double> a;
	a.resize(100);
	for(int i=0, n=a.size(); i<n; i++)
		a[i] = i/3 + (0.1 + i%3/10.0);
	// --- print ---
	for(int i=0, n=a.size(); i<n; i++)
		cerr << a[i] << ", ";
	cerr << endl;
	// --- random sort ---
	rand_sort(&a.front(), 3*sizeof(double), a.size()/3);
	// --- print ---
	for(int i=0, n=a.size(); i<n; i++)
		cerr << a[i] << ", ";
	cerr << endl;
	// --- quick sort ---
	quick_sort(&a.front(), a.size());


	// --- print ---
	for(int i=0, n=a.size(); i<n; i++)
		cerr << a[i] << ", ";
	cerr << endl;
	// -- check ---
	for(int i=1, n=a.size(); i<n; i++)
		assert(!LESS(a[i], a[i-1]));
	// -- check stable ---
	bool stable = true;
	for(int i=1, n=a.size(); i<n; i++){
		if(a[i] < a[i-1]){
			stable = false;
			break;
		}
	}
	cerr << (stable ? "---- stable ----" : "---- unstable ----") << endl;
}


