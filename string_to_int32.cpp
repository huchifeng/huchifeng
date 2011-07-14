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

#define int32_max 0x7FFFFFFF
#define int32_min ((int)0x80000000)

bool string_to_int32(const char* s, int* x){
	while(s && (*s==' ' || *s=='\t' || *s=='\r' || *s=='\n'))
		s ++;
	*x = 0;
	bool negative = false;
	if(s && *s=='-'){
		negative = true;
		s ++;
	}
	const char* p = s;
	for(; p && *p && (*p>='0' && *p<='9'); p++){
		int c = ((*p)-'0');
		if(!negative){
			if((*x)> (int32_max - c)/10)
				return false;
			*x = (*x) * 10 + c;
		}
		else{
			if((*x)< (int32_min + c)/10)
				return false;
			*x = (*x) * 10 - c;
		}		
	}
	if(p==s)
		return false;
	while(p && (*p==' ' || *p=='\t' || *p=='\r' || *p=='\n'))
		p ++;
	if(*p)
		return false;
	return true;
}
void test(const char*s, bool b1, int x1){
	int x;
	bool b = string_to_int32(s, &x);
	cerr << "'" << s << "' " << b << "," << x << " should: " << b1 << "," << x1 << endl;
	assert(b && b1 && x==x1 || !b && !b1);
}

void main()
{
	test("1", true, 1);
	test("-1", true, -1);
	test(" 1", true, 1);
	test(" -1", true, -1);
	test("1 ", true, 1);
	test("-1 ", true, -1);
	test(" 1 ", true, 1);
	test(" -1 ", true, -1);
	test("255", true, 0xFF);
	test("-255", true, -255);
	test("1  a", false, 0);
	test("abc", false, 0);
	test("-abc", false, 0);
	test("", false, 0);
	test("-", false, 0);
	test("- 1", false, 0);

	test(" 2147483647", true, 0x7FFFFFFF);
	test("-2147483648 ", true, 0x80000000);
	test("2147483648", false, 0);
	test("-2147483649", false, 0);
}


