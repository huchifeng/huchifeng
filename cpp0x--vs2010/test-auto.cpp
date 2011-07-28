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

*/


#include <time.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator> // front-inserter
#include <vector>
#include <deque>
#include <map>
#include <functional> // tr1::function
#include <assert.h>
using namespace std;


void main()
{
	map<string, string> m;
	m["1"] = "one";
	m["2"] = "two";
	for (auto i = m.cbegin(); i != m.cend(); ++i) { // const-begin
        cout << i->second << " are " << i->first << endl;
    }
	cout << endl;
}

