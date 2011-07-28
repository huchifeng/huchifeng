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
#include <algorithm>
#include <assert.h>
using namespace std;

void main()
{
	srand(time(NULL));
	vector<int> a;
	cout << "---------insert----------------" << endl;
	for(int i=0; i<10; i++)
		a.insert(a.begin()+i/2, i);
	for_each(a.begin(), a.end(), [](int i){ cout << i << ","; });
	cout << endl;
	cout << "---------insert----------------" << endl;
	for(int i=0; i<10; i++)
		a.insert(a.end(), i+20);
	for_each(a.begin(), a.end(), [](int i){ cout << i << ","; });
	cout << endl;
}

