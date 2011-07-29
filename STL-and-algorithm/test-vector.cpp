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
	for(int i=0; i<5; i++)
		a.insert(a.begin()+i/2, i);
	for_each(a.begin(), a.end(), [](int i){ cout << i << ","; });
	cout << endl;

	cout << "---------insert----------------" << endl;
	for(int i=0; i<5; i++)
		a.insert(a.end(), i+20);
	for_each(a.begin(), a.end(), [](int i){ cout << i << ","; });
	cout << endl;

	cout << "---------remove----------------" << endl;
	a.erase(a.begin());
	a.erase(a.begin()+1, a.begin()+3);
	for_each(a.begin(), a.end(), [](int i){ cout << i << ","; });
	cout << endl;

	cout << "---------rbegin--back--------------" << endl;
	cout << *a.rbegin() << "," << a.back();
	cout << endl;

	cout << "---------rend----------------" << endl;
	for_each(a.rbegin(), a.rend(), [](int i){ cout << i << ","; });
	cout << endl;

	cout << "---------std::copy----------------" << endl;
	vector<int> b;
	b.resize(a.size()); // need pre alloc
	std::copy(a.rbegin(), a.rend(), b.begin());
	for_each(b.begin(), b.end(), [](int i){ cout << i << ","; });
	cout << endl;

}

