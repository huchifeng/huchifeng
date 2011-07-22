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
 merge sort, invented by John von Neumann in 1945


*/
#include <time.h>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

#define XCHG(T, a, b) do{T t=(a); (a)=(b); (b)=t; }while(0)
#define LESS(a,b) ((int)(a) < (int)(b))
//#define LESS(a,b) ((a) < (b))


void merge_sort(double* a, int n, double *t){
	if(n<=1)
		return;
	int m = n/2;
	if(m > 1)
		merge_sort(a, m, t);
	if(n - m > 1)
		merge_sort(a+m, n-m, t+m);
	// -- merge --
	for(int i=0, j=m; i<m || j<n; ){
		if(i<m && (j>=n || !LESS(a[j], a[i]))){
			t[i+j-m] = a[i];
			i ++;
		}
		else{
			t[i+j-m] = a[j];
			j ++;
		}
	}
	memcpy(a, t, n*sizeof(a[0]));
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
	srand(time(NULL));
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
	// --- sort ---
	vector<double> t;
	t.resize(a.size());
	merge_sort(&a.front(), a.size(), &t.front());

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


