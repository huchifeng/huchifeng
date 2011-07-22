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

//#define _countof(a) (sizeof(a)/sizeof((a)[0]))

// a+b+c==0
bool check_sum3_zero(int* x, int n){
	for(int i=0; i<n; i++){
		//
		for(int a=0, b=0; a<n && b<n;){
			int diff = x[i]+x[a]+x[n-1-b];
			if(diff == 0)
				return true;
			if(diff < 0)
				a ++;
			else
				b ++;
		}
	}
	return false;
}

// cannot pass {1,2} to a1
//#define TEST(a1, b1)								\
//{													\
//	int a[]=(a1);										\
//	bool b = check_sum3_zero(a, _countof(a));		\
//	cerr << b << endl;								\
//	assert(b==b1);									\
//}
template<class T>
vector<T>& operator<<(vector<T>& a, const T& b){
	a.push_back(b);
	return a;
}
#define TEST(b1, a1)								\
{													\
	vector<int> a;	a a1;							\
	bool b = check_sum3_zero(&a[0], a.size());		\
	cerr << b << endl;								\
	assert(b==b1);									\
}

void main()
{
	//TEST({ 1}, true);
	//vector<int> a;	a<<5;	cerr << a[0];
	TEST(true, <<(-19)<<1<<2<<3<<17<<100);
	TEST(false, <<(-19)<<1<<2<<3<<27<<100);
}


