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
#include <assert.h>
using namespace std;

int myfind(vector<string>& a, int begin, int end, const string& k, bool first, int *begin2=NULL, int *end2=NULL){
	if(begin2)
		*begin2 = begin;
	if(end2)
		*end2 = end;
	int b = begin;
	int e = end - 1;
	int klen = k.length();
	int res = 1;
	while(b < e){
		int c = (b+e)/2;
		res = strncmp(a[c].c_str(), k.c_str(), klen);
		if(res > 0){
			e = c - 1;
			if(end2)
				* end2 = c;
		}
		else if(res < 0){
			b = c + 1;
			if(begin2)
				* begin2 = b;
		}
		else{
			if(first)
				e = c;
			else{
				if(e==b+1){
					if(strncmp(a[e].c_str(), k.c_str(), klen)==0){
						return e;
					}
					return b;
				}
				b = c;
			}
		}
	}
	if(b == e && res==0)
		return b;
	return -1;
}

void main()
{
	vector<string> a;
	a.resize(20);
	for(int i=0; i<a.size(); i++){
		char b[100];
		sprintf(b, "%06d", i*i); 
		a[i] = b;
	}
	for(int i=0; i<a.size(); i++)
		cerr << i << ":" << a[i] << endl;
	int begin2, end2;
	cerr << myfind(a, 0, a.size(), "0001", true, &begin2, &end2) << endl;
	cerr << begin2 << "~" << end2 << endl;
	cerr << myfind(a, begin2, end2, "0001", false) << endl;
}


