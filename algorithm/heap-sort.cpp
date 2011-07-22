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

#include <time.h>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

#define XCHG(T, a, b) do{T t=(a); (a)=(b); (b)=t; }while(0)
#define LESS(a,b) ((int)(a) < (int)(b))
//#define LESS(a,b) ((a) < (b))

void make_sub_heap_1(double* a, int n, int i)
{
	int largest = i;
	largest = i*2+1<n && LESS(a[largest], a[i*2+1]) ? i*2+1 : largest;
	largest = i*2+2<n && LESS(a[largest], a[i*2+2]) ? i*2+2 : largest;
	if(largest != i)
	{
		XCHG(double, a[i], a[largest]);
		make_sub_heap_1(a, n, largest);
	}
}
void make_sub_heap(double* a, int n, int i)
{
	// non-recursive version, or tail recursive
	while(1){
		int largest = i;
		largest = i*2+1<n && LESS(a[largest], a[i*2+1]) ? i*2+1 : largest;
		largest = i*2+2<n && LESS(a[largest], a[i*2+2]) ? i*2+2 : largest;
		if(largest == i)
			return;
		XCHG(double, a[i], a[largest]);
		i = largest; // make_sub_heap_1(a, n, largest);
	}
}

void heap_sort(double* a, int n){
	// make heap
	// from root to leaves, the indices are
	// 0
	// 01 10
	// 011 100 101 110
	// 0111 1000 1001 1010 1011 1100 1101 1110
	// to find parent(i), just (i-1)/2 
	// children(p) : 2*p+1, 2*p+2
	for(int i=n/2-1; i>=0; i--)
	{
		make_sub_heap(a, n, i);
	}
	// -- check heap --
	bool is_heap = true;
	for(int i=n-1; i>0; i--)
	{
		int p = (i-1)/2;
		if(LESS(a[p], a[i]))
		{
			is_heap = false;
			//cerr << "wrong at:" << i << "," << a[i] << ","<<p<<","<<a[p]<<endl;
			break;
		}
	}
	cerr << (is_heap ? "is heap" : "not a heap") << endl;
	for(int i=n-1; i>0; i--)
	{
		XCHG(double, a[i], a[0]);
		make_sub_heap(a, i, 0);
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
	heap_sort(&a.front(), a.size());

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


