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
#include <iostream>
#include <algorithm>
#include <iterator> // front-inserter
#include <vector>
#include <deque>
#include <functional> // tr1::function
#include <assert.h>
using namespace std;

struct LambdaFunctor {
    void operator()(int n) const {
        cout << n << ",";
    }
};

void meow(const vector<int>& v, const function<void (int)>& f) {
    for_each(v.begin(), v.end(), f);
    cout << endl;
}

void main()
{
	vector<int> v(10);
	for (int i = v.size()-1; i>=0; i--) {
		v[i] = i;
	}
	for_each(v.begin(), v.end(), [](int n) { cout << n << " "; });
	cout << endl;
	// actually lambda  expr def a class and construct an obj
	for_each(v.begin(), v.end(), LambdaFunctor());
	cout << endl;
	int N = v.size();
	for_each(v.begin(), v.end(), [v, N](int n) { // put in capture list
		cout << n << " ";
		if(n == N-1) cout << "!";
		// N --; // cannot modify
	});
	cout << endl;

	for_each(v.begin(), v.end(), [=](int n){ //capture by default
		cout << n << " ";
		if(n >= N-1) cout << "!";
	});
	cout << endl;

	for_each(v.begin(), v.end(), [N](int n) mutable {
		cout << n << " ";
		if(n >= N-1) cout << "!";
		N --; // not modify origin
	});
	cout << endl;
	cerr << N << endl;

	for_each(v.begin(), v.end(), [&N](int n){
		cout << n << " ";
		if(n >= N-1) cout << "!";
		N --; // modify origin
	});
	cout << endl;
	cerr << N << endl;

	int N2 = v.size();
	for_each(v.begin(), v.end(), [&, N](int n){  // capture other by ref, except N by value
		cout << n << " ";
		if(n >= N-1) cout << "!";
		N2 ++; // modify origin
		//N ++; // cannot modify
	});
	cout << endl;
	cerr << N2 << endl;

	struct A{
		vector<int> v;
		A(){
			v.resize(10);
			//for_each(v.begin(), v.end(), [v](int n){cout << n << ":" << v.size();}); // error
			for_each(v.begin(), v.end(), [&](int n){cout << n << ":" << v.size();}); // ok, by ref
			// cannot pass &this
			cout << endl;
			for_each(v.begin(), v.end(), [=](int n){cout << n << ":" << v.size();}); // ok, by value
			cout << endl;
			for_each(v.begin(), v.end(), [this](int n){cout << n << ":" << this->v.size();});// ok
			// cannot access lambda itself's this
			cout << endl;
		}
	} a;

	{
		deque<int> d;
		transform(v.begin(), v.end(), front_inserter(d), [](int n) { return n * n * n; });
		// automatic guess return int
		for_each(d.begin(), d.end(), [](int n) { cout << n << " "; });
		cout << endl;
	}

	[](){}();
	[]{}(); // () can be ignored

	meow(v, [](int n){ cout << n << "-"; });
	function<void (int)> g = [](int n) { cout << n * n * n << " "; };
	meow(v, g);
	cout << endl;
}

