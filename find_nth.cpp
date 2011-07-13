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
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
using namespace std;

int int_cmp(const void* a, const void* b){
	return *(int*)a > *(int*)b ? 1 : *(int*)a < *(int*)b ? -1 : 0;
}
#define XCHG(T, a, b) do{T t=(a); (a)=(b); (b)=t; }while(0)
#define LESS(a,b) ((int)(a) < (int)(b))

void my_nth(int* a, int nth, int n){
	if(n<=1)
		return;
	int x = a[0];
	int k = 0;
	for(int i=1; i<n; i++){
		if(LESS(a[i], x)){
			k ++;
			if(k<i)
				XCHG(int, a[i], a[k]);
		}
	}
	XCHG(int, a[0], a[k]);
	if(k == nth)
		return;
	if(k < nth){
		my_nth(a+k+1, nth-k-1, n-k-1);
	}
	else{
		my_nth(a, nth, k);
	}
}

void main()
{
	vector<int> a;
	a.resize(120);
	for(int i=0; i<a.size(); i++){
		char b[100];
		a[i] = rand(); //%a.size();
	}
	for(int i=0; i<a.size(); i++)
		cerr << i << ":" << a[i] << endl;
	int nth = 13;
	if(a.size()>0){
		//qsort(&a.front(), a.size(), sizeof(a[0]), int_cmp);
		//nth_element(a.begin(), a.begin()+nth, a.end());
		my_nth(&a[0], nth, a.size());
	}
	for(int i=0; i<a.size(); i++){
		if(!(i<nth && a[i]<a[nth] || i==nth || i>nth && a[i]>=a[nth])){
			cerr << i << "," << a[i] << " vs " << nth << "," << a[nth] << endl;
			assert(0);
		}
	}
	for(int i=0; i<a.size(); i++)
		cerr << i << ":" << a[i] << endl;
}


